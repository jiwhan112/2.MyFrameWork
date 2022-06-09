#include "stdafx.h"
#include "AI/AI_Sequnce.h"
#include "AI/AI_Action.h"
#include "AI/AI_Deco.h"
#include "AI/AI_Select.h"

#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_Mine.h"


HRESULT CSequnce_Base::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	mDynamicObject = obj;
	return S_OK;
}

void CSequnce_Base::Restart(void * SeqData)
{
	__super::Restart(SeqData);
	
}

void CSequnce_Base::Setup_TargetNode(CGameObject_3D_Dynamic * obj)
{
	// 내부에 같은 타겟 세팅
	for (auto & node : mListSequnce)
	{
		CNode_LeafTree::E_LEAFTREE_ID eId = node->Get_TREEID();

		switch (eId)
		{
		case Engine::CNode_LeafTree::LEAFTREE_ID_ACTION:
			static_cast<CAction_DynamicBase*>(node)->SetUp_Target(obj);
			break;
		case Engine::CNode_LeafTree::LEAFTREE_ID_DECORATOR:
			static_cast<CDeco_DynamicBase*>(node)->SetUp_Target(obj);
			break;
		case Engine::CNode_LeafTree::LEAFTREE_ID_SELECTER:
			static_cast<CSelect_DynamicBase*>(node)->SetUp_Target(obj);
			break;
		case Engine::CNode_LeafTree::LEAFTREE_ID_END:
			break;
		default:
			break;
		}
		
	}
}

void CSequnce_Base::Setup_RestartNodes()
{
	// 전체 초기화
	for (auto & node : mListSequnce)
	{
		FAILED_CHECK_NONERETURN(node->ReStart());
	}
}

void CSequnce_Base::Free()
{
	mDynamicObject = nullptr;
}


HRESULT CSequnce_IDLE::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	// IDLE 시퀀스를 길게 만듬

	// 생성용
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;

	// IDLE에서 사용할 상태 정의
	CAction_DEALY*	dealyTime = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_DEALY*	dealyAniIdle = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_DEALY*	dealyAniDance = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_MOVE*	moveWalk = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));
	CAction_MOVE*	moveRun = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));

	// Set Action
	_float WalkTimeMax = obj->Get_TimeForSpeed();
	_float RunTimeMax = WalkTimeMax * 0.5f;

	//dealyTime->Set_TimeMax(data);
	//dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	//dealyAniDance->Set_Animation(CAnimatior::E_COMMON_ANINAME_DANCE);

	//moveWalk->Set_AniType(CAction_MOVE::MOVE_ANI_WALK);
	//moveWalk->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	//moveWalk->Set_TimeMax(WalkTimeMax);

	//moveRun->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	//moveRun->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	//moveRun->Set_TimeMax(RunTimeMax);

	//// SetSeq
	//// IDLE 상태 설정

	//Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_IDLE1->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_IDLE1->PushBack_LeafNode(moveWalk->Clone());

	//Seq_IDLE2->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_IDLE2->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE2->PushBack_LeafNode(moveRun->Clone());

	//Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE3->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_IDLE3->PushBack_LeafNode(moveWalk->Clone());



	//Safe_Release(dealyTime);
	//Safe_Release(dealyAniIdle);
	//Safe_Release(dealyAniDance);
	//Safe_Release(moveWalk);
	//Safe_Release(moveRun);

	//// 객체 연결
	//Setup_TargetNode(obj);


	return S_OK;
}

void CSequnce_IDLE::Restart(void * SeqData)
{
	__super::Restart(SeqData);
	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQIDLE));
	}


}

CSequnce_IDLE * CSequnce_IDLE::Create(CGameObject_3D_Dynamic* targetobj)
{
	CSequnce_IDLE* pInstance = NEW CSequnce_IDLE();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_IDLE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSequnce_IDLE::Free()
{
}
