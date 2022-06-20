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
	//Thread �Լ��� �� �ְ�, 2���ڿ� �ڱⰡ ����� ������ ��Ƴ��� Class�� �ּҸ� ����
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