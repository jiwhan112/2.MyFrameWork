#include "stdafx.h"
#include "GameObject/GameObject_Orc.h"
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
