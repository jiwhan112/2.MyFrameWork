#include "stdafx.h"
#include "Level_Logo.h"
#include "Level_Loader.h"
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
	FAILED_CHECK(Ready_Prototype_GameObject());

	FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));
	FAILED_CHECK(Ready_Layer_BackGround(TAGLAY(LAY_BACKGROUND)));

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

	CGameInstance*	pGameInstance = GetSingle(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_RETURN) & DIS_Down)
	{
		FAILED_CHECK(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY)));
	}

	if (pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		FAILED_CHECK(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, LEVEL_TOOL)));
	}

	return 0;
}

HRESULT CLevel_Logo::Render()
{
	FAILED_CHECK(__super::Render());

#ifdef  _DEBUG
	SetWindowText(g_hWnd, TEXT("LOGO Level"));

#endif //  _DEBUG
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

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	UIDESC desc = { g_iWinCX*0.5f,g_iWinCY *0.5f,
		 g_iWinCX,g_iWinCY,_float2(0.5f, 0.5f),
	};

	CGameObject* obj = GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	static_cast<CGameObject_2D*>(obj)->Set_LoadUIDesc(desc);
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