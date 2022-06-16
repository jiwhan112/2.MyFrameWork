#include "BehaviorTree.h"

#include "AI/AI_Action.h"
#include "AI/AI_Deco.h"
#include "AI/AI_Select.h"

BEGIN(Client)
class CGameObject_3D_Dynamic;

// 시퀀스 클라 구현

#pragma region SEQ_BASE

static void Set_Static_Animation(CGameObject_3D_Dynamic* obj, CAnimatior::E_COMMON_ANINAME e, int index);

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
	CDeco_DynamicBase* Find_Deco(CDeco_DynamicBase::E_DecoID id, _int index = 0);
	//CSelect_DynamicBase* Find_Select();

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
		CAnimatior::E_COMMON_ANINAME	AniType = CAnimatior::E_COMMON_ANINAME_IDLE;
		EasingTypeID					mMoveEasingId= TYPE_Linear;
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

// 임의의 위치 이동 시퀀스
class CSequnce_MOVETARGET 
	:public CSequnce_Base
{
public:	
	typedef struct tag_SeqMoveTarget
	{
		// 대기 -> 움직임 -> 애니메이션;
		_float Dealytime = 1;
		_float TimeMax = 1.0f;
		EasingTypeID EasingID = TYPE_Linear;
		CAnimatior::E_COMMON_ANINAME AniType;
		_float3 StartPosition;
		_float3 EndPosition;
		CAction_Function::E_FUNCION eExitFunc = CAction_Function::E_FUNCION::FUNCION_NONE;

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
// 타일 이동 시퀀스
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
// 클릭 상태 시퀀스
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
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQPICK					mSeqData;

public:
	static CSequnce_PICK* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_WorldIdle
class CSequnce_WorldIdle :
	public CSequnce_Base
{
public:
	typedef struct tag_SeqWorldIdle
	{
		CAnimatior::E_COMMON_ANINAME AniType;

	}SEQWORLDIDLE;

protected:
	explicit CSequnce_WorldIdle() = default;
	virtual ~CSequnce_WorldIdle() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQWORLDIDLE					mSeqData;

public:
	static CSequnce_WorldIdle* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};


// Seq_WorldMove
// 월드에서 움직임
class CSequnce_WorldMove_Player:
	public CSequnce_Base
{
public:
	typedef struct tag_SeqWorldMove
	{
		_float3 GoalPosition = _float3();

	}SEQWORLDMOVE_PlAYER;

protected:
	explicit CSequnce_WorldMove_Player() = default;
	virtual ~CSequnce_WorldMove_Player() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQWORLDMOVE_PlAYER					mSeqData;

public:
	static CSequnce_WorldMove_Player* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_WorldMove_ENEMY
// 월드에서 움직임
class CSequnce_WorldMove_Enemy :
	public CSequnce_Base
{
public:
	typedef struct tag_SeqWorldMove
	{
		_float3 GoalPostition = _float3();

	}SEQWORLDMOVE_ENEMY;

protected:
	explicit CSequnce_WorldMove_Enemy() = default;
	virtual ~CSequnce_WorldMove_Enemy() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQWORLDMOVE_ENEMY					mSeqData;

public:
	static CSequnce_WorldMove_Enemy* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// Seq_Attack_Player
// 플레이어 공격 시퀀스
class CSequnce_WorldAttack_Player:
	public CSequnce_Base
{
public:
	typedef struct tag_SeqWorldAttack
	{
		CGameObject_3D_Dynamic* Target;

	}SEQWORLDATTACK_PLY;

protected:
	explicit CSequnce_WorldAttack_Player() = default;
	virtual ~CSequnce_WorldAttack_Player() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQWORLDATTACK_PLY					mSeqData;
	_uint CurrentPath;

public:
	static CSequnce_WorldAttack_Player* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

// 적 자돟공격 시퀀스
class CSequnce_WorldAutoAttack:
	public CSequnce_Base
{
public:
	typedef struct tag_SeqWorldAutoAttack
	{
		CGameObject_3D_Dynamic* Target;

	}SEQWORLDAUTOATTACK;

protected:
	explicit CSequnce_WorldAutoAttack() = default;
	virtual ~CSequnce_WorldAutoAttack() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQWORLDAUTOATTACK					mSeqData;

public:
	static CSequnce_WorldAutoAttack* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};




// Seq_BossPattern
class CSequnce_BossPattern1 :
	public CSequnce_Base
{
public:
	typedef struct tag_SeqBossPattern1
	{

	}SEQBOSS1;

protected:
	explicit CSequnce_BossPattern1() = default;
	virtual ~CSequnce_BossPattern1() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQBOSS1					mSeqData;

public:
	static CSequnce_BossPattern1* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};
class CSequnce_BossPattern2 :
	public CSequnce_Base
{
public:
	typedef struct tag_SeqBossPattern2
	{

	}SEQBOSS2;

protected:
	explicit CSequnce_BossPattern2() = default;
	virtual ~CSequnce_BossPattern2() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQBOSS2					mSeqData;

public:
	static CSequnce_BossPattern2* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};

class CSequnce_BossPattern3 :
	public CSequnce_Base
{
public:
	typedef struct tag_SeqBossPattern1
	{

	}SEQBOSS3;

protected:
	explicit CSequnce_BossPattern3() = default;
	virtual ~CSequnce_BossPattern3() = default;

public:
	// 초기화와 다시시작시 정보 전달과 다름
	virtual HRESULT NativeConstruct(CGameObject_3D_Dynamic* obj)override;
	virtual void Restart(void* SeqData = nullptr)override;

protected:
	SEQBOSS3					mSeqData;

public:
	static CSequnce_BossPattern3* Create(CGameObject_3D_Dynamic* targetobj);
	virtual void Free()override;
};



#pragma endregion SEQ_BASE


END