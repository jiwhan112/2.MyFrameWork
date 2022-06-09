#include "BehaviorTree.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

// 시퀀스 클라 구현

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

protected:
	void	Setup_TargetNode(CGameObject_3D_Dynamic* obj);
	void	Setup_RestartNodes();


protected:
	CGameObject_3D_Dynamic* mDynamicObject = nullptr;

public:
	virtual void Free()override;
};

// IDLE 상태 시퀀스 
class CSequnce_IDLE :
	public CSequnce_Base
{
public:
	typedef struct tag_SeqIdle
	{
		int data;

	}SEQIDLE;

protected:
	explicit CSequnce_IDLE() = default;
	virtual ~CSequnce_IDLE() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;


protected:
	SEQIDLE					mSeqData;

public:
	static CSequnce_IDLE* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_MOVETARGET
class CSequnce_MOVETARGET :
public CSequnce_Base
{
public:
	enum E_MOVETYPE
	{
		MOVETYPE_FALL_CREATE,
		MOVETYPE_FALL,
		MOVETYPE_DOOR,
		MOVETYPE_END
	};
	typedef struct tag_SeqMoveTarget
	{
		E_MOVETYPE eMoveType;

	}SEQMOVETARGET;

protected:
	explicit CSequnce_MOVETARGET() = default;
	virtual ~CSequnce_MOVETARGET() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQMOVETARGET					mSeqData;

public:
	static CSequnce_MOVETARGET* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_TIle
class CSequnce_TILE:
	public CSequnce_Base
{
public:
	typedef struct tag_SeqTILE
	{
		int data;

	}SEQTILE;

protected:
	explicit CSequnce_TILE() = default;
	virtual ~CSequnce_TILE() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;


protected:
	SEQTILE					mSeqData;

public:
	static CSequnce_TILE* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_Pick
class CSequnce_PICK:
	public CSequnce_Base
{
public:
	typedef struct tag_SeqTILE
	{
		int data;

	}SEQPICK;

protected:
	explicit CSequnce_PICK() = default;
	virtual ~CSequnce_PICK() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
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