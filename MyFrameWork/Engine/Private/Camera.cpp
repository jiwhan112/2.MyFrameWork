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

	mComTransform->Set_State(CTransform::STATE_UP, vUp);
	mComTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	mComTransform->Set_State(CTransform::STATE_LOOK, vLook);
	// float3 ���� 4�� vecotr�� �ٲ㼭 �־��ش�. &�� �ּҸ� �Ѱܾ��ϱ� ������
	mComTransform->Set_State(CTransform::STATE_POSITION, mCameraDesc.vEye.ToVec4(1));
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

void CCamera::Set_NewCameraPos(_float3 CameraPos , _float3 LookPos)
{

	// ī�޶� ��ġ������ ������� ���ϱ�
	_float3		vLook = LookPos - CameraPos;
	vLook.Normalize();

	_float3		vRight = _float3::Up.Cross(vLook);
	vRight.Normalize();


	_float3		vUp = vLook.Cross(vRight);
	vUp.Normalize();

	mComTransform->Set_State(CTransform::STATE_UP, vUp);
	mComTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	mComTransform->Set_State(CTransform::STATE_LOOK, vLook);
	// float3 ���� 4�� vecotr�� �ٲ㼭 �־��ش�. &�� �ּҸ� �Ѱܾ��ϱ� ������
	mComTransform->Set_State(CTransform::STATE_POSITION, CameraPos.ToVec4(1));
}

void CCamera::Free()
{
	__super::Free();
	Safe_Release(mpPipeLine);
}