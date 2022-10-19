#include "..\Public\Collider.h"
#include "DebugDraw.h"
#include "PipeLine.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}

HRESULT CCollider::Initialize_Prototype(TYPE eType)
{
	m_eType = eType;

	


#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif // _DEBUG

	return S_OK;
}

HRESULT CCollider::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB_Original = new BoundingBox(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f));
		m_pAABB = new BoundingBox(*m_pAABB_Original);
		break;
	case TYPE_OBB:
		m_pOBB_Original = new BoundingOrientedBox(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f), m_ColliderDesc.vRotation);
		m_pOBB = new BoundingOrientedBox(*m_pOBB_Original);
		break;
	case TYPE_SPHERE:
		m_pSphere_Original = new BoundingSphere(m_ColliderDesc.vTranslation, m_ColliderDesc.vScale.x * 0.5f);
		m_pSphere = new BoundingSphere(*m_pSphere_Original);
		break;
	}

	return S_OK;
}

void CCollider::Update(_fmatrix TransformMatrix)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB_Original->Transform(*m_pAABB, Remove_Rotation(TransformMatrix));
		break;
	case TYPE_OBB:
		m_pOBB_Original->Transform(*m_pOBB, TransformMatrix);
		break;
	case TYPE_SPHERE:
		m_pSphere_Original->Transform(*m_pSphere, TransformMatrix);
		break;
	}

	
}

_bool CCollider::Collision_AABB(CCollider * pTargetCollider)
{
	if (TYPE_AABB != m_eType ||
		TYPE_AABB != pTargetCollider->m_eType)
		return false;

	_float3		vSourMin, vDestMin;
	_float3		vSourMax, vDestMax;

	vSourMin = _float3(m_pAABB->Center.x - m_pAABB->Extents.x, 
		m_pAABB->Center.y - m_pAABB->Extents.y, 
		m_pAABB->Center.z - m_pAABB->Extents.z);

	vDestMin = _float3(pTargetCollider->m_pAABB->Center.x - pTargetCollider->m_pAABB->Extents.x,
		pTargetCollider->m_pAABB->Center.y - pTargetCollider->m_pAABB->Extents.y,
		pTargetCollider->m_pAABB->Center.z - pTargetCollider->m_pAABB->Extents.z);

	vSourMax = _float3(m_pAABB->Center.x + m_pAABB->Extents.x,
		m_pAABB->Center.y + m_pAABB->Extents.y,
		m_pAABB->Center.z + m_pAABB->Extents.z);

	vDestMax = _float3(pTargetCollider->m_pAABB->Center.x + pTargetCollider->m_pAABB->Extents.x,
		pTargetCollider->m_pAABB->Center.y + pTargetCollider->m_pAABB->Extents.y,
		pTargetCollider->m_pAABB->Center.z + pTargetCollider->m_pAABB->Extents.z);

	m_isColl = false;

	/* ³Êºñ·Î °ãÃÆ´ÂÁö?! */
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return m_isColl;


	/* ³ôÀÌ·Î °ãÃÆ´ÂÁö?! */
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return m_isColl;


	/* ±íÀÌ·Î °ãÃÆ´ÂÁö?! */
	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return m_isColl;
	
	m_isColl = true;	

	return m_isColl;
}

_bool CCollider::Collision_AABB(RAYDESC & ray, _float & distance)
{
	return m_pAABB->Intersects(ray.vRayOrigin, ray.vRayDir, distance);
}

_bool CCollider::Collision_OBB(CCollider * pTargetCollider)
{
	OBBDESC			OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc();
	OBBDesc[1] = pTargetCollider->Compute_OBBDesc();

	m_isColl = false;

	_float		fDistance[3] = { 0 };


	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return m_isColl;
		}
	}

	m_isColl = true;

	return m_isColl;
}

_bool CCollider::Collision(CCollider * pTargetCollider)
{
	if (TYPE_AABB == m_eType)
	{
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isColl = m_pAABB->Intersects(*pTargetCollider->m_pAABB);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isColl = m_pAABB->Intersects(*pTargetCollider->m_pOBB);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isColl = m_pAABB->Intersects(*pTargetCollider->m_pSphere);
	}

	if (TYPE_OBB == m_eType)
	{
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isColl = m_pOBB->Intersects(*pTargetCollider->m_pAABB);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isColl = m_pOBB->Intersects(*pTargetCollider->m_pOBB);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isColl = m_pOBB->Intersects(*pTargetCollider->m_pSphere);
	}

	if (TYPE_SPHERE == m_eType)
	{
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isColl = m_pSphere->Intersects(*pTargetCollider->m_pAABB);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isColl = m_pSphere->Intersects(*pTargetCollider->m_pOBB);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isColl = m_pSphere->Intersects(*pTargetCollider->m_pSphere);
	}	

	return m_isColl;
}

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix			Transform = TransformMatrix;
	
	Transform.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	Transform.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	Transform.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	return Transform;
}
#ifdef _DEBUG
HRESULT CCollider::Render()
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);	

	m_pEffect->SetView(pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	m_pEffect->Apply(m_pContext);

	//_vector		vColor = m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	m_pBatch->Begin();

	switch (m_eType)
	{
	case TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABB, XMLoadFloat4(&m_vColor));
		break;
	case TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBB, XMLoadFloat4(&m_vColor));
		break;
	case TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&m_vColor));
		break;
	}

	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

CCollider::OBBDESC CCollider::Compute_OBBDesc()
{
	OBBDESC		OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	_float3		vPoints[8];
	m_pOBB->GetCorners(vPoints);

	XMStoreFloat3(&OBBDesc.vCenter, (XMLoadFloat3(&vPoints[0]) + XMLoadFloat3(&vPoints[6])) * 0.5f);
	
	XMStoreFloat3(&OBBDesc.vCenterAxis[0], 
		(XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[5])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1],
		(XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[7])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2],
		(XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[0])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));

	XMStoreFloat3(&OBBDesc.vAlignAxis[0],
		XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[0])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1],
		XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2],
		XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[2])));

	return OBBDesc;
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType)
{
	CCollider*		pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*		pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCollider::Free()
{
	__super::Free();
#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}
#endif // _DEBUG


	Safe_Delete(m_pAABB_Original);
	Safe_Delete(m_pOBB_Original);
	Safe_Delete(m_pSphere_Original);

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);
#ifdef _DEBUG
	Safe_Release(m_pInputLayout);
#endif // _DEBUG
}
