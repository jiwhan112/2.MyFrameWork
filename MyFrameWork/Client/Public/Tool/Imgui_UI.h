#include "Tool/Imgui_Base.h"

BEGIN(Client)

// IMGUI 사용할 것만 업데이트

class CImgui_UI final:
	public CImgui_Base
{

private:
	explicit CImgui_UI(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_UI(const CImgui_UI& rhs);
	virtual ~CImgui_UI() = default;
	
public:
	// CImgui_Base을(를) 통해 상속됨
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;
public:
	void DemoTest();
	void FrameUI(_double time);

	void UISaver();
	void TextureSaver();

public:

	// 데모에 나오는 기능 정리
	void TestMenuBar();
	void TestWidgets();
	void TestUI3();
	void TestUI4();



private:
	_double dClock=0;
	int		mFrameCount=0;
	int		mCurrentFrame=0;
	bool bDemo= false;
	float f=0;

public:
	static CImgui_UI* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;





};

END
