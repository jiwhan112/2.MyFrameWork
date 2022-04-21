#include "stdafx.h"
#include "Level/Level_MyGamePlay.h"
#include "Camera.h"

CLevel_MyGamePlay::CLevel_MyGamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
	mLevelIndex = LEVEL_MYGAMEPLAY;
}

HRESULT CLevel_MyGamePlay::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());

	FAILED_CHECK(Ready_Light());	
	FAILED_CHECK(Ready_Layer_Mouse(TAGLAY(LAY_PLAYER)));
	FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));
	FAILED_CHECK(Ready_Layer_BackGround(TAGLAY(LAY_BACKGROUND)));

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

HRESULT CLevel_MyGamePlay::Ready_Light()
{
	// �� ����
	CGameInstance*	pGameInstance = GetSingle(CGameInstance);

	LIGHTDESC		LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	LightDesc.vDirection = _float4(0, -1.f, 0, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MyGamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	// ���� ī�޶� ������� ����

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

	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_CAMERA_GAME), &CameraDesc));
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
	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_SKY)));
	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_TERRAIN)));
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_FBXTEST)));
	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_FBXTEST_ANI)));

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