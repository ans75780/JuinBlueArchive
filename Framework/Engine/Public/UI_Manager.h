#pragma once

/* ��ü���� ������ �����ϳ�. */
/* ������ü�� ã�Ƽ� �����Ͽ� ��(����)���� ��Ƽ� �����ϴ�.*/

#include "Base.h"

BEGIN(Engine)

class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)
private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;
	
public:
	HRESULT Reserve_Container(_uint iNumLevels);

public:
	/* ������ü�����Ӥ� �����Ͽ� �纻��ü�� �߰��Ѵ�. */
	HRESULT Add_UI(_uint iLevelIndex, const _tchar* pUITag, class CUI* pUI, void* pArg = nullptr);

	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	void DisableUIs(_uint iLevelIndex);
	void Clear(_uint iLevelIndex);

private:
	map<const _tchar*, class CUI*>*				m_pUIs = nullptr;
	typedef map<const _tchar*, class CUI*>		UIS;

	_uint					m_iNumLevels = 0;
private:
	class CGameObject* Find_UI(_uint iLevelIndex, const _tchar* UITag);

public:
	virtual void Free() override;
};

END