#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODELTYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	HRESULT Initialize_Prototype(MODELTYPE eType, 
		const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	HRESULT Initialize(void * pArg, CModel* pPrototype);
	HRESULT Render(_uint iMeshIndex, class CShader* pShader, const char* pConstantBoneName);
	HRESULT Render(class CMeshContainer* pMesh, class CShader* pShader, const char* pConstantBoneName);

	HRESULT NonAnimRender(_uint iMeshIndex);

	void Set_CurrentAnimation(_uint iAnimIndex);

public:
	HRESULT	Play_Animation(_float fTimeDelta);
	void	Update_CombinedMatrix();
	HRESULT Bind_SRV(class CShader* pShader, const char* pContantName, class CMeshContainer* pMesh, aiTextureType eType);
	HRESULT Bind_SRV(class CShader* pShader, const char* pContantName, _uint iMeshContainerIndex, aiTextureType eType);
	HRESULT Bind_Texture(class CShader* pShader, const char* pContantName,	class CTexture* pTexture);

public:
	_uint	Get_NumMeshContainers() const { return m_iNumMeshContainers; }


public:
	class CAnimation*	Get_AnimationFromName(const char* pName);
public:
	class CMeshContainer*		Get_MeshContainers(_uint iIndex);

public:
	class CBoneNode* Find_Bone(const char* pBoneName);


private:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	MODELTYPE					m_eModelType = TYPE_END;

private:
	vector<MODEL_MATERIAL>					m_Materials;
	vector<class CMeshContainer*>			m_MeshContainers;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	_uint			m_iNumMeshContainers = 0;
	_uint			m_iNumMaterials = 0;

private:
	vector<class CBoneNode*>			m_vecBones;
	typedef vector<class CBoneNode*>	BONES;

private:
	_uint									m_iCurrentAnimationIndex = 0;
	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

private:
	HRESULT Create_MeshContainers();
	HRESULT	Create_Materials(const char*	pModelFilePath);
	HRESULT Create_Bones(aiNode* pNode, class CBoneNode* pParent, _uint iDepth);
	HRESULT Create_Animations();




private:
	_float4x4	m_TransformMatrix;

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, 
		const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END