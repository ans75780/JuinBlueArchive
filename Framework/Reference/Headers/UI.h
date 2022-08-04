#pragma once
#include "GameObject.h"
#include "GameInstance.h"

//�⺻������ �����Ǵ� UIŬ����. �̸� ��ӹ޾Ƽ� UI�Լ��� �������Ѵ�.
BEGIN(Engine)
class ENGINE_DLL CUI abstract : public CGameObject
{
private:
	CUI(ID3D11Device*	pDevice, ID3D11DeviceContext *pDeviceCotext);
	virtual ~CUI() = default;

public:
	static _float4x4		g_UIMatProj;

public:
	virtual HRESULT Initialize(void *arg);
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

public:
	const _tchar*	Get_UIName() { return m_pUIName; }

public:
	virtual HRESULT	LoadUIImage(const _tchar* TextureTag, _uint iLevel = 0);

public:
	virtual void	OnMouseOver(void* pArg);
	virtual void	OnMouseOut(void* pArg);

public:
	virtual void	OnLButtonDown(void* pArg);
	virtual void	OnLButtonUp(void* pArg);
	virtual void	OnLButtonClicked(void* pArg);

protected:
	virtual void	OnEvent(_uint iEventNum);

protected:
	CTexture*		m_pTextureCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	
protected:
	_tchar			m_pUIName[MAX_PATH] = L"";
	_float3			m_fPos;
	_float3			m_fSize;
	CUI*			m_pParent = nullptr;
private:
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
};

END