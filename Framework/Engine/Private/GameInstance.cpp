#include "..\Public\GameInstance.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pPipeLine(CPipeLine::Get_Instance())
	, m_pKey_Manager(CKey_Manager::Get_Instance())
	, m_pUI_Manager(CUI_Manager::Get_Instance())
	, m_pFont_Manager(CFont_Manager::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
	, m_pFrustum(CFrustum::Get_Instance())

	
{	
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pUI_Manager);
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pUI_Manager);
	Safe_AddRef(m_pFrustum);



	
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pInput_Device ||
		nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	/* 그래픽디바이스. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.isWindowMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;

	/* 인풋 디바이스. */
	if (FAILED(m_pInput_Device->Initialize(hInst, GraphicDesc.hWnd)))
		return E_FAIL;


	/* 오브젝트 매니져의 예약. */
	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	/* 컴포넌트 매니져의 예약. */
	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	/* 키매니저 예약. */
	if (FAILED(m_pKey_Manager->Initialize(GraphicDesc)))
		return E_FAIL;

	if (FAILED(m_pUI_Manager->Setup_Manager(*ppDeviceOut, *ppDeviceContextOut, GraphicDesc, iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pFrustum->Initialize()))
		return E_FAIL;

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pInput_Device ||
		nullptr == m_pPipeLine || 
		nullptr == m_pObject_Manager ||
		nullptr == m_pKey_Manager ||
		nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pInput_Device->SetUp_DeviceState();

	m_pLevel_Manager->Tick(fTimeDelta);	

	m_pKey_Manager->Tick();//UI보다 먼저돌아야함.

	m_pObject_Manager->Tick(fTimeDelta);

	m_pUI_Manager->Tick(fTimeDelta);

	m_pPipeLine->Tick();

	m_pFrustum->Tick();

	m_pObject_Manager->LateTick(fTimeDelta);

	m_pUI_Manager->LateTick(fTimeDelta);
	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
	
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Clear(_uint iLevelID)
{
	if (nullptr == m_pObject_Manager || 
		nullptr == m_pComponent_Manager || 
		nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pObject_Manager->Clear(iLevelID);
	m_pComponent_Manager->Clear(iLevelID);
	m_pUI_Manager->DisableCanvas(iLevelID);

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);

	return S_OK;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Present();

	return S_OK;
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseKeyState(eMouseButtonID);
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMove);
}

HRESULT CGameInstance::Open_Level(_uint iLevelID, CLevel * pLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelID, pLevel);
}

_uint CGameInstance::Get_CurrentLevelID(void)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_CurrentLevelID();
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg, CGameObject** pObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg, pObject);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr; 

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);	
}

list<class CGameObject*> CGameInstance::Get_GameObjects(_uint iLevelIndex, const _tchar * pLayerTag)
{
	return m_pObject_Manager->Get_GameObjects(iLevelIndex, pLayerTag);
}

map<const _tchar*, class CLayer*> CGameInstance::Get_Layer(_uint iLevelIndex)
{
	return m_pObject_Manager->Get_Layer(iLevelIndex);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

map<const _tchar*, class CComponent*> CGameInstance::Get_Prototype_Component_Map(_uint iLevelIndex)
{
	return m_pComponent_Manager->Get_Prototype_Component_Map(iLevelIndex);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);	
}

_float CGameInstance::Compute_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_Timer(pTimerTag);
}

KEY_STATE CGameInstance::Get_KeyState(KEY eKey)
{
	return m_pKey_Manager->Get_KeyState(eKey);
}

const POINT & CGameInstance::Get_MousePos()
{
	return m_pKey_Manager->Get_MousePos();
}

HRESULT CGameInstance::Add_UI(_uint iLevelIndex, CUI * pUI, void * pArg)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pUI_Manager->Add_UI(iLevelIndex, pUI, pArg);


	return S_OK;
}

HRESULT CGameInstance::Save_UIVec(void)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	return m_pUI_Manager->Save_UIVec();
}

vector<class CUI_Canvas*> CGameInstance::Get_Canvases()
{
	return m_pUI_Manager->Get_Canvases();
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

_bool CGameInstance::IsIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange)
{
	if (m_pFrustum == nullptr)
		return false;

	return m_pFrustum->IsIn_Frustum_InWorldSpace(vWorldPoint, fRange);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformState);
}

_matrix CGameInstance::Get_Transform(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform(eState);
}

const _float4x4 * CGameInstance::Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_float4x4(eState);
}

const _float4x4 * CGameInstance::Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_TP(eState);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4(0.f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CamPosition();
}

HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice, pContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar * pFontTag, const _tchar * pString, const _float2 & vPosition, _fvector vColor)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(pFontTag, pString, vPosition, vColor);
}

RAYDESC & CGameInstance::Get_Ray()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pPipeLine->Get_Ray();
}



void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Destroy_Instance();		

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();	

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CPipeLine::Get_Instance()->Destroy_Instance();
	
	CFont_Manager::Get_Instance()->Destroy_Instance();

	CLight_Manager::Get_Instance()->Destroy_Instance();

	CInput_Device::Get_Instance()->Destroy_Instance();	

	CGraphic_Device::Get_Instance()->Destroy_Instance();

	CKey_Manager::Get_Instance()->Destroy_Instance();

	CUI_Manager::Get_Instance()->Destroy_Instance();

	CFrustum::Get_Instance()->Destroy_Instance();

}

void CGameInstance::Free()
{
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pFrustum);

}
