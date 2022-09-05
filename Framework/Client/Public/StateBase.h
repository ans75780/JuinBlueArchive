#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Base.h"
namespace Engine
{
	class CAnimation;
	class CModel;
}

BEGIN(Client)

class CStateBase : public CBase
{
protected:
		CStateBase(class CStudent* pStudent);
virtual	~CStateBase() = default;

public:
	virtual HRESULT		Initialize() = 0;
	virtual void		Enter() = 0;
	virtual _bool		Loop(_float fTimeDelta) = 0;
	virtual CStateBase*	Exit() = 0;
public:
	class CAnimation*	Get_Animation() { return m_pAnimation; }
protected:
	class CAnimation*	m_pAnimation = nullptr;
	class CModel*		m_pModel = nullptr;
	class CStudent*		m_pStudent = nullptr;
public:
	virtual void			Free();
};
END