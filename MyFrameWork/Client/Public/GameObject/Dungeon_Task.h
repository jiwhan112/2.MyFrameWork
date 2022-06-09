#pragma once

#include "Base.h"

BEGIN(Client)

// 게임 전역 이벤트 관리
struct TASKBASE;

class CDungeon_Task final :
	public CBase
{
public:
	enum E_TASK_TYPE
	{
		TASK_TILE,
		TASK_GOLD,

		// 월드 명령
		TASK_MOVE_WORLD, 
		TASK_ATTACK_WORLD, 

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


private:
	HRESULT Add_Task(TASKBASE* task);
	HRESULT Add_Task(CDungeon_Task::E_TASK_TYPE id, void* desc);

public:
	// 글로벌 작업 저장
	HRESULT Add_Task_Tile_Rock(_uint index);
	HRESULT Add_Task_Tile_Gold(_uint index);
	HRESULT Add_Task_Tile_MoveWorld(_float3 Worldpos);

	TASKBASE* Get_BackTask();

private:
	list<TASKBASE*> mListTask;

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

//class CMyTask
//	: public CBase
//{
//private:
//	CMyTask() = default;
//	virtual ~CMyTask() = default;
//
//public:
//
//	// CBase을(를) 통해 상속됨
//	virtual void Free() override;
//	
//public:
//	CDungeon_Task::E_TASK_TYPE mTaskID = CDungeon_Task::TASK_END;
//};
//
//class CMyTask_Tile
//	:public CMyTask
//{
//	// 타일 인덱스 저장
//private:
//	CMyTask_Tile() = default;
//	virtual ~CMyTask_Tile() = default;
//public:
//	HRESULT NativeConstruct(CDungeon_Task::E_TASK_TYPE id)
//	{
//		mTaskID = id;
//		return S_OK;
//	}
//	virtual void Free() override;
//	_uint						mTileIndex = 0;
//
//
//};




END