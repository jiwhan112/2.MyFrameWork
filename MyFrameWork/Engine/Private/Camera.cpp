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

	// super�� ȣ���ؾ� Transform�� �����ȴ�.
	FAILED_CHECK(__super::NativeConstruct(&mCameraDesc.TransformDesc));

	// ī�޶� ��ġ������ ������� ���ϱ�
	_vector		vLook = XMLoadFloat3(&mCameraDesc.vAt) - XMLoadFloat3(&mCameraDesc.vEye);
	vLook = XMVector3Normalize(vLook);

	_vector		vRight = XMVector3Cross(XMLoadFloat3(&mCameraDesc.vAxisY), vLook);
	vRight = XMVector3Normalize(vRight);

	_vector		vUp = XMVector3Cross(vLook, vRight);
	vUp = XMVector3Normalize(vUp);

	mComTransform->SetState(CTransform::STATE_UP, vUp);
	mComTransform->SetState(CTransform::STATE_RIGHT, vRight);
	mComTransform->SetState(CTransform::STATE_LOOK, vLook);
	// float3 ���� 4�� vecotr�� �ٲ㼭 �־��ش�. &�� �ּҸ� �Ѱܾ��ϱ� ������
	mComTransform->SetState(CTransform::STATE_POSITION, XMLoadFloat4(&_float4(mCameraDesc.vEye, 1.f)));

	return S_OK;
}

_int CCamera::Tick(_double fTimeDelta)
{
	if (mpPipeLine == nullptr)
		return UPDATEERROR;

	// View ��� Proj ��� ����
	FAILED_CHECK(mpPipeLine->SetTransform(
		CPipeLine::D3DTS_VIEW, XMMatrixInverse(nullptr, mComTransform->GetWorldMatrix())));

	// ���ڴ� DX9�� ����
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