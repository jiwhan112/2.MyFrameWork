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

protected:
	explicit CDungeon_Task();
	virtual ~CDungeon_Task() = default;

public:
	HRESULT NativeConstruct_Prototype();

public:
	// �۷ι� �۾� ����
	HRESULT Add_Task_Tile();
	HRESULT Add_Task_Gold();


	list<MyTask*> mListTask;


public:
	static CDungeon_Task* Create();
	virtual void Free() override;
};

// Task �θ�
class MyTask
{
public:
	MyTask() = default;
	virtual~MyTask() = default;

};



END