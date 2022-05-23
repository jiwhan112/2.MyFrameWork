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
	void Combo_MapMode();

	void RENDER_CREATEEMPTY();
	void CREATE_LOAD_DESC();

	void RENDER_MAP_MODE();
	void RENDER_MAP_OBJ_MODE();
	//	void INIT_FBXPathList();

	void WINDOW_TERRAIN();

	void Tick_PickObject(_double time);

private:

	HRESULT Edit_TERRAIN();
	HRESULT Edit_OBJECTS();

	HRESULT SAVER_MODE();

	void Set_PickObject(CGameObject_Base* obj);


private:
	class CCamera_Client*			mCameraClient = nullptr;

	// ���� ������Ʈ
	class CGameObject_MyTerrain*	mCurrent_TerrainObject = nullptr;

	// ��ŷ ������Ʈ
	class CGameObject_Base*			mCurrent_PickObject = nullptr;

	// �ʿ� ��ġ�� ������Ʈ
	list<CGameObject_Base*>			mListWorldObjects;

	list<_uint>						mListNoTileIndex_Desc;
	list<MODEL_WORLD_DESC>			mListWorldObjects_Desc;

	// ������� ���� �̸�
	list<string>*					mProtoParentObjectList = nullptr;
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
