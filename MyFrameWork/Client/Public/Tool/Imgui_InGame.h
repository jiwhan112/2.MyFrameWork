#include "Tool/Imgui_Base.h"

BEGIN(Client)

// Terrain��

class CImgui_InGame final :
	public CImgui_Base
{
public:
	enum E_TOOLMODE_TERRAIN
	{
		E_TOOLMODE_TERRAIN_MAP, // �� ���ؽ�
		E_TOOLMODE_TERRAIN_OBJ, // �� ������Ʈ ���
		E_TOOLMODE_TERRAIN_END
	};

private:
	explicit CImgui_InGame(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_InGame(const CImgui_InGame& rhs);
	virtual ~CImgui_InGame() = default;

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

	void INIT_FBXPathList();
private:
	HRESULT Edit_FBX(); // Fbx ���� ����
	HRESULT Edit_ANI(); // �ִϸ��̼� ��
	HRESULT Edit_COL(); // �浹ü ��

//	HRESULT Edit_Texture();

private:
	class CCamera_Client*			mCameraClient = nullptr;

	// ���� ������Ʈ
	class CGameObject_3D_Static*	mCurrent_TerrainObject = nullptr;

	// ������ ������Ʈ
	// class CGameObject_3D_Dynamic*	mCurrent_TerrainObject = nullptr;

//	// FBX���� �̸�
//	list<string>*					mFBX_Static_pathList = nullptr;
//	list<string>*					mFBX_Dynamic_pathList = nullptr;
//
//	// ������� ���� �̸�
//	list<string>*					mProtoStaticModelList = nullptr;
//	list<string>*					mProtoDynamicModelList = nullptr;

private:
	E_TAYLAY						meCreateTERRAIN_Layer = LAY_TERRAIN;
	E_TAYLAY						meCreateOBJ_Layer = LAY_OBJECT;

public:
	static CImgui_InGame* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
