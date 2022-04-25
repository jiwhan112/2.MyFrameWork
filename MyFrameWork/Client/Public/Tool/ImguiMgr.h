#pragma once
#include "Base.h"


BEGIN(Client)

// IMGUI 사용할 것만 업데이트

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

	class CImgui_Base* Get_IMGUI(_uint index);

private:
	vector<class CImgui_Base*> mVecIMGUI;
	bool misUpdate = false;

public:
	static CImguiMgr* Create(HWND hwnd,ID3D11Device* d, ID3D11DeviceContext* c);

	virtual void Free() override;
};

END
