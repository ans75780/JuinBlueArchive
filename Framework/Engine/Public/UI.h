#pragma once
#include "GameObject.h"
#include "GameInstance.h"

//기본적으로 제공되는 UI클래스. 이를 상속받아서 UI함수를 만들어야한다.
BEGIN(Engine)
class ENGINE_DLL CUI abstract : public CGameObject
{
private:
	CUI(ID3D11Device*	pDevice, ID3D11DeviceContext *pDeviceCotext);
	CUI(const CUI&	rhs);
	virtual ~CUI() = default;
public:
	virtual HRESULT Initialize(_float3 _fPos, _float3 _fSize, void* pArg) PURE;
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	OnLButtonDown(void* pArg);
	virtual void	OnLButtonUp(void* pArg);
	virtual void	OnLButtonClicked(void* pArg);
protected:
	virtual void	OnEvent(_uint iEventNum);
	virtual HRESULT	LoadUIImage(const _tchar* TextureTag);

protected:
	CTexture*		m_pTextureCom = nullptr;
	
protected:
	_float3			m_fPos;
	_float3			m_fSize;
	CUI*			m_pParent = nullptr;
	vector<CUI*>	m_vecChild;

};

END