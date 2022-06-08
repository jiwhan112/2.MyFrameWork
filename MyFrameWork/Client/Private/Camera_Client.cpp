#include "stdafx.h"
#include "Camera_Client.h"
#include "GameObject/GameObject_MyTerrain.h"

CCamera_Client::CCamera_Client(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCamera(pDevice, pDeviceContext)
{
	mObjectTypeid = OBJECT_TYPE_CAMERA;
}

CCamera_Client::CCamera_Client(const CCamera_Client & rhs)
	: CCamera(rhs)
	, meCameraMode(rhs.meCameraMode)
	, mTargetObject(nullptr)

{}

HRESULT CCamera_Client::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Client::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	// 직교 투영 행렬 초기화
	FAILED_CHECK(GetSingle(CGameInstance)->SetTransform(CPipeLine::D3DTS_PROJ_ORI, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f)));

	mStartWorlMat = mComTransform->GetWorldFloat4x4();
	meCameraMode = CCamera_Client::CAMERA_MODE_DEFAULT;

	return S_OK;
}

_int CCamera_Client::Tick(_double TimeDelta)
{
	switch (meCameraMode)
	{
	case CCamera_Client::CAMERA_MODE_DEFAULT:
		Update_Default(TimeDelta);
		break;

	case CCamera_Client::CAMERA_MODE_RETURN:
		mComTransform->Set_WorldMat(mStartWorlMat);
		meCameraMode = CAMERA_MODE_DEFAULT;
		break;

	case CCamera_Client::CAMERA_MODE_TARGET:
		if (mTargetObject)
		{
			FAILED_CHECK_NONERETURN(Update_Target_Unit(TimeDelta));
		}
		break;
	case CCamera_Client::CAMERA_MODE_MOVE:
		CameraMoving_Bezior(TimeDelta);
		//CameraMoving(TYPE_Linear, TimeDelta);
		break;
	case CCamera_Client::CAMERA_MODE_TERRAIN:
		if (mTargetObject)
		{
			FAILED_CHECK_NONERETURN(Update_Target_Terrain(TimeDelta));
		}
		break;
	case CCamera_Client::CAMERA_MODE_END:
		meCameraMode = CAMERA_MODE_DEFAULT;
		break;
	}

	// View Proj 행렬세팅
	return __super::Tick(TimeDelta);
}

_int CCamera_Client::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;
}

HRESULT CCamera_Client::Render()
{
	FAILED_CHECK(__super::Render());
	return S_OK;
}

void CCamera_Client::Set_CameraMode(E_CAMERA_MODE e, E_CAMERA_MOVEPOS_STATE eMove,CGameObject * target)
{
	if (target == mTargetObject)
	{
		meCameraMode = e;
	}
	else
	{
		mbTargetSet = false;
		meCameraMode = e;
		Safe_Release(mTargetObject);
		mTargetObject = target;
		Safe_AddRef(mTargetObject);
	}
	if (meCameraMode == CCamera_Client::CAMERA_MODE_TARGET)
		return;
	

	EnterCamera(eMove, 2.0f);
	
}

void CCamera_Client::ReleaseTarget()
{
	Safe_Release(mTargetObject);
	mTargetObject = nullptr;
}

HRESULT CCamera_Client::Update_Default(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		mComTransform->GO_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		mComTransform->GO_Backward(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		mComTransform->GO_Left(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		mComTransform->GO_Right(TimeDelta);
	}

	_long		MouseMove = 0;

	const _float MouseSpeed = 0.3f;
	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	{
		mComTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * MouseSpeed);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		mComTransform->Turn(mComTransform->GetState(CTransform::STATE_RIGHT), TimeDelta * MouseMove *MouseSpeed);
	}

	return S_OK;
}

