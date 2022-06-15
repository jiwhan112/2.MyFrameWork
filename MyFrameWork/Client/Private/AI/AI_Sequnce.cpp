#include "stdafx.h"
#include "AI/AI_Sequnce.h"
#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_Mine.h"

void Set_Static_Animation(CGameObject_3D_Dynamic * obj, CAnimatior::E_COMMON_ANINAME e, int index)
{
	// 애니메이션 세팅용 전역 함수
	obj->Set_AniEnum(e, index);
}


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

CDeco_DynamicBase * CSequnce_Base::Find_Deco(CDeco_DynamicBase::E_DecoID id, _int index)
{
	int cnt = 0;
	for (auto& obj : mListLeafNodes)
	{
		if (obj->Get_TREEID() == CNode_LeafTree::LEAFTREE_ID_DECORATOR)
		{
			CDeco_DynamicBase* castObj = static_cast<CDeco_DynamicBase*>(obj);
			if (castObj->Get_DECOID() == id)
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
		case Engine::CNode_LeafTree::LEAFTREE_ID_DECORATOR:
			static_cast<CDeco_DynamicBase*>(node)->SetUp_Target(obj);
			break;
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
	
	Set_SeqType(CNode_Seqeunce::E_SEQTYPE::SEQTYPE_IDLE0);

	// 생성은 각 객체가 복사될 떄 한다.

	// IDLE 시퀀스를 길게 만듬
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;

	// IDLE에서 사용할 상태 정의
	// 딜레이 -> 애니메이션 -> PathMove
	CAction_DEALY*	dealyAni = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_MOVE*	pathMove = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));

	_float defaultSpeed = obj->Get_TimeForSpeed();

	dealyAni->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);

	pathMove->Set_AniType(CAction_MOVE::MOVE_ANI_WALK);
	pathMove->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	pathMove->Set_TimeMax(defaultSpeed);

	PushBack_LeafNode(dealyAni->Clone());
	PushBack_LeafNode(pathMove->Clone());

	Safe_Release(dealyAni);
	Safe_Release(pathMove);

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

	//auto delay = Find_Action(CAction_DynamicBase::E_ACION_DEALY);
	//NULL_CHECK_BREAK(delay);
	//int randTime = CHelperClass::RandomInt(mSeqData.MinTime, mSeqData.MaxTime);
	//((CAction_DEALY*)delay)->Set_TimeMax(randTime);

	auto ani = Find_Action(CAction_DynamicBase::E_ACION_DEALY);
	NULL_CHECK_BREAK(ani);
	((CAction_DEALY*)ani)->Set_Animation(mSeqData.AniType);


	auto pathMove = Find_Action(CAction_DynamicBase::E_ACION_MOVEPATH);
	NULL_CHECK_BREAK(pathMove);
	((CAction_MOVE*)pathMove)->Set_Easing((EasingTypeID)mSeqData.mMoveEasingId);
	((CAction_MOVE*)pathMove)->Set_AniType(CAction_MOVE::MOVE_ANI_WALK);
	((CAction_MOVE*)pathMove)->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);

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
	CAction_Function*		func	= (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION));

	// Create Fall
	dealyTime->Set_TimeMax(0.2f);
	dealyAnimation->Set_Animation(CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_UP);

	PushBack_LeafNode(dealyTime->Clone());
	PushBack_LeafNode(moveTarget->Clone());
	PushBack_LeafNode(dealyAnimation->Clone());
	PushBack_LeafNode(dealyAnimation->Clone());
	PushBack_LeafNode(func->Clone());

	Safe_Release(dealyTime);
	Safe_Release(dealyAnimation);
	Safe_Release(moveTarget);
	Safe_Release(func);

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
	NULL_CHECK_BREAK(delay);
	((CAction_DEALY*)delay)->Set_TimeMax(mSeqData.Dealytime);


	auto targetMove = Find_Action(CAction_DynamicBase::E_ACION_MOVETARGET);
	NULL_CHECK_BREAK(targetMove);
	((CAction_MOVE_TARGET*)targetMove)->Set_EasingFlag(mSeqData.EasingID);
	((CAction_MOVE_TARGET*)targetMove)->Set_Postition(mSeqData.StartPosition, mSeqData.EndPosition);
	((CAction_MOVE_TARGET*)targetMove)->Set_MoveTimeMax(mSeqData.TimeMax);

	auto ani = Find_Action(CAction_DynamicBase::E_ACION_DEALY, 1);
	NULL_CHECK_BREAK(ani);
	((CAction_DEALY*)ani)->Set_Animation((CAnimatior::E_COMMON_ANINAME)mSeqData.AniType);

	auto ani2 = Find_Action(CAction_DynamicBase::E_ACION_DEALY, 2);
	NULL_CHECK_BREAK(ani2);
	((CAction_DEALY*)ani2)->Set_Animation((CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_IDLE));

	auto func = Find_Action(CAction_DynamicBase::E_ACION_FUNCTION);
	NULL_CHECK_BREAK(func);
	((CAction_Function*)func)->Set_Funcion(mSeqData.eExitFunc);

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

	// 타일 움직임 -> 애니메이션 -> 함수 실행;
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;


	CAction_DEALY* ani = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_MOVE* movepath = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));
	CAction_Function* funcion= (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION));

	ani->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);
	movepath->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	movepath->Set_TimeMax(0.6f);
	movepath->Set_Postition(CAction_MOVE::MOVE_POS_TILE);



	funcion->Set_Funcion(CAction_Function::FUNCION_LOOKTILE);

	// SetSeq
	// Tile: 타일을 채굴
	PushBack_LeafNode(movepath->Clone());
	PushBack_LeafNode(funcion->Clone());
	PushBack_LeafNode(ani->Clone());
	funcion->Set_Funcion(CAction_Function::FUNCION_REMOVE_TILE);
	PushBack_LeafNode(funcion->Clone());


	Safe_Release(ani);
	Safe_Release(movepath);
	Safe_Release(funcion);


	// 객체 연결
	Setup_TargetNode(obj);
	return S_OK;
}

