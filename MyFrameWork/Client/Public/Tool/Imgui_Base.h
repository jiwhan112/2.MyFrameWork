#include "Base.h"

BEGIN(Client)

// IMGUI 사용할 것만 업데이트

class CImgui_Base abstract:
	public CBase
{
public:
	// 메인 창이름 정의
	enum E_IMGUI_CHANEL
	{
		IMGUI_CHANEL_TEST,
		IMGUI_CHANEL_MAP,
		IMGUI_CHANEL_END,

	};
	const char* IMGUI_MAIN[IMGUI_CHANEL_END] =
	{
		"TestBar",
		"MapTool"

	};

	const char* TAG_IMGUI(E_IMGUI_CHANEL e)
	{
		return IMGUI_MAIN[e];

	}

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
