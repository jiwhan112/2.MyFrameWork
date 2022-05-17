#include "stdafx.h"
#include "../Public/GameManager.h"
#include "../Public/Tool/ImguiMgr.h"
#include "../Public/FIleIO/GameObject_Creater.h"
#include "../Public/FIleIO/ObjectIO.h"
#include "../Public/GameObject/Daungon_Manager.h"
#include "../Public/System/ColliderManager.h"


#include "Level_Logo.h"
#include "Level_Loader.h"

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

	if (mDaungonManager == nullptr)
		mDaungonManager = CDaungon_Manager::Create();
	if (mColliderManager == nullptr)
		mColliderManager = CColliderManager::Create();

	if (mGameInstance == nullptr)
	{
		mGameInstance = GetSingle(CGameInstance);
		Safe_AddRef(mGameInstance);
	}
	Initialize_PathData();
	return S_OK;
}

HRESULT CGameManager::Tick(_double timer)
{
	mColliderManager->Tick_ColliderCheck(timer);
	mDaungonManager->Tick(timer);
	mIMGUIManager->Update(timer);

	CGameInstance*	pGameInstance = GetSingle(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_RETURN) & DIS_Down)
	{
		//	FAILED_CHECK(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY)));
	}
	int idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	if (idx == E_LEVEL::LEVEL_LOGO)
	{
		if (pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
			// 엔진에서 클리어 하기전에 한번 해준다. 
			LevelLoading(LEVEL_TOOL);

		if (pGameInstance->Get_DIKeyState(DIK_F2) & DIS_Down)
			LevelLoading(LEVEL_MYGAMEPLAY);
	}
	else
	{
		if (pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
			LevelLoading(LEVEL_LOGO);
	}

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
CDaungon_Manager * CGameManager::Get_DaungonManager()
{
	return mDaungonManager;
}
CColliderManager * CGameManager::Get_ColliderManager()
{
	if (mColliderManager == nullptr)
		return nullptr;

	return mColliderManager;
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
	Set_PathData(&mListPath[PATHTYPE_FBX_STATIC], STR_FILEPATH_RESOURCE_3DPATHTXT_STATIC_L, "fbx");
	Set_PathData(&mListPath[PATHTYPE_FBX_DYNAMIC], STR_FILEPATH_RESOURCE_3DPATHTXT_DYNAMIC_L, "fbx");
	Set_PathData(&mListPath[PATHTYPE_FBXTEX], STR_FILEPATH_RESOURCE_3DTEXPATHHTXT_L, "png");
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

	case Client::CGameManager::PATHTYPE_FBX_STATIC:
		Set_PathData(&mListPath[PATHTYPE_FBX_STATIC], STR_FILEPATH_RESOURCE_3DPATHTXT_STATIC_L, "fbx");

	case Client::CGameManager::PATHTYPE_FBX_DYNAMIC:
		Set_PathData(&mListPath[PATHTYPE_FBX_DYNAMIC], STR_FILEPATH_RESOURCE_3DPATHTXT_DYNAMIC_L, "fbx");
		break;

	case Client::CGameManager::PATHTYPE_FBXTEX:
		Set_PathData(&mListPath[PATHTYPE_FBXTEX], STR_FILEPATH_RESOURCE_3DTEXPATHHTXT_L, "png");
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
	auto GameObjectList = GetSingle(CGameInstance)->Get_GameObjectLayerList(levelIndex, layerTag);
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

const list<CGameObject*>* CGameManager::Get_LevelObjectList(const wchar_t * layerTag)
{
	_uint levelIndex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	const list<CGameObject*>* GameObjectList = GetSingle(CGameInstance)->Get_GameObjectLayerList(levelIndex, layerTag);
	if (GameObjectList == nullptr)
		return nullptr;
	return GameObjectList;
}
const _float3 & CGameManager::Get_PickPos() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	if (mColliderManager == nullptr)
		return _float3();

	return mColliderManager->Get_PickPos();
}
const _bool & CGameManager::Get_IsMousePick() const
{
	if (mColliderManager == nullptr)
		return false;

	// TODO: 여기에 반환 구문을 삽입합니다.
	return mColliderManager->Get_IsMousePick();
}
HRESULT CGameManager::Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE e, CGameObject_Base * col)
{
	if (mColliderManager != nullptr)
		return mColliderManager->Add_ColliderObject(e, col);

	return E_FAIL;
}
HRESULT CGameManager::ClearLevel()
{
	if (mDaungonManager != nullptr)
		mDaungonManager->Release_DaungonData();
	if (mColliderManager != nullptr)
		mColliderManager->ReleaseObjects();

	return S_OK;
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
	for (auto& path : *outData)
	{
		Safe_Delete(path);
	}
	outData->clear();

	return S_OK;
}

HRESULT CGameManager::LevelLoading(E_LEVEL nextLevel)
{
	CGameInstance*	pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(ClearLevel());
	FAILED_CHECK(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, nextLevel)));


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
	Safe_Release(mDaungonManager);
	Safe_Release(mColliderManager);
	Safe_Release(mGameInstance);
}