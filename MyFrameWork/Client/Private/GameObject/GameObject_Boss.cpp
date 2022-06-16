#include "stdafx.h"
#include "GameObject/GameObject_BOSS.h"
#include "AI/AI_Sequnce.h"

CGameObject_BOSS::CGameObject_BOSS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_BOSS;
}

CGameObject_BOSS::CGameObject_BOSS(const CGameObject_BOSS& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_BOSS::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_BOSS::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	return S_OK;
}


HRESULT CGameObject_BOSS::Tick_World(_double TimeDelta)
{
	if (UPDATEERROR == __super::Tick_World(TimeDelta))
		return UPDATEERROR;

	return UPDATENONE;
}

HRESULT CGameObject_BOSS::LateTick_World(_double TimeDelta)
{
	if (UPDATEERROR == __super::LateTick_World(TimeDelta))
		return UPDATEERROR;

	return UPDATENONE;
}

HRESULT CGameObject_BOSS::Init_Unit()
{
	mHP = 200;


	// 모델 결정
	string str("npc_Ent.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	// Transform
	_float3 SpawnPos = mSpawnPostitionBOSS;
	Set_Position(SpawnPos);
	mStartPostition = SpawnPos;
	Set_LookDir(_float3(-1, 0, -1));

	_float size = 5.0f;
	mComTransform->Scaled(_float3(size, size, size));


	// 유닛 타입
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());
	Set_BehaviorMode(1);

	meUnitType = UNIT_BOSS;
	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
	mTimeForSpeed = 2.0f;
	mTimeForSpeed_World = 3.0f;


	// 충돌 정보
	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Init_Collider();

	// 애니메이션
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_SKINPOSE));

	//// 소켓
	//// 보스 뼈 이름
	//// LDigit11 / LDigit12 // 손목 / 손가락
	//// LDigit21 / LDigit22 // 숫자가 크면 더 안쪽 뼈
	////Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_1), "LDigit21");
	//Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_1), "LDigit22");
	////Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_3), "RDigit21");
	//Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_2), "RDigit22");

	//CGameObject_3D_Socket* socketL = Find_Socket(TAGSOCKET(SOCKET_ANY_1));
	//CGameObject_3D_Socket* socketR = Find_Socket(TAGSOCKET(SOCKET_ANY_2));

	//COLLIDER_DESC coldesc1;
	//coldesc1.mSize = _float3(1.5f, 1.5f, 1.5f);
	//socketL->Set_LoadColliderDESC(coldesc1);
	//COLLIDER_DESC coldesc2;
	//coldesc2.mSize = _float3(1.5f, 1.5f, 1.5f);
	//socketR->Set_LoadColliderDESC(coldesc2);

	mIsTerrainHeight = true;
	


	return S_OK;
}

HRESULT CGameObject_BOSS::Init_AI()
{
	FAILED_CHECK(__super::Init_AI());
	FAILED_CHECK(Init_AI_Boss());
}

HRESULT CGameObject_BOSS::Init_AI_Boss()
{
	// 보스 패턴

	// 생성
	CSequnce_WorldIdle* Seq_WorldIdle = CSequnce_WorldIdle::Create(this);
	CSequnce_WorldIdle::SEQWORLDIDLE worldIdle;
	worldIdle.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_SLEEPING;
	Seq_WorldIdle->Restart(&worldIdle);
	mComBehavior->Add_Seqeunce("WORLD_IDLE", Seq_WorldIdle);
	mComBehavior->Select_Sequnce("WORLD_IDLE");

	// 대기하면서 범위이동
	CSequnce_WorldMove_Boss* Seq_WorldMove= CSequnce_WorldMove_Boss::Create(this);
	CSequnce_WorldMove_Boss::SEQWORLDMOVE_BOSS worldMove;
	worldMove.GoalPostition = _float3();
	Seq_WorldMove->Restart(&worldMove);
	mComBehavior->Add_Seqeunce("WORLD_MOVE", Seq_WorldMove);



	// 적발견 후 포효
	CSequnce_BossDealy* Seq_WorldDealy= CSequnce_BossDealy::Create(this);
	CSequnce_BossDealy::SEQBOSSDEALY dealyDesc;
	dealyDesc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_SKILL;
	Seq_WorldDealy->Restart(&dealyDesc);
	mComBehavior->Add_Seqeunce("WORLD_WARRIOR", Seq_WorldDealy);

	


	// 패턴 123 




	return S_OK;
}

void CGameObject_BOSS::Select_Move()
{
	// 꺠어나서 주위 배외
	CSequnce_WorldMove_Boss::SEQWORLDMOVE_BOSS worldMove;
	
	int RandIntX = CHelperClass::RandomInt_Min(-5, 5);
	int RandIntZ = CHelperClass::RandomInt_Min(-5, 5);

	_float3 newRandPostition = mStartPostition;
	newRandPostition.x += RandIntX;
	newRandPostition.z += RandIntZ;

	worldMove.GoalPostition = newRandPostition;

	mComBehavior->Select_Sequnce("WORLD_MOVE",&worldMove);


}

void CGameObject_BOSS::Select_Warrior(CAnimatior::E_COMMON_ANINAME type, int index)
{
	CSequnce_BossDealy::SEQBOSSDEALY desc;

	desc.AniType = type;
	desc.index = index;

	mComBehavior->Select_Sequnce("WORLD_WARRIOR", &desc);


}

void CGameObject_BOSS::Select_Warrior_()
{
	// 포효 명령
	CSequnce_BossDealy::SEQBOSSDEALY desc;
	desc.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_SKILL;
	desc.index = 2;

	mComBehavior->Select_Sequnce("WORLD_WARRIOR", &desc);

}


CGameObject_BOSS * CGameObject_BOSS::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_BOSS*	pInstance = NEW CGameObject_BOSS(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_BOSS");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_BOSS* CGameObject_BOSS::Clone(void* pArg)
{
	CGameObject_BOSS*	pInstance = NEW CGameObject_BOSS(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_BOSS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_BOSS::Free()
{
	__super::Free();
}
