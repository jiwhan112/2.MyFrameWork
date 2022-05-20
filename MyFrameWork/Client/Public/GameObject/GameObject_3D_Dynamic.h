#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// 애니메이션 3D 오브젝트 출력
BEGIN(Client)

class CGameObject_3D_Dynamic  :
	public CGameObject_Base
{
public:
	//enum E_BASEAI
	//{
	//	BASEAI_IDLE,
	//	BASEAI_MOVE,
	//	BASEAI_END,
	//};

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
	CModel*						Get_ComModel() const { return mComModel; }
	const list<CCollider*>*		Get_ComListCollider() const { return mComListCollider; }
	CNavigation*				Get_ComNaviMesh() const { return mComNaviMesh; }

	const MODEL_DYNAMIC_DESC&	Get_ModelDESC() const { return mModelDesc; }

	HRESULT		Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC& desc);
	HRESULT		Setup_Colluder2(_float4x4 worldmat, _float scale);

	HRESULT		Add_ColliderDesc(COLLIDER_DESC desc);
	HRESULT		Add_ColliderDesc(COLLIDER_DESC* desc,int size);

public:
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex);


protected:
	virtual HRESULT Set_Component() override;
	HRESULT Update_Collider();


//	void		GOMOVE(_double timer);

protected: // 3D모델 Com / DESC 추가
	CModel*						mComModel = nullptr;

	// 콜라이더 리스트 추가 / 콜라이더는 맵에 넣지 않고 여기서만 관리한다.
	list<CCollider*>*			mComListCollider = nullptr;

	// 생성시 무조건 Terrain 정보가 있어야한다.
//	CGameObject_Base*			mGameObjectTerrain = nullptr;
	CNavigation*				mComNaviMesh = nullptr;


	MODEL_DYNAMIC_DESC			mModelDesc;
	list<COLLIDER_DESC>			mListColliderDesc;

	_float3						mGoalPosition;
	_float3						mStartPosition;
	_double						mTimer;
	_double						mTimeMax;
	bool						mIsMove = false;
//	E_BASEAI					meAI;

public:
	static CGameObject_3D_Dynamic* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Dynamic* Clone(void* pArg);
	virtual void Free() override;
};

END