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
	HRESULT	Setup_Manager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const GRAPHICDESC & GraphicDesc, _uint iNumLevels);
	virtual void Free() override;

public:
	HRESULT Add_UI(_uint iLevelIndex,class CUI* pUI, void* pArg = nullptr);

	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	void DisableCanvas(_uint iLevelIndex);
	vector<class CUI_Canvas*> Get_Canvases();

private:
	vector<class CUI_Canvas*>			m_vecCanvas;
	typedef vector<class CUI_Canvas*>	CANVASES;
	_uint					m_iNumLevels = 0;

public:

};

END