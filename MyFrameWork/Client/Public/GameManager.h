#pragma once

#include "Base.h"
BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

// 클라이언트의 게임 매니저
// IMGUI / Creater도 여기서 중개자패턴으로 사용

// 경로 저장
// 각종 컴포넌트 이름 저장 // 툴에서 사용

class CGameObject_Creater;
class CImguiMgr;
class CObjectIO;
class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);

public:
	enum E_PATHTYPE
	{
		PATHTYPE_SPRITE, PATHTYPE_FBX, PATHTYPE_FBXTEX, PATHTYPE_END
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

	list<MYFILEPATH*>* Get_PathList(E_PATHTYPE type);




private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	// 클라이언트 전용 매니저들
//	class CGameInstance*		mGameImstance = nullptr;
	CGameObject_Creater*		mCreaterManager = nullptr;
	CImguiMgr*					mIMGUIManager = nullptr;
	CObjectIO*					mObjectIoManager = nullptr;

private:
	list<MYFILEPATH*> mListPath[PATHTYPE_END];

public:
	virtual void Free() override;
};

END