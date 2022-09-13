#include "..\Public\CFrustum.h"
#include "PipeLine.h"


IMPLEMENT_SINGLETON(CFrustum)

HRESULT CFrustum::Initialize()
{
	m_vOriginPoints[0] = _float4(-1.f, 1.f, 0.f, 1.f);
	m_vOriginPoints[1] = _float4(1.f, 1.f, 0.f, 1.f);
	m_vOriginPoints[2] = _float4(1.f, -1.f, 0.f, 1.f);
	m_vOriginPoints[3] = _float4(-1.f, -1.f, 0.f, 1.f);


	m_vOriginPoints[4] = _float4(-1.f, 1.f, 1.f, 1.f);
	m_vOriginPoints[5] = _float4(1.f, 1.f, 1.f, 1.f);
	m_vOriginPoints[6] = _float4(1.f, -1.f, 1.f, 1.f);
	m_vOriginPoints[7] = _float4(-1.f, -1.f, 1.f, 1.f);


	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix	ViewMatrixInv = XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	_matrix	ProjMatrixInv = XMMatrixInverse(nullptr, pPipeLine->
		Get_Transform(CPipeLine::D3DTS_PROJ));

	_vector vPoints[8];

	for (_uint i = 0; i < 8; ++i)
	{
		vPoints[i] = XMVector3TransformCoord(XMLoadFloat4(&m_vOriginPoints[i]), ProjMatrixInv);
		vPoints[i] = XMVector3TransformCoord(vPoints[i], ViewMatrixInv);
	}

	XMStoreFloat4(&m_vWorldPlane[0], XMPlaneFromPoints(vPoints[1], vPoints[5], vPoints[6]));
	XMStoreFloat4(&m_vWorldPlane[1], XMPlaneFromPoints(vPoints[4], vPoints[0], vPoints[3]));
	XMStoreFloat4(&m_vWorldPlane[2], XMPlaneFromPoints(vPoints[4], vPoints[5], vPoints[1]));
	XMStoreFloat4(&m_vWorldPlane[3], XMPlaneFromPoints(vPoints[3], vPoints[2], vPoints[6]));
	XMStoreFloat4(&m_vWorldPlane[4], XMPlaneFromPoints(vPoints[5], vPoints[4], vPoints[7]));
	XMStoreFloat4(&m_vWorldPlane[5], XMPlaneFromPoints(vPoints[0], vPoints[1], vPoints[2]));

	RELEASE_INSTANCE(CPipeLine);
		
	return S_OK;
}

void CFrustum::Tick()
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix	ViewMatrixInv = XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	_matrix	ProjMatrixInv = XMMatrixInverse(nullptr, pPipeLine->
		Get_Transform(CPipeLine::D3DTS_PROJ));

	_vector vPoints[8];

	for (_uint i = 0; i < 8; ++i)
	{
		vPoints[i] = XMVector4Transform(XMLoadFloat4(&m_vOriginPoints[i]), ProjMatrixInv);
		vPoints[i] = XMVector4Transform(vPoints[i], ViewMatrixInv);
	}

	XMStoreFloat4(&m_vWorldPlane[0], XMPlaneFromPoints(vPoints[1], vPoints[5], vPoints[6]));
	XMStoreFloat4(&m_vWorldPlane[1], XMPlaneFromPoints(vPoints[4], vPoints[0], vPoints[3]));
	XMStoreFloat4(&m_vWorldPlane[2], XMPlaneFromPoints(vPoints[4], vPoints[5], vPoints[1]));
	XMStoreFloat4(&m_vWorldPlane[3], XMPlaneFromPoints(vPoints[3], vPoints[2], vPoints[6]));
	XMStoreFloat4(&m_vWorldPlane[4], XMPlaneFromPoints(vPoints[5], vPoints[4], vPoints[7]));
	XMStoreFloat4(&m_vWorldPlane[5], XMPlaneFromPoints(vPoints[0], vPoints[1], vPoints[2]));

	RELEASE_INSTANCE(CPipeLine);
}

_bool CFrustum::IsIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange)
{
	for (_uint i = 0; i < 6; i++)
	{
		if (fRange < XMVectorGetX(
			XMPlaneDotCoord(XMLoadFloat4(&m_vWorldPlane[i]), vWorldPoint)
		))
		{
			return false;
		}
	}
	return true;
}
