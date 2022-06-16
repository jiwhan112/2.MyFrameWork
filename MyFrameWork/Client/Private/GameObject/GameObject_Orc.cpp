#include "stdafx.h"
#include "GameObject/GameObject_Orc.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"
#include "AI/AI_Sequnce.h"

CGameObject_Orc::CGameObject_Orc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_ORC;
}

CGameObject_Orc::CGameObject_Orc(const CGameObject_Orc& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Orc::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_Orc::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
//	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);

	

	return S_OK;
}

HRESULT CGameObject_Orc::Tick_Dungeon(_double TimeDelta)
{
	FAILED_CHECK(__super::Tick_Dungeon(TimeDelta));
	
	return S_OK;
}

HRESULT CGameObject_Orc::LateTick_Dungeon(_double TimeDelta)
{
	FAILED_CHECK(__super::LateTick_Dungeon(TimeDelta));

	return S_OK;
}

HRESULT CGameObject_Orc::Tick_World(_double TimeDelta)
{
	FAILED_CHECK(__super::Tick_World(TimeDelta));
	
	// 자동 전투
	/*if (mComBehavior->Get_CurrentSequnce()->Get_SeqMoveType() != CNode_Seqeunce::SEQMOTAIONTYPE_ATTACK)
	{
		auto EnemyList = GetSingle(CGameManager)->Get_DaungonManager()->Get_DungeonObjects()->Get_ListUnitID(UNIT_ENEMY);
		const _float Range = 5.f;
		for (auto& plyobj : EnemyList)
		{
			if (plyobj->Get_CurrentMap() == CGameObject_3D_Dynamic::MAPTYPE_WORLD)
			{
				_float dis = _float3::Distance(plyobj->Get_WorldPostition(), Get_WorldPostition());
				if (dis < Range)
				{
					Select_WorldAttack(plyobj);
					break;
				}
			}
		}
	}*/



	return S_OK;
}

HRESULT CGameObject_Orc::LateTick_World(_double TimeDelta)
{
	FAILED_CHECK(__super::LateTick_World(TimeDelta));

	return S_OK;
}


HRESULT CGameObject_Orc::Init_Unit()
{
	// 모델 결정
	string str("crea_Orc.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	// Transform
	_float3 SpawnPos = mSpawnPostitionDAUNGEON;
	SpawnPos.y += 10;
	Set_Position(SpawnPos);

	Set_LookDir(_float3(-1, 0, -1));


	_float size = 0.8f;
	mComTransform->Scaled(_float3(size, size, size));

	// 유닛 타입
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	meUnitType = UNIT_PLAYER;
	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
	mTimeForSpeed = 1.0f;// Test
	mTimeForSpeed_World = 1.0f;

	// 충돌 정보

	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mOffset = _float3(0, size, 0);
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Init_Collider();

	// 애니메이션
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_SPAWNPOS));

	// 소켓
	Add_Socket_Model(STR_TAYSOCKET(SOCKET_WEAPON_1), "wep_OrcAxe.fbx", "RArmDigit22");

	mIsPickTurn = false;

	return S_OK;
}

HRESULT CGameObject_Orc::Init_AI()
{
	FAILED_CHECK(__super::Init_AI());
	Init_AI_Default();

	return S_OK;
}


HRESULT CGameObject_Orc::Init_AI_Default()
{	// AI 세부 구현

	CSequnce_IDLE* Seq_IDLE = CSequnce_IDLE::Create(this);
	CSequnce_IDLE::SEQIDLE DefaultIdleDesc;

	DefaultIdleDesc.mMoveEasingId = TYPE_Linear;
	DefaultIdleDesc.AniType = CAnimatior::E_COMMON_ANINAME_IDLE;

	Seq_IDLE->Restart(&DefaultIdleDesc);
	mComBehavior->Add_Seqeunce("IDLE", Seq_IDLE);

	CSequnce_PICK* Seq_Pick = CSequnce_PICK::Create(this);
	CSequnce_PICK::SEQPICK DefaultPickDesc;
	DefaultPickDesc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_CARRIED;
	Seq_Pick->Restart(&DefaultPickDesc);
	mComBehavior->Add_Seqeunce("PICK", Seq_Pick);


	CSequnce_WorldAttack_Player* Seq_Attack = CSequnce_WorldAttack_Player::Create(this);
	CSequnce_WorldAttack_Player::SEQWORLDATTACK_PLY DefaulAttackDesc;
	DefaulAttackDesc.Target = nullptr;
	Seq_Attack->Restart(&DefaulAttackDesc);
	mComBehavior->Add_Seqeunce("WORLD_ATTACK", Seq_Attack);

	
	return S_OK;
}


HRESULT CGameObject_Orc::Select_WorldAttack(CGameObject_3D_Dynamic* target )
{
	// 적을 클릭할 떄 실행

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
				mTarget_Attack = nullptr;
		}

	}

	mTarget_Attack = target;
	CSequnce_WorldAttack_Player::SEQWORLDATTACK_PLY desc;
	desc.Target = mTarget_Attack;
	mComBehavior->Select_Sequnce("WORLD_ATTACK", &desc);

	return S_OK;
}



CGameObject_Orc * CGameObject_Orc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Orc*	pInstance = NEW CGameObject_Orc(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Orc");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Orc* CGameObject_Orc::Clone(void* pArg)
{
	CGameObject_Orc*	pInstance = NEW CGameObject_Orc(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Orc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Orc::Free()
{
	__super::Free();


	//	Safe_Release(mComNaviMesh);
}
