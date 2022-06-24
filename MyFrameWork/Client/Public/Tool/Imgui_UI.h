#include "Tool/Imgui_Base.h"

BEGIN(Client)

// UI��
// UI DESC ����
// �н� ������ ����

class CImgui_UI final :
	public CImgui_Base
{
public:
	enum E_UITOOL_MODE
	{
		UITOOL_MODE_2D,
		UITOOL_MODE_PARTICLE,
		UITOOL_MODE_END,

	};
private:
	explicit CImgui_UI(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_UI(const CImgui_UI& rhs);
	virtual ~CImgui_UI() = default;

public:
	// CImgui_Base��(��) ���� ��ӵ�
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
	// UI ������Ʈ 1�������� ���� �����ڵ� ������
	class CGameObject_2D*	mCurrentUIObject = nullptr;
	list<string>*			mSpritepathList = nullptr;
	list<string>*			mProtoUIList = nullptr;

	// UI ������Ʈ ����Ʈ
	// ���⿡ ����� UISet�� �� ������ ���·� ����ȴ�.
	// list<class CGameObject_2D*> mListUISet;

	bool						mIsDataSetting = false;
	//	E_UITOOL_MODE			meUIToolMode = UITOOL_MODE_END;

public:
	static CImgui_UI* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
