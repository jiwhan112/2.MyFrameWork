#pragma once
#include "Component.h"

BEGIN(Engine)

class CNode_LeafTree;
class CNode_Seqeunce;
class CNode_Decorator;
class CNode_Action;



// �ൿƮ�� ������Ʈ
class ENGINE_DLL CBehaviorTree final : public CComponent
{
private:
	CBehaviorTree(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CBehaviorTree(const CBehaviorTree& rhs);
	virtual ~CBehaviorTree() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Tick(_double timer);
	virtual HRESULT LateTick(_double timer);

	CNode_Seqeunce* Get_CurrentSequnce() { return mCurrentSequnence; }
	const char* Get_StrCurrentLeafName();

	HRESULT				Add_Seqeunce(string strtag, CNode_Seqeunce * seq);
	CNode_Seqeunce*		Find_Seqeunce(string strtag);
	HRESULT				Select_Sequnce(string seqTag,void* SeqData = nullptr);
	string				Get_CurrentSeqKey() const
	{
		return mCurrentKey;
	}

public: // Leaf
	HRESULT				Add_Leaf_Proto(string strtag, CNode_LeafTree * seq);
	CNode_LeafTree*		Find_Leaf(string strtag);
	CNode_LeafTree*		Clone_Leaf(string strtag);


private:
	HRESULT Round_Sequnce();

private:
	// �� �������� ������ ����
	map<string, CNode_Seqeunce*>		mMapSequence;
	string								mCurrentKey = "";
	CNode_Seqeunce*						mCurrentSequnence = nullptr;

	// �� �׼��� Ŭ�� ������ ���� ������ �����ص���.
	map<string, CNode_LeafTree*>		mMapLeafNode;

public:
	static CBehaviorTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CBehaviorTree* Clone(void* pArg) override;
	virtual void Free() override;
};


// �������� MOVE ������ IDLE ������������ Ŭ�� ���ǽ��� ����
// �������� ���� ���������� ���� �ʿ�

class ENGINE_DLL CNode_Seqeunce
	:public CBase
{
public:
	enum E_SEQTYPE
	{
		SEQTYPE_IDLE, // �⺻ ��ȸ ����
		SEQTYPE_LOOP, // ���� ����
		SEQTYPE_ONETIME, // �ѹ���
		SEQTYPE_END,
	};

protected:
	explicit CNode_Seqeunce() = default;
	virtual ~CNode_Seqeunce() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual HRESULT Tick_Sequnce(_double timer);

	// �ٽ� ������ �� ���� Select���� �������� ������ �������� �����Ѵ�.
	virtual void Restart(void* SeqData = nullptr);

	// ��尡 ���������� ������� ������Ʈ
	virtual HRESULT Iter_Sequnce(bool next);
	HRESULT End_Sequnce();

	// ���� ��� ��ȯ
	CNode_LeafTree* NextNode();
	CNode_LeafTree* PreNode();

public:
	// ������ ������
	HRESULT PushFront_LeafNode(CNode_LeafTree* leaf);
	HRESULT PushBack_LeafNode(CNode_LeafTree* leaf);

public:
	_bool Get_SeqEnd() const { return mbEnd_Sequnce; };
	CNode_LeafTree* Get_CurrentLeafNode() const { return mCurrentLeafTree; }

	void Set_SeqType(E_SEQTYPE e)  { meSeqType = e; };
	void Set_SeqLevel(_uint level) { mSeqLevel = level;};
	E_SEQTYPE Get_SeqType() const { return meSeqType; };
	_uint Get_SeqLevel() const { return mSeqLevel; };


protected:
	list<CNode_LeafTree*>::iterator Find_LeafTree_Iter(CNode_LeafTree* currentTree);

protected:
	CNode_LeafTree* mCurrentLeafTree;
	CNode_LeafTree* mPreLeafTree;
	
	// ������������ �ش��ϴ� Leaf ��� ��Ʈ�� ������ �ִ´�.
	list< CNode_LeafTree*> mListSequnce;	
	_bool		mbEnd_Sequnce = false;
	E_SEQTYPE	meSeqType = SEQTYPE_IDLE;
	_uint		mSeqLevel = 0;


public:

	static CNode_Seqeunce* Create();
	virtual void Free()override;

};

// ���� ��� Ʈ�� �θ�
class ENGINE_DLL CNode_LeafTree
	:public CBase

{
public:
	enum E_LEAFTREE_ID
	{
		LEAFTREE_ID_ACTION, 
		LEAFTREE_ID_DECORATOR,
		LEAFTREE_ID_SELECTER,
		LEAFTREE_ID_END,
	};

protected:
	explicit CNode_LeafTree(const char* str)
	{
		mIsEnd = false;
		mStrNodeName = str;
	}
	explicit CNode_LeafTree(const CNode_LeafTree& c) = default;
	virtual ~CNode_LeafTree() = default;

public:
	// �ߺ�������� �ʱ�ȭ �ʿ�
	E_LEAFTREE_ID Get_TREEID() { return meTreeID; }
	void Set_TREEID(E_LEAFTREE_ID e) { meTreeID = e;}
	_bool Get_IsEnd() { return mIsEnd; }
	_bool Get_IsSucceed() { return mIsSucceed; }
	const char* Get_NodeName() { 
		return mStrNodeName.c_str();
	}

	void Init_Parent()
	{
		mIsEnd = false;
		mIsSucceed = false;
	}

	void End_Succed()
	{
		mIsEnd = true;
		mIsSucceed = true;
	}

	void End_Fail()
	{
		mIsEnd = true;
		mIsSucceed = false;
	}

public:
	// �ߺ�������� �ʱ�ȭ �ʿ�
	virtual HRESULT NativeConstruct() = 0;
	// �ߺ���� Ŭ��
	virtual CNode_LeafTree* Clone(void* pArg =nullptr) = 0;

protected:
	string			mStrNodeName;
	E_LEAFTREE_ID	meTreeID = LEAFTREE_ID_END;
	// ���� ����
	_bool			mIsEnd = false;
	// ���� ����
	_bool			mIsSucceed = false;

public:
	virtual void Free() override;
};


// ���� �׼� 
class ENGINE_DLL CNode_Action
	: public CNode_LeafTree
{
protected:
	explicit CNode_Action(const char* str);
	virtual ~CNode_Action() = default;


public:
	virtual HRESULT ReStart(void* pArg = nullptr) = 0;
	virtual CNode_Action* Clone() = 0;
	virtual HRESULT Action(_double timer) = 0;
};

// ���� �Ǵ�
class ENGINE_DLL CNode_Decorator
	: public CNode_LeafTree
{
public:
	enum E_DECOTYPE
	{
		DECOTYPE_NEXT,
		DECOTYPE_PREV,
		DECOTYPE_BREAK,
		DECOTYPE_BACKFIRST,
		DECOTYPE_END,
	};

protected:
	explicit CNode_Decorator(const char* str);
	virtual ~CNode_Decorator() = default;

public:

	virtual HRESULT ReStart(void* pArg = nullptr) = 0;
	virtual CNode_Decorator* Clone() = 0;
	virtual E_DECOTYPE IsCorect(_double timer) = 0;

};

// ���� ���� 
class ENGINE_DLL CNode_Selector
	: public CNode_LeafTree
{
protected:
	explicit CNode_Selector(const char* str);
	virtual ~CNode_Selector() = default;

public:
	virtual HRESULT ReStart(void* pArg = nullptr) = 0;
	virtual CNode_Decorator* Clone() = 0;
	virtual HRESULT Selection(_double timer) = 0;


protected:
	CNode_Action* mLeftNode;
	CNode_Action* mRightNode;

};

END


