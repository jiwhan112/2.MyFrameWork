#include "BehaviorTree.h"

#include "AI/AI_Action.h"
#include "AI/AI_Deco.h"
#include "AI/AI_Select.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

// ������ Ŭ�� ����

#pragma region SEQ_BASE

class CSequnce_Base :
	public CNode_Seqeunce
{
protected:
	explicit CSequnce_Base() = default;
	virtual ~CSequnce_Base() = default;

public:
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj);
	virtual void Restart(void* SeqData = nullptr)override;
	void	Setup_Dynamic(CGameObject_3D_Dynamic* obj) { mDynamicObject = obj; }

public: // Search_Action
	CAction_DynamicBase* Find_Action(CAction_DynamicBase::E_AcionID id,_int index = 0);
	//CDeco_DynamicBase* Find_Deco();
	//CSelect_DynamicBase* Find_Select();

protected:
	void	Setup_TargetNode(CGameObject_3D_Dynamic* obj);
	void	Setup_RestartNodes();

protected:
	CGameObject_3D_Dynamic* mDynamicObject = nullptr;

public:
	virtual void Free()override;
};

// IDLE ���� ������ 
class CSequnce_IDLE :
	public CSequnce_Base
{
public:
	typedef struct tag_SeqIdle
	{
		CAnimatior::E_COMMON_ANINAME	AniType = CAnimatior::E_COMMON_ANINAME_IDLE;
		EasingTypeID					mMoveEasingId= TYPE_Linear;
	}SEQIDLE;

protected:
	explicit CSequnce_IDLE() = default;
	virtual ~CSequnce_IDLE() = default;

public:
	// �ʱ�ȭ�� �ٽý��۽� ���� ���ް� �ٸ�
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQIDLE					mSeqData;

public:
	static CSequnce_IDLE* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// ������ ��ġ �̵� ������
class CSequnce_MOVETARGET 
	:public CSequnce_Base
{
public:	
	typedef struct tag_SeqMoveTarget
	{
		// ��� -> ������ -> �ִϸ��̼�;
		_float Dealytime = 1;
		_float TimeMax = 1.0f;
		EasingTypeID EasingID = TYPE_Linear;
		CAnimatior::E_COMMON_ANINAME AniType;
		_float3 StartPosition;
		_float3 EndPosition;

	}SEQMOVETARGET;

protected:
	explicit CSequnce_MOVETARGET() = default;
	virtual ~CSequnce_MOVETARGET() = default;

public:
	// �ʱ�ȭ�� �ٽý��۽� ���� ���ް� �ٸ�
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQMOVETARGET					mSeqData;

public:
	static CSequnce_MOVETARGET* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_TIle
// Ÿ�� �̵� ������
class CSequnce_TILE:
	public CSequnce_Base
{
public:
	typedef struct tag_SeqTILE
	{
		_float Runtime = 0.3f;

	}SEQTILE;

protected:
	explicit CSequnce_TILE() = default;
	virtual ~CSequnce_TILE() = default;

public:
	// �ʱ�ȭ�� �ٽý��۽� ���� ���ް� �ٸ�
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;


protected:
	SEQTILE					mSeqData;

public:
	static CSequnce_TILE* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_Pick
// Ŭ�� ���� ������
class CSequnce_PICK:
	public CSequnce_Base
{
public:
	typedef struct tag_SeqTILE
	{
		CAnimatior::E_COMMON_ANINAME AniType;

	}SEQPICK;

protected:
	explicit CSequnce_PICK() = default;
	virtual ~CSequnce_PICK() = default;

public:
	// �ʱ�ȭ�� �ٽý��۽� ���� ���ް� �ٸ�
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQPICK					mSeqData;

public:
	static CSequnce_PICK* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

#pragma endregion SEQ_BASE








END