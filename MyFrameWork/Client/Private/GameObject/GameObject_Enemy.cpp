#include "stdafx.h"
#include "GameObject/GameObject_Enemy.h"
#include "GameObject/Dungeon_Manager.h"
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


	mWorldCreateTimer += TimeDelta;
	if (mWorldCreateTimer > 5)
	{
		mTimer_Dungeon += TimeDelta;
		if (mTimer_Dungeon > 3)
		{
			mTimer_Dungeon = 0;
			Set_GoDungeion();
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
	// 모델 결정
	string str("hero_Warrior_T1.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	// Transform
	_float3 SpawnPos = mSpawnPostitionENEMY;
	SpawnPos.y += 10;
	Set_Position(SpawnPos);

	Set_LookDir(_float3(-1, 0, -1));
	_float size = 0.8f;
	mComTransform->Scaled(_float3(size, size, size));


	// 유닛 타입
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());
	Set_BehaviorMode(1);

	meUnitType = CGameObject_3D_Dynamic::UNIT_ENEMY;
	meEnemyType = CGameObject_Enemy::ENEMY_WARRIOR;
	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
	mTimeForSpeed = 0.5f;
	mRotSpeed = 10.0f;


	// 충돌 정보
	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();

	// 애니메이션
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_CARRIED));

	// 소켓
	// Add_Socket("crea_SnotPickaxe.fbx", "RArmDigit31");

	mWorldCreateTimer = 0;
	mTimer_Dungeon = 0;

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
	// 던전에 들어왔을떄

	// 월드 생성시 

	// 월드에서 던전으로 옴

	/*CSequnce_WorldMove_Enemy* Seq_WorldMove = CSequnce_WorldMove_Enemy::Create(this);
	CSequnce_WorldMove_Enemy::SEQWORLDMOVE_ENEMY WorldDesc;
	WorldDesc.TargetPos1 = _float3(0, 0, 0);
	Seq_WorldMove->Restart(&WorldDesc);
	mComBehavior->Add_Seqeunce("WORLD_ENEMY", Seq_WorldMove);*/

	

	// 전투 시퀀스 



	// IDLE TILE 정보 생성 예제
	// CSequnce_IDLE* Seq_IDLE = CSequnce_IDLE::Create(this);
	// CSequnce_IDLE::SEQIDLE DefaultIdleDesc;
	// 
	// DefaultIdleDesc.mMoveEasingId = TYPE_Linear;
	// DefaultIdleDesc.AniType = CAnimatior::E_COMMON_ANINAME_IDLE;
	// 
	// Seq_IDLE->Restart(&DefaultIdleDesc);
	// mComBehavior->Add_Seqeunce("IDLE", Seq_IDLE);
	// 
	// CSequnce_TILE* Seq_TILE = CSequnce_TILE::Create(this);
	// CSequnce_TILE::tag_SeqTILE DefaultTileDesc;
	// DefaultTileDesc.Runtime = mTimeForSpeed * 0.5f;
	// 
	// Seq_TILE->Restart(&DefaultTileDesc);
	// mComBehavior->Add_Seqeunce("DIG", Seq_TILE);
	// 

	return S_OK;
}

HRESULT CGameObject_Enemy::Set_GoDungeion()
{
	//string NextKey = "WORLD_ENEMY";
	//if (NextKey == mComBehavior->Get_CurrentSeqKey())
	//	return S_OK;

	////CSequnce_WorldMove* Seq_WorldMove = CSequnce_WorldMove::Create(this);
	//CSequnce_WorldMove_Enemy::SEQWORLDMOVE_ENEMY WorldDesc;
	//WorldDesc.TargetPos1 = mWorldTargetPos1;
	//WorldDesc.TargetPos2 = mWorldTargetPos2;
	//WorldDesc.TargetPos3 = mWorldTargetPos3;
	//FAILED_CHECK(mComBehavior->Select_Sequnce(NextKey, &WorldDesc));


	return S_OK;
}

HRESULT CGameObject_Enemy::CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex)
{
	FAILED_CHECK(__super::CollisionFunc(PickPosition, dist, ColliderIndex));
	
	
	if (KEYDOWN(DIK_E))
	{
		if (GetSingle(CGameManager)->Get_DaungonManager()->Get_CurrentGameMode() == CDungeon_Manager::GAMEMODE_WORLD)
		{
			GetSingle(CGameManager)->Get_DaungonManager()->Add_Task_Player_WorldAttack(this);
			return S_OK;
		}
	}

	return S_OK;
}

HRESULT CGameObject_Enemy::AttackFunc()
{
	return S_OK;
}

HRESULT CGameObject_Enemy::HitFunc()
{
	return S_OK;
}

HRESULT CGameObject_Enemy::DieFunc()
{
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
