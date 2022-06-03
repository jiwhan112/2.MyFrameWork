#pragma once

#include "Base.h"

BEGIN(Client)


// 게임 오브젝트 생성 저장
// 초기화 타일 생성등 다 여기로 옮김

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
	// 글로벌 작업 저장
	HRESULT Add_Task_Tile();
	HRESULT Add_Task_Gold();


	list<MyTask*> mListTask;


public:
	static CDungeon_Task* Create();
	virtual void Free() override;
};

// Task 부모
class MyTask
{
public:
	MyTask() = default;
	virtual~MyTask() = default;

};



END