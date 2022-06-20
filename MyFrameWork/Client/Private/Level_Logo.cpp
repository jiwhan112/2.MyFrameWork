#include "stdafx.h"
#include "Level_Logo.h"
#include "Camera_Client.h"
#include "GameObject/GameObject_2D.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
	mLevelIndex = LEVEL_LOGO;
}

HRESULT CLevel_Logo::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());

	// 사운드 체크
//	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"JY_Stage2_BGM.mp3")));

//	FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));

	// 로고 테스트
	FAILED_CHECK(Ready_Layer_UI_LOGO(TAGLAY(LAY_UI)));

	// 플레이어 == 마우스
	NULL_CHECK_BREAK(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, TAGLAY(LAY_MOUSE), TAGOBJ(GAMEOBJECT_MOUSE)));



	// 게임 화면 테스트
//	FAILED_CHECK(Ready_Layer_UI_GAMEPLAY(TAGLAY(LAY_UI)));
	
	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return 0;
}

_int CLevel_Logo::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	return 0;
}

HRESULT CLevel_Logo::Render()
{
	FAILED_CHECK(__super::Render());
	return S_OK;
}


HRESULT CLevel_Logo::Ready_Prototype_GameObject()
{
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 5.0f, -5.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.SpeedPersec = 10.f;
	CameraDesc.TransformDesc.RotPersec = XMConvertToRadians(90.0f);

	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_CAMERA), &CameraDesc));
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI_LOGO(const _tchar * pLayerTag)
{
	_float2 viewsize = Get_ViewSize();
	_rect rect;

	_float size = 0.7f;

	CGameObject_2D* BackGroundImage = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	BackGroundImage->Setup_UIType(CGameObject_2D::UITYPE_LOGO);


	CGameObject_2D* DungeonWorldImg = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	DungeonWorldImg->Setup_UIType(CGameObject_2D::UITYPE_IMAGE);
	DungeonWorldImg->Set_LoadTexDiffuse("GUI_Menu_Main_Logo.png");
	rect = _rect(viewsize.x*0.2f, viewsize.y*0.15f, 716 * size, 278 * size);
	UI_DESC DungeonWorldImgDesc;
	DungeonWorldImgDesc.mUIRECT = rect;
	DungeonWorldImgDesc.mDepth = 10;
	DungeonWorldImg->Setup_UIPosition(DungeonWorldImgDesc);


	CGameObject_2D* DarkBarImg = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	DarkBarImg->Setup_UIType(CGameObject_2D::UITYPE_IMAGE);
	DarkBarImg->Set_LoadTexDiffuse("GUI_Menu_Main_Banner.png");
	UI_DESC DarkBarImgDesc;
	rect = _rect(viewsize.x*0.2f, viewsize.y*0.1f, 437 * size, 1019 * size*1.5f);
	DarkBarImgDesc.mUIRECT = rect;
	DarkBarImgDesc.mDepth = 1;
	DarkBarImg->Setup_UIPosition(DarkBarImgDesc);

	size = 0.9f;

	CGameObject_2D* GameStart = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	GameStart->Setup_UIType(CGameObject_2D::UITYPE_BUTTON1);
	GameStart->Set_LoadTexButton("GUI_GameStart.png", "GUI_GameStart_Light.png");
	UI_DESC GameStartDesc;
	rect = _rect(viewsize.x*0.2f, viewsize.y*0.35f, 368 * size, 84 * size);
	GameStartDesc.mUIRECT = rect;
	GameStartDesc.mDepth = 10;
	GameStart->Setup_UIPosition(GameStartDesc);
	GameStart->AddCollisionFunction(&Change_GameLevel);




	return S_OK;
}


