#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CMeshContainer;
class CCollider;
class CAnimation;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect : public CGameObject
{
public:
	typedef struct tagEffectDesc
	{
		_float		fDuration;
		_bool		bLoop;
		_float		fStart_Delay;
		_float		fStart_Speed;

		_float		fStart_Size;
		_float		fEnd_Size;


	}FX_DESC;
protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& Prototype);
	virtual ~CEffect() = default;

public:

	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Reset();

protected:
	virtual HRESULT			SetUp_Component();
	virtual HRESULT			SetUp_ShaderResource();

protected:
	class CTexture*			m_pTexture = nullptr;
	class CRenderer*		m_pRenderer = nullptr;
	class CShader*			m_pShader = nullptr;
	class CVIBuffer_Rect*	m_pVIBuffer = nullptr;

protected:
	FX_DESC					m_FXDesc;
	_float					m_fLifeTime = 0.f;

	vector<FX_DESC>			m_vecFX;//이걸로 효과 조정할꺼임.


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;


//이거안써서 박아둔거임.
private:
	OBJ_DESC				m_desc;

};


END

