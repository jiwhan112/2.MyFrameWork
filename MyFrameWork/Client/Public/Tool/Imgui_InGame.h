#include "Tool/Imgui_Base.h"
#include "GameObject/Dungeon_Manager.h"

BEGIN(Client)

// Terrain툴

class CImgui_InGame final :
	public CImgui_Base
{
public:


private:
	explicit CImgui_InGame(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_InGame(const CImgui_InGame& rhs);
	virtual ~CImgui_InGame() = default;

public:
	// CImgui_Base을(를) 통해 상속됨
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	HRESULT Render_UI();

private:

private:
	HRESULT Edit_InGame();
	HRESULT Edit_Unit();
	HRESULT Edit_Camera();


private:
	// 카메라
	class CCamera_Client*			mCameraClient = nullptr;

	// 지형 오브젝트
	class CGameObject_MyTerrain*	mTerrainDungeon= nullptr;
	class CGameObject_MyTerrain*	mTerrainWorld = nullptr;

	class CGameObject_Base*	mSelectObject = nullptr;

	CDungeon_Manager::E_GAMEMODE	mGameMode;

	const _float3 mSpawnPostitionDAUNGEON = _float3(20.f, 0.f, 14.5f);
	const _float3 mSpawnPostitionENEMY = _float3(8.f, 8.f, 48.f);
	const _float3 mSpawnPostitionBOSS = _float3(52.f, 8.f, 51.f);
	const int mSpawnIndexDAUNGEON = 481;
	const int mSpawnIndexWORLD_UNIT = 480;
	const int mSpawnIndexWORLD_ENEMY= 1624;

	const char* ToTextFloat3(_float3 pos);


public:
	static CImgui_InGame* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
