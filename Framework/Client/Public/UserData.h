#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Base.h"


namespace Engine
{
	class CBase;
}

BEGIN(Client)

/*
재화나 캐릭터 보유여부같은 플레이어 정보를 담는 클래스.
*/

class CUserData final : public CBase 
{
private:
	CUserData() {};
	virtual ~CUserData() = default;

public:
	HRESULT Initialize();
	
	vector<wstring>& Get_Formation() { return m_vecFormation; }
	const vector<wstring>& Get_HavedStudent() { return m_vecHavedStudent; }

private:
	vector<wstring> m_vecFormation;
	vector<wstring> m_vecHavedStudent;


public:
	virtual void Free() override;	

};

END

