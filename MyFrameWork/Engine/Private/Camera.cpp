#include "..\Public\Camera.h"
#include "PipeLine.h"
#include "GameInstance.h"

_uint CALLBACK ShakeThread(void* _Prameter)
{

	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;

	CCamera* pCamera = (CCamera*)tThreadArg.pArg;
	pCamera->ShakeFunction(tThreadArg.IsClientQuit, tThreadArg.CriSec);
	return 0;
}


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

	mComTransform->Set_State(CTransform::STATE_UP, vUp);
	mComTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	mComTransform->Set_State(CTransform::STATE_LOOK, vLook);
	// float3 형을 4의 vecotr로 바꿔서 넣어준다. &는 주소를 넘겨야하기 때문에
	mComTransform->Set_State(CTransform::STATE_POSITION, mCameraDesc.vEye.ToVec4(1));
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

void CCamera::Set_NewCameraPos(_float3 CameraPos , _float3 LookPos)
{

	// 카메라 위치에따른 월드행렬 구하기
	_float3		vLook = LookPos - CameraPos;
	vLook.Normalize();

	_float3		vRight = _float3::Up.Cross(vLook);
	vRight.Normalize();


	_float3		vUp = vLook.Cross(vRight);
	vUp.Normalize();

	mComTransform->Set_State(CTransform::STATE_UP, vUp);
	mComTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	mComTransform->Set_State(CTransform::STATE_LOOK, vLook);
	// float3 형을 4의 vecotr로 바꿔서 넣어준다. &는 주소를 넘겨야하기 때문에
	mComTransform->Set_State(CTransform::STATE_POSITION, CameraPos.ToVec4(1));
}


HRESULT CCamera::Camera_Shaking(_float fDeltaTime, _float fTotalEftFrame)
{
	if (!mIsShaking)
	{

		mIsShaking = true;

		mTotalTime = fTotalEftFrame;
		mCurrentTime = fDeltaTime;
		FAILED_CHECK((GetSingle(CGameInstance)->PlayThread(ShakeThread, this)));
	}
	return S_OK;
}

void CCamera::ShakeFunction(_bool * IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	_float3 vShakeDir;
	DWORD dwSleepTime = DWORD(mCurrentTime * 1000);
	_uint TotalCnt = (_uint)((mTotalTime / mCurrentTime) * 0.25f);

	_float3 TotalMovement = _float3(0, 0, 0);
	TotalCnt *= 2;

	for (_uint i = 0; i < TotalCnt; i++)
	{
		EnterCriticalSection(_CriSec);
		switch (i % 2)
		{
		case 0:
			vShakeDir = _float3(0, 0, 0);
			vShakeDir += mComTransform->GetState(CTransform::STATE_RIGHT)* ((rand() % 100) - 50.f);
			vShakeDir += mComTransform->GetState(CTransform::STATE_UP)* ((rand() % 100) - 50.f);
			mComTransform->MovetoDir(vShakeDir, mCurrentTime);
			TotalMovement += vShakeDir;
			break;

		case 1:
			mComTransform->MovetoDir(vShakeDir*-1.f, mCurrentTime);
			TotalMovement += vShakeDir * -1.f;
			break;

		default:
			break;
		}

		LeaveCriticalSection(_CriSec);

		Sleep(dwSleepTime);
	}



	EnterCriticalSection(_CriSec);
	if (TotalMovement != _float3(0, 0, 0))
		mComTransform->MovetoDir(TotalMovement, mCurrentTime);

	mIsShaking = false;
	LeaveCriticalSection(_CriSec);
}


void CCamera::Free()
{
	__super::Free();
	Safe_Release(mpPipeLine);
}