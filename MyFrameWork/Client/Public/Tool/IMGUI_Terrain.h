#include "Tool/Imgui_Base.h"

BEGIN(Client)


// Terrain��

class CImgui_Terrain final :
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
	explicit CImgui_Terrain(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_Terrain(const CImgui_Terrain& rhs);
	virtual ~CImgui_Terrain() = default;

public:
	// CImgui_Base��(��) ���� ��ӵ�
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	HRESULT Render_UI();

private:

	void RENDER_CREATEEMPTY();
	void RENDER_CREATE_PROTO();

	void RENDER_MAP_MODE();
	void RENDER_MAP_OBJ_MODE();
//	void INIT_FBXPathList();

	void WINDOW_TERRAIN();
private:

	HRESULT Edit_TERRAIN();


private:
	class CCamera_Client*			mCameraClient = nullptr;

	// ���� ������Ʈ
	class CGameObject_MyTerrain*	mCurrent_TerrainObject = nullptr;

	// ������ ������Ʈ
	// class Static*	mCurrent_TerrainObject = nullptr;

//	// FBX���� �̸�
//	list<string>*					mFBX_Static_pathList = nullptr;
//	list<string>*					mFBX_Dynamic_pathList = nullptr;
//
//	// ������� ���� �̸�
//	list<string>*					mProtoStaticModelList = nullptr;
//	list<string>*					mProtoDynamicModelList = nullptr;


private:
	bool							mIsTerrainSetting = false;
	E_TAYLAY						meCreateTERRAIN_Layer = LAY_TERRAIN;
	E_TAYLAY						meCreateOBJ_Layer = LAY_OBJECT;

	E_TOOLMODE_TERRAIN				meToolMode = E_TOOLMODE_TERRAIN_END;

public:
	static CImgui_Terrain* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
