#include "Tool/Imgui_Base.h"
#include "GameObject/Dungeon_Manager.h"

BEGIN(Client)

// Terrain��

class CImgui_InGame final :
	public CImgui_Base
{
public:


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

private:
	HRESULT Edit_InGame(); 


private:
	// ī�޶�
	// class CCamera_Client*			mCameraClient = nullptr;

	// ���� ������Ʈ
	class CGameObject_MyTerrain*	mTerrainDungeon= nullptr;
	class CGameObject_MyTerrain*	mTerrainWorld = nullptr;

	CDungeon_Manager::E_GAMEMODE	mGameMode;

	const int mSpawnIndexDAUNGEON = 481;
	const int mSpawnIndexWORLD_UNIT = 480;
	const int mSpawnIndexWORLD_ENEMY= 1624;

public:
	static CImgui_InGame* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
