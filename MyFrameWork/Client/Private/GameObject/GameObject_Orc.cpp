#include "stdafx.h"
#include "GameObject/GameObject_Orc.h"
#include "AI/AI_Action.h"


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
	/*string str("crea_Orc.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);

	_float size = 0.8f;

	mComTransform->Scaled(_float3(size, size, size));

	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();

	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE));*/
	return S_OK;
}

HRESULT CGameObject_Orc::Init_AI()
{
	
	Init_AI_Default();

	return S_OK;
}


HRESULT CGameObject_Orc::Init_AI_Default()
{	// AI ¼¼ºÎ ±¸Çö

	// IDLE ½ÃÄö½º 3°³
	CNode_Seqeunce* Seq_IDLE1 = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_IDLE2 = CNode_Seqeunce::Create();

	// Set Action
	CAction_DEALY* dealyTime = CAction_DEALY::Create("DealyTime", this);
	CAction_DEALY* dealyAniIdle = CAction_DEALY::Create("DealyIdle", this);
	CAction_DEALY* dealyMelee = CAction_DEALY::Create("DealyMelee", this);

	dealyTime->Set_TimeMax(3.0f);
	dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	dealyMelee->Set_Animation(CAnimatior::E_COMMON_ANINAME_MELEE);

	CAction_MOVE* MoveWalk = CAction_MOVE::Create("Walk", this);
	CAction_MOVE* MoveRun = CAction_MOVE::Create("Run", this);
	MoveWalk->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	MoveWalk->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	MoveWalk->Set_TimeMax(1.2f);
	MoveRun->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	MoveRun->Set_TimeMax(0.8f);

	// SetSeq
	// IDLE1: µ¹¾Æ´Ù´Ô
	Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE1->PushBack_LeafNode(MoveWalk->Clone());

	// IDLE2: ¶Ù¾î´Ù´Ô
	Seq_IDLE2->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE2->PushBack_LeafNode(dealyMelee->Clone());
	Seq_IDLE2->PushBack_LeafNode(MoveRun->Clone());

	mComBehavior->Add_Seqeunce("IDLE1", Seq_IDLE1);
	mComBehavior->Add_Seqeunce("IDLE2", Seq_IDLE2);
	mComBehavior->Select_Sequnce("IDLE1");

	// Release
	Safe_Release(dealyTime);
	Safe_Release(dealyAniIdle);
	Safe_Release(dealyMelee);

	Safe_Release(MoveWalk);
	Safe_Release(MoveRun);
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
