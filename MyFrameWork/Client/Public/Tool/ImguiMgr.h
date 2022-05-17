#pragma once
#include "Base.h"

BEGIN(Client)

// IMGUI 사용할 것만 업데이트
// #Tag IMGUI 헤더 추가

class CImguiMgr :public CBase
{
private:
	explicit CImguiMgr() = default;
	virtual ~CImguiMgr() = default;

	void DestroyIMGUI();

public:
	HRESULT NativeConstruct(HWND hwnd, ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);

	HRESULT Add_IMGUI(class CImgui_Base* imgui);
	HRESULT Remove_IMGUI(_uint idx);

	HRESULT Update(_double time);
	HRESULT Render();

	HRESULT ClearScene();


public:
	class CImgui_Base* Get_IMGUI(_uint index);
	CGameObject* Get_SelectObject() const;

private:
	vector<class CImgui_Base*> mVecIMGUI;
	bool misUpdate = false;

private:
	// 기본 UI 저장해놓음
	class CImgui_CommonUI*		mCommonUI = nullptr;

public:
	static CImguiMgr* Create(HWND hwnd, ID3D11Device* d, ID3D11DeviceContext* c);

	virtual void Free() override;
};

END
