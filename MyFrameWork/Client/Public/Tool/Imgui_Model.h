#include "Tool/Imgui_Base.h"

BEGIN(Engine)
class CModel;
class CAnimationClip;
END
BEGIN(Client)

// Model툴
// 정적 모델의 모델 FBX 이름으로 Dat 파일을 만든다.
// 오브젝트 1개에대한 처리만 가능하게
// 충돌체 설정 / 애니메이션 설정 넣기

class CImgui_Model final :
	public CImgui_Base
{
public:
	enum E_TOOLMODE_MODEL
	{
		TOOLMODE_MODEL_STATIC,
		TOOLMODE_MODEL_DYNAMIC,
		TOOLMODE_MODEL_STATIC_PARENT,
		TOOLMODE_MODEL_TILE,
		TOOLMODE_MODEL_END,
	};
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

	void RENDER_CREATEEMPTY();
	void RENDER_CREATE_PROTO();

	void RENDER_STATIC_MODE();
	void RENDER_DYNAMIC_MODE();
	void RENDER_STATIC_PARENT_MODE();
	
	HRESULT SAVER_MODE();


	void INIT_FBXPathList();
private:
	HRESULT Edit_FBX(); // Fbx 파일 변경
	HRESULT Edit_ANI(); // 애니메이션 툴
	HRESULT Edit_COL(); // 충돌체 툴

//	HRESULT Edit_Texture();

private:
	class CCamera_Client*			mCameraClient = nullptr;

	// 모델 오브젝트
	class CGameObject_3D_Static*	mCurrent_ModelStaticObject = nullptr;

	// 애니메이션 오브젝트
	class CGameObject_3D_Dynamic*	mCurrent_ModelDynamicObject = nullptr;

	// 부모형 오브젝트
	class CGameObject_3D_Static2*	mCurrent_ModelStaticObject_Parent = nullptr;

	// 타일형 오브젝트
	class CGameObject_3D_Tile*		mCurrent_Model_Tiles = nullptr;


	// FBX파일 이름
	list<string>*					mFBX_Static_pathList = nullptr;
	list<string>*					mFBX_Dynamic_pathList = nullptr;

	// 만들어진 원본 이름
	list<string>*					mProtoStaticModelList = nullptr;
	list<string>*					mProtoDynamicModelList = nullptr;
	list<string>*					mProtoParentModelList = nullptr;

	list<class CGameObject_3D_Socket*>	mSocketList;

	CGameObject_3D_Socket* mSelectSocketObject = nullptr;

	
private:
	bool							mIsModelSetting = false;
	E_TAYLAY						meCreateLayer = LAY_OBJECT;
	E_TOOLMODE_MODEL				meModelMode = TOOLMODE_MODEL_END;

public:
	static CImgui_Model* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