HRESULT CLevel_Logo::Ready_Layer_UI_GAMEPLAY(const _tchar * pLayerTag)
{
	_float2 viewsize = Get_ViewSize();
	_rect rect;

	_float scale = 0.7f;
	_float Size_ori = 222;
	_float Size = Size_ori * scale;
	_float Size_half = Size * 0.5f;

	// 배경

	// 고블린 / 마인 / 오크 생성 UI
	// 적 생성 UI

	CGameObject_2D* Dungeon_OrcImg= (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	Dungeon_OrcImg->Setup_UIType(CGameObject_2D::UITYPE_BUTTON2_UNIT);
	Dungeon_OrcImg->Set_LoadTexButton2("Orc_icon.png", "Orc_icon1.png", "Orc_icon2.png");
	rect = _rect(viewsize.x*0.9f, viewsize.y*0.1f, Size, Size );
	UI_DESC Dungeon_OrcImgDesc;
	Dungeon_OrcImgDesc.mUIRECT = rect;
	Dungeon_OrcImgDesc.mDepth = 1;
	Dungeon_OrcImg->Setup_UIPosition(Dungeon_OrcImgDesc);

	
	CGameObject_2D* Dungeon_GoblinImg = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	Dungeon_GoblinImg->Setup_UIType(CGameObject_2D::UITYPE_BUTTON2_UNIT);
	Dungeon_GoblinImg->Set_LoadTexButton2("Goblin_icon.png", "Goblin_icon1.png", "Goblin_icon2.png");
	rect = _rect(viewsize.x*0.9f + Size_half, viewsize.y*0.1f + Size_half, Size, Size );
	UI_DESC Dungeon_GoblinImgDesc;
	Dungeon_GoblinImgDesc.mUIRECT = rect;
	Dungeon_GoblinImgDesc.mDepth = 1;
	Dungeon_GoblinImg->Setup_UIPosition(Dungeon_GoblinImgDesc);

	CGameObject_2D* Dungeon_MineImg = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	Dungeon_MineImg->Setup_UIType(CGameObject_2D::UITYPE_BUTTON2_UNIT);
	Dungeon_MineImg->Set_LoadTexButton2("Mine_icon.png", "Mine_icon1.png", "Mine_icon2.png");
	rect = _rect(viewsize.x*0.9f, viewsize.y*0.1f+ Size_half * 2, Size , Size );
	UI_DESC Dungeon_MineImgDesc;
	Dungeon_MineImgDesc.mUIRECT = rect;
	Dungeon_MineImgDesc.mDepth = 1;
	Dungeon_MineImg->Setup_UIPosition(Dungeon_MineImgDesc);


	scale = 0.5f;
	Size_ori = 222;
	Size = Size_ori * scale;
	Size_half = Size * 0.5f;

	CGameObject_2D* Dungeon_EnemyImg= (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	Dungeon_EnemyImg->Setup_UIType(CGameObject_2D::UITYPE_BUTTON1);
	Dungeon_EnemyImg->Set_LoadTexButton("Warrior_icon.png", "Warrior_icon1.png");
	rect = _rect(viewsize.x*0.8f+ Size_half, viewsize.y*0.85f, Size, Size);
	UI_DESC Dungeon_EnemyImgDesc;
	Dungeon_EnemyImgDesc.mUIRECT = rect;
	Dungeon_EnemyImgDesc.mDepth = 1;
	Dungeon_EnemyImg->Setup_UIPosition(Dungeon_EnemyImgDesc);

	CGameObject_2D* Dungeon_WorldImg = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	Dungeon_WorldImg->Setup_UIType(CGameObject_2D::UITYPE_BUTTON1);
	Dungeon_WorldImg->Set_LoadTexButton("Dungeon_icon1.png", "Dungeon_icon2.png");
	rect = _rect(viewsize.x*0.8f + Size*1.6f, viewsize.y*0.85f, Size, Size);
	UI_DESC Dungeon_WorldImgDesc;
	Dungeon_WorldImgDesc.mUIRECT = rect;
	Dungeon_WorldImgDesc.mDepth = 1;
	Dungeon_WorldImg->Setup_UIPosition(Dungeon_WorldImgDesc);

	return S_OK;
}

void CLevel_Logo::Change_GameLevel()
{
	GetSingle(CGameManager)->Change_GameLevel();
}


CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Logo*	pInstance = NEW CLevel_Logo(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}