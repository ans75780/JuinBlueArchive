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