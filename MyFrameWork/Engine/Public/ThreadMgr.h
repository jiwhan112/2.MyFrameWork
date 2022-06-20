#pragma once

#include "Base.h"

BEGIN(Engine)


class CThreadMgr final :public CBase
{
	DECLARE_SINGLETON(CThreadMgr)
private:
	explicit CThreadMgr();
	virtual ~CThreadMgr() = default;

public:
	//Thread 함수의 명 넣고, 2인자에 자기가 사용할 변수를 모아놓은 Class의 주소를 넣자
	HRESULT PlayThread(void* _ThreadFunc,  void* _pArg);
	vector<HANDLE>*		Get_vecThread() { return &m_vecThread; };


private:
	CRITICAL_SECTION	m_CriSec;
	vector<HANDLE>		m_vecThread;

	_bool				m_bIsClientQuit = false;

public:
	virtual void Free()override;
};



END