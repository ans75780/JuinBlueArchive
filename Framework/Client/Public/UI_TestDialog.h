#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CUI;
END

BEGIN(Client)
class CUI_TestDialog final: public CUI
{
private:
	CUI_TestDialog(ID3D11Device* pDevice, ID3D11DeviceContext *pContext);
	~CUI_TestDialog() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual CUI*	Get_MouseOveredUI(const POINT&	_pt);

public:
	virtual void	OnLButtonDown();
	virtual void	OnLButtonUp();
	virtual void	OnLButtonClicked();
	virtual void	OnEvent(_uint iEventNum);
private:
	HRESULT SetUp_ShaderResource();

public:
	static CUI_TestDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END