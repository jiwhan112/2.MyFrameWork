#include "stdafx.h"
// 시퀀스 해더 XXX
#include "AI/AI_Action.h"

#include "GameObject/GameObject_3D_Dynamic.h"
#include "GameObject/GameObject_Mine.h"
#include "GameObject/GameObject_Enemy.h"

CAction_DynamicBase::CAction_DynamicBase(const char * str, CGameObject_3D_Dynamic * obj)
	:CNode_Action(str), mDynamicObject(obj)
{
}

CAction_DynamicBase::CAction_DynamicBase(const CAction_DynamicBase & rhs)
	:CNode_Action(rhs)
{
	mDynamicObject = rhs.mDynamicObject;
}

HRESULT CAction_DynamicBase::ReStart(void* pArg)
{
	Init_Parent();
	return S_OK;
}

void CAction_DynamicBase::Free()
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
	lstrcpy(mStr, rhs.mStr);
	meAcionID = CAction_DynamicBase::E_ACION_DEALY;
}

HRESULT CAction_DEALY::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);
	mCurrentTimer = 0;
	mOneTime = false;

	if (meDealyType == CAction_DEALY::DEALY_ANI)
	{
		FAILED_CHECK(mDynamicObject->Set_AniEnum(meAnimation, mAniIndex, mIsFoward));
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
			End_Succed();
			return S_OK;
		}
	}
	else if (meDealyType == CAction_DEALY::DEALY_ANI)
	{
		if (mCurrentTimer > mTimeMax && mOneTime == false)
		{
			mOneTime = true;
			PLAYGAMESOUND(mStr, CHANNEL_EFFECT, SOUNDVOL_EFFECT);
		}

		_bool aniend = mDynamicObject->Get_ComModel()->Get_Animaitor()->Get_CurrentAniEnd();
		if (aniend)
		{
			End_Succed();
			return S_OK;
		}
	}
	return S_OK;
}

void CAction_DEALY::Set_Animation(CAnimatior::E_COMMON_ANINAME e,int index,_bool foward)
{
	meDealyType = CAction_DEALY::DEALY_ANI;
	meAnimation = e;
	mAniIndex = index;
	mIsFoward = foward;
}

void CAction_DEALY::Set_TimeMax(_double timeMax)
{
	mTimeMax = timeMax;
}

CAction_DEALY * CAction_DEALY::Create(const char * str, CGameObject_3D_Dynamic* obj)
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_DEALY * CAction_DEALY::Clone()
{
	CAction_DEALY* pInstance = NEW CAction_DEALY(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
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

CAction_MOVE::CAction_MOVE(const CAction_MOVE&  rhs)
	: CAction_DynamicBase(rhs)
{
	mTimeMax = rhs.mTimeMax;
	meMoveAni = rhs.meMoveAni;
	meMoveType = rhs.meMoveType;
	meAcionID = CAction_DynamicBase::E_ACION_MOVEPATH;


}

HRESULT CAction_MOVE::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);

	mStartPosition = mDynamicObject->Get_WorldPostition();

	// 목표위치 설정
	// 1. 랜덤
	// 2. 특정 위치
	// 3. 마우스 위치
	switch (meMoveType)
	{
	case Client::CAction_MOVE::MOVE_POS_NEAR:
		// 갈 수 있는 타일 랜덤 탐색
		if (mDynamicObject->FindPathRandDungeonAblePostition(10, 50, &mGoalPosition) == false)
		{
			// 탐색실패 
			mGoalPosition = mStartPosition;
		}
		break;
	case Client::CAction_MOVE::MOVE_POS_PICK:
		mGoalPosition = GetSingle(CGameManager)->Get_PickPos();
		mDynamicObject->FindPathForCurrentNavi(mGoalPosition);
		break;
	case Client::CAction_MOVE::MOVE_POS_GOALPOS:
		// Set_Goal을 넣어주고 탐색
		mDynamicObject->FindPathForCurrentNavi(mGoalPosition);
		break;
	case Client::CAction_MOVE::MOVE_POS_TARGET:
		if (mMoveTarget == nullptr)
			End_Succed();
		mGoalPosition = mMoveTarget->Get_WorldPostition();
		mDynamicObject->FindPathForCurrentNavi(mGoalPosition);
		break;
	case Client::CAction_MOVE::MOVE_POS_END:
		break;
	default:
		break;
	}
	bool isNear = (0.5f > _float3::Distance(mGoalPosition, mStartPosition)) ? true : false;
	if (isNear)
		End_Succed();
	else
	{
		mIsMoveNaviPath = true;
		mIsMoveCell = false;
		mCurrentTimer = 0;
	}

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
			if (meMoveAni == CAction_MOVE::MOVE_ANI_RUN)
				mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_RUN, 0);

			else if (meMoveAni == CAction_MOVE::MOVE_ANI_WALK)
				mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_WALK, 0);

			mDynamicObject->Set_RotationFlag(mNextGoalPosition);

			mCurrentTimer += TimeDelta;
			_float3 CurrentPosition = GetSingle(CGameInstance)->Easing3(meEasingID, mStartPosition, mNextGoalPosition, mCurrentTimer, mTimeMax);

			// Test
			//if (_float3::Distance(mStartPosition, mNextGoalPosition) < 5.0f)
			//{
			//	mDynamicObject->Set_Position(CurrentPosition);
			//	mIsMoveCell = false;

			//}
		
			if (mCurrentTimer > mTimeMax)
			{
				mIsMoveCell = false;
				mDynamicObject->Set_Position(CurrentPosition);
			}
			else
				mDynamicObject->Set_Position(CurrentPosition);

			//if (meMoveType == CAction_MOVE::MOVE_POS_TARGET)
			//{
			//	if (_float3::Distance(CurrentPosition, mNextGoalPosition) < 0.3f)
			//	{
			//		// mDynamicObject->Set_Position(mMoveTarget->GetAttackPos());
			//		mIsMoveCell = false;
			//	}

			//}

			//else
			//{
			//	if (mCurrentTimer > mTimeMax)
			//	{
			//		mIsMoveCell = false;
			//	}
			//	mDynamicObject->Set_Position(CurrentPosition);

			//}
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
				End_Succed();
			//	mDynamicObject->Set_Position(mGoalPosition);
				// mDynamicObject->Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE);

			}
		}
		return S_OK;
	}
}

