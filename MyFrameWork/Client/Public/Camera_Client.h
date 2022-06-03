#pragma once

#include "Camera.h"

BEGIN(Engine)
class CGameObject;
END
BEGIN(Client)
class CCamera_Client final :
	public CCamera
{
public:
	enum E_CAMERA_MODE
	{
		CAMERA_MODE_DEFAULT,
		CAMERA_MODE_RETURN,
		CAMERA_MODE_TARGET,
		CAMERA_MODE_TERRAIN,
		CAMERA_MODE_MOVE,
		CAMERA_MODE_END,
	};

	enum E_CAMERA_MOVEPOS_STATE
	{
		CAMERA_MOVEPOS_STATE_D,
		CAMERA_MOVEPOS_STATE_W,
		CAMERA_MOVEPOS_STATE_INTRO,
		CAMERA_MOVEPOS_STATE_BOSS,
		CAMERA_MOVEPOS_STATE_END
	};


protected:
	explicit CCamera_Client(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera_Client(const CCamera_Client& rhs);
	virtual ~CCamera_Client() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	void Set_CameraMode(E_CAMERA_MODE e, E_CAMERA_MOVEPOS_STATE eMove = CAMERA_MOVEPOS_STATE_END,CGameObject* target = nullptr);
	void ReleaseTarget();

	void Set_CamerDIr(_float3 Dir)
	{
		mComTransform->LookAtDir(Dir);
	}

	HRESULT Set_GameCameraMode();
	HRESULT EnterCamera(E_CAMERA_MOVEPOS_STATE mode, _double TimeMax);

private:
	HRESULT Update_Default(_double TimeDelta);
	HRESULT Update_Target_Unit(_double TimeDelta);
	HRESULT Update_Target_Terrain(_double TimeDelta);

	// 카메라 연출
	HRESULT CameraMoving(EasingTypeID EasingID, _double TimeDelta);
	HRESULT CameraMoving_Bezior(_double TimeDelta);

	HRESULT Update_Target_D(_double TimeDelta);
	HRESULT Update_Target_W(_double TimeDelta);
	
	void Set_ListPosition(list<_float3> VecLoadPos);
	void Set_NextMove(_double timeMax);


private:
	// 지속 상태
	E_CAMERA_MODE				meCameraMode;

	_float4x4					mStartWorlMat;
	CGameObject*				mTargetObject = nullptr;
	bool						mbTargetSet = false;

	_bool						mIsMovingCamera = false;
//	_bool						mIsMovingCamera_Delta = false;

	list<_float3>				mCurrentListPostition;

	_float3						mStartPosition;
	_float3						mCurrentMovePosition;
	_float3						mCurrentBeziorMovePosition;
	_double						mCurrentTimer;
	_double						mTimerMax;

public:
	// 저장된 위치
	list<_float3>				mListMove_Dungeon;
	list<_float3>				mListMove_World;

	// 각 던전의 중심위치

	const _float3				mDungeon_CenterPos = _float3(19.64f,8.74f,11.57f);
	const _float3				mWorld_CenterPos = _float3(60.35f,23.6f,5.9f);


public:
	static CCamera_Client* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CCamera_Client* Clone(void* pArg);
	virtual void Free() override;
};

END