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

public:
	HRESULT Reserve_Container(_uint iNumLevels);

public:



	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	void Clear(_uint iLevelIndex);

private:
	map<const _tchar*, class CUI*>			m_UIs;
	typedef map<const _tchar*, class CUI*>	UIS;

	_uint					m_iNumLevels = 0;

private:
	class CUI* Find_UI(const _tchar* pUITag);
	
public:
	virtual void Free() override;
};

END