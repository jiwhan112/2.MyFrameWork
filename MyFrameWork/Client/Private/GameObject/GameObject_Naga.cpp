#include "stdafx.h"
#include "GameObject/GameObject_Naga.h"
#include "AI/AI_Action.h"

CGameObject_Naga::CGameObject_Naga(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_3D_Dynamic(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_DYNAMIC_ORC;
}

CGameObject_Naga::CGameObject_Naga(const CGameObject_Naga& rhs)
	: CGameObject_3D_Dynamic(rhs)
{

}

HRESULT CGameObject_Naga::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_Naga::NativeConstruct(void* pArg)
{
	string str("crea_Orc.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
	Set_LoadModelDynamicDESC(mModelDesc);
	return S_OK;
}


HRESULT CGameObject_Naga::Init_Unit()
{

	return S_OK;
}

HRESULT CGameObject_Naga::Init_AI()
{
	Init_AI_Default();
	return S_OK;

}


HRESULT CGameObject_Naga::Init_AI_Default()
{
	//// AI ¼¼ºÎ ±¸Çö

	//// IDLE ½ÃÄö½º 3°³
	//CNode_Seqeunce* Seq_IDLE1 = CNode_Seqeunce::Create();
	//CNode_Seqeunce* Seq_IDLE2 = CNode_Seqeunce::Create();
	//CNode_Seqeunce* Seq_IDLE3 = CNode_Seqeunce::Create();

	//// Set Action
	//CAction_DEALY* dealyTime = CAction_DEALY::Create("DealyTime", this);
	//CAction_DEALY* dealyAniIdle = CAction_DEALY::Create("DealyIdle", this);
	//CAction_DEALY* dealyAniDig = CAction_DEALY::Create("DealyDig", this);
	//CAction_DEALY* dealyAniDance = CAction_DEALY::Create("DelayDance", this);

	//dealyTime->Set_TimeMax(3.0f);
	//dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	//dealyAniDig->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);
	//dealyAniDance->Set_Animation(CAnimatior::E_COMMON_ANINAME_DANCE);

	//CAction_MOVE* MoveWalk = CAction_MOVE::Create("Walk", this);
	//CAction_MOVE* MoveRun = CAction_MOVE::Create("Run", this);
	//MoveWalk->Set_AniType(CAction_MOVE::MOVE_WALK_ANI);
	//MoveWalk->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	//MoveWalk->Set_TimeMax(0.6f);
	//MoveRun->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	//MoveRun->Set_TimeMax(0.3f);

	//// SetSeq
	//// IDLE1: µ¹¾Æ´Ù´Ô
	//Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_IDLE1->PushBack_LeafNode(MoveWalk->Clone());

	//// IDLE2: ¶Ù¾î´Ù´Ô
	//Seq_IDLE2->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_IDLE2->PushBack_LeafNode(dealyAniDig->Clone());
	//Seq_IDLE2->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE2->PushBack_LeafNode(MoveRun->Clone());

	//// IDLE3: ÃãÃß±â
	//Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE3->PushBack_LeafNode(MoveWalk->Clone());

	//mComBehavior->Add_Seqeunce("IDLE1", Seq_IDLE1);
	//mComBehavior->Add_Seqeunce("IDLE2", Seq_IDLE2);
	//mComBehavior->Add_Seqeunce("IDLE3", Seq_IDLE3);
	//mComBehavior->Select_Sequnce("IDLE1");

	//// Release
	//Safe_Release(dealyTime);
	//Safe_Release(dealyAniIdle);
	//Safe_Release(dealyAniDig);
	//Safe_Release(dealyAniDance);

	//Safe_Release(MoveWalk);
	//Safe_Release(MoveRun);
	return S_OK;
}


CGameObject_Naga * CGameObject_Naga::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Naga*	pInstance = NEW CGameObject_Naga(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Naga");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Naga* CGameObject_Naga::Clone(void* pArg)
{
	CGameObject_Naga*	pInstance = NEW CGameObject_Naga(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Naga");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Naga::Free()
{
	__super::Free();
	//	Safe_Release(mComNaviMesh);
}
