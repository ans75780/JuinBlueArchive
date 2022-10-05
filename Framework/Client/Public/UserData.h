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
	void	Tick();

	const vector<CGameObject::OBJ_DESC>& Get_Formation() { return m_vecFormation; }
	void	Clear_Formation() { m_vecFormation.clear(); }
	void	Add_Formation(CGameObject::OBJ_DESC desc) { m_vecFormation.push_back(desc); }
	const map<const _tchar*, CGameObject::OBJ_DESC>* Get_Actors(UNIT_TYPE eType);

	const CGameObject::OBJ_DESC*		Find_Actors(UNIT_TYPE eType, const _tchar*	pActorName);


private:
	void		Add_Actor(const _tchar* pStudentName,UNIT_TYPE eType, UNIT_CLASS eClass, _float fDmg, _float fRange, _float fHp);



	//For.Student
private:
	vector<CGameObject::OBJ_DESC> m_vecFormation;
	map<const _tchar*, CGameObject::OBJ_DESC> m_HavedStudents;

	//for. Enemies
	/*
	일단 쫄몹들 데이터만 들고있음.
	*/
private:
	//vector<CGameObject::OBJ_DESC> m_vecFormation;
	map<const _tchar*, CGameObject::OBJ_DESC> m_EnemyData;



private:
	struct USER_DESC
	{
		_uint	uDiamond;	//청휘석
		CUI*	pDiamondText;

	};

	USER_DESC m_tUserDesc;

public:
	USER_DESC*	Get_UserDesc() { return &m_tUserDesc; }

	
public:
	virtual void Free() override;	

};

END

