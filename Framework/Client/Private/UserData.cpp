#include "stdafx.h"
#include "..\Public\UserData.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CUserData);

HRESULT CUserData::Initialize()
{
	Add_Actor(L"Aru_Original",   UNIT_TYPE::UNIT_TYPE_STUDENT,   UNIT_CLASS::UNIT_CLASS_BACK, 70.f, 7.f, 1000.f);
	Add_Actor(L"Haruka_Original",UNIT_TYPE::UNIT_TYPE_STUDENT,   UNIT_CLASS::UNIT_CLASS_FRONT, 5.f, 2.f, 4000.f);
	Add_Actor(L"Zunko_Original", UNIT_TYPE::UNIT_TYPE_STUDENT,   UNIT_CLASS::UNIT_CLASS_MIDDLE, 10.f, 5.f, 1000.f);


	Add_Actor(L"Soldier_Kaiserpmc_HG", UNIT_TYPE::UNIT_TYPE_ENEMY, UNIT_CLASS::UNIT_CLASS_FRONT, 20.f, 2.f, 100.f);


	ZeroMemory(&m_tUserDesc, sizeof(USER_DESC));

	m_tUserDesc.uDiamond = 5000;

	return S_OK;
}

const map<const _tchar*, CGameObject::OBJ_DESC>* CUserData::Get_Actors(UNIT_TYPE eType)
{
	switch (eType)
	{
	case Engine::UNIT_TYPE_STUDENT:
		return &m_HavedStudents;
		break;
	case Engine::UNIT_TYPE_ENEMY:
		return &m_EnemyData;
		break;
	case Engine::UNIT_TYPE_BOSS:
		break;
	case Engine::UNIT_TYPE_PROP:
		break;
	case Engine::UNIT_TYPE_END:
		break;
	default:
		break;
	}

	return nullptr;
}

const CGameObject::OBJ_DESC * CUserData::Find_Actors(UNIT_TYPE eType, const _tchar * pActorName)
{
	if (eType == UNIT_TYPE::UNIT_TYPE_STUDENT)
	{
		auto	iter = find_if(m_HavedStudents.begin(), m_HavedStudents.end(), CTag_Finder(pActorName));

		if (iter == m_HavedStudents.end())
			return nullptr;
		return &(iter->second);
	}
	else if (eType == UNIT_TYPE::UNIT_TYPE_ENEMY || UNIT_TYPE::UNIT_TYPE_BOSS)
	{
		auto	iter = find_if(m_EnemyData.begin(), m_EnemyData.end(), CTag_Finder(pActorName));

		if (iter == m_EnemyData.end())
			return nullptr;
		return &(iter->second);
	}
}

void CUserData::Add_Actor(const _tchar * pActorName, UNIT_TYPE eType, UNIT_CLASS eClass, _float fDmg, _float fRange, _float fHp)
{
	CGameObject::OBJ_DESC	desc;

	lstrcpy(desc.sz_Name, pActorName);
	desc.fDamage = fDmg;
	desc.eClass = eClass;
	desc.fMaxHp = fHp;
	desc.fHp = fHp;
	desc.eType = eType;
	desc.fRange = fRange;

	switch (eType)
	{
	case Engine::UNIT_TYPE_STUDENT:
		m_HavedStudents.emplace(pActorName, desc);
		break;
	case Engine::UNIT_TYPE_ENEMY:
		m_EnemyData.emplace(pActorName, desc);
		break;
	case Engine::UNIT_TYPE_BOSS:
		break;
	case Engine::UNIT_TYPE_END:
		break;
	default:
		break;
	}



}

void CUserData::Free()
{
	m_vecFormation.clear();
	m_HavedStudents.clear();
	m_EnemyData.clear();
}
