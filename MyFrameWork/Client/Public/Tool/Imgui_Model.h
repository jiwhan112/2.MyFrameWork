#include "Tool/Imgui_Base.h"

BEGIN(Client)


// Model툴
// 정적 모델의 모델 FBX 이름으로 Dat 파일을 만든다.
// 충돌 모델설정
class CImgui_Model final:
	public CImgui_Base
{

private:
	explicit CImgui_Model(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_Model(const CImgui_Model& rhs);
	virtual ~CImgui_Model() = default;

public:
	// CImgui_Base을(를) 통해 상속됨
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	HRESULT Render_UI();

private:
	void FBX_SETTINGMODE();
	void FBX_CREATEMODE();


private:
	HRESULT Edit_FBX();
	HRESULT Edit_Texture();

private:
	class CCamera_Client*			mCameraClient = nullptr;
	class CGameObject_3D_Static*	mCurrentModelObject = nullptr;

	list<string>*					mFBXpathList = nullptr;
	list<string>*					mProtoModelList = nullptr;


private:
	bool							mIsModelSetting = false;


public:
	static CImgui_Model* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
