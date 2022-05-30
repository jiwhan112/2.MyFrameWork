#include "BehaviorTree.h"

CBehaviorTree::CBehaviorTree(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice,pDeviceContext)
{
}

CBehaviorTree::CBehaviorTree(const CBehaviorTree & rhs)
	: CComponent(rhs)
{
	// 복사
	mMapSequence = rhs.mMapSequence;
	for (auto seq: mMapSequence)
	{
		Safe_AddRef(seq.second);
	}
	mCurrentSequnence = nullptr;
}

HRESULT CBehaviorTree::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBehaviorTree::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CBehaviorTree::Tick(_double timer)
{
	if (mCurrentSequnence)
		mCurrentSequnence->Tick_Sequnce(timer);

	return S_OK;
}

HRESULT CBehaviorTree::LateTick(_double timer)
{
	if (mCurrentSequnence->Get_SeqEnd())
	{
		// 다른 시퀀스 선택
	}

	return S_OK;
}

HRESULT CBehaviorTree::Add_Seqeunce(string strtag, CNode_Seqeunce * seq)
{
	if (seq == nullptr)
		return E_FAIL;

	auto findseq =  Find_Seqeunce(strtag);
	if (findseq)
		return E_FAIL;

	mMapSequence.emplace(strtag, seq);
	return S_OK;
}

CNode_Seqeunce* CBehaviorTree::Find_Seqeunce(string strtag)
{
	auto iter = mMapSequence.find(strtag);
	if (iter == mMapSequence.end())
		return nullptr;

	return iter->second;
}

CBehaviorTree * CBehaviorTree::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBehaviorTree*	pInstance = NEW CBehaviorTree(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CBehaviorTree");
		Safe_Release(pInstance);
	}

	return pInstance;

}

CBehaviorTree * CBehaviorTree::Clone(void * pArg)
{
	CBehaviorTree*	pInstance = NEW CBehaviorTree(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CBehaviorTree");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBehaviorTree::Free()
{
	__super::Free();
	mCurrentSequnence = nullptr;
	for (auto node: mMapSequence)
	{
		Safe_Release(node.second);
	}
	mMapSequence.clear();

	
}

////////////////////////////////////////////////////////////
// Sequnce_TREE

HRESULT CNode_Seqeunce::Tick_Sequnce(_double timer)
{
	if (mListSequnce.empty())
		return E_FAIL;
	if (mCurrentLeafTree == nullptr)
		mCurrentLeafTree = NextNode();

	CNode_LeafTree::E_LEAFTREE_ID id = mCurrentLeafTree->Get_TREEID();
	switch (id)
	{
	case CNode_LeafTree::LEAFTREE_ID_ACTION:
		// 현재 액션 실행 노드
		static_cast<CNode_Action*>(mCurrentLeafTree)->Action(timer);
		if (static_cast<CNode_Action*>(mCurrentLeafTree)->Get_IsEnd())
		{
			if (static_cast<CNode_Action*>(mCurrentLeafTree)->Get_IsSucceed())
			{
				mCurrentLeafTree = NextNode();
			}
			else
			{
				// 시퀀스 나가기
				End_Sequnce();
			}
		}
		break;

	case CNode_LeafTree::LEAFTREE_ID_DECORATOR:
	{
		// 조건 판단 노드
		// 성공하면 다음노드 이동
		// 실패하면 시퀀드 탈락
		// 실패하면 이전노드 이동 
		CNode_Decorator::E_DECOTYPE type = static_cast<CNode_Decorator*>(mCurrentLeafTree)->IsCorect(timer);

		switch (type)
		{
		case CNode_Decorator::DECOTYPE_NEXT:
			mCurrentLeafTree = NextNode();
			break;
		case CNode_Decorator::DECOTYPE_PREV:
			mCurrentLeafTree = mPreLeafTree;
			break;
		case CNode_Decorator::DECOTYPE_BREAK:
			End_Sequnce();
			break;
		case CNode_Decorator::DECOTYPE_BACKFIRST:
			mCurrentLeafTree = mListSequnce.front();
			break;
		default:
			break;
		}
	}
		break;
	case CNode_LeafTree::LEAFTREE_ID_SELECTER:
		// 선택 노드
		// 나중에 구현
		break;

	default:
		break;
	}

	if (mCurrentLeafTree == nullptr)
	{
		End_Sequnce();
		return S_OK;
	}

	return S_OK;
}

HRESULT CNode_Seqeunce::Iter_Sequnce(bool next)
{
	// 노드가 끝났을때 성공 여부에 따라 다음노드 결정
	// 혹은 시퀀스 종료
	if (mCurrentLeafTree->Get_IsEnd() == true)
	{
		// 다음 노드 
		mPreLeafTree = mCurrentLeafTree;
		mCurrentLeafTree = NextNode();
	}

	return S_OK;
}

HRESULT CNode_Seqeunce::End_Sequnce()
{
	// 시퀀스 나가기
	bSeqEnd = true;
	mCurrentLeafTree = nullptr;
	mPreLeafTree = nullptr;

	return S_OK;
}

CNode_LeafTree* CNode_Seqeunce::NextNode()
{
	if (mCurrentLeafTree == nullptr)
		return mListSequnce.front();

	// 현재 노드의 다음 노드를 넘긴다.
	auto iter = Find_LeafTree_Iter(mCurrentLeafTree);
	if (iter == mListSequnce.end())
		return nullptr;

	iter++;
	if (iter == mListSequnce.end())
		return nullptr;

	return *iter;
}

CNode_LeafTree * CNode_Seqeunce::PreNode()
{
	if (mCurrentLeafTree == nullptr)
		return nullptr;

	// 현재 노드의 다음 노드를 넘긴다.
	auto iter = Find_LeafTree_Iter(mCurrentLeafTree);
	if (iter == mListSequnce.begin())
		return nullptr;
	iter--;
	return *iter;
}

list< CNode_LeafTree*>::iterator CNode_Seqeunce::Find_LeafTree_Iter(CNode_LeafTree * currentTree)
{
	list< CNode_LeafTree*>::iterator  iter_begin = mListSequnce.begin();
	list< CNode_LeafTree*>::iterator  iter_end = mListSequnce.end();

	list< CNode_LeafTree*>::iterator  iter_retun = iter_end;

	for (auto iter = iter_begin;iter!= iter_end; iter++)
	{
		if (mCurrentLeafTree == *iter)
		{
			iter_retun = iter;
			return iter_retun;
		}
	}

	return iter_end;
}

HRESULT CNode_Seqeunce::NativeConstruct()
{
	bSeqEnd = true;
	return S_OK;
}

CNode_Seqeunce * CNode_Seqeunce::Create()
{
	CNode_Seqeunce*	pInstance = NEW CNode_Seqeunce();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CNode_Seqeunce");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNode_Seqeunce::Free()
{
	for (auto leaf: mListSequnce)
	{
		Safe_Release(leaf);
	}
	bSeqEnd = false;
}



/////////////////////////////////////////////////////////////
// LEAF_TREE
CNode_Action::CNode_Action(const char * str)
	: CNode_LeafTree(str)
{
	meTreeID = LEAFTREE_ID_ACTION;
}	


CNode_Selector::CNode_Selector(const char * str)
	: CNode_LeafTree(str)
{
	meTreeID = LEAFTREE_ID_SELECTER;
}

CNode_Decorator::CNode_Decorator(const char * str)
	: CNode_LeafTree(str)
{
	meTreeID = LEAFTREE_ID_DECORATOR;
}

void CNode_LeafTree::Free()
{
}
