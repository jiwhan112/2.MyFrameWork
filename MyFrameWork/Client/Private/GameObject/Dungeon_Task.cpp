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

HRESULT CDungeon_Task::Add_Task_Tile(CDungeon_Task::E_TASK_TYPE id, _uint index)
{
	MyTask* Task = MyTask::Create(id, index);
	if (Task == nullptr)
		return E_FAIL;
	mListTask.push_front(Task);
	return S_OK;
}

HRESULT CDungeon_Task::Add_Task_Tile_Rock(_uint index)
{
	return Add_Task_Tile(CDungeon_Task::E_TASK_TYPE::TASK_TILE, index);
}

HRESULT CDungeon_Task::Add_Task_Tile_Gold(_uint index)
{
	return Add_Task_Tile(CDungeon_Task::E_TASK_TYPE::TASK_GOLD, index);
}

MyTask * CDungeon_Task::Get_BackTask()
{
	if (mListTask.empty())
		return nullptr;

	MyTask* task =  mListTask.back();
	mListTask.pop_back();

	return task;
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
		Safe_Release(task);
	}
}

MyTask * MyTask::Create(CDungeon_Task::E_TASK_TYPE id, _uint index)
{
	MyTask*	pInstance = NEW MyTask();

	if (FAILED(pInstance->NativeConstruct(id, index)))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MyTask::Free()
{
}
