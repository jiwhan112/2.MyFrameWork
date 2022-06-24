#pragma once
#include "Base.h"

BEGIN(Client)

// IMGUI ����� �͸� ������Ʈ
// #Tag IMGUI ��� �߰�

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
	// �⺻ UI �����س���
	class CImgui_CommonUI*		mCommonUI = nullptr;

public:
	static CImguiMgr* Create(HWND hwnd, ID3D11Device* d, ID3D11DeviceContext* c);

	virtual void Free() override;
};

END
