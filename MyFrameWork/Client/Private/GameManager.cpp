#include "stdafx.h"
#include "../Public/GameManager.h"
#include "../Public/Tool/ImguiMgr.h"
#include "../Public/FIleIO/GameObject_Creater.h"
#include "../Public/FIleIO/ObjectIO.h"

IMPLEMENT_SINGLETON(CGameManager);

CGameManager::CGameManager()
{

}
HRESULT CGameManager::Update(_double timer)
{

	mIMGUIManager->Update(timer);
	return S_OK;
}
HRESULT CGameManager::Render()
{
	mIMGUIManager->Render();
	return S_OK;
}

CGameObject_Creater * CGameManager::Get_CreaterManager()
{
	return mCreaterManager;
}
CImguiMgr * CGameManager::Get_ImGuiManager()
{
	return mIMGUIManager;
}
CObjectIO * CGameManager::Get_ObjectIOManager()
{
	return mObjectIoManager;
}
list<MYFILEPATH*>* CGameManager::Get_PathList(E_PATHTYPE type)
{
	if (type == PATHTYPE_END)
		return nullptr;
	return &mListPath[type];
}
HRESULT CGameManager::Initialize(ID3D11Device * d, ID3D11DeviceContext * c)
{
	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
	{
		m_pDevice = d;
		m_pDeviceContext = c;
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pDeviceContext);
	}

	// 매니저 초기화
	if (mIMGUIManager == nullptr)
		mIMGUIManager = CImguiMgr::Create(g_hWnd,d,c);
	if (mCreaterManager == nullptr)
		mCreaterManager = CGameObject_Creater::Create(d,c);
	if (mObjectIoManager == nullptr)
		mObjectIoManager = CObjectIO::Create();
	return S_OK;
}

void CGameManager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	Safe_Release(mCreaterManager);
	Safe_Release(mIMGUIManager);
	Safe_Release(mObjectIoManager);
}
