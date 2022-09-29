#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Base.h"
#include "GameObject.h"

namespace Engine
{
	class CBase;
	class CGameObject;
}

BEGIN(Client)

/*
재화나 캐릭터 보유여부같은 플레이어 정보를 담는 클래스.
*/

class CUserData final : public CBase 
{
	DECLARE_SINGLETON(CUserData)
private:
	CUserData() {};
	virtual ~CUserData() = default;

public:
	HRESULT Initialize();
	
	const vector<CGameObject::OBJ_DESC>& Get_Formation() { return m_vecFormation; }
	void	Clear_Formation() { m_vecFormation.clear(); }
	void	Add_Formation(CGameObject::OBJ_DESC desc) { m_vecFormation.push_back(desc); }


	map<const _tchar*, CGameObject::OBJ_DESC>& Get_HavedStudent() { return m_HavedStudents; }


public:
	const CGameObject::OBJ_DESC*		Find_Student(const _tchar*	pStudentName);


private:
	vector<CGameObject::OBJ_DESC> m_vecFormation;
	map<const _tchar*, CGameObject::OBJ_DESC> m_HavedStudents;


private:
	void		Add_Student(const _tchar* pStudentName, UNIT_CLASS eClass, _float fDmg, _float fRange, _float fHp);

public:
	virtual void Free() override;	

};

END

