#include "Tool/Imgui_Base.h"

BEGIN(Client)


// Model��
// ���� ���� �� FBX �̸����� Dat ������ �����.
// �浹 �𵨼���
class CImgui_Model final:
	public CImgui_Base
{

private:
	explicit CImgui_Model(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_Model(const CImgui_Model& rhs);
	virtual ~CImgui_Model() = default;
	
public:
	// CImgui_Base��(��) ���� ��ӵ�
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
	// UI ������Ʈ 1�������� ���� �����ڵ� ������
	class CGameObject*		mCurrentObject = nullptr;
	list<string>*			mFBXPathData = nullptr;


	bool					mIsObjectList = false;
	bool					mIsDataSetting = false;

	// ������Ʈ ����
	class CGameObject_Creater* mCreaterManager = nullptr;

	// ������Ʈ ������
	class CObjectIO*		mObjectIoManager = nullptr;



public:
	static CImgui_Model* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
