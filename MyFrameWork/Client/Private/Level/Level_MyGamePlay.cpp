#include "stdafx.h"
#include "Level/Level_MyGamePlay.h"
#include "Tool/Imgui_MyDemo.h"
#include "Tool/Imgui_InGame.h"
#include "Camera_Client.h"

#include "GameObject/Client_Object.h"

CLevel_MyGamePlay::CLevel_MyGamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
	mLevelIndex = LEVEL_MYGAMEPLAY;
}

HRESULT CLevel_MyGamePlay::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());

#ifdef _DEBUG
	FAILED_CHECK(Ready_IMGUI());
#endif

//	FAILED_CHECK(GetSingle(CGameManager)->Get_DaungonManager()->Init_GameLevel((E_LEVEL)mLevelIndex));

	//// 주면 지형 생성
	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));
	FAILED_CHECK(Ready_Layer_Mouse(TAGLAY(LAY_PLAYER)));


	FAILED_CHECK(Ready_Layer_BackGround(TAGLAY(LAY_BACKGROUND)));

	//// 기본 게임 레벨 생성
	FAILED_CHECK(Ready_Level_BaseGame());

	

	return S_OK;
}

_int CLevel_MyGamePlay::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return UPDATENONE;
}

_int CLevel_MyGamePlay::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;
}

HRESULT CLevel_MyGamePlay::Render()
{
	FAILED_CHECK(__super::Render());

#ifdef  _DEBUG
	SetWindowText(g_hWnd, TEXT("MyGame Level"));

#endif //  _DEBUG
	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Prototype_GameObject()
{
	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_IMGUI()
{
	// IMGUI 생성
	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_InGame::Create(m_pDevice, m_pDeviceContext));

//	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_MyDemo::Create(m_pDevice, m_pDeviceContext));
	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Light()
{
	// 빛 세팅
	CGameInstance*	pGameInstance = GetSingle(CGameInstance);

	LIGHTDESC		LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
//	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vDiffuse = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);

	LightDesc.vDirection = _float4(1, -1.f, 1, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	// 고정 카메라 방식으로 변경

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(-5, 10.f, -5.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.SpeedPersec = 10.f;
	CameraDesc.TransformDesc.RotPersec = XMConvertToRadians(90.0f);

	CCamera_Client* client = (CCamera_Client*)(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_CAMERA), &CameraDesc));
	client->Set_CameraMode(CCamera_Client::CAMERA_MODE_GAME_D);

	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Layer_Mouse(const _tchar * pLayerTag)
{
	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_MOUSE)));
	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D)));
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_SKY)));
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, TAGLAY(LAY_TERRAIN), TAGOBJ(GAMEOBJECT_MYTERRAIN)));
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_3D_DYNAMIC)));

	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Level_BaseGame()
{
	CDungeon_Manager*	 pDaungonManager =  GetSingle(CGameManager)->Get_DaungonManager();
	CGameObject_Creater* pCreateManager = GetSingle(CGameManager)->Get_CreaterManager();

	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, TAGLAY(LAY_BACKGROUND), TAGOBJ(GAMEOBJECT_SKY)));

	// 맵 생성
	CGameObject_Base* terrainobj = pCreateManager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);

	// 타일 추가
	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(mLevelIndex, TAGLAY(LAY_TERRAIN), terrainobj));
	FAILED_CHECK(GetSingle(CGameManager)->Get_DaungonManager()->Setup_Terrain((CGameObject_MyTerrain*)terrainobj, (E_LEVEL)mLevelIndex));

	// 플레이어 객체 생성
	CGameObject_Base* mineobj = pCreateManager->CreateEmptyObject(GAMEOBJECT_3D_DYNAMIC);
	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(mLevelIndex, TAGLAY(LAY_OBJECT), mineobj));

	

	// 그외
	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	return S_OK;
}

CLevel_MyGamePlay * CLevel_MyGamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_MyGamePlay*	pInstance = NEW CLevel_MyGamePlay(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_MyGamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_MyGamePlay::Free()
{
	__super::Free();
}