void CSequnce_TILE::Restart(void * SeqData)
{
	__super::Restart(SeqData);
	// 대기 -> 움직임 -> 애니메이션;

	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQTILE));
	}

	auto pathmove = Find_Action(CAction_DynamicBase::E_ACION_MOVEPATH);
	NULL_CHECK_BREAK(pathmove);
	((CAction_MOVE*)pathmove)->Set_TimeMax(mSeqData.Runtime);

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

	auto ani = Find_Action(CAction_DynamicBase::E_ACION_DEALY);
	NULL_CHECK_BREAK(ani);
	((CAction_DEALY*)ani)->Set_Animation(mSeqData.AniType);
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

HRESULT CSequnce_WorldIdle::NativeConstruct(CGameObject_3D_Dynamic * obj)
{

	__super::NativeConstruct(obj);

	Set_SeqType(CNode_Seqeunce::SEQTYPE_IDLE1);

	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;

	// 월드의 IDLE 상태 
	// 애니메이션 -> 주변탐색 -> 적 발견하면 공격으로 변경

	CAction_DEALY* ani = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));

	// Set_Pick
	mSeqData.AniType = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_IDLE;
	ani->Set_Animation(mSeqData.AniType);
	PushBack_LeafNode(ani->Clone());

	Safe_Release(ani);

	// 객체 연결
	Setup_TargetNode(obj);
	return S_OK;
}

void CSequnce_WorldIdle::Restart(void * SeqData)
{
	__super::Restart(SeqData);
	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQWORLDIDLE));
	}

	auto ani = Find_Action(CAction_DynamicBase::E_ACION_DEALY);
	NULL_CHECK_BREAK(ani);
	((CAction_DEALY*)ani)->Set_Animation(mSeqData.AniType);
}

CSequnce_WorldIdle * CSequnce_WorldIdle::Create(CGameObject_3D_Dynamic * targetobj)
{
	CSequnce_WorldIdle* pInstance = NEW CSequnce_WorldIdle();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_WorldIdle");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSequnce_WorldIdle::Free()
{
	__super::Free();

}

HRESULT CSequnce_WorldMove_Player::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);

	Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);

	// 월드에서는 싸움이 걸리면 싸워야한다.
	// 타일 움직임 -> 애니메이션 -> 함수 실행;
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;


