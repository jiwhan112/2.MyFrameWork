#include "BehaviorTree.h"

CBehaviorTree::CBehaviorTree(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice,pDeviceContext)
{
}

CBehaviorTree::CBehaviorTree(const CBehaviorTree & rhs)
	: CComponent(rhs)
{
	// ����
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

	if (mCurrentSequnence && (mCurrentSequnence->Get_SeqEnd() == false))
	{
		FAILED_CHECK(mCurrentSequnence->Tick_Sequnce(timer));
	}

	return S_OK;
}

HRESULT CBehaviorTree::LateTick(_double timer)
{

	// ���� ���°� �´ٸ� ���� ��ȯ�ȴ� �׷��� �ʴٸ� ����� ������ ���¸� �����Ѵ�.

	// �켱 �������� �������� ������ ������ ����
	if (mCurrentSequnence->Get_SeqEnd())
	{
		auto iter = mMapSequence.find(mCurrentKey);
		iter++;
		if (iter == mMapSequence.end())
		{
			mCurrentKey = (*mMapSequence.begin()).first;
			mCurrentSequnence = (*mMapSequence.begin()).second;
			mCurrentSequnence->Restart();

		}
		else
		{
			mCurrentKey = (*iter).first;
			mCurrentSequnence = (*iter).second;
			mCurrentSequnence->Restart();
		}
	}

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

HRESULT CBehaviorTree::Select_Sequnce(string seqTag)
{
	CNode_Seqeunce* seq = Find_Seqeunce(seqTag);
	if (seq == nullptr)
		return E_FAIL;

	mCurrentSequnence = seq;
	mCurrentKey = seqTag;

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
		// ���� �׼� ���� ���
		static_cast<CNode_Action*>(mCurrentLeafTree)->Action(timer);
		if (static_cast<CNode_Action*>(mCurrentLeafTree)->Get_IsEnd())
		{
			if (static_cast<CNode_Action*>(mCurrentLeafTree)->Get_IsSucceed())
			{
				mCurrentLeafTree = NextNode();
			}
			else
			{
				// ������ ������
				End_Sequnce();
			}
		}
		break;

	case CNode_LeafTree::LEAFTREE_ID_DECORATOR:
	{
		// ���� �Ǵ� ���
		// �����ϸ� ������� �̵�
		// �����ϸ� ������ Ż��
		// �����ϸ� ������� �̵� 
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
		// ���� ���
		// ���߿� ����
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
	// ��尡 �������� ���� ���ο� ���� ������� ����
	// Ȥ�� ������ ����
	if (mCurrentLeafTree->Get_IsEnd() == true)
	{
		// ���� ��� 
		mPreLeafTree = mCurrentLeafTree;
		mCurrentLeafTree = NextNode();
	}

	return S_OK;
}

HRESULT CNode_Seqeunce::End_Sequnce()
{
	// ������ ������
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
		FAILED_CHECK_NONERETURN(returnNode->NativeConstruct());

		return returnNode;
	}

	// ���� ����� ���� ��带 �ѱ��.
	auto iter = Find_LeafTree_Iter(mCurrentLeafTree);
	if (iter == mListSequnce.end())
		return nullptr;
	
	iter++;
	if (iter == mListSequnce.end())
		return nullptr;

	returnNode = (*iter);
	FAILED_CHECK_NONERETURN(returnNode->NativeConstruct());
	return returnNode;
}

CNode_LeafTree * CNode_Seqeunce::PreNode()
{
	if (mCurrentLeafTree == nullptr)
		return nullptr;

	// ���� ����� ���� ��带 �ѱ��.
	auto iter = Find_LeafTree_Iter(mCurrentLeafTree);
	if (iter == mListSequnce.begin())
		return nullptr;

	iter--;
	FAILED_CHECK_NONERETURN((*iter)->NativeConstruct());
	return *iter;
}

HRESULT CNode_Seqeunce::PushFront_LeafNode(CNode_LeafTree * leaf)
{
	// Action / Deco ��带 �ִ´�.
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
	mbEnd_Sequnce = true;
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