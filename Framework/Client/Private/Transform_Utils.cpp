#include "stdafx.h"
#include "Transform_Utils.h"
#include "GameInstance.h"
#include "Transform.h"

BEGIN(Client)

void CTransform_Utils::Set_Pos(CTransform * pTransform, const _float4 & fPos)
{
	pTransform->Set_State(CTransform::STATE_TRANSLATION,
		XMLoadFloat4(&fPos)
		);
}

void CTransform_Utils::Add_Pos(CTransform * pTransform, const _float4 & fPos)
{
	_vector vOriginPos;

	vOriginPos = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	vOriginPos += XMLoadFloat4(&fPos);

	pTransform->Set_State(CTransform::STATE_TRANSLATION, vOriginPos);
}

END