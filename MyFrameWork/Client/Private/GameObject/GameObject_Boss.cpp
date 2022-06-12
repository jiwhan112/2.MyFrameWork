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
	// 모델 결정
	string str("npc_Ent.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	// Transform
	_float3 SpawnPos = mSpawnPostitionBOSS;
	Set_Position(SpawnPos);

	Set_LookDir(_float3(-1, 0, -1));

	_float size = 1.0f;
	mComTransform->Scaled(_float3(size, size, size));


	// 유닛 타입
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());
	Set_BehaviorMode(1);

	meUnitType = CGameObject_3D_Dynamic::UNIT_BOSS;
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
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_SKINPOSE));

	// 소켓
	// 보스 뼈 이름
	// LDigit11 / LDigit12 // 손목 / 손가락
	// LDigit21 / LDigit22 // 숫자가 크면 더 안쪽 뼈
	Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_1), "LDigit21");
	Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_2), "LDigit22");
	Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_3), "RDigit21");
	Add_Socket_NULL(TAGSOCKET(SOCKET_ANY_4), "RDigit22");

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

	// 대기
	CSequnce_WorldIdle* Seq_WorldIdle = CSequnce_WorldIdle::Create(this);
	CSequnce_WorldIdle::SEQWORLDIDLE worldIdle;
	worldIdle.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_SLEEPING;

	Seq_WorldIdle->Restart(&worldIdle);
	mComBehavior->Add_Seqeunce("WORLDIDLE", Seq_WorldIdle);
	mComBehavior->Select_Sequnce("WORLDIDLE");

	// 움직임
	CSequnce_WorldMove_Player* Seq_WorldMove = CSequnce_WorldMove_Player::Create(this);
	//	CSequnce_WorldMove::tag_SeqWorldMove DefaultDoorDesc;
	//	Seq_Door->Restart(&DefaultDoorDesc);
	mComBehavior->Add_Seqeunce("WORLDMOVE", Seq_WorldMove);


	// 패턴 1,2,3




	return S_OK;
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
