#pragma once
#include "Base.h"

BEGIN(Client)
// IMGUI 생성 삭제 코드만
class CImguiMgr :public CBase
{

	DECLARE_SINGLETON(CImguiMgr);

private:
	explicit CImguiMgr();
	virtual ~CImguiMgr() = default;

	void DestroyIMGUI();
public:
	void InitImGUI(HWND hwnd,ID3D11Device* ppDeviceOut, ID3D11DeviceContext* ppDeviceContextOut);
	void UpdateGUI();
public:
	virtual void Free() override;

};

END

