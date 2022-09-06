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
	HRESULT	Setup_Manager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const GRAPHICDESC & GraphicDesc, _uint iNumLevels);
	virtual void Free() override;

public:
	HRESULT Add_UI(_uint iLevelIndex,class CUI* pUI, void* pArg = nullptr);

	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	void	DisableCanvas(_uint iLevelIndex);
	vector<class CUI_Canvas*> Get_Canvases();

	void	Set_LevelEditMode(_bool _check)	{ m_bLevelEditMode = _check; }
	void	Set_EditLevel(_uint _Level)		{ m_iEditLevel = _Level; }

private:
	vector<class CUI_Canvas*>			m_vecCanvas;
	typedef vector<class CUI_Canvas*>	CANVASES;
	_uint					m_iNumLevels = 0;

	_bool					m_bLevelEditMode;
	_uint					m_iEditLevel;

public:

};

END