//	CAction_DEALY* ani = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_MOVE* movepath = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));
//	CAction_Function* funcion = (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION));

//	ani->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	movepath->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	movepath->Set_TimeMax(obj->Get_TimeForSpeed());
	movepath->Set_Postition(CAction_MOVE::MOVE_POS_PICK);


	PushBack_LeafNode(movepath->Clone());
	Safe_Release(movepath);
//	Safe_Release(funcion);

	// 객체 연결
	Setup_TargetNode(obj);

	return S_OK;
}

void CSequnce_WorldMove_Player::Restart(void * SeqData)
{
	__super::Restart(SeqData);

	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQWORLDMOVE_PlAYER));
	}
	

	//auto movepath = Find_Action(CAction_DynamicBase::E_ACION_MOVEPATH);
	//NULL_CHECK_BREAK(movepath);
	//((CAction_MOVE*)movepath)->Set_GoalPostiton(mSeqData.GoalPosition);

}

CSequnce_WorldMove_Player * CSequnce_WorldMove_Player::Create(CGameObject_3D_Dynamic * targetobj)
{
	CSequnce_WorldMove_Player* pInstance = NEW CSequnce_WorldMove_Player();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_WorldMoveS");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSequnce_WorldMove_Player::Free()
{
	__super::Free();
	
}

HRESULT CSequnce_WorldMove_Enemy::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);

	Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);

	// 월드에서는 싸움이 걸리면 싸워야한다.
	// 타일 움직임 -> 애니메이션 -> 함수 실행;
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;


	CAction_MOVE* movepath = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));
	// 카운터를 올림
	CAction_Function* funcion = (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION)); 


	movepath->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	movepath->Set_TimeMax(obj->Get_TimeForSpeed());
	movepath->Set_Postition(CAction_MOVE::MOVE_POS_GOALPOS);

	funcion->Set_Funcion(CAction_Function::FUNCION_ENEMY_MOVENEXT);

	mSeqData.GoalPostition = _float3();
	PushBack_LeafNode(movepath->Clone());
	PushBack_LeafNode(funcion->Clone());

	Safe_Release(movepath);
	Safe_Release(funcion);

	// 객체 연결
	Setup_TargetNode(obj);

	return S_OK;
}

void CSequnce_WorldMove_Enemy::Restart(void * SeqData)
{
	__super::Restart(SeqData);

	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQWORLDMOVE_ENEMY));
	}

	 auto move1 = Find_Action(CAction_DynamicBase::E_ACION_MOVEPATH);
	 NULL_CHECK_BREAK(move1);
	 ((CAction_MOVE*)move1)->Set_GoalPostiton(mSeqData.GoalPostition);
}

CSequnce_WorldMove_Enemy * CSequnce_WorldMove_Enemy::Create(CGameObject_3D_Dynamic * targetobj)
{
	CSequnce_WorldMove_Enemy* pInstance = NEW CSequnce_WorldMove_Enemy();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_WorldMove_Enemy");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSequnce_WorldMove_Enemy::Free()
{
	__super::Free();

}

HRESULT CSequnce_WorldAttack_Player::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);

	// 공격은 적이 죽을떄까지 루프를 돈다.
	Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);

	// 싸움이 걸리면 싸운다.
	CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	if (ComBehavior == nullptr)
		return E_FAIL;

	// 타겟에 다가간다 -> 싸우기 -> 피격 
	CAction_MOVE* movepath = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));
	CAction_DEALY* ani = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	CAction_Function* attackFunc = (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION));
	// 조건 판단
	CDeco_Minus* MinusDeco = (CDeco_Minus*)ComBehavior->Clone_Leaf(TAGDECO(DECO_MINUS));


	movepath->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	movepath->Set_TimeMax(obj->Get_TimeForSpeed());
	movepath->Set_Postition(CAction_MOVE::MOVE_POS_TARGET);

	ani->Set_Animation(CAnimatior::E_COMMON_ANINAME_MELEE);
	attackFunc->Set_Funcion(CAction_Function::FUNCION_ATTACK);

	MinusDeco->Set_Value(nullptr);

	PushBack_LeafNode(movepath->Clone());
	PushBack_LeafNode(ani->Clone());
	PushBack_LeafNode(attackFunc->Clone());
	PushBack_LeafNode(MinusDeco->Clone());


	Safe_Release(movepath);
	Safe_Release(ani);
	Safe_Release(attackFunc);
	Safe_Release(MinusDeco);

	// 객체 연결
	Setup_TargetNode(obj);
	return S_OK;
}

