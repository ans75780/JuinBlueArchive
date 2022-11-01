#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CGameObject;
	class CRenderer;
	class CVIBuffer_Rect;
	class CShader;
};


//보스 HP바는 직교투영이라 따로 만들꺼임.
BEGIN(Client)

class CHpBar final : public CGameObject
{
private:
	CHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBar(const CHpBar& Prototype);
	virtual ~CHpBar() = default;
	
public:
	virtual HRESULT	Start_Level() override;
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();


public:
	void	Set_Render(_bool _bSet) { m_bRender = _bSet; }

private:
	HRESULT		SetUp_Component();
	HRESULT		SetUp_ShaderResource();


private:
	class CTexture*					m_pTextureCom = nullptr;
	class CRenderer*				m_pRendererCom = nullptr;
	class CShader*					m_pShaderCom = nullptr;
	class CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float3		m_vOffset;
	_float3		m_vSize;
	class CGameObject*	m_pTarget = nullptr;
	_float4		m_vColor;
			
	_float		m_fRatio = 1.f;//uv와 계산하여 비율을 처리함.
	/*
		픽셀 쉐이더는 uv의 값을 보간하여 사용한다고 했는데,
		렉트 버퍼의 왼쪽의 U는 0, 오른쪽은 1.
		이걸 활용하여 체력바의 비율을 결정할 수 있음.
	*/

	_bool		m_bRender = true;

public:
	//Add ProtoType용
	static CHpBar*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

