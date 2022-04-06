#include "Tool/Imgui_Base.h"

BEGIN(Client)

// IMGUI 사용할 것만 업데이트

class CImgui_MyDemo final:
	public CImgui_Base
{

private:
	explicit CImgui_MyDemo(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_MyDemo(const CImgui_MyDemo& rhs);
	virtual ~CImgui_MyDemo() = default;
	
public:
	// CImgui_Base을(를) 통해 상속됨
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;
public:
	void DemoTest();
	void FrameUI(_double time);

private:
	// 데모에 나오는 기능 정리
	void IMGUI_TEST_Window();
	void IMGUI_TEST_MyDemo();
	void IMGUI_TEST_Basic();
	void IMGUI_TEST_Details();	
	void AppSimpleOverlay(bool* p_open);


private:
	_double dClock=0;
	int		mFrameCount=0;
	int		mCurrentFrame=0;
	bool bDemo= false;
	float f=0;

public:
	static CImgui_MyDemo* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;





};

END
