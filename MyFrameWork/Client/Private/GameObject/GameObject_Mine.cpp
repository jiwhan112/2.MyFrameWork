#include "stdafx.h"
#include "GameObject/GameObject_Mine.h"
#include "GameObject/GameObject_3D_Tile.h"
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

	string str("crea_Snot_a.fbx");
	strcpy_s(mModelDesc.mModelName, str.c_str());
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
	FAILED_CHECK(Init_AI_Default());
	FAILED_CHECK(Init_AI_Tile());
	return S_OK;
}

HRESULT CGameObject_Mine::Init_AI_Default()
{
	// AI 세부 구현

	// IDLE 시퀀스 3개
	CNode_Seqeunce* Seq_IDLE1 = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_IDLE2 = CNode_Seqeunce::Create();
	CNode_Seqeunce* Seq_IDLE3 = CNode_Seqeunce::Create();

	// Set Action
	CAction_DEALY* dealyTime = CAction_DEALY::Create("DealyTime", this);
	CAction_DEALY* dealyAniIdle = CAction_DEALY::Create("DealyIdle", this);
//	CAction_DEALY* dealyAniDig = CAction_DEALY::Create("DealyDig", this);
	CAction_DEALY* dealyAniDance = CAction_DEALY::Create("DelayDance", this);

	dealyTime->Set_TimeMax(3.0f);
	dealyAniIdle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
//	dealyAniDig->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);
	dealyAniDance->Set_Animation(CAnimatior::E_COMMON_ANINAME_DANCE);

	CAction_MOVE* MoveWalk = CAction_MOVE::Create("Walk", this);
	CAction_MOVE* MoveRun = CAction_MOVE::Create("Run", this);
	MoveWalk->Set_AniType(CAction_MOVE::MOVE_WALK_ANI);
	MoveWalk->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	MoveWalk->Set_TimeMax(0.6f);
	MoveRun->Set_Postition(CAction_MOVE::MOVE_POS_NEAR);
	MoveRun->Set_TimeMax(0.3f);

	// SetSeq
	// IDLE1: 돌아다님
	Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE1->PushBack_LeafNode(dealyAniDance->Clone());
	Seq_IDLE1->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE1->PushBack_LeafNode(MoveWalk->Clone());

	// IDLE2: 뛰어다님
	Seq_IDLE2->PushBack_LeafNode(dealyAniIdle->Clone());
	Seq_IDLE2->PushBack_LeafNode(dealyAniDance->Clone());
	Seq_IDLE2->PushBack_LeafNode(MoveRun->Clone());

	// IDLE3: 춤추기
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
	Safe_Release(dealyAniDance);

	Safe_Release(MoveWalk);
	Safe_Release(MoveRun);
	return S_OK;
}

HRESULT CGameObject_Mine::Init_AI_Tile()
{
	// AI 세부 구현

	// Tile 시퀀스 2개
	CNode_Seqeunce* Seq_Dig_Tile = CNode_Seqeunce::Create();
//	CNode_Seqeunce* Seq_Dig_Gold = CNode_Seqeunce::Create();


	// Set Action
	CAction_DEALY* dealyTime = CAction_DEALY::Create("DealyTime", this);
	CAction_DEALY* dealyAniDig = CAction_DEALY::Create("DealyDig", this);

	dealyTime->Set_TimeMax(3.0f);
	dealyAniDig->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);

	CAction_MOVE* MoveRun = CAction_MOVE::Create("Run", this);
	MoveRun->Set_AniType(CAction_MOVE::MOVE_RUN_ANI);
	MoveRun->Set_TimeMax(0.3f);
	MoveRun->Set_Postition(CAction_MOVE::MOVE_POS_TILE);

	CAction_Function* Function1 = CAction_Function::Create("RemoveTile", this);
	Function1->Set_Funcion(CAction_Function::FUNCION_REMOVE_TILE);

	CAction_Function* Function2 = CAction_Function::Create("LookFunction", this);
	Function2->Set_Funcion(CAction_Function::FUNCION_LOOK);

	// SetSeq
	// Tile: 타일을 채굴
	Seq_Dig_Tile->PushBack_LeafNode(MoveRun->Clone());
	Seq_Dig_Tile->PushBack_LeafNode(Function2->Clone());
	Seq_Dig_Tile->PushBack_LeafNode(dealyAniDig->Clone());
	Seq_Dig_Tile->PushBack_LeafNode(Function1->Clone());



	// Gold: 계속 캐기
	//Seq_Dig_Gold->PushBack_LeafNode(dealyAniIdle->Clone());
	//Seq_Dig_Gold->PushBack_LeafNode(dealyAniDig->Clone());
	//Seq_Dig_Gold->PushBack_LeafNode(dealyAniDance->Clone());
	//Seq_Dig_Gold->PushBack_LeafNode(MoveRun->Clone());

	Seq_Dig_Tile->Set_SeqType(CNode_Seqeunce::E_SEQTYPE::SEQTYPE_ONETIME);
	mComBehavior->Add_Seqeunce("DIG", Seq_Dig_Tile);
	// mComBehavior->Add_Seqeunce("GOLD", Seq_Dig_Gold);

	// Release
	Safe_Release(dealyTime);
	Safe_Release(dealyAniDig);
	Safe_Release(MoveRun);
	Safe_Release(Function1);
	Safe_Release(Function2);

	return S_OK;
}

void CGameObject_Mine::Set_Dig_Tile(CGameObject_3D_Tile * tile)
{
	// 못가는 타일 뒤에 넣기
	if (tile->Get_IsBlocked())
	{
		tile->Add_TileTask_this();
		return;
	}

	// 뚫린 타일 위치 찾기
	_float3 GoalPos =  tile->Get_AbleTilePos(0.5f);
	mTileGoalPostiton = GoalPos;

	_uint StartIndex = mCurrentNavi->Get_CurrentCellIndex();
	_uint GoalIndex = StartIndex;

	// 해당 위치의 네비메시 셀 인덱스 반환
	if (mCurrentNavi->Get_PickPosForIndex(mTileGoalPostiton, &GoalIndex))
	{
		// 경로 탐색
		mCurrentPathList = mCurrentNavi->AstartPathFind(StartIndex, GoalIndex);
	}

	else
	{
		tile->Add_TileTask_this();
		return;
	}

	// 타일 채굴
	mSearchTile = tile;
	mComBehavior->Select_Sequnce("DIG");

}

void CGameObject_Mine::Set_Dig_Gold(CGameObject_3D_Tile * tile)
{
	//if (tile->Get_IsBlocked())
	//{
	//	tile->Add_TileTask_this();
	//	return;
	//}

	//if (FindPathRandAblePostition)
	//{

	//}

	//// 골드 채굴
	//mSearchTile = tile;
	//mComBehavior->Select_Sequnce("GOLD");

}

HRESULT CGameObject_Mine::RemoveTile()
{
	if (mSearchTile)
	{
		mSearchTile->RemoveThisTile();
		mSearchTile = nullptr;

		// 이펙트 추가
	}

	return S_OK;
}

HRESULT CGameObject_Mine::LookTile()
{
	_float3 dirPos = mSearchTile->Get_WorldPostition();
	mComTransform->LookAt(dirPos);
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
