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
��ȭ�� ĳ���� �������ΰ��� �÷��̾� ������ ��� Ŭ����.
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

