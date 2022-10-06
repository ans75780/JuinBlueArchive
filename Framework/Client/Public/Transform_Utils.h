#pragma once
#include "GameInstance.h"

namespace Engine
{
	class CTransform;
}


BEGIN(Client)

class CTransform_Utils
{
private:
	CTransform_Utils() {};
	~CTransform_Utils() {};

public:
	static	void	Set_Pos(class CTransform* pTransform, const _float4& fPos);
	static	void	Add_Pos(class CTransform* pTransform, const _float4& fPos);
	static	_float	Get_Range(class CTransform* pDest, class CTransform* pSour);


};

END
