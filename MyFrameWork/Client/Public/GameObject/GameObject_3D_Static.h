#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
class CCollider;
END

// 기본 3D 오브젝트 출력
BEGIN(Client)
class CGameObject_3D_Static :
	public CGameObject_Base
{
protected:
	explicit CGameObject_3D_Static(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Static(const CGameObject_3D_Static& rhs);
	virtual ~CGameObject_3D_Static() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	// 충돌 함수
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist);


public:
	CModel*		Get_ComModel() const { return mComModel; }
	CCollider*	Get_ComCollider() const { return mComCollider; }
	const MODEL_STATIC_DESC& Get_ModelDESC() const { return mModelStatic_Desc; }
	const COLLIDER_DESC& Get_ColliderDESC() const { return mCollider_Desc; }

	HRESULT		Set_LoadModelDESC(const MODEL_STATIC_DESC& desc);
	HRESULT		Set_LoadColliderDESC(const COLLIDER_DESC& desc);

	void		Update_CombinedTransformationMatrix(_fmatrix combine);


protected:
	virtual HRESULT Set_Component()override;

	virtual HRESULT Set_ConstantTable_Model(); // 모델 설정
//	virtual HRESULT Set_ConstantTable_Light(_uint lightid = 0); // 라이팅

protected: // 3D모델 Com / DESC 추가
	CModel*						mComModel = nullptr;
	CCollider*					mComCollider = nullptr;
	CTexture_map*				mComTexture = nullptr;

	MODEL_STATIC_DESC			mModelStatic_Desc;
	COLLIDER_DESC				mCollider_Desc;



public:
	static CGameObject_3D_Static* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Static* Clone(void* pArg);
	virtual void Free() override;
};

END