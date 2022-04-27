#include "stdafx.h"
#include "../Public/GameManager.h"
#include "../Public/Tool/ImguiMgr.h"
#include "../Public/FIleIO/GameObject_Creater.h"
#include "../Public/FIleIO/ObjectIO.h"

IMPLEMENT_SINGLETON(CGameManager);

CGameManager::CGameManager()
{

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
		mIMGUIManager = CImguiMgr::Create(g_hWnd, d, c);
	if (mCreaterManager == nullptr)
		mCreaterManager = CGameObject_Creater::Create(d, c);
	if (mObjectIoManager == nullptr)
		mObjectIoManager = CObjectIO::Create();

	if (mGameInstance == nullptr)
	{
		mGameInstance = GetSingle(CGameInstance);
		Safe_AddRef(mGameInstance);
	}
	Initialize_PathData();
	return S_OK;
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
const list<MYFILEPATH*>* CGameManager::Get_PathList(E_PATHTYPE type) const
{
	if (type == PATHTYPE_END)
		return nullptr;
	return &mListPath[type];
}
HRESULT CGameManager::Initialize_PathData()
{
	// #INIT: 경로 리스트 초기화

	Set_PathData(&mListPath[PATHTYPE_SPRITE], STR_FILEPATH_RESOURCE_SPRITETXT_L, "png");
	Set_PathData(&mListPath[PATHTYPE_FBX], STR_FILEPATH_RESOURCE_3DPATHTXT_L, "fbx");
	Set_PathData(&mListPath[PATHTYPE_FBXTEX], STR_FILEPATH_RESOURCE_3DPATHTXT_L, "png");
	Set_PathData(&mListPath[PATHTYPE_DATA], STR_FILEPATH_RESOURCE_DATPATHTXT_L, "dat");
	
	return S_OK;
}
HRESULT CGameManager::Set_ReListPath(E_PATHTYPE type)
{
	// 경로 업데이트
	switch (type)
	{
	case Client::CGameManager::PATHTYPE_SPRITE:
		Set_PathData(&mListPath[PATHTYPE_SPRITE], STR_FILEPATH_RESOURCE_SPRITETXT_L, "png");
		break;
	case Client::CGameManager::PATHTYPE_FBX:
		Set_PathData(&mListPath[PATHTYPE_FBX], STR_FILEPATH_RESOURCE_3DPATHTXT_L, "fbx");
		break;
	case Client::CGameManager::PATHTYPE_FBXTEX:
		Set_PathData(&mListPath[PATHTYPE_FBXTEX], STR_FILEPATH_RESOURCE_3DPATHTXT_L, "png");
		break;
	case Client::CGameManager::PATHTYPE_DATA:
		Set_PathData(&mListPath[PATHTYPE_DATA], STR_FILEPATH_RESOURCE_DATPATHTXT_L, "dat");
		break;
	case Client::CGameManager::PATHTYPE_END:
		break;
	default:
		break;
	}

	return S_OK;
}
CGameObject * CGameManager::Get_LevelObject_LayerTag(const wchar_t * layerTag, _uint index)
{
	_uint levelIndex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	auto GameObjectList = GetSingle(CGameInstance)->Get_GameObjectLayerList(levelIndex,layerTag);
	if (GameObjectList == nullptr)
		return nullptr;

	auto iter = GameObjectList->begin();
	if (index == 0)
		return  *iter;

	int count = 0;

	while (iter != GameObjectList->end())
	{
		if (index == count)
			break;
		++count;
		++iter;
	}

	if (iter == GameObjectList->end())
		return nullptr;

	return (*iter);
}

HRESULT CGameManager::Set_PathData(list<MYFILEPATH*>* outPathList, wstring str, const char * filetype)
{
	if (outPathList->empty())
		*outPathList = mGameInstance->Load_ExtensionList(str, filetype);
	else
	{
		for (auto pathdata : *outPathList)
		{
			Safe_Delete(pathdata);
		}
		outPathList->clear();
		*outPathList = mGameInstance->Load_ExtensionList(str, filetype);
	}

	return S_OK;
}

HRESULT CGameManager::Safe_Delete_Path(list<MYFILEPATH*>* outData)
{
	for (auto& path: *outData)
	{
		Safe_Delete(path);
	}
	outData->clear();

	return S_OK;
}


void CGameManager::Free()
{
	// Path 데이터 삭제
	for (auto& pathlist : mListPath)
		Safe_Delete_Path(&pathlist);
	


	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	Safe_Release(mCreaterManager);
	Safe_Release(mIMGUIManager);
	Safe_Release(mObjectIoManager);
	Safe_Release(mGameInstance);
}
