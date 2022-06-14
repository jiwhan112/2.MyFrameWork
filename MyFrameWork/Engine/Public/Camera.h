#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3		vEye, vAt, vAxisY;
		_float		fFovy, fAspect, fNear, fFar;
		CTransform::TRANSFORMDESC TransformDesc;
	}CAMERADESC;

protected:
	explicit CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

protected:

	virtual HRESULT Set_Component() override;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double fTimeDelta);
	virtual _int LateTick(_double fTimeDelta);
	virtual HRESULT Render();

	void Set_NewCameraPos(_float3 CameraPos, _float3 LookPos);
protected:
	CAMERADESC					mCameraDesc;
	class CPipeLine*			mpPipeLine = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END