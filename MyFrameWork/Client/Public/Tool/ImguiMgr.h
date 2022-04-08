#pragma once
#include "Base.h"


BEGIN(Client)

// IMGUI 사용할 것만 업데이트

class CImguiMgr :public CBase
{
	DECLARE_SINGLETON(CImguiMgr);

private:
	explicit CImguiMgr();
	virtual ~CImguiMgr() = default;

	void DestroyIMGUI();

public:
	void InitImGUI(HWND hwnd, ID3D11Device* ppDeviceOut, ID3D11DeviceContext* ppDeviceContextOut);

	HRESULT Add_IMGUI(class CImgui_Base* imgui);
	HRESULT Remove_IMGUI(_uint idx);

	HRESULT Update(_double time);
	HRESULT Render();

	
private:
	vector<class CImgui_Base*> mVecIMGUI;

	bool misUpdate = false;


public:
	virtual void Free() override;
};

END
