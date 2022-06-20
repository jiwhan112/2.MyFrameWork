#pragma once

#include "GameObject_Base.h"
#include "Animatior.h"
#include "GameObject_Socket.h"

BEGIN(Engine)
class CModel;
class CCell;
END

// 애니메이션 3D 오브젝트 출력
BEGIN(Client)
class CGameObject_3D_Socket;

class CGameObject_3D_Dynamic  :
	public CGameObject_Base
{
public:
	enum E_MAPTYPE
	{
		MAPTYPE_DUNGEON,
		MAPTYPE_WORLD,
		MAPTYPE_END,
	};

	enum E_TICK_TYPE
	{
		TICK_TYPE_NONE,
		TICK_TYPE_DUNGION_PICK, // 객체를 클릭하면 Pick으로 전환
		TICK_TOOL,
		TICK_TYPE_END,
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

	virtual HRESULT Tick_Dungeon(_double TimeDelta);
	virtual HRESULT LateTick_Dungeon(_double TimeDelta);

	virtual HRESULT Tick_World(_double TimeDelta);
	virtual HRESULT LateTick_World(_double TimeDelta);

	
	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI();
	HRESULT Init_AI_CommonDynamic();


public: // GetSet
	HRESULT						Set_MapSetting(E_MAPTYPE type);

	CModel*						Get_ComModel() const { return mComModel; }
	CBehaviorTree*				Get_ComBehavior() const { return mComBehavior; }
	const list<CCollider*>*		Get_ComListCollider() const { return mComListCollider; }

	CGameObject_MyTerrain*		Get_Terrain(E_MAPTYPE type) const { return mTerrain_Maps[type]; }
	CNavigation*				Get_ComNaviMesh(E_MAPTYPE type) const { return mComNavi[type]; }

	const MODEL_DYNAMIC_DESC&	Get_ModelDESC() const { return mModelDesc; }

	HRESULT		Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC& desc);

	HRESULT		Add_ColliderDesc(COLLIDER_DESC desc);
	HRESULT		Add_ColliderDesc(COLLIDER_DESC* desc,int size);
	//void		Set_LookPos(_float3 lookpos)
	//{
	//	mLookPostiton = lookpos;
	//}

	void		Set_LookDir(_float3 dir);

	HRESULT Switch_MapType();

	E_MAPTYPE Get_CurrentMap() const
	{
		return meCurrentMap;
	}
	_float Get_TimeForSpeed() const { return mTimeForSpeed; }
	_float Get_TimeForSpeed_World() const { return mTimeForSpeed_World; }

	E_UNITTYPE Get_UnitType()const
	{
		return meUnitType;
	}

	void Set_LookAt_RotateXZ(_float4x4 mat);
	void Set_RotateXZ(_float4x4 mat);
	void Set_AttackTarget(CGameObject_3D_Dynamic* target)
	{
		Safe_Release(mTarget_Attack);
		mTarget_Attack = target;
		Safe_AddRef(mTarget_Attack);

	}
	CGameObject_3D_Dynamic* Get_AttackTarget() const
	{
		return mTarget_Attack;
	}

	
	
	// HP MP Damage
	_int& Get_Hp() { return mHP; };
	_int& Get_Mp() { return mMP; };
	_int& Get_Damage() { return mDamage; };


	// Collision
public: 
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex);
	virtual HRESULT CollisionFunc(CGameObject_3D_Dynamic* dynamic,_double Timer);
//	virtual HRESULT CollisionFunc(CGameObject_Base* object);

	// Move AI
public: 
	HRESULT FindPathForCurrentNavi(_float3 GoalPosition);
	_bool	Get_PathList_Frontpop(_float3* NextPosition);
	// 갈 수 있는 위치 뽑기
	_bool	FindPathRandAblePostition(_int fRange, _float3* GoalPos);
	_bool	FindPathRandDungeonAblePostition(_uint Mincount,_uint NebiorMaxCount,_float3* GoalPos);

	_float3 Get_PathGoalPostition()const;

	void	Set_IsTerrainHeight(_bool b) { mIsTerrainHeight = b; }
	_bool	Get_IsTerrainHeight() const { return mIsTerrainHeight; }
	_float3	Get_TerrainHeightPostition();
	void	Set_RotationFlag(_float3 Target);
	HRESULT WorldPathLerp(_float range=3);



	// Animation
