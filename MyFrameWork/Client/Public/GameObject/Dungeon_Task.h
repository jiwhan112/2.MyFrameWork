#pragma once

#include "Base.h"

BEGIN(Client)

// 게임 전역 이벤트 관리
class CGameObject_Base;

struct TASKBASE;
struct TASKMAP_TIMER;

class CDungeon_Task final :
	public CBase
{
public:
	enum E_TASK_TYPE
	{
		TASK_TILE,
		TASK_GOLD,
		TASK_TILE_ROOM, // 방 타일 바꾸기
		TASK_BUILD,		// 방에 침대 생성

		// 월드 명령
		TASK_PLAYER_MOVE_WORLD, 
		TASK_PLAYER_ATTACK_WORLD, 

		TASK_ENEMY_MOVE_WORLD,
		TASK_ENEMY_MOVE_WORLD_DEALY,
		TASK_ENEMY_ATTACK_WORLD,


		// 마법 명령
		TASK_MAGIC_AA,
		TASK_MAGIC_BB,

		// 카메라 나중에 구현
		TASK_CAMERA_BOSS,
		TASK_CAMERA_INTRO,
		TASK_END
	};
protected:
	explicit CDungeon_Task();
	virtual ~CDungeon_Task() = default;

public:
	HRESULT NativeConstruct_Prototype();

	HRESULT Add_ReTask(TASKBASE* task);

private:
	HRESULT Add_Task(TASKBASE* task);
	HRESULT Add_Task(CDungeon_Task::E_TASK_TYPE id, void* desc);

public:
	// 글로벌 작업 저장
	HRESULT Add_Task_Tile_Rock(_uint index);
	HRESULT Add_Task_Tile_Gold(_uint index);
	HRESULT Add_Task_Tile_Room(_float3 pos);
	HRESULT Add_Task_Tile_Build(_float3 pos);

	// 플레이어 캐릭터 공격
	HRESULT Add_Task_PlayerMoveWorld(_float3 Worldpos);
	HRESULT Add_Task_PlayerAttackWorld(CGameObject_Base* Target);

	// 적 캐릭터 공격
	HRESULT Add_Task_EnemyMoveWorld(_float3 Worldpos);
	HRESULT Add_Task_EnemyMoveWorld_Dealy(TASKMAP_TIMER desc);

	TASKBASE* Get_BackTask();

private:
	list<TASKBASE*> mListTask;
	list<TASKBASE*> mReListTask;

public:
	static CDungeon_Task* Create();
	virtual void Free() override;
};

// Task 부모

struct TASKBASE
{
public:
	CDungeon_Task::E_TASK_TYPE mTaskID = CDungeon_Task::TASK_END;

};

struct TASKTILE
	:public TASKBASE
{
public:
	TASKTILE(_uint index) { mTileIndex = index; }

public:
	_uint						mTileIndex = 0;

};

struct TASKMAP
	:public TASKBASE
{
public:
	TASKMAP(_float3 Pos) { mWorldMapPickPos = Pos; }

public:
	_float3						mWorldMapPickPos ;

};

struct TASKMAP_TIMER
	:public TASKBASE
{
	TASKMAP_TIMER(_double timeMax,_float3 worldPos) 
	{
		mTimer = timeMax;
		mWorldMapPickPos = worldPos;
	}

	_float3						mWorldMapPickPos;
	_double						mTimer;
};

struct TASK_OBJECT
	:public TASKBASE
{
	TASK_OBJECT(CGameObject_Base* obj)
	{
		Target = obj;
	}

	CGameObject_Base* Target = nullptr;
};


struct TASK_VOID
	:public TASKBASE
{
	TASK_VOID(void* arg)
	{
		pArg = arg;
	}

	void* pArg = nullptr;
};
END