#pragma once

#include "Base.h"

BEGIN(Client)

// IMGUI ����� �͸� ������Ʈ


// IMGUI â DEFINE
#define IMGUI_TREE_BEGIN(x)	if(ImGui::TreeNode(x))
#define IMGUI_TREE_END		ImGui::TreePop();
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))


class CImgui_Base abstract:
	public CBase
{
public:
	// ���� â�̸� ����
	enum E_IMGUI_CHANEL
	{
		IMGUI_TITLE_TEST,
		IMGUI_TITLE_UI,
		IMGUI_TITLE_OBJECT,
		IMGUI_TITLE_FBX,
		IMGUI_TITLE_END,

	};

	const char* STR_IMGUITITLE(E_IMGUI_CHANEL e)
	{
		switch (e)
		{
		case IMGUI_TITLE_TEST:
			return "IMGUI_TITLE_TEST";
		case IMGUI_TITLE_UI:
			return "IMGUI_TITLE_UI";
		case IMGUI_TITLE_OBJECT:
			return "IMGUI_TITLE_OBJECT";
		case IMGUI_TITLE_FBX:
			return "IMGUI_TITLE_FBX";
		case IMGUI_TITLE_END:
			return "IMGUI_TITLE_END";
		default:
			return "IMGUI_TITLE_END";
		}
	};

protected:
	explicit CImgui_Base(ID3D11Device* device , ID3D11DeviceContext* context);
	explicit CImgui_Base(const CImgui_Base& rhs) = default;
	virtual ~CImgui_Base() = default;

public:
	virtual HRESULT NativeConstruct()PURE;
	virtual HRESULT Update(_double time)PURE;

protected:
	ID3D11Device*			mDevice = nullptr;
	ID3D11DeviceContext*	mDeviceContext = nullptr;

	
public:

	virtual void Free() override;
};

END
