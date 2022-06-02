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
	return S_OK;
}


HRESULT CGameObject_Mine::Init_Unit()
{
	mComTransform->Scaled(_float3(0.5f, 0.5f, 0.5f));

	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::E_COLLIDER_TYPE::COL_AABB;
	desc.mSize = _float3(0.5f, 0.5f, 0.5f);
	Add_ColliderDesc(&desc, 1);
	Update_Collider();

	FAILED_CHECK(Set_AniEnum(CAnimatior::E_COMMON_ANINAME_IDLE));	
	return S_OK;
}

HRESULT CGameObject_Mine::Init_AI()
{
	// AI 세부 구현

	CNode_Seqeunce* Seq_DealyA = CNode_Seqeunce::Create();

	// 클론 만들기
	CAction_DEALY* dealy5 = CAction_DEALY::Create("Dealy0.5", this, 0.5f);
	CAction_DEALY* dealyidle = CAction_DEALY::Create("dealyidle", this, 0.0);
	CAction_DEALY* dealydig = CAction_DEALY::Create("dealydig", this, 0.0);
	dealyidle->Set_Animation(CAnimatior::E_COMMON_ANINAME_IDLE);
	dealydig->Set_Animation(CAnimatior::E_COMMON_ANINAME_DIG);

	// CAction_MOVE* MoveRun = CAction_MOVE::Create("run", this, _float3(0, 0, 0), 0.2f);
	// MoveRun->Set_AniType(CAction_MOVE::MOVE_RUN_ANI);
	CAction_MOVE* MoveWalk = CAction_MOVE::Create("walk", this, _float3(0, 0, 0), 0.6f);
	MoveWalk->Set_AniType(CAction_MOVE::MOVE_WALK_ANI);

	//	Seq_DealyA->PushBack_LeafNode(dealyidle->Clone());
	Seq_DealyA->PushBack_LeafNode(dealydig->Clone());

	// 이동
//	Seq_DealyA->PushBack_LeafNode(MoveRun->Clone());
	Seq_DealyA->PushBack_LeafNode(MoveWalk->Clone());


	//for (int i = 0; i < 2; ++i)
	//{
	//	Seq_DealyA->PushBack_LeafNode(dealy5->Clone());
	//}

	//Seq_DealyA->PushBack_LeafNode(MoveWalk->Clone());

	mComBehavior->Add_Seqeunce("IDLE1", Seq_DealyA);
	mComBehavior->Select_Sequnce("IDLE1");

	Safe_Release(dealy5);
	//Safe_Release(MoveRun);
	Safe_Release(MoveWalk);
	Safe_Release(dealyidle);
	Safe_Release(dealydig);

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
