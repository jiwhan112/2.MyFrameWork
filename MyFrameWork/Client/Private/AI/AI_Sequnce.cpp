#include "stdafx.h"
#include "AI/AI_Sequnce.h"
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

CAction_DynamicBase * CSequnce_Base::Find_Action(CAction_DynamicBase::E_AcionID id, _int index)
{
	int cnt = 0;
	for (auto& obj: mListLeafNodes)
	{
		if (obj->Get_TREEID() == CNode_LeafTree::LEAFTREE_ID_ACTION)
		{
			CAction_DynamicBase* castObj = static_cast<CAction_DynamicBase*>(obj);
			if (castObj->Get_ACIONID() == id)
			{
				if (cnt == index)
					return castObj;
				else
					cnt++;
			}
		}
	}

	return nullptr;
}

void CSequnce_Base::Setup_TargetNode(CGameObject_3D_Dynamic * obj)
{
	// 내부에 같은 타겟 세팅
	for (auto & node : mListLeafNodes)
	{
		CNode_LeafTree::E_LEAFTREE_ID eId = node->Get_TREEID();

		switch (eId)
		{
		case Engine::CNode_LeafTree::LEAFTREE_ID_ACTION:
			static_cast<CAction_DynamicBase*>(node)->SetUp_Target(obj);
			break;
		//case Engine::CNode_LeafTree::LEAFTREE_ID_DECORATOR:
		//	static_cast<CDeco_DynamicBase*>(node)->SetUp_Target(obj);
		//	break;
		//case Engine::CNode_LeafTree::LEAFTREE_ID_SELECTER:
		//	static_cast<CSelect_DynamicBase*>(node)->SetUp_Target(obj);
		//	break;
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
	for (auto & node : mListLeafNodes)
	{
		FAILED_CHECK_NONERETURN(node->ReStart());
	}
}

void CSequnce_Base::Free()
{
	__super::Free();
	mDynamicObject = nullptr;
}


HRESULT CSequnce_IDLE::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);
	
	Set_SeqType(CNode_Seqeunce::E_SEQTYPE::SEQTYPE_IDLE);

	// 생성은 각 객체가 복사될 떄 한다.

	// IDLE 시퀀스를 길게 만듬
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

	dealyTime->Set_TimeMax(2.0f);
	dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	dealyAniDance->Set_Animation(CAnimatior::E_COMMON_ANINAME_DANCE);

	moveWalk->Set_AniType(CAction_MOVE::MOVE_ANI_WALK);
	moveWalk->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	moveWalk->Set_TimeMax(WalkTimeMax);

	PushBack_LeafNode(dealyTime->Clone());
	PushBack_LeafNode(dealyAniDance->Clone());
	PushBack_LeafNode(dealyAniIdle->Clone());
	PushBack_LeafNode(moveWalk->Clone());


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



	Safe_Release(dealyTime);
	Safe_Release(dealyAniIdle);
	Safe_Release(dealyAniDance);
	Safe_Release(moveWalk);
	Safe_Release(moveRun);

	// 객체 연결
	Setup_TargetNode(obj);
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
	__super::Free();
}

HRESULT CSequnce_MOVETARGET::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);
	Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);

	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;

	// CloneAction
	// 대기 -> 움직임 -> 애니메이션;
	CAction_DEALY*			dealyTime = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_MOVE_TARGET*	moveTarget = (CAction_MOVE_TARGET*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVETARGET));
	CAction_DEALY*			dealyAnimation = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));

	// Create Fall
	dealyTime->Set_TimeMax(0.2f);
	dealyAnimation->Set_Animation(CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_UP);

	PushBack_LeafNode(dealyTime->Clone());
	PushBack_LeafNode(moveTarget->Clone());
	PushBack_LeafNode(dealyAnimation->Clone());

	Safe_Release(dealyTime);
	Safe_Release(dealyAnimation);
	Safe_Release(moveTarget);

	// 객체 연결
	Setup_TargetNode(obj);

	return S_OK;
}

