#include "Tool/Imgui_Base.h"

BEGIN(Client)

// IMGUI UI 툴

class CImgui_UI final:
	public CImgui_Base
{
public:
	const char* mUIItems[2] = { "PathFinder", "UIEdit",};

private:
	explicit CImgui_UI(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_UI(const CImgui_UI& rhs);
	virtual ~CImgui_UI() = default;
	
public:
	// CImgui_Base을(를) 통해 상속됨
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	void Set_UIObject(class CGameObject_2D* obj);

	HRESULT Render_UI();
	//	void Render_Object(CGameObject* obj);

private:
	void UIMODE();
	void PATHMODE();

private:
	void Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname);
	void Button_TextureLoader();
	HRESULT Edit_ProtoObjectList();
	HRESULT Edit_UIObject();
	HRESULT Edit_Texture();

	//void Button_Create2DBuffer();
	//void Texturebb();


	//// UI 정보 저장
	//void Button_UISave();

	HRESULT Update_ObjectList();
	void Update_ChildObject_ListBox(CGameObject* parent, _uint* cnt, _int* selectindex);

private:
	int item_current = 0;
	// UI 오브젝트 1개에대한 정보 저장코드 만들어보기
	class CGameObject_2D*	mCurrentUIObject = nullptr;
	list<string>*			mListTextureKey = nullptr;

	// UI 오브젝트 리스트
	// 여기에 저장된 UISet이 한 묶음의 형태로 저장된다.
	// list<class CGameObject_2D*> mListUISet;

	// 오브젝트 저장기능
	class CObjectIO*		mObjectSaverClass = nullptr;

	// 오브젝트 생성 
	// CGameObject_Creater*		mCreater = nullptr;

	bool					mIsObjectList = false;


public:
	static CImgui_UI* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;


};

END
