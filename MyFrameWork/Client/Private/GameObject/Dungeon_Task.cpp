#include "stdafx.h"
#include "GameObject/Dungeon_Task.h"
#include "GameObject/Client_Object.h"

CDungeon_Task::CDungeon_Task()
	:CBase()
{
}

HRESULT CDungeon_Task::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CDungeon_Task::Add_Task(TASKBASE * task)
{
	if (task != nullptr)
	{
		mListTask.push_front(task);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CDungeon_Task::Add_Task(CDungeon_Task::E_TASK_TYPE id,_uint index)
{
	TASKBASE* base = nullptr;
	switch (id)
	{
	case Client::CDungeon_Task::TASK_TILE:
		base = NEW TASKTILE(index);
		base->mTaskID = id;
		break;
	case Client::CDungeon_Task::TASK_GOLD:
		base = NEW TASKTILE(index);
		base->mTaskID = id;	
		break;
	case Client::CDungeon_Task::TASK_MOVE_WORLD:
		base = NEW TASKMAP(index);
		base->mTaskID = id; 
		break;
	case Client::CDungeon_Task::TASK_END:
		break;
	default:
		break;
	}
	FAILED_CHECK(Add_Task(base));
	return S_OK;
}

HRESULT CDungeon_Task::Add_Task_Tile_Rock(_uint index)
{
	return Add_Task(CDungeon_Task::E_TASK_TYPE::TASK_TILE, index);
}

HRESULT CDungeon_Task::Add_Task_Tile_Gold(_uint index)
{
	return Add_Task(CDungeon_Task::E_TASK_TYPE::TASK_GOLD, index);
}

HRESULT CDungeon_Task::Add_Task_Tile_MoveWorld(_uint index)
{

	return Add_Task(CDungeon_Task::E_TASK_TYPE::TASK_MOVE_WORLD, index);
}


TASKBASE * CDungeon_Task::Get_BackTask()
{
	if (mListTask.empty())
		return nullptr;

	TASKBASE* baseTask = mListTask.back();
	mListTask.pop_back();

	return baseTask;
}

CDungeon_Task * CDungeon_Task::Create()
{
	CDungeon_Task*	pInstance = NEW CDungeon_Task();

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CDungeon_Task::Free()
{
	for (auto& task: mListTask)
	{
		Safe_Delete(task);
	}
	mListTask.clear();
}