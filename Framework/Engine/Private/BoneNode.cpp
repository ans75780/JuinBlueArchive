#include "..\Public\\BoneNode.h"

CBoneNode::CBoneNode()
{
}

HRESULT CBoneNode::Initialize(aiNode * pAINode, CBoneNode* pParent, _uint iDepth)
{
	m_iDepth = iDepth;

	m_pParent = pParent;

	Safe_AddRef(m_pParent);

	strcpy_s(m_szName, pAINode->mName.data);

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_TransformationMatrix,
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CBoneNode::Update_CombinedTransformationMatrix()
{
	//부모가 없을 경우 트랜스폼의 매트릭스만
	if (nullptr == m_pParent)
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,XMLoadFloat4x4(&m_TransformationMatrix)  * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
}

void CBoneNode::Reset_CombinedTransformationMatrix()
{
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
}

CBoneNode * CBoneNode::Create(aiNode * pAINode, CBoneNode* pParent, _uint iDepth)
{
	CBoneNode*		pInstance = new CBoneNode();

	if (FAILED(pInstance->Initialize(pAINode, pParent, iDepth)))
	{
		MSG_BOX("Failed to Created : CBoneNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoneNode::Free()
{
	Safe_Release(m_pParent);
}

