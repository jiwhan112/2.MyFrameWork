#pragma once
#include "Component.h"

BEGIN(Engine)

class CNode_LeafTree;
class CNode_Seqeunce;
class CNode_Decorator;
class CNode_Action;



// 행동트리 컴포넌트
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
	// 각 시퀀스를 맵으로 저장
	map<string, CNode_Seqeunce*>		mMapSequence;
	string								mCurrentKey = "";
	CNode_Seqeunce*						mCurrentSequnence = nullptr;

	// 각 액션의 클론 생성을 위해 원본을 저장해두자.
	map<string, CNode_LeafTree*>		mMapLeafNode;

public:
	static CBehaviorTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CBehaviorTree* Clone(void* pArg) override;
	virtual void Free() override;
};


// 시퀀스는 MOVE 시퀀스 IDLE 시퀀스등으로 클라에 정의시켜 두자
// 시퀀스를 좀더 세부적으로 구현 필요

class ENGINE_DLL CNode_Seqeunce
	:public CBase
{
public:
	enum E_SEQTYPE
	{
		SEQTYPE_IDLE, // 기본 순회 가능
		SEQTYPE_LOOP, // 연속 상태
		SEQTYPE_ONETIME, // 한번만
		SEQTYPE_END,
	};

protected:
	explicit CNode_Seqeunce() = default;
	virtual ~CNode_Seqeunce() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual HRESULT Tick_Sequnce(_double timer);

	// 다시 시작할 떄 마다 Select에서 전달해준 정보로 시퀀스를 실행한다.
	virtual void Restart(void* SeqData = nullptr);

	// 노드가 끝났을떄만 다음노드 업데이트
	virtual HRESULT Iter_Sequnce(bool next);
	HRESULT End_Sequnce();

	// 다음 노드 반환
	CNode_LeafTree* NextNode();
	CNode_LeafTree* PreNode();

public:
	// 시퀀스 노드생성
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
	
	// 시퀀스에서는 해당하는 Leaf 노드 라스트로 가지고 있는다.
	list< CNode_LeafTree*> mListSequnce;	
	_bool		mbEnd_Sequnce = false;
	E_SEQTYPE	meSeqType = SEQTYPE_IDLE;
	_uint		mSeqLevel = 0;


public:

	static CNode_Seqeunce* Create();
	virtual void Free()override;

};

// 하위 노드 트리 부모
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
	// 중복사용으로 초기화 필요
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
	// 중복사용으로 초기화 필요
	virtual HRESULT NativeConstruct() = 0;
	// 중복사용 클론
	virtual CNode_LeafTree* Clone(void* pArg =nullptr) = 0;

protected:
	string			mStrNodeName;
	E_LEAFTREE_ID	meTreeID = LEAFTREE_ID_END;
	// 끝난 여부
	_bool			mIsEnd = false;
	// 성공 여부
	_bool			mIsSucceed = false;

public:
	virtual void Free() override;
};


// 실제 액션 
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

// 조건 판단
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

// 조건 선택 
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


