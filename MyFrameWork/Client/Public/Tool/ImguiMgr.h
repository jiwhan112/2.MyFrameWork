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
	void UpdateGUI();

public:
	void InitImGUI(HWND hwnd, ID3D11Device* ppDeviceOut, ID3D11DeviceContext* ppDeviceContextOut);

	HRESULT Update(_double time);
	HRESULT Render();

private:
	class CImgui_Base* mIMGUI = nullptr;

public:
	virtual void Free() override;
};

END
