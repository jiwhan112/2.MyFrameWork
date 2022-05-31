#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
class CCell;
END

// 애니메이션 3D 오브젝트 출력
BEGIN(Client)

class CGameObject_3D_Dynamic  :
	public CGameObject_Base
{
public:
	enum E_UNITTYPE
	{
		UNIT_PLAYER,
		UNIT_ENEMY,
		UNIT_END,
	};

	enum E_MAPTYPE
	{
		MAPTYPE_DUNGEON,
		MAPTYPE_WORLD,
		MAPTYPE_END,
	};

protected:
	explicit CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs);
	virtual ~CGameObject_3D_Dynamic() = default;

public: // Base
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
	
	virtual HRESULT Init_Unit();



public: // GetSet
	HRESULT						Set_MapSetting(E_MAPTYPE type);

	CModel*						Get_ComModel() const { return mComModel; }
	CBehaviorTree*				Get_ComBehavior() const { return mComBehavior; }
	const list<CCollider*>*		Get_ComListCollider() const { return mComListCollider; }

	CGameObject_MyTerrain*		Get_Terrain(E_MAPTYPE type) const { return mTerrain_Maps[type]; }
	CNavigation*				Get_ComNaviMesh(E_MAPTYPE type) const { return mComNavi[type]; }

	const MODEL_DYNAMIC_DESC&	Get_ModelDESC() const { return mModelDesc; }

	HRESULT		Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC& desc);
	HRESULT		Setup_Colluder2(_float4x4 worldmat, _float scale);

	HRESULT		Add_ColliderDesc(COLLIDER_DESC desc);
	HRESULT		Add_ColliderDesc(COLLIDER_DESC* desc,int size);

public: // Collision
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex);

protected: // Move AI
	HRESULT PathTrigger(CNavigation* MyNaviMesh, _float3 TargetXZ);
	HRESULT Update_Move(_double Timer);

	// AI 루팅 설정
	HRESULT Create_Sequnce();


protected:
	virtual HRESULT Set_Component() override;
	HRESULT Update_Collider();
	HRESULT Set_Terrain_HeightY(class CGameObject_MyTerrain* terrain);


	

protected: // 3D모델 Com / DESC 추가
	CModel*						mComModel = nullptr;

	// 콜라이더 리스트 추가 / 콜라이더는 맵에 넣지 않고 여기서만 관리한다.
	// 뼈애 달리게 수정
	list<CCollider*>*			mComListCollider = nullptr;



	MODEL_DYNAMIC_DESC			mModelDesc;
	list<COLLIDER_DESC>			mListColliderDesc;

	
	E_UNITTYPE					meUnitType = UNIT_END;

	// AI
	CBehaviorTree*				mComBehavior = nullptr;

	// MOVE
	_float3						mGoalPosition;
	_float3						mStartPosition;
	_double						mTimer;
	_double						mTimeMax;
	bool						mIsNaviPath = false;
	bool						mIsMoveCell = false;

	list<CCell*>				mCurrentPathList;
	CCell*						mMoveCell; // 현재 셀

	_float3						mCurrentPosition;

	// 지형
	E_MAPTYPE					meCurrentMap = MAPTYPE_END;
	// 현재 

	// 버퍼들 
	CGameObject_MyTerrain*		mTerrain_Maps[MAPTYPE_END] = {nullptr,nullptr };
	CNavigation*				mComNavi[MAPTYPE_END] = { nullptr, nullptr };

	CGameObject_MyTerrain*		mCurrentMap = nullptr;
	CNavigation*				mCurrentNavi = nullptr;

	
//	E_BASEAI					meAI;

public:
	static CGameObject_3D_Dynamic* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Dynamic* Clone(void* pArg);
	virtual void Free() override;
};

END