#pragma once

#include "Base.h"
#include "System/ColliderManager.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)

// 클라이언트의 게임 매니저
// IMGUI / Creater도 여기서 중개자패턴으로 사용

// 경로 저장
// 각종 컴포넌트 이름 저장 // 툴에서 사용

class CGameObject_Creater;
class CImguiMgr;
class CObjectIO;
class CDungeon_Manager;
class CGameObject_MyTerrain;
class CCamera_Client;

class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);

public:
	// 리소스에 해당하는 파일 경로
	// 스프라이트 3D데이터 바이너리 사운드...
	enum E_PATHTYPE
	{
		PATHTYPE_SPRITE, PATHTYPE_FBX_STATIC, PATHTYPE_FBX_DYNAMIC, PATHTYPE_FBXTEX, PATHTYPE_DATA, PATHTYPE_DESC, PATHTYPE_SOUND, PATHTYPE_END
	};

private:
	CGameManager();
	virtual ~CGameManager() = default;

public:
	HRESULT Initialize(ID3D11Device * d, ID3D11DeviceContext * c);
	HRESULT Tick(_double timer);
	HRESULT LateTick(_double timer);
	HRESULT Render();

public:
	CGameObject_Creater*		Get_CreaterManager();
	CImguiMgr*					Get_ImGuiManager();
	CObjectIO*					Get_ObjectIOManager();
	CDungeon_Manager*			Get_DaungonManager();
	CColliderManager*			Get_ColliderManager();

	const list<MYFILEPATH*>* Get_PathList(E_PATHTYPE type) const;

	// 경로데이터 업데이트
	HRESULT Set_ReListPath(E_PATHTYPE type);

	// 카메라나 플레이어 가져올때 좀더 편하게 가져옴
	CGameObject* Get_LevelObject_LayerTag(const wchar_t* layerTag, _uint index = 0);
	CCamera_Client* Get_LevelObject_Camera();
	CGameObject_MyTerrain* Get_LevelObject_DUNGEONMAP();
	CGameObject_MyTerrain* Get_LevelObject_WORLDMAP();

	const list<CGameObject*>* Get_LevelObject_List(const wchar_t* layerTag);
	E_LEVEL Get_CurrentLevel() const { return mCurrentLevel; }
	HRESULT Set_VisibleTag(const wchar_t* layerTag,bool b);


public: // Collider 
	const _float3& Get_PickPos() const;
	const _bool& Get_IsMousePick()const;
	HRESULT Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE e, CGameObject_Base * col);


private:// 경로 저장	
	HRESULT Initialize_PathData();
	HRESULT Set_PathData(list<MYFILEPATH*>* outData, wstring str, const char* filetype, bool bFlag = true);
	HRESULT Safe_Delete_Path(list<MYFILEPATH*>* outData);

private:
	// 게임 매니저용 클리어 레벨
	HRESULT LevelLoading(E_LEVEL nextLevel);
	HRESULT ClearLevel();
	HRESULT LevelChanger();


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	// 클라이언트 전용 매니저들
	CGameInstance*				mGameInstance = nullptr;
	CGameObject_Creater*		mCreaterManager = nullptr;
	CImguiMgr*					mIMGUIManager = nullptr;
	CObjectIO*					mObjectIoManager = nullptr;

	CDungeon_Manager*			mDaungonManager = nullptr;	
	CColliderManager*			mColliderManager = nullptr;

	E_LEVEL						mCurrentLevel = LEVEL_END;

private:
	list<MYFILEPATH*> mListPath[PATHTYPE_END];

public:
	virtual void Free() override;
};

END