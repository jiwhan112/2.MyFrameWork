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
		CAMERA_MODE_GAME_D,
		CAMERA_MODE_GAME_W,
		CAMERA_MODE_END,
	};

	enum E_CAMERA_MOVE_STATE
	{
		CAMERA_UPDATE_STATE_NONE,
		CAMERA_UPDATE_STATE_ENTER,
		CAMERA_UPDATE_STATE_STAY,
		CAMERA_UPDATE_STATE_EXIT,
		CAMERA_UPDATE_STATE_END,

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

	void Set_CameraMode(E_CAMERA_MODE e, CGameObject* target = nullptr);
	void ReleaseTarget();

	void Set_CamerDIr(_float3 Dir)
	{
		mComTransform->LookAtDir(Dir);
	}

private:
	HRESULT Update_Default(_double TimeDelta);
	HRESULT Update_Target_Unit(_double TimeDelta);
	HRESULT Update_Target_Terrain(_double TimeDelta);
	HRESULT Update_Target_Dungeon(_double TimeDelta);
	HRESULT Update_Target_World(_double TimeDelta);

private:
	// 지속 상태
	E_CAMERA_MODE meCameraMode;

	// 움직일 때 연출용
	E_CAMERA_MOVE_STATE meCameraMoveState;

	_float4x4 mStartWorlMat;
	CGameObject* mTargetObject = nullptr;
	bool		mbTargetSet = false;

public:
	static CCamera_Client* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CCamera_Client* Clone(void* pArg);
	virtual void Free() override;
};

END