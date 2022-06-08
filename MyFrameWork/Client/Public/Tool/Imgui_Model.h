#include "Tool/Imgui_Base.h"

BEGIN(Engine)
class CModel;
class CAnimationClip;
END
BEGIN(Client)

// Model��
// ���� ���� �� FBX �̸����� Dat ������ �����.
// ������Ʈ 1�������� ó���� �����ϰ�
// �浹ü ���� / �ִϸ��̼� ���� �ֱ�

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
	// CImgui_Base��(��) ���� ��ӵ�
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
	HRESULT Edit_FBX(); // Fbx ���� ����
	HRESULT Edit_ANI(); // �ִϸ��̼� ��
	HRESULT Edit_COL(); // �浹ü ��

//	HRESULT Edit_Texture();

private:
	class CCamera_Client*			mCameraClient = nullptr;

	// �� ������Ʈ
	class CGameObject_3D_Static*	mCurrent_ModelStaticObject = nullptr;

	// �ִϸ��̼� ������Ʈ
	class CGameObject_3D_Dynamic*	mCurrent_ModelDynamicObject = nullptr;

	// �θ��� ������Ʈ
	class CGameObject_3D_Static2*	mCurrent_ModelStaticObject_Parent = nullptr;

	// Ÿ���� ������Ʈ
	class CGameObject_3D_Tile*		mCurrent_Model_Tiles = nullptr;


	// FBX���� �̸�
	list<string>*					mFBX_Static_pathList = nullptr;
	list<string>*					mFBX_Dynamic_pathList = nullptr;

	// ������� ���� �̸�
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
