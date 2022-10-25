#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "Key_Manager.h"
#include "UI_Manager.h"
#include "PipeLine.h"
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "CFrustum.h"
#include "Sound_Device.h"


/* 1. ���ӳ��� �ʿ��� ��ü(�Ŵ�����)���� ��Ƽ� �����Ѵ�. */
/* 2. Ŭ���̾�Ʈ �����ڰ� �����ϱ����� ��Ʈ�� �������ش�. ��. */
/* 3. ��¥ �ʿ��� �Լ��� Ŭ���׾�Ʈ�����ڿ��� ���������ֳ�. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* ���� ���̺귲�� ����Ҷ� �ʿ��� �ʱ�ȭ �������� ��ĥ�ž�. */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear(_uint iLevelID);

public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
	_float2	Get_ViewPort() { return m_pGraphic_Device->Get_ViewPort(); }

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte byKeyID);
	_byte Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID);
	_long Get_DIMouseMoveState(MOUSEMOVE eMouseMove);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelID, class CLevel* pLevel);
	_uint				Get_CurrentLevelID(void);



public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr,  CGameObject** pObject = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	list<class CGameObject*> Get_GameObjects(_uint iLevelIndex, const _tchar * pLayerTag);
	HRESULT	Start_Level(_uint iLevelIndex);

	map<const _tchar*, class CLayer*> Get_Layer(_uint iLevelIndex);

	void	Start_Event();
	void	Add_EventObject(class CGameObject*	pGameObject);
	void	Clear_Event();

public: /* For.Component_Mananger */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);	

	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

	map<const _tchar*, class CComponent*>		Get_Prototype_Component_Map(_uint iLevelIndex);

	
public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag);


public : /*For. Key_Manager*/
	KEY_STATE	Get_KeyState(KEY eKey);
	const POINT&	Get_MousePos();

public: /*For. UI_Manager*/
	HRESULT Add_UI(_uint iLevelIndex, class CUI* pUI, void* pArg = nullptr);
	HRESULT Save_UIVec(void);
	vector<class CUI_Canvas*> Get_Canvases();
	void	Set_LevelEditMode(_bool _check) { m_pUI_Manager->Set_LevelEditMode(_check); }
	void	Set_EditLevel(_uint _Level) { m_pUI_Manager->Set_EditLevel(_Level); }
	void	Set_DisableUI(_bool _Set) { m_pUI_Manager->Set_DisableUI(_Set); }
	void	Clear_UIVec() { m_pUI_Manager->Clear_UIVec(); }
	void	AllUI_Initialization() { m_pUI_Manager->AllUI_Initialization(); }
	void	Clear_Canvas(_uint _iLevel) { m_pUI_Manager->Clear_Canvas(_iLevel); }


public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState);
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState);
	_float4 Get_CamPosition();

	RAYDESC	Get_Ray();
public: /* For.Font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor, _float fScale = 1.f);


public: /* For.Light_Manager */
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC & LightDesc);
	const LIGHTDESC* Get_LightDesc(_uint iIndex);

public: /* For.Frustom */
	_bool	IsIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange = 0);


public:
	CSound_Device*					Get_SoundManager();


private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CObject_Manager*				m_pObject_Manager = nullptr;
	CComponent_Manager*				m_pComponent_Manager = nullptr;
	CTimer_Manager*					m_pTimer_Manager = nullptr;
	CPipeLine*						m_pPipeLine = nullptr;
	CKey_Manager*					m_pKey_Manager = nullptr;
	CUI_Manager*					m_pUI_Manager = nullptr;
	CFont_Manager*					m_pFont_Manager = nullptr;
	CLight_Manager*					m_pLight_Manager = nullptr;
	CFrustum*						m_pFrustum = nullptr;
	CSound_Device*					m_pSoundManager = nullptr;
public:
	inline	_float4	Get_BackBufferColor() { return m_BackBuffer_Color; }
	inline	void	Set_BackBufferColor(_float4 _color) { m_BackBuffer_Color = _color; }


public:
	void	Set_TimeMagnifiaction(_float fMag) { m_fMagnification = fMag; }
	void	Reset_TimeMagnifiaction() { m_fMagnification = 1.f; }


	inline	_bool	Get_LobbyMemorialOnce() { return m_bLobbyMemorialOnce; }
	inline	void	Set_LobbyMemorialOnce(_bool _Set) { m_bLobbyMemorialOnce = _Set; }

private:
	_float4 m_BackBuffer_Color = { 0.f, 0.f, 1.f, 1.f};
	_bool	m_bLobbyMemorialOnce = true;

private:
	_float		m_fMagnification = 1.f;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END