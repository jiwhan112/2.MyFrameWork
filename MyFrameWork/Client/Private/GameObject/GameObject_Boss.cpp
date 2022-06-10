#include "stdafx.h"
#include "GameObject/GameObject_BOSS.h"

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
	// �� ����
	string str("npc_Ent.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	// ��ġ
	_float3 SpawnPos = mSpawnPostitionBOSS;
	Set_Position(SpawnPos);

	// ���� Ÿ��
	Set_MapSetting(CGameObject_3D_Dynamic::MAPTYPE_WORLD);
	mCurrentNavi->Move_OnNavigation(Get_WorldPostition());

	meUnitType = CGameObject_3D_Dynamic::UNIT_BOSS;
	meTickType = CGameObject_3D_Dynamic::TICK_TYPE_NONE;
	mTimeForSpeed = 0.5f;
	mRotSpeed = 10.0f;

	// ���� ũ��
	_float size = 1.0f;
	mComTransform->Scaled(_float3(size, size, size));

	// �浹 ����
	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();

	// �ִϸ��̼�
	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_SKINPOSE));

	// ����
	// Add_Socket("crea_SnotPickaxe.fbx", "RArmDigit31");

	return S_OK;
}

HRESULT CGameObject_BOSS::Init_AI()
{
	FAILED_CHECK(__super::Init_AI());
	FAILED_CHECK(Init_AI_Boss());
}

HRESULT CGameObject_BOSS::Init_AI_Boss()
{

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
