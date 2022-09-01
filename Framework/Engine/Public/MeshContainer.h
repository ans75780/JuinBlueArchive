#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class ENGINE_DLL CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}
public:
	virtual HRESULT Initialize_Prototype(CModel::MODELTYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix TransformMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix TransformMatrix);
	HRESULT Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, class CModel* pModel);
	HRESULT SetUp_BonesPtr(class CModel* pModel);
	void				SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix);

public:
	const char*			Get_Name() const { return m_szName; }

private: 
	char				m_szName[MAX_PATH] = "";

	/* 현재ㅑ 메시컨테이너에게 적용되어야할 머테리얼 인덱스*/
	_uint				m_iMaterialIndex = 0;
	
	
	_uint				m_iNumBones = 0;
	const aiMesh*		m_pAIMesh = nullptr;

	vector<class CBoneNode*> m_vecBones;

public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		CModel::MODELTYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END