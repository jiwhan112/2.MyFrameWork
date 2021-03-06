#include "stdafx.h"
#include "GameObject/GameObject_Enemy.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"
#include "AI/AI_Sequnce.h"


CGameObject_Enemy::CGameObject_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_ENEMY;
}

CGameObject_Enemy::CGameObject_Enemy(const CGameObject_Enemy& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Enemy::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Enemy::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}

HRESULT CGameObject_Enemy::Tick_Dungeon(_double TimeDelta)
{
	if (UPDATEERROR == __super::Tick_Dungeon(TimeDelta))
		return UPDATEERROR;
	
	return UPDATENONE;
}

HRESULT CGameObject_Enemy::LateTick_Dungeon(_double TimeDelta)
{
	if (UPDATEERROR == __super::LateTick_Dungeon(TimeDelta))
		return UPDATEERROR;

	return UPDATENONE;
}

HRESULT CGameObject_Enemy::Tick_World(_double TimeDelta)
{
	if (UPDATEERROR == __super::Tick_World(TimeDelta))
		return UPDATEERROR;

	if (mComBehavior->Get_CurrentSequnce()->Get_SeqMoveType() == CNode_Seqeunce::SEQMOTAIONTYPE_IDLE)
	{
		mWorldIdleTimer += TimeDelta;
		if (mWorldIdleTimer > mWorldMoveTimeMax && mIsCreateOrder == false)
		{
			Set_GoDungeon();
			mIsCreateOrder = true;

		}
	}

	return UPDATENONE;
}

HRESULT CGameObject_Enemy::LateTick_World(_double TimeDelta)
{
	if (UPDATEERROR == __super::LateTick_World(TimeDelta))
		return UPDATEERROR;

	

	return UPDATENONE;
}


HRESULT CGameObject_Enemy::Init_Unit()
{
	// ???? ????
	string str("hero_Warrior_T1.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);
	mHP = 30;

	// Transform
	_float3 SpawnPos = mSpawnPostitionENEMY;
	SpawnPos.y += 10;
	Set_Position(SpawnPos);

	Set_LookDir(SPAWN_DIR);
	_float size = 0.8f;
	mComTransform->Scaled(_float3(size, size, size));


	// ???? Ÿ??
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());
	Set_BehaviorMode(1);

	meUnitType = UNIT_ENEMY;
	meEnemyType = CGameObject_Enemy::ENEMY_WARRIOR;
	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
	mTimeForSpeed = 1.0f;
	mTimeForSpeed_World = 1.0f;


	// ?浹 ????
	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mOffset = _float3(0, size, 0);
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Init_Collider();

	// ?ִϸ??̼?
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_CARRIED));

	// ????
	Add_Socket_Model(STR_TAYSOCKET(SOCKET_WEAPON_1), "wep_WarriorSword_T1.fbx", "RArmDigit22");


	mMoveTarget[ENEMY_MOVETARGET_WORLD1] = mWorldTargetPos1;
	mMoveTarget[ENEMY_MOVETARGET_WORLD2] = mWorldTargetPos2;
	mMoveTarget[ENEMY_MOVETARGET_WORLD3] = mWorldTargetPos3;
	mMoveTarget[ENEMY_MOVETARGET_DUNGEON] = _float3();
	mWorldMoveIndex = 0;

	mWorldIdleTimer = 0;
	mWorldMoveTimeMax = 5;
	mIsCreateOrder = false;

	return S_OK;
}

HRESULT CGameObject_Enemy::Init_AI()
{
	FAILED_CHECK(__super::Init_AI());
	FAILED_CHECK(Init_AI_Enemy());

	return S_OK;
}

