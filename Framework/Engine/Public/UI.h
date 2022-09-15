#pragma once
#include "GameObject.h"
#include "GameInstance.h"

//기본적으로 제공되는 UI클래스. 이를 상속받아서 UI함수를 만들어야한다.
BEGIN(Engine)
class ENGINE_DLL CUI abstract : public CGameObject
{
	friend	class CUI_Canvas;
protected:
	CUI(ID3D11Device*	pDevice, ID3D11DeviceContext *pDeviceCotext);
	virtual ~CUI() = default;

public:
	static _float4x4		g_UIMatProj;

public:
	virtual HRESULT Initialize(void *arg = nullptr);
	virtual HRESULT initialization();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void	Free() override;
//추후 IMGUI써서 UI좌표 바꿔야하니까 이니셜라이즈에서 받는것보다는 게터 세터 만드는 것이 좋음.
public:
	_float3			Get_Pos() { return m_fPos; }
	void			Set_Pos(_float3	_fPos) { m_fPos = _fPos; }

	_float3			Get_Size() { return m_fSize; }
	void			Set_Size(_float3 _fSize) { m_fSize = _fSize; }

public:
	UI_TYPE			Get_UIType() { return m_eUIType; }
	void			Set_UIType(UI_TYPE _eType) { m_eUIType = _eType; }
public:
	const _tchar*	Get_UIName() { return m_szUIName; }
	void			Set_UIName(_tchar* _szName) { lstrcpy(m_szUIName, _szName); }
public:
	const _tchar*	Get_UIClassName() { return m_szUIClass; }
	const _tchar*	Get_UITextureName() { return m_szUITextureTag; }

public:
	CUI*			Get_Parent() { return m_pParent; }
	void			Add_Child(CUI*	_pChild);
public:
	_uint			Get_UILevel() { return m_iLevel; }
	void			Set_UILevel(_uint _level) { m_iLevel = _level; }

public:
	virtual CUI*	Get_MouseOveredUI(const POINT& pt);
public:
	virtual HRESULT	LoadUIImage(const _tchar* TextureTag, _uint iLevel = 0);

public:
	virtual void	OnEvent(_uint iEventNum);
	void			Dead() { m_bDead = true; }
public:
	virtual void	OnLButtonDown();
	virtual void	OnLButtonUp();
	virtual void	OnLButtonClicked();


protected:
	virtual HRESULT	SetUp_ShaderResource();

protected:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

protected:
	_tchar			m_szUIName[MAX_PATH] = L"";
	_tchar			m_szUITextureTag[MAX_PATH] = L"";
	_tchar			m_szUIClass[MAX_PATH] = L"";

	_float3			m_fPos;
	_float3			m_fSize;
	CUI*			m_pParent = nullptr;
	vector<CUI*>	m_vecChild;

	_uint			m_iLevel = 0;
	
	_bool			m_bDead = false;

protected:
	/*
		캔버스 레벨이란?
		예를 들어서 옵션 키 등을 눌러서 팝업 창이 떴다고 치자.
		보통의 게임에서는 팝업창 바깥쪽 배경을 누르면 팝업창이 꺼지고, 팝업 창 내부를 터치했을 때만
		기능이 작동한다.

		이를 게임에서 구현한다고 치면, 매 업데이트마다 플레이어의 터치를 입력받아서 처리할 것이다.
		근데 만약 팝업 창이 떠 있는 상태에서 뒤쪽 UI들이 먼저 업데이트가 돌면 어떻게 될까?
		팝업 창의 기능이 정상적으로 작동하지 않거나, 팝업이 아닌 다른 공간을 터치했기에(실제로 팝업 화면을 터치했지만)
		창이 꺼질 것이다.

		그걸 사전에 방지하기 위해, 그려지는 캔버스의 우선 순위를 둬서 최대한 앞에서 Enable되어있는
		UI의 터치를 우선적으로 받을 수 있도록 되어있다.
	*/
	UI_TYPE	m_eUIType = UI_END;
	_bool	m_bMouseOver;
	_bool	m_bMouseClicked;
//UI는 클론을 사용하지 않지만, CGameOjbect를 상속받기에 외부에서 못쓰도록 막아놓았음. 쓰지 말도록.

protected:
	void	Compute_Transform();
//LateTick이 끝나고 자동으로 매트릭스 계산 UI는 사실상 트랜스폼을 건드리지 않아도 됨
private:
	virtual CGameObject* Clone(void* pArg) { return nullptr; }

};


END