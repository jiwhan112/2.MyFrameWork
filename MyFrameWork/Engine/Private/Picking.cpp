#include "Picking.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPicking)
CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, HWND hWnd, void * pArg )
{
	m_hWnd = hWnd;
	m_pDevice = device;
	m_pDeviceContext = context;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}



HRESULT	CPicking::Transform_ToWorldSpace()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ClientToScreen(m_hWnd, &ptMouse);

	D3D11_VIEWPORT ViewPortDesc;
	UINT ViewIndex = 1;
	m_pDeviceContext->RSGetViewports(&ViewIndex, &ViewPortDesc);


	_float4 vMousePos;
	vMousePos.x = ptMouse.x / (ViewPortDesc.Width*0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPortDesc.Height*0.5f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	_float4x4 ProjMatrixInverse;

	ProjMatrixInverse = GetSingle(CGameInstance)->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);
	ProjMatrixInverse = ProjMatrixInverse.Invert();

	_float4::Transform(vMousePos, ProjMatrixInverse, vMousePos);

	mRayWorld.direction = vMousePos - _float3(0.f, 0.f, 0.f);
	mRayWorld.position = _float3(0, 0, 0);

	_float4x4 ViewMatrixInverse;

	ViewMatrixInverse = GetSingle(CGameInstance)->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);
	ViewMatrixInverse = ViewMatrixInverse.Invert();

	_float3::TransformNormal(mRayWorld.direction, ViewMatrixInverse, mRayWorld.direction);
	mRayWorld.direction.Normalize();

	_float3::Transform(mRayWorld.position, ViewMatrixInverse, mRayWorld.position);

	return S_OK;
}

//HRESULT CPicking::Transform_ToWorldSpace(POINT mousePos)
//{
//	D3D11_VIEWPORT ViewPortDesc;
//	m_pDeviceContext->RSGetViewports(0, &ViewPortDesc);
//
//	_float4 vMousePos;
//	vMousePos.x = mousePos.x / (ViewPortDesc.Width*0.5f) - 1.f;
//	vMousePos.y = mousePos.y / -(ViewPortDesc.Height*0.5f) + 1.f;
//	vMousePos.z = 0.f;
//	vMousePos.w = 1.f;
//
//	_float4x4 ProjMatrixInverse;
//	ProjMatrixInverse = GetSingle(CGameInstance)->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);
//	ProjMatrixInverse = ProjMatrixInverse.Invert();
//	_float4::Transform(vMousePos, ProjMatrixInverse, vMousePos);
//
//	memcpy(&m_vRayDir, &(vMousePos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float4));
//	m_vRayPos = _float4(0.f, 0.f, 0.f,1.f);
//
//	_float4x4 ViewMatrixInverse;
//	ViewMatrixInverse = GetSingle(CGameInstance)->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);
//	ViewMatrixInverse = ViewMatrixInverse.Invert();
//
//	_float4::Transform(m_vRayDir, ViewMatrixInverse, m_vRayDir);
//	_float4::Transform(m_vRayPos, ViewMatrixInverse, m_vRayPos);
//
//	m_vRayDir.Normalize();
//
//	m_vRayDir.w = 0.f;
//	m_vRayPos.w = 1.f;
//
//	return S_OK;
//}

HRESULT CPicking::Transform_ToLocalSpace(_fmatrix WorldMatrixInverse)
{
	_float3::TransformNormal(mRayLocal.direction, WorldMatrixInverse, mRayLocal.direction);
	mRayLocal.direction.Normalize();

	_float3::Transform(mRayLocal.position, WorldMatrixInverse, mRayLocal.position);

	return S_OK;
}

_bool CPicking::isPick(_float3* pLocalPoint, _float3 *pOut)
{
	_float fDist;
	if (mRayLocal.Intersects(pLocalPoint[0], pLocalPoint[1], pLocalPoint[2], fDist))
	{
		*pOut = mRayLocal.position + mRayLocal.direction * fDist;
		return true;
	}
	else
	{

		return false;
	}
}

//_bool CPicking::is_On_Rect(const _rect* TargetRect)
//{
//	POINT ptMouse;
//	GetCursorPos(&ptMouse);
//	ScreenToClient(m_hWnd, &ptMouse);
//
//	return PtInRect(TargetRect, ptMouse);
//}


void CPicking::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