CAction_MOVE * CAction_MOVE::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_MOVE* pInstance = NEW CAction_MOVE(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_MOVE * CAction_MOVE::Clone()
{
	CAction_MOVE* pInstance = NEW CAction_MOVE(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
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
	meAcionID = CAction_DynamicBase::E_ACION_FUNCTION;
}

HRESULT CAction_Function::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);
	
	return S_OK;
}

HRESULT CAction_Function::Action(_double timer)
{
	switch (meFuncion)
	{
	case CAction_Function::FUNCION_NONE:
		break;
	case CAction_Function::FUNCION_REMOVE_TILE:
		((CGameObject_Mine*)mDynamicObject)->RemoveTile();
		break;
	case CAction_Function::FUNCION_LOOKTILE:
		((CGameObject_Mine*)mDynamicObject)->LookTile();
		break;
	case CAction_Function::FUNCION_ATTACK:
		mDynamicObject->AttackFunc();
		break;
	case CAction_Function::FUNCION_DIE:
	//	mDynamicObject->Set_Dead();
		break;
	case CAction_Function::FUNCION_ENEMY_MOVENEXT:
		((CGameObject_Enemy*)mDynamicObject)->Set_MoveCount();
		break;
	case CAction_Function::FUNCION_ROTFLAG:
		mDynamicObject->RotTargetFunc();
		break;

	case CAction_Function::FUNCION_BOSS_FUNC1:
	//	((CGameObject_BOSS*)mDynamicObject)->Set_MoveCount();
		break;

	case CAction_Function::FUNCION_BOSS_FUNC2:
	//	((CGameObject_BOSS*)mDynamicObject)->Set_MoveCount();
		break;
	default:
		break;
	}

	End_Succed();

	return S_OK;
}

CAction_Function * CAction_Function::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_Function* pInstance = NEW CAction_Function(str, obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_Function");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_Function * CAction_Function::Clone()
{
	CAction_Function* pInstance = NEW CAction_Function(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_Function");
		DEBUGBREAK;
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CAction_Function::Free()
{
	__super::Free();
}

CAction_MOVE_TARGET::CAction_MOVE_TARGET(const char * str, CGameObject_3D_Dynamic * obj)
	:CAction_DynamicBase(str, obj)
	
{
	
}

CAction_MOVE_TARGET::CAction_MOVE_TARGET(const CAction_MOVE_TARGET & rhs)
	: CAction_DynamicBase(rhs)
{
	mCurrentTimer = 0;
	mTimeMax = rhs.mTimeMax;
	meEasingID = rhs.meEasingID;
	meAcionID = CAction_DynamicBase::E_ACION_MOVETARGET;
}

HRESULT CAction_MOVE_TARGET::ReStart(void* pArg)
{
	if (mDynamicObject == nullptr)
		return S_FALSE;

	__super::ReStart(pArg);
	
	mCurrentTimer = 0;
	mDynamicObject->Set_IsTerrainHeight(false);

	return S_OK;
}

HRESULT CAction_MOVE_TARGET::Action(_double TimeDelta)
{
	mCurrentTimer += TimeDelta;
	_float3 CurrentPosition = GetSingle(CGameInstance)->Easing3(meEasingID, mStartPosition, mGoalPosition, mCurrentTimer, mTimeMax);
	// mDynamicObject->Get_ComTransform()->LookAt(CurrentPosition);

	if (mCurrentTimer > mTimeMax)
	{
		mDynamicObject->Set_Position(mGoalPosition);
		mDynamicObject->Set_IsTerrainHeight(true);
		End_Succed();
	}
	else
		mDynamicObject->Set_Position(CurrentPosition);

	// if (_float3::Distance(CurrentPosition, mGoalPosition) < 0.2f)
	// {
	// IsMoving = false;
	// eturn S_OK;
	// }
	return S_OK;

}


CAction_MOVE_TARGET * CAction_MOVE_TARGET::Create(const char * str, CGameObject_3D_Dynamic * obj)
{
	CAction_MOVE_TARGET* pInstance = NEW CAction_MOVE_TARGET(str,obj);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAction_MOVE_TARGET * CAction_MOVE_TARGET::Clone()
{
	CAction_MOVE_TARGET* pInstance = NEW CAction_MOVE_TARGET(*this);

	if (FAILED(pInstance->ReStart()))
	{
		MSGBOX("Failed to Created CAction_MOVE");
		DEBUGBREAK;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAction_MOVE_TARGET::Free()
{
	__super::Free();
}
