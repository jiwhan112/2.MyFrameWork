#include "stdafx.h"
#include "GameObject/GameObject_Goblin.h"
#include "AI/AI_Sequnce.h"

CGameObject_Goblin::CGameObject_Goblin(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_GOBLIN;
}

CGameObject_Goblin::CGameObject_Goblin(const CGameObject_Goblin& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Goblin::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Goblin::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}


HRESULT CGameObject_Goblin::Init_Unit()
{
	// 모델 결정
	string str("crea_Goblin.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	// Transform
	_float3 SpawnPos = mSpawnPostitionDAUNGEON;
	SpawnPos.y += 10;
	Set_Position(SpawnPos);

	Set_LookDir(_float3(-1, 0, -1));


	_float size = 0.6f;
	mComTransform->Scaled(_float3(size, size, size));

	// 유닛 타입
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_DUNGEON);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	meUnitType = CGameObject_3D_Dynamic::UNIT_PLAYER;
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
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_SPAWNPOS));

	// 소켓
	Add_Socket_Model(STR_TAYSOCKET(SOCKET_WEAPON_1), "crea_SnotPickaxe.fbx", "RArmDigit31");

	mIsPickTurn = false;
	return S_OK;

}

HRESULT CGameObject_Goblin::Init_AI()
{
	FAILED_CHECK(__super::Init_AI());
	
	Init_AI_Default();

	return S_OK;
}

HRESULT CGameObject_Goblin::Init_AI_Default()
{
	// AI 세부 구현
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

	return S_OK;
}


HRESULT CGameObject_Goblin::Select_WorldAttack(CGameObject_3D_Dynamic* target)
{
	// 적을 클릭할 떄 실행

	if (target == false)
		return E_FAIL;

	if (mTarget_Attack)
	{
		bool targetlife = mTarget_Attack->Get_IsLife();
		if (targetlife)
			return S_OK;
		else
			targetlife = nullptr;
	}
	mTarget_Attack = target;

	CSequnce_WorldAttack_Player::SEQWORLDATTACK_PLY desc;
	desc.Target = mTarget_Attack;
	mComBehavior->Select_Sequnce("ATTACK_WORLD", &desc);
	return S_OK;
}




CGameObject_Goblin * CGameObject_Goblin::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Goblin*	pInstance = NEW CGameObject_Goblin(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Goblin");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Goblin* CGameObject_Goblin::Clone(void* pArg)
{
	CGameObject_Goblin*	pInstance = NEW CGameObject_Goblin(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Goblin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Goblin::Free()
{
	__super::Free();

}
