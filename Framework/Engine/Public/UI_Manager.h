#pragma once

/* 객체들의 원형을 관리하낟. */
/* 원형객체를 찾아서 복제하여 씬(레벨)별로 모아서 관린하다.*/

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
	/* 원형객체르ㅏㅣㄹ 복사하여 사본객체를 추가한다. */
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