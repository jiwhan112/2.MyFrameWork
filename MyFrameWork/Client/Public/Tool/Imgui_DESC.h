#include "Tool/Imgui_Base.h"

BEGIN(Engine)
class CModel;
class CAnimationClip;
END
BEGIN(Client)

// DESC 툴 

class CImgui_DESC final :
	public CImgui_Base
{
public:
	enum E_TOOLMODE_DESC
	{
		TOOLMODE_DESC_NONE,
		TOOLMODE_DESC_END,
	};
private:
	explicit CImgui_DESC(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_DESC(const CImgui_DESC& rhs);
	virtual ~CImgui_DESC() = default;

public:
	// CImgui_Base을(를) 통해 상속됨
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	HRESULT Render_UI();

private:
	HRESULT		INIT_DESCPathList();
	void		DESC_SETTINGMODE();
	void		Reset_PathData();


private:
	HRESULT Edit_DESC();

	wstring Get_EXEDescName(E_DESC_DATA descid);


private:
	// 경로 이름 모두 저장해둔다.
	list<MYFILEPATH*>*				mListFilePath = nullptr;
//	list<string>					mListDescNameSTR;

	list<string>					mListDescFileNameSTR[E_DESC_DATA::DESC_DATA_END];
	int								mSelectIndex[E_DESC_DATA::DESC_DATA_END] = { -1, };

	bool							mIsDesc = false;
	
private:
	E_TOOLMODE_DESC				meModelMode = TOOLMODE_DESC_END;

public:
	static CImgui_DESC* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
