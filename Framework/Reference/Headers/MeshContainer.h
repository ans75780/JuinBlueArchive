#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	virtual HRESULT Initialize_Prototype(CModel::MODELTYPE eType, const aiMesh* pAIMesh);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh);
	HRESULT Ready_VertexBuffer_Anim(const aiMesh* pAIMesh);


public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::MODELTYPE eType, const aiMesh* pAIMesh);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END