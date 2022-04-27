#include "Tool/Imgui_Base.h"

BEGIN(Client)

// UI툴
// UI DESC 저장
// 패스 데이터 저장

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
	HRESULT Render_UI();

private:
	void UI_SETTINGMODE();
	void UI_CREATEMODE();

private:
	HRESULT Edit_UIObject();
	HRESULT Edit_Texture();

private:
	// UI 오브젝트 1개에대한 정보 저장코드 만들어보기
	class CGameObject_2D*	mCurrentUIObject = nullptr;
	list<string>*			mSpritepathList = nullptr;

	// UI 오브젝트 리스트
	// 여기에 저장된 UISet이 한 묶음의 형태로 저장된다.
	// list<class CGameObject_2D*> mListUISet;

	bool					mIsDataSetting = false;



public:
	static CImgui_UI* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;


};

END
