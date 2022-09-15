#include "..\Public\VIBuffer_Terrain.h"
#include "CFrustum.h"
#include "CQuadTree.h"
CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_pFaceIndices(rhs.m_pFaceIndices)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh;
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	BITMAPINFOHEADER			ih;
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	_ulong*			pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, (pPixel[iIndex] & 0x000000ff) / 10.f, (_float)i);
			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}	

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pPixel);
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	m_pFaceIndices= new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pFaceIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);


	_ulong		dwNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			_uint	iIndices[] = {
				iIndex + m_iNumVerticesX, 				
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1, 
				iIndex
			};

			m_pFaceIndices[dwNumFaces]._1 = iIndices[0];
			m_pFaceIndices[dwNumFaces]._2 = iIndices[1];
			m_pFaceIndices[dwNumFaces]._3 = iIndices[2];
			++dwNumFaces;

			m_pFaceIndices[dwNumFaces]._1 = iIndices[0];
			m_pFaceIndices[dwNumFaces]._2 = iIndices[2];
			m_pFaceIndices[dwNumFaces]._3 = iIndices[3];
			++dwNumFaces;
		}
	}
	

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pFaceIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Terrain::Culling(_fmatrix WorldMatrixInv)
{
	CFrustum*	pFrustum = GET_INSTANCE(CFrustum);

	/*
	�����������ϴ� ������ ����ü�� �񱳸� ���� ȭ�鿡 �׷������� �ﰢ���� �����ϴ� ������
	�ε����� �ε������ۿ� ���Ӱ� �߰��Ѵ�. �̸� ���ϱ� ���ؼ��� ���� ���� �����̽��� ��ġ�ؾ��Ѵ�.
	*/
	/*
	����ü Ŭ������ �����ϰ� �ִ� ���� ������ ���÷� ������.
	*/
	pFrustum->Transform_ToLocalSpace(WorldMatrixInv);

	_uint		iNumFaces = 0;
	m_pQuadTree->Culling();
	/*��� �ﰢ������ ����ü �ȿ� �����ϴ��� ���Ѵ�.*/
	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			_uint	iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};
			_bool isIn[] =
			{
				pFrustum->IsIn_Frustum_InLocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[0]])),
				pFrustum->IsIn_Frustum_InLocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[1]])),
				pFrustum->IsIn_Frustum_InLocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[2]])),
				pFrustum->IsIn_Frustum_InLocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[3]]))
			};
			//���� ��
			if (3 == (isIn[0] + isIn[1] + isIn[2]))
			{
				m_pFaceIndices[iNumFaces]._1 = iIndices[0];
				m_pFaceIndices[iNumFaces]._2 = iIndices[1];
				m_pFaceIndices[iNumFaces]._3 = iIndices[2];
				++iNumFaces;
			}
			//������ �Ʒ�
			if (3 == (isIn[0] + isIn[2] + isIn[3]))
			{
				m_pFaceIndices[iNumFaces]._1 = iIndices[0];
				m_pFaceIndices[iNumFaces]._2 = iIndices[2];
				m_pFaceIndices[iNumFaces]._3 = iIndices[3];
				++iNumFaces;
			}
		}
	}
	m_iNumIndices = 3 * iNumFaces;

	D3D11_MAPPED_SUBRESOURCE	SubResources;
	ZeroMemory(&SubResources, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0 , &SubResources);

	memcpy(SubResources.pData, m_pFaceIndices, sizeof(FACEINDICES32) * m_iNumIndices);

	m_pContext->Unmap(m_pIB,0);

	RELEASE_INSTANCE(CFrustum);
}


CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	if (m_isCloned == false)
	{
		Safe_Delete(m_pFaceIndices);
		//Safe_Delete(m_pVerticesPos);
	}
	

}
