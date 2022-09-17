#include "..\Public\PipeLine.h"
#include "GameInstance.h"
IMPLEMENT_SINGLETON(CPipeLine);

CPipeLine::CPipeLine()
{
}



RAYDESC CPipeLine::Get_Ray()
{
	RAYDESC ray;

	ZeroMemory(&ray, sizeof(RAYDESC));

	_float2	fViewPort;
	POINT	vMousePt;
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	
	fViewPort = pGameInstance->Get_ViewPort();
	vMousePt = pGameInstance->Get_MousePos();
	

	_float4	MousePt;

	MousePt.x = (((2.0f * (_float)vMousePt.x) / fViewPort.x - 1.f) / m_TransformState[D3DTS_PROJ]._11);
	MousePt.y = (((-2.0f * (_float)vMousePt.y) / fViewPort.y + 1.f) / m_TransformState[D3DTS_PROJ]._22);
	MousePt.z = 1.f;
	MousePt.w = 1.f;

	ray.vRayDir = XMLoadFloat4(&MousePt);
	

	_matrix matViewInv;
	matViewInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW]));
	ray.vRayOrigin = XMVector3TransformCoord(ray.vRayOrigin, matViewInv);
	ray.vRayDir =  XMVector3Normalize(XMVector3TransformNormal(ray.vRayDir, matViewInv));

	RELEASE_INSTANCE(CGameInstance);

	return ray;
}

void CPipeLine::Tick()
{
	_matrix		ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW]));
	XMStoreFloat4(&m_vCamPosition, ViewMatrixInv.r[3]);

	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_VIEW], XMMatrixTranspose(XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW])));
	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_PROJ], XMMatrixTranspose(XMLoadFloat4x4(&m_TransformState[D3DTS_PROJ])));
}

void CPipeLine::Free()
{
}
