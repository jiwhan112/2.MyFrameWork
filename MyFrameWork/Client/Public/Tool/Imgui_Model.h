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
//	void Set_UIObject(class CGameObject_2D* obj);
	HRESULT Render_UI();

private:
	void FBXMODE();

	void Button_PathTxtSave(wstring path, wstring txtpath, wstring txtname);
	void Button_FBXLoader();

//	HRESULT Edit_ProtoObjectList();
	HRESULT Edit_StaticObject();
	HRESULT Edit_FBX();


private:

private:
	// UI 오브젝트 1개에대한 정보 저장코드 만들어보기
	class CGameObject*		mCurrentObject = nullptr;
	list<string>*			mFBXPathData = nullptr;


	bool					mIsObjectList = false;
	bool					mIsDataSetting = false;

	// 오브젝트 생성
	class CGameObject_Creater* mCreaterManager = nullptr;

	// 오브젝트 저장기능
	class CObjectIO*		mObjectIoManager = nullptr;



public:
	static CImgui_Model* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
