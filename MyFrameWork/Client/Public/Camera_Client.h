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
		CAMERA_MODE_MAP,
		CAMERA_MODE_END,
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
	void ReleaseTarget()
	{
		Safe_Release(mTargetObject);
		mTargetObject = nullptr;
	}

private:
	HRESULT Update_Default(_double TimeDelta);
	HRESULT Update_Target_Unit(_double TimeDelta);
	HRESULT Update_Target_Terrain(_double TimeDelta);
	HRESULT Update_Map(_double TimeDelta);


private:
	E_CAMERA_MODE meCameraMode;

	_float4x4 mStartWorlMat;
	CGameObject* mTargetObject = nullptr;



public:
	static CCamera_Client* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CCamera_Client* Clone(void* pArg);
	virtual void Free() override;
};

END