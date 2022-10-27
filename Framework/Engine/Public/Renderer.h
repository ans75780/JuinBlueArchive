#pragma once

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƽ� �����ϳ�.  */
/* ������ �ִ� ��ü���� �����Լ��� ȣ�����ֳ�. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UIBG, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, class CGameObject* pGameObject);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Draw_RenderGroup();
	void			Set_DeferredColor(_float4 vColor) { m_vDeferredColor = vColor; }
	_float4			Get_DeferredColor() { return m_vDeferredColor; }
	void			Reset_DeferredColor() { m_vDeferredColor = _float4(1.f, 1.f, 1.f, 1.f); }


private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef list<class CGameObject*>		RENDEROBJECTS;

private:
	class CTarget_Manager*					m_pTarget_Manager = nullptr;
	class CLight_Manager*					m_pLight_Manager = nullptr;


/* ���������� ���� �����̴�. */
private:
		_float4x4					m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
		class CShader*				m_pShader = nullptr;
		class CVIBuffer_Rect*		m_pVIBuffer = nullptr;
		_float4						m_vDeferredColor;




private:
	vector<class CComponent*>		m_DebugComponents;

public:
	HRESULT Render_Priority();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_Lights();
	HRESULT Render_Blend(); /* Diffuse * Shade ����ۿ� �׸���. */
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();
	HRESULT Render_UIBG();
	HRESULT Render_UI();

#ifdef _DEBUG
public:
	HRESULT Render_Debug();
	HRESULT	Add_DebugRenderGroup(class CComponent* pComponent);
#endif // _DEBUG

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END