void CSequnce_WorldAttack_Player::Restart(void * SeqData)
{
	__super::Restart(SeqData);

	if (SeqData)
	{
		memcpy(&mSeqData, SeqData, sizeof(SEQWORLDATTACK_PLY));

		auto movepath = Find_Action(CAction_DynamicBase::E_ACION_MOVEPATH);
		NULL_CHECK_BREAK(movepath);
		((CAction_MOVE*)movepath)->Set_MoveTarget(mSeqData.Target);

		auto hpDeco = Find_Deco(CDeco_DynamicBase::E_DECI_MINUS);
		NULL_CHECK_BREAK(hpDeco);
		((CDeco_Minus*)hpDeco)->Set_Value(&(mSeqData.Target->Get_Hp()));

	}
}

CSequnce_WorldAttack_Player * CSequnce_WorldAttack_Player::Create(CGameObject_3D_Dynamic * targetobj)
{
	CSequnce_WorldAttack_Player* pInstance = NEW CSequnce_WorldAttack_Player();

	if (FAILED(pInstance->NativeConstruct(targetobj)))
	{
		MSGBOX("Failed to Created CSequnce_WorldAttack_Player");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSequnce_WorldAttack_Player::Free()
{
	__super::Free();
	

}

HRESULT CSequnce_WorldAutoAttack::NativeConstruct(CGameObject_3D_Dynamic * obj)
{
	__super::NativeConstruct(obj);

	//// 공격은 적이 죽을떄까지 루프를 돈다.
	//Set_SeqType(CNode_Seqeunce::SEQTYPE_ONETIME);

	//// 싸움이 걸리면 싸운다.
	//CBehaviorTree* ComBehavior = obj->Get_ComBehavior();
	//if (ComBehavior == nullptr)
	//	return E_FAIL;

	//// 타겟에 다가간다 -> 싸우기 -> 피격 
	//CAction_MOVE* movepath = (CAction_MOVE*)ComBehavior->Clone_Leaf(TAGAI(AI_MOVE));
	//CAction_DEALY* ani = (CAction_DEALY*)ComBehavior->Clone_Leaf(TAGAI(AI_DEALY));
	//CAction_Function* attackFunc = (CAction_Function*)ComBehavior->Clone_Leaf(TAGAI(AI_FUNCTION));
	//// 조건 판단
	//CDeco_Minus* MinusDeco = (CDeco_Minus*)ComBehavior->Clone_Leaf(TAGDECO(DECO_MINUS));


	//movepath->Set_AniType(CAction_MOVE::MOVE_ANI_RUN);
	//movepath->Set_TimeMax(obj->Get_TimeForSpeed());
	//movepath->Set_Postition(CAction_MOVE::MOVE_POS_TARGET);

	//ani->Set_Animation(CAnimatior::E_COMMON_ANINAME_MELEE);
	//attackFunc->Set_Funcion(CAction_Function::FUNCION_ATTACK);

	//testMinusDeco->Set_Value(nullptr);

	//PushBack_LeafNode(movepath->Clone());
	//PushBack_LeafNode(ani->Clone());
	//PushBack_LeafNode(attackFunc->Clone());
	//PushBack_LeafNode(testMinusDeco->Clone());


	//Safe_Release(movepath);
	//Safe_Release(ani);
	//Safe_Release(attackFunc);
	//Safe_Release(testMinusDeco);

	//// 객체 연결
	//Setup_TargetNode(obj);
	return S_OK;
}

void CSequnce_WorldAutoAttack::Restart(void * SeqData)
{
}

CSequnce_WorldAutoAttack * CSequnce_WorldAutoAttack::Create(CGameObject_3D_Dynamic * targetobj)
{
	return nullptr;
}

void CSequnce_WorldAutoAttack::Free()
{
}
