#include "..\Public\Model.h"
#include "MeshContainer.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
{
	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);

}
HRESULT CModel::Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName)
{
	m_eModelType = eType;

	char			szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	if (TYPE_NONANIM == eType)
		m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	else
		m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (FAILED(Create_MeshContainers()))
		return E_FAIL;

	return S_OK;
}



HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CModel::Render()
{
	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		m_MeshContainers[i]->Render();
	}

	return S_OK;
}

HRESULT CModel::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i]);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, MODELTYPE eType, const char * pModelFilePath, const char * pModelFileName)
{
	CModel*		pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, pModelFileName)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	m_MeshContainers.clear();

	if (false == m_isCloned)
		m_Importer.FreeScene();

}
