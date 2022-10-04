#include "stdafx.h"
#include "..\Public\UserData.h"
#include "GameObject.h"

#include "UI.h"
#include "UI_Text.h"

IMPLEMENT_SINGLETON(CUserData);

HRESULT CUserData::Initialize()
{
	Add_Student(L"Aru", UNIT_CLASS::UNIT_CLASS_BACK, 30.f, 15.f, 100.f);
	Add_Student(L"Haruka", UNIT_CLASS::UNIT_CLASS_FRONT, 10.f, 3.f, 200.f);
	Add_Student(L"Zunko", UNIT_CLASS::UNIT_CLASS_MIDDLE, 20.f, 5.f, 100.f);


	ZeroMemory(&m_tUserDesc, sizeof(USER_DESC));

	m_tUserDesc.uDiamond = 5000;

	return S_OK;
}

void CUserData::Tick()
{
	if (m_tUserDesc.pDiamondText)
	{
		_tchar temp[10] = {};
		_itow_s(m_tUserDesc.uDiamond, temp, 10, 10);

		static_cast<CUI_Text*>(m_tUserDesc.pDiamondText)->SetUIText(temp);
	}
}


const CGameObject::OBJ_DESC* CUserData::Find_Student(const _tchar * pStudentName)
{

	auto	iter = find_if(m_HavedStudents.begin(), m_HavedStudents.end(), CTag_Finder(pStudentName));

	if (iter == m_HavedStudents.end())
		return nullptr;

	return &(iter->second);

}

void CUserData::Add_Student(const _tchar * pStudentName, UNIT_CLASS eClass, _float fDmg, _float fRange, _float fHp)
{
	CGameObject::OBJ_DESC	desc;

	lstrcpy(desc.sz_Name, pStudentName);
	desc.m_fDamage = fDmg;
	desc.m_eClass = eClass;
	desc.m_fMaxHp = fHp;
	desc.m_fHp = fHp;
	desc.m_fRange = fRange;

	m_HavedStudents.emplace(pStudentName, desc);
}

void CUserData::Free()
{
	m_vecFormation.clear();
	m_HavedStudents.clear();
}
