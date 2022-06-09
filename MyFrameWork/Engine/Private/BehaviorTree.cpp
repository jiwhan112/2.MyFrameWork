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

	mMapLeafNode = rhs.mMapLeafNode;
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

	if (mCurrentSequnence && (mCurrentSequnence->Get_SeqEnd() == false))
	{
		FAILED_CHECK(mCurrentSequnence->Tick_Sequnce(timer));
	}

	return S_OK;
}

HRESULT CBehaviorTree::LateTick(_double timer)
{
	FAILED_CHECK(Round_Sequnce());
	return S_OK;
}

const char* CBehaviorTree::Get_StrCurrentLeafName()
{
	if (mCurrentSequnence == nullptr)
		return "";
	if (mCurrentSequnence->Get_CurrentLeafNode() == nullptr)
		return "";

	return mCurrentSequnence->Get_CurrentLeafNode()->Get_NodeName();
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

HRESULT CBehaviorTree::Select_Sequnce(string seqTag, void* SeqData)
{
	CNode_Seqeunce* seq = Find_Seqeunce(seqTag);
	if (seq == nullptr)
		return E_FAIL;
	if (mCurrentSequnence)
		mCurrentSequnence->End_Sequnce();
	mCurrentSequnence = seq;
	mCurrentKey = seqTag;
	mCurrentSequnence->Restart(SeqData);
	return S_OK;
}

HRESULT CBehaviorTree::Add_Leaf_Proto(string strtag, CNode_LeafTree * seq)
{
	CNode_LeafTree* leaf = Find_Leaf(strtag);
	if (leaf == nullptr)
	{
		mMapLeafNode.emplace(strtag, seq);
		return S_OK;
	}
	return E_FAIL;
}

CNode_LeafTree * CBehaviorTree::Find_Leaf(string strtag)
{
	auto findNode = mMapLeafNode.find(strtag);
	if (findNode == mMapLeafNode.end())
		return nullptr;
	return findNode->second;
}

CNode_LeafTree * CBehaviorTree::Clone_Leaf(string strtag)
{
	auto leaf = Find_Leaf(strtag);
	if (leaf)
		return leaf->Clone();
	return nullptr;

}

HRESULT CBehaviorTree::Round_Sequnce()
{
	// INIT
	if (mCurrentKey == "")
	{
		auto iter = mMapSequence.begin();
		while (!(iter->second->Get_SeqType() == CNode_Seqeunce::SEQTYPE_IDLE))
		{
			iter++;
			if (iter == mMapSequence.end())
			return E_FAIL;
		}
		mCurrentKey = iter->first;
		mCurrentSequnence = iter->second;
		mCurrentSequnence->Restart();
	}

	// 다음 시퀀스 결정
	if (mCurrentSequnence->Get_SeqEnd())
	{
		if (mCurrentSequnence->Get_SeqType() == CNode_Seqeunce::SEQTYPE_LOOP)
		{
			mCurrentSequnence->Restart();
			return S_OK;
		}

		auto iter = mMapSequence.find(mCurrentKey);
		iter++;
		if (iter == mMapSequence.end())
			iter = mMapSequence.begin();

		while (!(iter->second->Get_SeqType() == CNode_Seqeunce::SEQTYPE_IDLE))
		{
			iter++;
			if (iter == mMapSequence.end())
			{
				iter = mMapSequence.begin();				
			}
		}

		mCurrentKey = iter->first;
		mCurrentSequnence = iter->second;
		mCurrentSequnence->Restart();
	}

	return S_OK;
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

	for (auto node : mMapSequence)
	{
		Safe_Release(node.second);
	}
	mMapSequence.clear();

	if (m_isCloned == false)
	{
		for (auto seq : mMapLeafNode)
		{
			Safe_Release(seq.second);

		}
		mMapLeafNode.clear();
	}
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
		/*CNode_Decorator::E_DECOTYPE type = static_cast<CNode_Decorator*>(mCurrentLeafTree)->IsCorect(timer);

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
		}*/
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

void CNode_Seqeunce::Restart(void * SeqData)
{
	mbEnd_Sequnce = false;
	mCurrentLeafTree = nullptr;
	mPreLeafTree = nullptr;
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
	mbEnd_Sequnce = true;
	mCurrentLeafTree = nullptr;
	mPreLeafTree = nullptr;

	return S_OK;
}

CNode_LeafTree* CNode_Seqeunce::NextNode()
{
	CNode_LeafTree* returnNode = nullptr;
	if (mCurrentLeafTree == nullptr)
	{
		returnNode = mListSequnce.front();
		FAILED_CHECK_NONERETURN(returnNode->ReStart(nullptr));

		return returnNode;
	}

	// 현재 노드의 다음 노드를 넘긴다.
	auto iter = Find_LeafTree_Iter(mCurrentLeafTree);
	if (iter == mListSequnce.end())
		return nullptr;
	
	iter++;
	if (iter == mListSequnce.end())
		return nullptr;

	returnNode = (*iter);
	FAILED_CHECK_NONERETURN(returnNode->ReStart(nullptr));
	return returnNode;
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
	FAILED_CHECK_NONERETURN((*iter)->ReStart(nullptr));
	return *iter;
}

HRESULT CNode_Seqeunce::PushFront_LeafNode(CNode_LeafTree * leaf)
{
	// Action / Deco 노드를 넣는다.
	mListSequnce.push_front(leaf);
	return S_OK;
}

HRESULT CNode_Seqeunce::PushBack_LeafNode(CNode_LeafTree * leaf)
{
	mListSequnce.push_back(leaf);
	return S_OK;
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
	mbEnd_Sequnce = false;
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
	mbEnd_Sequnce = false;
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
