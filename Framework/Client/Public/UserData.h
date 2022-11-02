#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Base.h"
#include "GameObject.h"

namespace Engine
{
	class CUI;
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

public:
	struct USER_DESC
	{
		_uint	uDiamond;	//û�ּ�
	};

private:
	CUserData() {};
	virtual ~CUserData() = default;

public:
	HRESULT Initialize();

	const vector<CGameObject::OBJ_DESC>& Get_Formation() { return m_vecFormation; }
	void	Clear_Formation() { m_vecFormation.clear(); }
	void	Add_Formation(CGameObject::OBJ_DESC desc) { m_vecFormation.push_back(desc); }
	const map<const _tchar*, CGameObject::OBJ_DESC>* Get_Actors(UNIT_TYPE eType);

	const CGameObject::OBJ_DESC*		Find_Actors(UNIT_TYPE eType, const _tchar*	pActorName);


private:
	void		Add_Actor(const _tchar* pStudentName,UNIT_TYPE eType, UNIT_CLASS eClass, _float fDmg, _float fRange, _float fHp, _float fAttackStartRatio);

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

private:
	USER_DESC m_tUserDesc;

public:
	USER_DESC	Get_UserDesc()	{ return m_tUserDesc; }
	void		Set_UserDesc(USER_DESC _desc)	{ m_tUserDesc = _desc; }
	
public:
	virtual void Free() override;	

};

END