HRESULT CCamera_Client::Update_Target_Unit(_double TimeDelta)
{
	// 타겟을 돌려가면서 랜더링
	if (mTargetObject == nullptr)
		return E_FAIL;

	CGameInstance*		pGameInstance = GetSingle(CGameInstance);
	if (mbTargetSet == false)
	{
		CTransform* targetTrans = mTargetObject->Get_ComTransform();
		_float3 targetPos = targetTrans->GetState(CTransform::STATE_POSITION);
		mComTransform->LookAt(targetPos);
		mbTargetSet = true;
	}


	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		mComTransform->GO_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		mComTransform->GO_Backward(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		mComTransform->GO_Left(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		mComTransform->GO_Right(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{
		mComTransform->Turn(_float3::Up, TimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{
		mComTransform->Turn(-_float3::Up,TimeDelta);
	}

	return S_OK;
}

const _float3 GameCameraDir = _float3(-0.7, -1.5f, 1);
HRESULT CCamera_Client::Update_Target_Terrain(_double TimeDelta)
{
	if (mTargetObject == nullptr)
		return E_FAIL;

	if (mbTargetSet == false)
	{
		/*	_uint size =  static_cast<CGameObject_MyTerrain*>(mTargetObject)->Get_MapSize();
			size *= 0.3f;

			_float3 Postition = _float3(size, 5, size);

			_float3 Dir = GameCameraDir;

			mComTransform->Set_State(CTransform::STATE_POSITION, Postition);
			mComTransform->LookAtDir(Dir);*/

		_float3 Dir = GameCameraDir;
		mComTransform->LookAtDir(Dir); 
		mbTargetSet = true;
	}


	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		mComTransform->GO_WorldVec(_float3(0, 0, 1), -45, CTransform::ROTTYPE_Y, TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		mComTransform->GO_WorldVec(_float3(0, 0, -1), -45, CTransform::ROTTYPE_Y, TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		mComTransform->GO_Left(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		mComTransform->GO_Right(TimeDelta);
	}

	const _float MouseSpeed = 4.0f;
	long MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_WHEEL);

	if (MouseMove > 0)
	{
		// 범위 제한
		// mTargetObject;

		mComTransform->GO_Straight(TimeDelta * MouseSpeed);
	}

	if (MouseMove < 0)
	{
		mComTransform->GO_Backward(TimeDelta * MouseSpeed);
	}
	return S_OK;
}

HRESULT CCamera_Client::EnterCamera(E_CAMERA_MOVEPOS_STATE mode, _double TimeMax)
{
	// 카메라 시작

	switch (mode)
	{
	case Client::CCamera_Client::CAMERA_MOVEPOS_STATE_D:
		mCurrentMovePosition = mDungeon_CenterPos;
		break;
	case Client::CCamera_Client::CAMERA_MOVEPOS_STATE_W:
		mCurrentMovePosition = mWorld_CenterPos;
		break;
	case Client::CCamera_Client::CAMERA_MOVEPOS_STATE_INTRO:
		break;
	case Client::CCamera_Client::CAMERA_MOVEPOS_STATE_BOSS:
		break;
	case Client::CCamera_Client::CAMERA_MOVEPOS_STATE_END:
		break;
	default:
		return E_FAIL;
	}

	meCameraMode = CCamera_Client::CAMERA_MODE_MOVE;

	// Enter
	/*mCurrentMovePosition = mCurrentListPostition.front();
	mCurrentListPostition.pop_front();*/
	mStartPosition = mComTransform->GetState(CTransform::STATE_POSITION);
	mCurrentBeziorMovePosition = mStartPosition- mCurrentMovePosition;
	

	mCurrentBeziorMovePosition *= 0.5f;
	_float3 UpVec = _float3::Up;
	mCurrentBeziorMovePosition += UpVec * 50;

	mIsMovingCamera = true;
	mTimerMax = TimeMax;
	mCurrentTimer = 0;
}


HRESULT CCamera_Client::CameraMoving_Bezior(_double TimeDelta)
{
	if (mIsMovingCamera == false)
	{
		meCameraMode = CCamera_Client::CAMERA_MODE_TERRAIN;
		return S_OK;
	}

	mCurrentTimer += TimeDelta;

	// 현재 위치 -> 중심점
	// 중심점 -> 목표 위치
	_float3 lerpPos1 = GetSingle(CGameInstance)->Easing3(TYPE_Linear, mStartPosition, mCurrentBeziorMovePosition, mCurrentTimer, mTimerMax);
	_float3 lerpPos2 = GetSingle(CGameInstance)->Easing3(TYPE_Linear, mCurrentBeziorMovePosition, mCurrentMovePosition, mCurrentTimer, mTimerMax);
	_float3 CurrentPosition = GetSingle(CGameInstance)->Easing3(TYPE_Linear, lerpPos1, lerpPos2, mCurrentTimer, mTimerMax);
	mComTransform->Set_State(CTransform::STATE_POSITION, CurrentPosition);

	if (mCurrentTimer > mTimerMax)
	{
		mIsMovingCamera = false;		
	}
	return S_OK;
}

HRESULT CCamera_Client::CameraMoving(EasingTypeID EasingID, _double TimeDelta)
{
	// 카메라 움직임 테스트
	if (mIsMovingCamera == false)
	{
		meCameraMode = CCamera_Client::CAMERA_MODE_TERRAIN;
		return S_OK;
	}

	mCurrentTimer += TimeDelta;

	_float3 CurrentPosition = GetSingle(CGameInstance)->Easing3(EasingID, mStartPosition, mCurrentMovePosition, mCurrentTimer, mTimerMax);
	mComTransform->Set_State(CTransform::STATE_POSITION, CurrentPosition);

	if (mCurrentTimer > mTimerMax)
	{
		mStartPosition = CurrentPosition;
		if (mCurrentListPostition.empty())
		{
			mIsMovingCamera = false;
		}

		else
		{
			Set_NextMove(0.5f);
		}
	}

	return S_OK;
}

HRESULT CCamera_Client::Update_Target_D(_double TimeDelta)
{
	return S_OK;
}

HRESULT CCamera_Client::Update_Target_W(_double TimeDelta)
{
	return S_OK;
}


void CCamera_Client::Set_ListPosition(list<_float3> VecLoadPos)
{
	mCurrentListPostition.clear();
	for (auto pos: VecLoadPos)
	{
		// 위치 복사
		mCurrentListPostition.push_back(pos);
	}
}

void CCamera_Client::Set_NextMove(_double timeMax)
{
	mCurrentMovePosition = mCurrentListPostition.front();
	mCurrentListPostition.pop_front();
	mCurrentTimer = 0;

}

CCamera_Client * CCamera_Client::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCamera_Client*	pInstance = NEW CCamera_Client(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Client");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera_Client * CCamera_Client::Clone(void * pArg)
{
	CCamera_Client*	pInstance = NEW CCamera_Client(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Client");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Client::Free()
{
	__super::Free();
	Safe_Release(mTargetObject);
}