#include "stdafx.h"
#include "AI/AI_Action.h"

#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_Mine.h"

CAction_DynamicBase::CAction_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
	:CNode_Action(str), mDynamicObject(obj)
{
}

HRESULT CAction_DynamicBase::NativeConstruct()
{
	Init_Parent();
	return S_OK;
}

void CAction_DynamicBase::Free()
{
	mDynamicObject = nullptr;
}

CDecorator_DynamicBase::CDecorator_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
	:CNode_Decorator(str), mDynamicObject(obj)

{

}

void CDecorator_DynamicBase::Free()
{
	mDynamicObject = nullptr;
}



CAction_DEALY::CAction_DEALY(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str, obj)
{}

CAction_DEALY::CAction_DEALY(const CAction_DEALY & rhs)
	: CAction_DynamicBase(rhs)
{
	meDealyType = rhs.meDealyType;
	meAnimation = rhs.meAnimation;
	mTimeMax = rhs.mTimeMax;

}

HRESULT CAction_DEALY::NativeConstruct()
{
	__super::NativeConstruct();
	mCurrentTimer = 0;

	if (meDealyType == CAction_DEALY::DEALY_ANI)
	{
		// #BUG 애니메이션 연속시 블랜딩 떄문에 End 판정 받음
		mDynamicObject->Set_AniEnum(meAnimation);
	}

	return S_OK;
}

HRESULT CAction_DEALY::Action(_double timer)
{
	mCurrentTimer += timer;

	if (meDealyType == CAction_DEALY::DEALY_NONE)
	{
		if (mCurrentTimer > mTimeMax)
		{
			mIsEnd = true;
			mIsSucceed = true;
			return S_OK;
		}
	}
	else if (meDealyType == CAction_DEALY::DEALY_ANI)
	{
		_bool aniend = mDynamicObject->Get_ComModel()->Get_Animaitor()->Get_CurrentAniEnd();
		if (aniend)
		{
			mIsEnd = true;
			mIsSucceed = true;
			return S_OK;
		}
	}
	return S_OK;
}

void CAction_DEALY::Set_Animation(CAnimatior::E_COMMON_ANINAME e)
{
	meDealyType = CAction_DEALY::DEALY_ANI;
	meAnimation = e;
}

void CAction_DEALY::Set_TimeMax(_double timeMax)
{
	mTimeMax = timeMax;
}

CAction_DEALY * CAction_DEALY::Create(const char * str, CGameObject_3D_Dynamic* obj)
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(str, obj);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAction_DEALY");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_DEALY * CAction_DEALY::Clone(void * pArg)
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(*this);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAction_DEALY");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAction_DEALY::Free()
{
	__super::Free();
	
}

CAction_MOVE::CAction_MOVE(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str, obj)

{
}

CAction_MOVE::CAction_MOVE(const CAction_MOVE & rhs)
	: CAction_DynamicBase(rhs)
{
	mTimeMax = rhs.mTimeMax;
	meMoveAni = rhs.meMoveAni;
	meMoveType = rhs.meMoveType;

}

HRESULT CAction_MOVE::NativeConstruct()
{
	__super::NativeConstruct();

	if (mDynamicObject == nullptr)
		return E_FAIL;

	// 목표위치 설정
	// 1. 랜덤
	// 2. 특정 위치
	// 3. 마우스 위치
	switch (meMoveType)
	{
	case Client::CAction_MOVE::MOVE_POS_NEAR:
		if (mDynamicObject->FindPathRandAblePostition(3, &mGoalPosition) == false)
		{
			return S_FALSE;
		}
		break;
	case Client::CAction_MOVE::MOVE_POS_TILE:
		// 이미 탐색된 정보 활용
		mGoalPosition = mDynamicObject->Get_GoalPostiton();
		if (mGoalPosition == _float3())
			return E_FAIL;

		break;
	case Client::CAction_MOVE::MOVE_POS_PICK:
		mGoalPosition = GetSingle(CGameManager)->Get_PickPos();
		mDynamicObject->FindPathForCurrentNavi(mGoalPosition);
		break;
	case Client::CAction_MOVE::MOVE_POS_END:
		break;
	default:
		break;
	}


	mIsMoveNaviPath = true;
	mIsMoveCell = false;
	mCurrentTimer = 0;

	return S_OK;
}

HRESULT CAction_MOVE::Action(_double TimeDelta)
{
	// 패스 이동 후 End 체크
	if (mIsMoveNaviPath)
	{
		if (mIsMoveCell)
		{
			// 애니메이션 결정
			if (meMoveAni == CAction_MOVE::MOVE_RUN_ANI)
				mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_RUN);

			else if (meMoveAni == CAction_MOVE::MOVE_WALK_ANI)
				mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_WALK);

			_float3 newLook = _float3(mNextGoalPosition.x, mStartPosition.y, mNextGoalPosition.z);
			mDynamicObject->Get_ComTransform()->LookAt(newLook);
			mCurrentTimer += TimeDelta;
			_float3 CurrentPosition = GetSingle(CGameInstance)->Easing3(TYPE_Linear, mStartPosition, mNextGoalPosition, mCurrentTimer, mTimeMax);

			if (mCurrentTimer > mTimeMax)
			{
				mIsMoveCell = false;
			}
			if (_float3::Distance(CurrentPosition, mNextGoalPosition) < 0.3f)
			{
				mIsMoveCell = false;
			}
			mDynamicObject->Set_Position(CurrentPosition);
		}

		else
		{
			// 셀 설정
			if (mDynamicObject->Get_PathList_Frontpop(&mNextGoalPosition))
			{
				mStartPosition = mDynamicObject->Get_WorldPostition();
				mCurrentTimer = 0;
				mIsMoveCell = true;
			}

			else
			{
				mIsMoveNaviPath = false;
				mIsEnd = true;
				mIsSucceed = true;
				mDynamicObject->Set_Position(mGoalPosition);
				// mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE);

			}
		}
		return S_OK;
	}
}

CAction_MOVE * CAction_MOVE::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_MOVE* pInstance = NEW CAction_MOVE(str, obj);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_MOVE * CAction_MOVE::Clone(void * pArg)
{
	CAction_MOVE* pInstance = NEW CAction_MOVE(*this);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAction_MOVE::Free()
{
	__super::Free();
	
}

CAction_Function::CAction_Function(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str,obj)
{
}

CAction_Function::CAction_Function(const CAction_Function & rhs)
	: CAction_DynamicBase(rhs)
	, meFuncion(rhs.meFuncion)

{
}

HRESULT CAction_Function::NativeConstruct()
{
	__super::NativeConstruct();
	
	return S_OK;
}

HRESULT CAction_Function::Action(_double timer)
{
	switch (meFuncion)
	{
	case CAction_Function::FUNCION_REMOVE_TILE:
		((CGameObject_Mine*)mDynamicObject)->RemoveTile();
		break;
	case CAction_Function::FUNCION_LOOK:
		((CGameObject_Mine*)mDynamicObject)->LookTile();
	default:
		break;
	}

	mIsEnd = true;
	mIsSucceed = true;

	return S_OK;
}

CAction_Function * CAction_Function::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_Function* pInstance = NEW CAction_Function(str, obj);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAction_Function");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_Function * CAction_Function::Clone(void * pArg)
{
	CAction_Function* pInstance = NEW CAction_Function(*this);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAction_Function");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAction_Function::Free()
{
	__super::Free();
}
