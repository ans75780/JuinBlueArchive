#pragma once
#include "GameObject.h"
#include "GameInstance.h"

//�⺻������ �����Ǵ� UIŬ����. �̸� ��ӹ޾Ƽ� UI�Լ��� �������Ѵ�.
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
//���� IMGUI�Ἥ UI��ǥ �ٲ���ϴϱ� �̴ϼȶ������ �޴°ͺ��ٴ� ���� ���� ����� ���� ����.
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
		ĵ���� �����̶�?
		���� �� �ɼ� Ű ���� ������ �˾� â�� ���ٰ� ġ��.
		������ ���ӿ����� �˾�â �ٱ��� ����� ������ �˾�â�� ������, �˾� â ���θ� ��ġ���� ����
		����� �۵��Ѵ�.

		�̸� ���ӿ��� �����Ѵٰ� ġ��, �� ������Ʈ���� �÷��̾��� ��ġ�� �Է¹޾Ƽ� ó���� ���̴�.
		�ٵ� ���� �˾� â�� �� �ִ� ���¿��� ���� UI���� ���� ������Ʈ�� ���� ��� �ɱ�?
		�˾� â�� ����� ���������� �۵����� �ʰų�, �˾��� �ƴ� �ٸ� ������ ��ġ�߱⿡(������ �˾� ȭ���� ��ġ������)
		â�� ���� ���̴�.

		�װ� ������ �����ϱ� ����, �׷����� ĵ������ �켱 ������ �ּ� �ִ��� �տ��� Enable�Ǿ��ִ�
		UI�� ��ġ�� �켱������ ���� �� �ֵ��� �Ǿ��ִ�.
	*/
	UI_TYPE	m_eUIType = UI_END;
	_bool	m_bMouseOver;
	_bool	m_bMouseClicked;
//UI�� Ŭ���� ������� ������, CGameOjbect�� ��ӹޱ⿡ �ܺο��� �������� ���Ƴ�����. ���� ������.

protected:
	void	Compute_Transform();
//LateTick�� ������ �ڵ����� ��Ʈ���� ��� UI�� ��ǻ� Ʈ�������� �ǵ帮�� �ʾƵ� ��
private:
	virtual CGameObject* Clone(void* pArg) { return nullptr; }

};


END