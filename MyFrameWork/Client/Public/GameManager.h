#pragma once

#include "Base.h"
BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

// Ŭ���̾�Ʈ�� ���� �Ŵ���
// IMGUI / Creater�� ���⼭ �߰����������� ���

// ��� ����
// ���� ������Ʈ �̸� ���� // ������ ���

class CGameObject_Creater;
class CImguiMgr;
class CObjectIO;
class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);

public:
	enum E_PATHTYPE
	{
		PATHTYPE_SPRITE, PATHTYPE_FBX, PATHTYPE_FBXTEX, PATHTYPE_DATA, PATHTYPE_END
	};

private:
	CGameManager();
	virtual ~CGameManager() = default;

public:
	HRESULT Initialize(ID3D11Device * d, ID3D11DeviceContext * c);
	HRESULT Update(_double timer);
	HRESULT Render();

public:
	CGameObject_Creater*		Get_CreaterManager();
	CImguiMgr*					Get_ImGuiManager();
	CObjectIO*					Get_ObjectIOManager();

	const list<MYFILEPATH*>* Get_PathList(E_PATHTYPE type) const;

	// ��ε����� ������Ʈ
	HRESULT Set_ReListPath(E_PATHTYPE type);

private:
	// ��� ����
	HRESULT Initialize_PathData();
	HRESULT Set_PathData(list<MYFILEPATH*>* outData,wstring str, const char* filetype);
	HRESULT Safe_Delete_Path(list<MYFILEPATH*>* outData);


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	// Ŭ���̾�Ʈ ���� �Ŵ�����
	CGameInstance*				mGameInstance = nullptr;
	CGameObject_Creater*		mCreaterManager = nullptr;
	CImguiMgr*					mIMGUIManager = nullptr;
	CObjectIO*					mObjectIoManager = nullptr;

private:
	list<MYFILEPATH*> mListPath[PATHTYPE_END];

public:
	virtual void Free() override;
};

END