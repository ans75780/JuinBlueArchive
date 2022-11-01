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


//���� HP�ٴ� ���������̶� ���� ���鲨��.
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
			
	_float		m_fRatio = 1.f;//uv�� ����Ͽ� ������ ó����.
	/*
		�ȼ� ���̴��� uv�� ���� �����Ͽ� ����Ѵٰ� �ߴµ�,
		��Ʈ ������ ������ U�� 0, �������� 1.
		�̰� Ȱ���Ͽ� ü�¹��� ������ ������ �� ����.
	*/

	_bool		m_bRender = true;

public:
	//Add ProtoType��
	static CHpBar*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

