#include "stdafx.h"
#include "GameObject/GameObject_Mine.h"
#include "AI/AI_Action.h"

CGameObject_Mine::CGameObject_Mine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_MINE;
}

CGameObject_Mine::CGameObject_Mine(const CGameObject_Mine& rhs)
	: CGameObject_3D_Dynamic(rhs)
{
	
}

HRESULT CGameObject_Mine::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Mine::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	if (strlen(mModelDesc.mModelName) < 2)
	{
		string str("crea_Snot_a.fbx");
		strcpy_s(mModelDesc.mModelName, str.c_str());
	}
	Set_LoadModelDynamicDESC(mModelDesc);

	return S_OK;
}


HRESULT CGameObject_Mine::Init_Unit()
{
	_float size = 0.6f;

	mComTransform->Scaled(_float3(size, size, size));

	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
	desc.mSize = _float3(size, size, size);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();

	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE));	
	return S_OK;
}

HRESULT CGameObject_Mine::Init_AI()
{
	// AI ¼¼ºÎ ±¸Çö

	// IDLE ½ÃÄö½º 3°³
	CNode_Seqeunce* Seq_IDLE1 = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_IDLE2 = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_IDLE3 = CNode_Seqeunce::Create();

	// Set Action
	CAction_DEALY* dealyTime = CAction_DEALY::Create("DealyTime", this);
	CAction_DEALY* dealyAniIdle = CAction_DEALY::Create("DealyIdle", this);
	CAction_DEALY* dealyAniDig = CAction_DEALY::Create("DealyDig", this);
	CAction_DEALY* dealyAniDance = CAction_DEALY::Create("DelayDance", this);

	dealyTime->Set_TimeMax(3.0f);
	dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	dealyAniDig->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);
	dealyAniDance->Set_Animation(CAnimatior::E_COMMON_ANINAME_DANCE);

	CAction_MOVE* MoveWalk = CAction_MOVE::Create("walk", this, _float3(0, 0, 0), 0.6f);
	CAction_MOVE* MoveRun = CAction_MOVE::Create("run", this, _float3(0, 0, 0), 0.3f);
	MoveWalk->Set_AniType(CAction_MOVE::MOVE_WALK_ANI);
	MoveRun->Set_AniType(CAction_MOVE::MOVE_RUN_ANI);

	// SetSeq
	// IDLE1: µ¹¾Æ´Ù´Ô
	Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE1->PushBack_LeafNode(MoveWalk->Clone());

	// IDLE2: ¶Ù¾î´Ù´Ô
	Seq_IDLE2->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE2->PushBack_LeafNode(dealyAniDig->Clone());
	Seq_IDLE2->PushBack_LeafNode(dealyAniDance->Clone());
	Seq_IDLE2->PushBack_LeafNode(MoveRun->Clone());

	// IDLE3: ÃãÃß±â
	Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	Seq_IDLE3->PushBack_LeafNode(MoveWalk->Clone());

	mComBehavior->Add_Seqeunce("IDLE1", Seq_IDLE1);
	mComBehavior->Add_Seqeunce("IDLE2", Seq_IDLE2);
	mComBehavior->Add_Seqeunce("IDLE3", Seq_IDLE3);
	mComBehavior->Select_Sequnce("IDLE1");

	// Release
	Safe_Release(dealyTime);
	Safe_Release(dealyAniIdle);
	Safe_Release(dealyAniDig);
	Safe_Release(dealyAniDig);

	Safe_Release(MoveWalk);
	Safe_Release(MoveRun);
	return S_OK;
}


CGameObject_Mine * CGameObject_Mine::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Mine*	pInstance = NEW CGameObject_Mine(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Mine* CGameObject_Mine::Clone(void* pArg)
{
	CGameObject_Mine*	pInstance = NEW CGameObject_Mine(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Mine::Free()
{
	__super::Free();
}
