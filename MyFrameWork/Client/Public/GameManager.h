#pragma once

#include "Base.h"
#include "System/ColliderManager.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)

// Ŭ���̾�Ʈ�� ���� �Ŵ���
// IMGUI / Creater�� ���⼭ �߰����������� ���

// ��� ����
// ���� ������Ʈ �̸� ���� // ������ ���

class CGameObject_Creater;
class CImguiMgr;
class CObjectIO;
class CDaungon_Manager;
// class CColliderManager;

class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);

public:
	// ���ҽ��� �ش��ϴ� ���� ���
	// ��������Ʈ 3D������ ���̳ʸ� ����...
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
	HRESULT Render();

public:
	CGameObject_Creater*		Get_CreaterManager();
	CImguiMgr*					Get_ImGuiManager();
	CObjectIO*					Get_ObjectIOManager();
	CDaungon_Manager*			Get_DaungonManager();
	CColliderManager*			Get_ColliderManager();

	const list<MYFILEPATH*>* Get_PathList(E_PATHTYPE type) const;

	// ��ε����� ������Ʈ
	HRESULT Set_ReListPath(E_PATHTYPE type);

	// ī�޶� �÷��̾� �����ö� ���� ���ϰ� ������
	CGameObject* Get_LevelObject_LayerTag(const wchar_t* layerTag, _uint index = 0);
	const list<CGameObject*>* Get_LevelObjectList(const wchar_t* layerTag);

public: // Collider 
	const _float3& Get_PickPos() const;
	const _bool& Get_IsMousePick()const;
	HRESULT Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE e, CGameObject_Base * col);


private:
	// ��� ����
	HRESULT Initialize_PathData();
	HRESULT Set_PathData(list<MYFILEPATH*>* outData, wstring str, const char* filetype, bool bFlag = true);
	HRESULT Safe_Delete_Path(list<MYFILEPATH*>* outData);

private:
	HRESULT LevelLoading(E_LEVEL nextLevel);
	HRESULT ClearLevel();


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	// Ŭ���̾�Ʈ ���� �Ŵ�����
	CGameInstance*				mGameInstance = nullptr;
	CGameObject_Creater*		mCreaterManager = nullptr;
	CImguiMgr*					mIMGUIManager = nullptr;
	CObjectIO*					mObjectIoManager = nullptr;

	CDaungon_Manager*			mDaungonManager = nullptr;	
	CColliderManager*			mColliderManager = nullptr;


private:
	list<MYFILEPATH*> mListPath[PATHTYPE_END];

public:
	virtual void Free() override;
};

END