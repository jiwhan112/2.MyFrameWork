#include "Frstum.h"
#include "PipeLine.h"
#include "Transform.h"


IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{

}

HRESULT CFrustum::Initialize()
{
	// 투영점 8개 정의
	// 시계방향으로 그림

	m_vPoint[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _float3(1.f, 1.f, 0.f);
	m_vPoint[2] = _float3(1.f, -1.f, 0.f);
	m_vPoint[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _float3(1.f, 1.f, 1.f);
	m_vPoint[6] = _float3(1.f, -1.f, 1.f);
	m_vPoint[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Tick()
{
	CPipeLine*		pPipeLine = GetSingle(CPipeLine);

	// 8개의 점을 월드 공간으로 변환
	// * 투영의 역 * 뷰행렬의 역
	_float4x4		ProjMatrixInv, ViewMatrixInv;

	ProjMatrixInv = pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);

	ViewMatrixInv = pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);


	// 8개의 점을 변환한다.
	for (_uint i = 0; i < 8; ++i)
	{
		_float3			vPoint = m_vPoint[i];

		// 투영
		vPoint = _float3::Transform(vPoint, ProjMatrixInv);

		// 뷰
		vPoint = _float3::Transform(vPoint, ViewMatrixInv);

		m_vPointInWorld[i] = vPoint;
	//	XMStoreFloat3(&m_vPointInWorld[i], vPoint);
	}

	// 평면 저장
	Compute_Plane(m_vPointInWorld, m_PlaneInWorld);

}

HRESULT CFrustum::Transform_ToLocalSpace(CTransform * pTransform)
{
	// 월드 행렬의 역행렬을 곱해 같은 스페이스로 맞춰준다.
	for (_uint i = 0; i < 8; ++i)
	{
		
		_vector			vPoint;
		vPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPointInWorld[i]), pTransform->GetInvert());
		XMStoreFloat3(&m_vPointInLocal[i], vPoint);
	}
	// 평면을 생성한다.
	Compute_Plane(m_vPointInLocal, m_PlaneInLocal);
	return S_OK;
}

_bool CFrustum::IsIn_WorldSpace(_fvector vPoint, _float fRange)
{
	// 월드에서의 판단

	/*
	a b c d
	x y z 1
	ax + by + cz + d
	*/

	for (_uint i = 0; i < 6; ++i)
	{
		// 월드 평면과 비교
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInWorld[i]), vPoint)) > fRange)
			return false;
	}

	return true;
}

_bool CFrustum::IsIn_LocalSpace(_fvector vPoint, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		// 로컬 평면과 비교
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInLocal[i]), vPoint)) > fRange)
			return false;
	}


	return true;
}
void CFrustum::Compute_Plane(const _float3 * pPoints, _float4 * pPlanes)
{ 
	// 각 평면을 pPlanes에 대입.
	// +x
	XMStoreFloat4(&pPlanes[1], XMPlaneFromPoints(XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[6])));
	// -x 
	XMStoreFloat4(&pPlanes[1], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[3])));
	// +y
	XMStoreFloat4(&pPlanes[2], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[1])));
	// -y 
	XMStoreFloat4(&pPlanes[3], XMPlaneFromPoints(XMLoadFloat3(&pPoints[6]), XMLoadFloat3(&pPoints[7]), XMLoadFloat3(&pPoints[3])));
	// +z 
	XMStoreFloat4(&pPlanes[4], XMPlaneFromPoints(XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[7])));
	// -z
	XMStoreFloat4(&pPlanes[5], XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[2])));

}

void CFrustum::Free()
{
}