void CSequnce_MOVETARGET::Restart(void * SeqData)
{
	__super::Restart(SeqData);
	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQMOVETARGET));
	}
	// 초기화시 정의된 Key 값으로 객체에 넣는다. 

	// 대기 -> 움직임 -> 애니메이션;
	auto delay = Find_Action(CAction_DynamicBase::E_ACION_DEALY);
	((CAction_DEALY*)delay)->Set_TimeMax(mSeqData.Dealytime);


	auto targetMove = Find_Action(CAction_DynamicBase::E_ACION_MOVETARGET);
	((CAction_MOVE_TARGET*)targetMove)->Set_EasingFlag((EasingTypeID)mSeqData.eEasingID);
	((CAction_MOVE_TARGET*)targetMove)->Set_Postition(mSeqData.StartPosition, mSeqData.EndPosition);

	auto ani = Find_Action(CAction_DynamicBase::E_ACION_DEALY, 1);
	((CAction_DEALY*)ani)->Set_Animation((CAnimatior::E_COMMON_ANINAME)mSeqData.AniType);

}

CSequnce_MOVETARGET * CSequnce_MOVETARGET::Create(CGameObject_3D_Dynamic * targetobj)
{
	CSequnce_MOVETARGET* pInstance = NEW CSequnce_MOVETARGET();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_MOVETARGET");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSequnce_MOVETARGET::Free()
{
	__super::Free();
}



HRESULT CSequnce_TILE::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);
	Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);

	// IDLE에서 사용할 상태 정의
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;


	CAction_DEALY* dealyAniDig = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_MOVE* moveRun = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));
	CAction_Function* function1 = (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION));
	CAction_Function* function2 = (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION));

	_float WalkTimeMax = obj->Get_TimeForSpeed();
	_float RunTimeMax = WalkTimeMax * 0.5f;


	dealyAniDig->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);
	moveRun->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	moveRun->Set_TimeMax(RunTimeMax);
	moveRun->Set_Postition(CAction_MOVE::MOVE_POS_TILE);

	function1->Set_Funcion(CAction_Function::FUNCION_REMOVE_TILE);
	function2->Set_Funcion(CAction_Function::FUNCION_LOOK);

	// SetSeq
	// Tile: 타일을 채굴
	PushBack_LeafNode(moveRun->Clone());
	PushBack_LeafNode(function2->Clone());
	PushBack_LeafNode(dealyAniDig->Clone());
	PushBack_LeafNode(function1->Clone());


	Safe_Release(dealyAniDig);
	Safe_Release(moveRun);
	Safe_Release(function1);
	Safe_Release(function2);

	// 객체 연결
	Setup_TargetNode(obj);
	return S_OK;
}

void CSequnce_TILE::Restart(void * SeqData)
{
	__super::Restart(SeqData);
	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQTILE));
	}

}

CSequnce_TILE * CSequnce_TILE::Create(CGameObject_3D_Dynamic * targetobj)
{
	CSequnce_TILE* pInstance = NEW CSequnce_TILE();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_TILE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSequnce_TILE::Free()
{
	__super::Free();
	
}

HRESULT CSequnce_PICK::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);

	Set_SeqType(CNode_Seqeunce::SEQTYPE_LOOP);

	// IDLE에서 사용할 상태 정의
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;

	CAction_DEALY* dealyAnimation = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));

	// Set_Pick
	dealyAnimation->Set_Animation(CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_DRAG);
	PushBack_LeafNode(dealyAnimation->Clone());

	Safe_Release(dealyAnimation);

	// 객체 연결
	Setup_TargetNode(obj);

	return S_OK;
}

void CSequnce_PICK::Restart(void * SeqData)
{
	__super::Restart(SeqData);
	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQPICK));
	}
}

CSequnce_PICK * CSequnce_PICK::Create(CGameObject_3D_Dynamic * targetobj)
{
	CSequnce_PICK* pInstance = NEW CSequnce_PICK();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_PICK");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSequnce_PICK::Free()
{
	__super::Free();
	
}

