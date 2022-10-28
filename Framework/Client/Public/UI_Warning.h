#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CRenderer;
class CCollider;
class CLayer;
END


BEGIN(Client)

class CUI_Warning final : public CGameObject
{
public:
	CUI_Warning(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Warning() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);

private:
	class CUI_Default*		m_pWarning = nullptr;
	class CUI_Default*		m_pWarningDangerIcon = nullptr;//¥¿≥¶«•
	class CUI_Default*		m_pWarningDangerLine = nullptr;//
	class CRenderer*		m_pRenderer = nullptr;
	class  CCollider*		m_pCollider = nullptr;
private:
	_float					m_fAlphaStart = 0.f;
	_float					m_fAlphaCurrent = 0.f;
	_float					m_fAlphaEnd = 0.f;
	_float					m_fAlphaTickForAmount = 0.1f;


	_float					m_fWarningTick = 0.f;
	_float					m_fWarningTime = 0.f;


	_bool					m_bWarning;
public:
	static CUI_Warning* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject * Clone(void * pArg) override;
	virtual void Free() override;

	// CGameObject¿ª(∏¶) ≈Î«ÿ ªÛº”µ 

};

END