HRESULT CGameObject_Enemy::Init_AI_Enemy()
{
	// 1. ?????ڿ? ???????? ??ã??.

	CSequnce_WorldMove_Enemy* Seq_WorldMove = CSequnce_WorldMove_Enemy::Create(this);
	CSequnce_WorldMove_Enemy::SEQWORLDMOVE_ENEMY WorldDesc;
	Seq_WorldMove->Restart(&WorldDesc);
	mComBehavior->Add_Seqeunce("WORLD_ENEMY", Seq_WorldMove);

	// IDLE 
	CSequnce_IDLE* Seq_IDLE = CSequnce_IDLE::Create(this);
	CSequnce_IDLE::SEQIDLE DefaultIdleDesc;

	DefaultIdleDesc.mMoveEasingId = TYPE_Linear;
	DefaultIdleDesc.AniType = CAnimatior::E_COMMON_ANINAME_MELEE;

	Seq_IDLE->Restart(&DefaultIdleDesc);
	mComBehavior->Add_Seqeunce("IDLE", Seq_IDLE);

	// ???? ?̵? ?ڵ? ????
	CSequnce_WorldAutoAttack* Seq_WorldAttack = CSequnce_WorldAutoAttack::Create(this);
	CSequnce_WorldAutoAttack::SEQWORLDAUTOATTACK attackDesc;
	attackDesc.Target = nullptr;
	Seq_WorldAttack->Restart(&attackDesc);
	mComBehavior->Add_Seqeunce("WORLD_ATTACK", Seq_WorldAttack);

	// ?????? ??????????
	// ???? Heart?? ?????Ѵ?.
	
	return S_OK;
}

HRESULT CGameObject_Enemy::DieFunc()
{
	__super::DieFunc();
	GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->Add_EnemyDieCount();

	return S_OK;
}

HRESULT CGameObject_Enemy::Set_MoveCount()
{
	mWorldMoveIndex++;
	if (mWorldMoveIndex >= ENEMY_MOVETARGET_DUNGEON)
	{

		mWorldMoveIndex = ENEMY_MOVETARGET_DUNGEON;
	}
	mWorldIdleTimer = 0;
	mWorldMoveTimeMax = 2;
	mIsCreateOrder = false;

	return S_OK;
}

HRESULT CGameObject_Enemy::Set_GoDungeon()
{
	// ???? ?ε????? GoalPoint?? ?Ǽ? ???? ?̵??? ?????Ѵ?.
	if (mWorldMoveIndex == E_ENEMY_MOVETARGET::ENEMY_MOVETARGET_DUNGEON)
	{
		Switch_MapType();
	}

	else
	{
		// ???????? ?̵?

		// ???? ?̵?
		_float3 GoalPos = mMoveTarget[mWorldMoveIndex];
		CSequnce_WorldMove_Enemy::SEQWORLDMOVE_ENEMY MoveEnemyDesc;
		MoveEnemyDesc.GoalPostition = GoalPos;
		mComBehavior->Select_Sequnce("WORLD_ENEMY", &MoveEnemyDesc);
	}

	return S_OK;
}

HRESULT CGameObject_Enemy::CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex)
{
	FAILED_CHECK(__super::CollisionFunc(PickPosition, dist, ColliderIndex));
	
	
	/*if (KEYDOWN(DIK_E))
	{
		if (GetSingle(CGameManager)->Get_DaungonManager()->Get_CurrentGameMode() == CDungeon_Manager::GAMEMODE_WORLD)
		{
			GetSingle(CGameManager)->Get_DaungonManager()->Add_Task_Player_WorldAttack(this);
			return S_OK;
		}
	}*/

	return S_OK;
}

HRESULT CGameObject_Enemy::Select_WorldAttack(CGameObject_3D_Dynamic* target)
{
	// ???? Ŭ???? ?? ????
	if (target == false)
		return E_FAIL;

	if (mTarget_Attack)
	{
		if (mTarget_Attack == target)
		{
			bool targetlife = mTarget_Attack->Get_IsLife();
			if (targetlife)
				return S_OK;
			else
				Set_AttackTarget(nullptr);
		}
	}
	Set_AttackTarget(target);
	CSequnce_WorldAutoAttack::SEQWORLDAUTOATTACK desc;
	desc.Target = mTarget_Attack;
	mComBehavior->Select_Sequnce("WORLD_ATTACK", &desc);

	return S_OK;
}


CGameObject_Enemy * CGameObject_Enemy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Enemy*	pInstance = NEW CGameObject_Enemy(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Enemy* CGameObject_Enemy::Clone(void* pArg)
{
	CGameObject_Enemy*	pInstance = NEW CGameObject_Enemy(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Enemy::Free()
{
	__super::Free();
}
