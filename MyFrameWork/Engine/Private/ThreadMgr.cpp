#include "ThreadMgr.h"


IMPLEMENT_SINGLETON(CThreadMgr)


//다쓴 쓰래드를 정리하려고 만든 쓰래드
void CALLBACK EraseFinishedThread(void* _Prameter)
{
	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;

	vector<HANDLE>*	 vecThread = ((CThreadMgr*)(tThreadArg.pArg))->Get_vecThread();

	BOOL	bResult = false;
	DWORD	dwExitCode = 0;

	while (true)
	{
		//매번 돌릴 필요 없으니까 1초에 한번씩 돌리자... 근데 이거 종료 타이밍 잘 못 맞추면 오류있을거같음
		//근데 다쓴 쓰래드를 삭제하는 코드는 여기랑 Free 함수에 밖에 없기 때문에 괜찬을거같음
		Sleep(1000);
		if ((*tThreadArg.IsClientQuit))
			break;


		//어차피 스래드 컬랙터는 계속 돌테니까 1개 이상일떄만 돌리자
		if (vecThread->size() > 1) 
		{

			//포문 돌면서 오류 날수도있을거같음 ex 다른곳에서 추가하다가 갯수 초과해서 넘어가질 수 도 있지 않을까 생각함
			auto iter = vecThread->begin();

			for (; iter != vecThread->end();)
			{
				bResult = false;
				dwExitCode = 0;

				bResult = GetExitCodeThread((*iter), &dwExitCode);
				//작동 중이라면 넘어가고
				if (dwExitCode == STILL_ACTIVE)
				{
					iter++;
					continue;
				}

				//작업이 끝난 녀석은 삭제하자

				WaitForSingleObject((*iter), INFINITE);
				CloseHandle((*iter));
				DeleteObject((*iter));

				iter = vecThread->erase(iter);
			}

		}

	}



}

CThreadMgr::CThreadMgr()
	:m_bIsClientQuit(false)
{
	InitializeCriticalSection(&m_CriSec);
	m_vecThread.reserve(10);

	//사용되지 않는 쓰래드를 정리하는 콜랙터
	PlayThread(EraseFinishedThread, this);
}


//Client에서 Thread함수 만들 때 무조건 마지막에 클라 종료를 알리는 bool 변수와 Mutex Pointer를 맴버 변수로 받을 것
HRESULT CThreadMgr::PlayThread(void* _ThreadFunc, void* _pArg)
{
	//포인터로 관리할꺼면 바인드 써서 만들면 될듯
	if (_ThreadFunc == nullptr)
		return E_FAIL;

	THREADARG* pArg = new THREADARG;
	pArg->pArg			= _pArg;
	pArg->CriSec			= &m_CriSec;
	pArg->IsClientQuit	= &m_bIsClientQuit;


	m_vecThread.emplace_back(
		(HANDLE)_beginthreadex(nullptr, 0, _beginthreadex_proc_type(_ThreadFunc), pArg, 0, nullptr)
	);

	return S_OK;

}


void CThreadMgr::Free()
{
	m_bIsClientQuit = true;

	for (auto& Handle : m_vecThread)
	{
		WaitForSingleObject(Handle, INFINITE);
		CloseHandle(Handle);
		DeleteObject(Handle);
	}

	m_vecThread.clear();
	m_vecThread.swap(vector<HANDLE>());

	DeleteCriticalSection(&m_CriSec);

}
