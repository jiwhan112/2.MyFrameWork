#pragma once

#include "Base.h"

BEGIN(Client)

// IMGUI 사용할 것만 업데이트

// IMGUI 창 DEFINE
#define IMGUI_TREE_BEGIN(x)		if(ImGui::TreeNode(x))
#define IMGUI_TREE_END			ImGui::TreePop();
#define IMGUI_LISTBOX_BEGIN(x)	if(ImGui::ListBox(x))
#define IMGUI_LISTBOX_END		ImGui::EndListBox();
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

	class CImgui_Base abstract :
	public CBase
{
public:
	// 메인 창이름 정의
	enum E_IMGUI_CHANEL
	{
		IMGUI_TITLE_TEST,
		IMGUI_TITLE_MAIN,
		IMGUI_TITLE_OBJECTLIST, // 오브젝트 리스트 출력
		IMGUI_TITLE_UI, // UI 세팅창
		IMGUI_TITLE_FBX, // FBX 세팅창
		IMGUI_TITLE_FBX_ANI, // FBX_ANI 세팅창
		IMGUI_TITLE_TERRAIN, // TERRAIAN 세팅창
		IMGUI_TITLE_INGAME, // INGAME 세팅창
		IMGUI_TITLE_END, //
	};

	const char* STR_IMGUITITLE(E_IMGUI_CHANEL e)
	{
		switch (e)
		{
		case IMGUI_TITLE_TEST:
			return "IMGUI_TITLE_TEST";
		case IMGUI_TITLE_MAIN:
			return "IMGUI_TITLE_MAIN";
		case IMGUI_TITLE_UI:
			return "IMGUI_TITLE_UI";
		case IMGUI_TITLE_OBJECTLIST:
			return "IMGUI_TITLE_OBJECTLIST";
		case IMGUI_TITLE_FBX:
			return "IMGUI_TITLE_FBX";
		case IMGUI_TITLE_FBX_ANI:
			return "IMGUI_TITLE_FBX_ANI";
		case IMGUI_TITLE_TERRAIN:
			return "IMGUI_TITLE_TERRAIN";
		case IMGUI_TITLE_END:
			return "IMGUI_TITLE_END";
		default:
			return "IMGUI_TITLE_END";
		}
	};

	string STR_IMGUITITLE_MINI(E_IMGUI_CHANEL e)
	{
		switch (e)
		{
		case IMGUI_TITLE_TEST:
			return "TEST";
		case IMGUI_TITLE_MAIN:
			return "MAIN";
		case IMGUI_TITLE_UI:
			return "UI";
		case IMGUI_TITLE_OBJECTLIST:
			return "OBJECTLIST";
		case IMGUI_TITLE_FBX:
			return "FBX";
		case IMGUI_TITLE_FBX_ANI:
			return "ANI";
		case IMGUI_TITLE_TERRAIN:
			return "TERRAIN";
		case IMGUI_TITLE_END:
			return "ERROR";
		default:
			return "ERROR";
		}
	};

	// 같은 기능 ID 구분을 위함
	const char* STR_IMGUI_IDSTR(E_IMGUI_CHANEL type, const char* str)
	{
		mTestStr = STR_IMGUITITLE_MINI(type);
		mTestStr += '_';
		mTestStr += str;

		return mTestStr.c_str();
	}

protected:
	explicit CImgui_Base(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_Base(const CImgui_Base& rhs) = default;
	virtual ~CImgui_Base() = default;

public:
	virtual HRESULT NativeConstruct()PURE;
	virtual HRESULT Update(_double time)PURE;

protected:
	ID3D11Device*			mDevice = nullptr;
	ID3D11DeviceContext*	mDeviceContext = nullptr;

	string					mTestStr = "";

public:

	virtual void Free() override;
};

END
