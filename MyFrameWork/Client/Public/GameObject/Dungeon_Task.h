#pragma once

#include "Base.h"

BEGIN(Client)


// ���� ������Ʈ ���� ����
// �ʱ�ȭ Ÿ�� ������ �� ����� �ű�

class MyTask;
class CDungeon_Task final :
	public CBase
{
public:
	enum E_TASK_TYPE
	{
		TASK_TILE,
		TASK_GOLD,
		TASK_END
	};
protected:
	explicit CDungeon_Task();
	virtual ~CDungeon_Task() = default;

public:
	HRESULT NativeConstruct_Prototype();

private:
	list<MyTask*> mListTask;

public:
	// �۷ι� �۾� ����
	HRESULT Add_Task_Tile(CDungeon_Task::E_TASK_TYPE id, _uint index);
	HRESULT Add_Task_Tile_Rock(_uint index);
	HRESULT Add_Task_Tile_Gold(_uint index);
	MyTask* Get_BackTask();


public:
	static CDungeon_Task* Create();
	virtual void Free() override;
};

// Task �θ�
class MyTask
	: public CBase
{
private:
	MyTask() = default;
	virtual~MyTask() = default;

public:
	HRESULT NativeConstruct(CDungeon_Task::E_TASK_TYPE id, _uint index)
	{
		mTaskID = id;
		mTileIndex = index;
		return S_OK;
	}

	static MyTask* Create(CDungeon_Task::E_TASK_TYPE id, _uint index);
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
	
public:
	CDungeon_Task::E_TASK_TYPE mTaskID = CDungeon_Task::TASK_END;
	_uint						mTileIndex = 0;
};

END