public: 
	HRESULT Set_AniEnum(CAnimatior::E_COMMON_ANINAME name, _int index=-1,_bool isFoward = true);
	HRESULT Set_AniIndex(_uint AniIndex);
	HRESULT Set_AniString(string str);


	// AI
protected:
	virtual HRESULT Set_Component() override;
	HRESULT Init_Collider();
	HRESULT Set_Terrain_HeightY(class CGameObject_MyTerrain* terrain);
	HRESULT Set_Sequnce(const char* statename, void* desc);
	HRESULT	Check_AutoAttackForWorld();

public:
	HRESULT Select_Door();
	HRESULT Select_Fall();
	HRESULT Select_Die();
	HRESULT Select_WorldPostition(_float3 pos);
	virtual HRESULT Select_WorldAttack(CGameObject_3D_Dynamic* obj);


	
	HRESULT Set_BehaviorMode(int index = -1);

	// 전투 연출
	virtual HRESULT AttackFunc();
	virtual HRESULT HitFunc(_int Damage);
	virtual HRESULT DieFunc();
	HRESULT RotTargetFunc();


	// SOCKET
protected:
	HRESULT Tick_Socket(_double timer);
	HRESULT Render_Socket();

	HRESULT						Add_Socket_Model(string tag, string modelName, string boneName);
	HRESULT						Add_Socket_NULL(string tag, string boneName);
	CGameObject_3D_Socket*		Find_Socket(string tag);
	
public:
	HRESULT						Set_SocketVisible(string tag, _bool vis);
	CGameObject_3D_Socket*		Get_SocketObj(string tag);
	CTransform*					Get_Socket_Trans(string tag);

	// DEBUG
	HRESULT Tick_DEBUG(_double timer);

protected: // 3D모델 Com / DESC 추가
	CModel*							mComModel = nullptr;
	// AI
	CBehaviorTree*					mComBehavior = nullptr;

	// 콜라이더 리스트 추가 / 콜라이더는 맵에 넣지 않고 여기서만 관리한다.
	// 뼈애 달리게 수정
	list<CCollider*>*				mComListCollider = nullptr;

	// 소켓
	map<string, CGameObject_3D_Socket*> mMapListSocket;

	MODEL_DYNAMIC_DESC				mModelDesc;
	list<COLLIDER_DESC>				mListColliderDesc;

	

	E_UNITTYPE						meUnitType = UNIT_END;

	// MOVE
	list<CCell*>					mCurrentPathList;
	_float3							mGoalPosition; // 목표위치
//	_float3							mLookPostiton;

	// 지형
	E_MAPTYPE						meCurrentMap = MAPTYPE_END;
	
	// 현재 

	// 버퍼들 
	CGameObject_MyTerrain*		mTerrain_Maps[MAPTYPE_END] = {nullptr,nullptr };
	CNavigation*				mComNavi[MAPTYPE_END] = { nullptr, nullptr };

	CGameObject_MyTerrain*		mCurrentMap = nullptr;
	CNavigation*				mCurrentNavi = nullptr;

	_bool						mIsTerrainHeight = false;

	_float						mTimeForSpeed=0.3f;
	_float						mTimeForSpeed_World = 1.0f;


	// TICKTYPE
	E_TICK_TYPE					meTickType = TICK_TYPE_END;
	const _float				mMouseOffset = 4;
	_bool						mIsPickTurn = true;

	// 공격 타겟
	CGameObject_3D_Dynamic* mTarget_Attack = nullptr;


	// 속성
	_int							mHP=30;
	_int							mMP=0; // 스킬
	_int							mDamage = 10;

public:
	// 생성 위치
	const _float3 mSpawnPostitionDAUNGEON = _float3(20.f, 0.f, 14.5f);
	const _float3 mSpawnPostitionENEMY = _float3(8.f, 8.f, 48.f);
	const _float3 mSpawnPostitionBOSS = _float3(52.f, 8.f, 51.f);

	// 던전입구
	const _float3 mDungeonDoorGoalPos = _float3(22.4f, 0.010f, 23.5f);
	const _float3 mDungeonDoorStartPos = _float3(22, 0.010f, 28.5f);
	const _float3 mWorldDoorStartPos = _float3(59.3f, 8.72f, 4.15f);
	const _float3 mWorldDoorGoalPos = _float3(52.24f, 8.72f, 11.3f);

public:
	static CGameObject_3D_Dynamic* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Dynamic* Clone(void* pArg);
	virtual void Free() override;
};

END