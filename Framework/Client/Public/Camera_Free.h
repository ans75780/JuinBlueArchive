#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCamera_Free final : public CCamera
{
public:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	PlayExs(class CCharater* _chara);
	void	EndExs();
private:
	void	ExCamPlay(_float& fTimeDelta);
	void	GetModelCam();
	void	RotateMat(_matrix*	_Mat, _fvector vAxis, _float fRadian);

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CCharater*	m_pChara = nullptr;
	CModel*		m_pModelCom = nullptr;
	CModel*		m_pModelCom_AruCam = nullptr;
	CModel*		m_pModelCom_HarukaCam = nullptr;
	CModel*		m_pModelCom_ZunkoCam = nullptr;


	_matrix		m_tOrigineMat;


private:
	_bool	m_bCameraControl = false;
	_bool	m_bExCamOnce = false;
};

END