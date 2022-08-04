#pragma once
#include "GameObject.h"
#include "GameInstance.h"

interface ENGINE_DLL CUIBase abstract:
	public CGameObject
{
public:
	CUIBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIBase(const CUIBase& Prototype);
	~CUIBase();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	void	Set_Pos_LeftTop_Standard(_float3 _fPosition);

public:
	virtual void OnLButtonDown(void *arg = nullptr);
	virtual void OnLButtonUp(void *arg = nullptr);
	virtual void OnLButtonClicked(void *arg = nullptr);

protected:
	_float3		m_fSize;
	_float3		m_fPosition;
	_float4x4	m_ProjMatrix;
};

