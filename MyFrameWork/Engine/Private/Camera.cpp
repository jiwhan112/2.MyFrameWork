#include "..\Public\Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
	, mpPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(mpPipeLine);
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, mpPipeLine(rhs.mpPipeLine)
{
	Safe_AddRef(mpPipeLine);
}

HRESULT CCamera::Set_Component()
{
	return S_OK;
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&mCameraDesc, pArg, sizeof(CAMERADESC));

	// super를 호출해야 Transform이 생성된다.
	FAILED_CHECK(__super::NativeConstruct(&mCameraDesc.TransformDesc));

	// 카메라 위치에따른 월드행렬 구하기
	_vector		vLook = XMLoadFloat3(&mCameraDesc.vAt) - XMLoadFloat3(&mCameraDesc.vEye);
	vLook = XMVector3Normalize(vLook);

	_vector		vRight = XMVector3Cross(XMLoadFloat3(&mCameraDesc.vAxisY), vLook);
	vRight = XMVector3Normalize(vRight);

	_vector		vUp = XMVector3Cross(vLook, vRight);
	vUp = XMVector3Normalize(vUp);

	mComTransform->SetState(CTransform::STATE_UP, vUp);
	mComTransform->SetState(CTransform::STATE_RIGHT, vRight);
	mComTransform->SetState(CTransform::STATE_LOOK, vLook);
	// float3 형을 4의 vecotr로 바꿔서 넣어준다. &는 주소를 넘겨야하기 때문에
	mComTransform->SetState(CTransform::STATE_POSITION, XMLoadFloat4(&_float4(mCameraDesc.vEye, 1.f)));

	return S_OK;
}

_int CCamera::Tick(_double fTimeDelta)
{
	if (mpPipeLine == nullptr)
		return UPDATEERROR;

	// View 행렬 Proj 행렬 세팅
	FAILED_CHECK(mpPipeLine->SetTransform(
		CPipeLine::D3DTS_VIEW, XMMatrixInverse(nullptr, mComTransform->GetWorldMatrix())));

	// 인자는 DX9과 같음
	_matrix	ProjMatrix = XMMatrixPerspectiveFovLH(
		mCameraDesc.fFovy, mCameraDesc.fAspect, mCameraDesc.fNear, mCameraDesc.fFar);
	FAILED_CHECK(mpPipeLine->SetTransform(CPipeLine::D3DTS_PROJ, ProjMatrix));

	return UPDATENONE;
}

_int CCamera::LateTick(_double fTimeDelta)
{
	return UPDATENONE;
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
	Safe_Release(mpPipeLine);
}