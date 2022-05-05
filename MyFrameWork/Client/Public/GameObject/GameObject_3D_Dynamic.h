#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// 애니메이션 3D 오브젝트 출력
BEGIN(Client)

class CGameObject_3D_Dynamic final :
	public CGameObject_Base
{
public :
	enum E_BASEAI
	{
		BASEAI_IDLE,
		BASEAI_MOVE,
		BASEAI_END,

	};
protected:
	explicit CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs);
	virtual ~CGameObject_3D_Dynamic() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	CModel*		Get_ComModel() const { return mComModel; }
	const MODEL_DYNAMIC_DESC& Get_ModelDESC() const { return mModelDesc; }

	HRESULT		Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC& desc);


protected:
	virtual HRESULT Set_Component() override;

	void		GOMOVE(_double timer);

protected: // 3D모델 Com / DESC 추가
	CModel*						mComModel = nullptr;
	CCollider*					mComCollider = nullptr;
	CNavigation*				mComNaviMesh = nullptr;
	MODEL_DYNAMIC_DESC			mModelDesc;


	_float3						mGoalPosition;
	_float3						mStartPosition;
	_float						mTimer;
	_float						mTimeMax;

	E_BASEAI					meAI;

public:
	static CGameObject_3D_Dynamic* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Dynamic* Clone(void* pArg);
	virtual void Free() override;
};

END