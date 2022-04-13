#include "Tool/Imgui_Base.h"

BEGIN(Client)

// IMGUI UI ��

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
	// CImgui_Base��(��) ���� ��ӵ�
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	void Set_UIObject(class CGameObject_2D* obj);

	HRESULT Render_UI();
	//	void Render_Object(CGameObject* obj);


private:
	void Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname);
	void Button_TextureLoader();
	HRESULT Edit_UIObject();
	HRESULT Edit_Texture();

	//void Button_Create2DBuffer();
	//void Texturebb();


	//// UI ���� ����
	//void Button_UISave();

private:
	int item_current = 0;
	// UI ������Ʈ 1�������� ���� �����ڵ� ������
	class CGameObject_2D*	mCurrentUIObject = nullptr;
	list<string>*			mListTextureKey = nullptr;

	// UI ������Ʈ ����Ʈ
	// ���⿡ ����� UISet�� �� ������ ���·� ����ȴ�.
	// list<class CGameObject_2D*> mListUISet;


public:
	static CImgui_UI* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;


};

END
