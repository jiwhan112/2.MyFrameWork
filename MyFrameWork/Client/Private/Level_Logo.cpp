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
	FAILED_CHECK(Ready_Layer_UI(TAGLAY(LAY_UI)));
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

HRESULT CLevel_Logo::Ready_Layer_UI(const _tchar * pLayerTag)
{
	_float2 viewsize = Get_ViewSize();

	CGameObject_2D* backImage = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	backImage->Setup_UIType(CGameObject_2D::UITYPE_LOGO);


	CGameObject_2D* BackImage2 = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	BackImage2->Setup_UIType(CGameObject_2D::UITYPE_IMAGE);
	BackImage2->Set_LoadTexDiffuse("GUI_Menu_Main_Logo.png");
	UI_DESC imageDesc;
	_rect rect = _rect(viewsize.x*0.5f, viewsize.y*0.8f, 500, 200);
	imageDesc.mUIRECT = rect;
	imageDesc.mDepth = 1;
	BackImage2->Setup_UIPosition(imageDesc);


	CGameObject_2D* GameStart = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	GameStart->Setup_UIType(CGameObject_2D::UITYPE_BUTTON1);
	GameStart->Set_LoadTexButton("GUI_Menu_Generic_Button_Center_01.png", "GUI_Menu_Generic_Button_CenterLight.png");
	UI_DESC GameStartDesc;
	rect = _rect(viewsize.x*0.5f, viewsize.y*0.3f, 600, 100);
	GameStartDesc.mUIRECT = rect;
	GameStartDesc.mDepth = 1;
	GameStart->Setup_UIPosition(GameStartDesc);


	//CGameObject_2D* EXIT = (CGameObject_2D*)GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	//EXIT->Setup_UIType(CGameObject_2D::UITYPE_BUTTON1);
	//EXIT->Set_LoadTexButton("GUI_Menu_Generic_Button_Center_01.png", "GUI_Menu_Generic_Button_CenterLight.png");



	return S_OK;
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