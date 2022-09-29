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
��ȭ�� ĳ���� �������ΰ��� �÷��̾� ������ ��� Ŭ����.
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
	const map<const _tchar*, CGameObject::OBJ_DESC>* Get_Actors(UNIT_TYPE eType);


public:
	const CGameObject::OBJ_DESC*		Find_Actors(UNIT_TYPE eType, const _tchar*	pActorName);


private:
	void		Add_Actor(const _tchar* pStudentName,UNIT_TYPE eType, UNIT_CLASS eClass, _float fDmg, _float fRange, _float fHp);



	//For.Student
private:
	vector<CGameObject::OBJ_DESC> m_vecFormation;
	map<const _tchar*, CGameObject::OBJ_DESC> m_HavedStudents;

	//for. Enemies
	/*
	�ϴ� �̸��� �����͸� �������.
	*/
private:
	//vector<CGameObject::OBJ_DESC> m_vecFormation;
	map<const _tchar*, CGameObject::OBJ_DESC> m_EnemyData;


public:
	virtual void Free() override;	

};

END

