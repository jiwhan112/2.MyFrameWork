#include "stdafx.h"
#include "Level_Tool.h"
#include "Camera_Client.h"
#include "GameObject/GameObject_2D.h"
#include "Tool/Imgui_MyDemo.h"
#include "Tool/Imgui_UI.h"


CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
	mLevelIndex = LEVEL_TOOL;

	mIMGUI_DEMO = nullptr;
	mIMGUI_UI = nullptr;
}

HRESULT CLevel_Tool::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());
	
	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(ReadyTools());
	FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));
	FAILED_CHECK(Ready_Layer_BackGround(TAGLAY(LAY_BACKGROUND)));

	return S_OK;
}

_int CLevel_Tool::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	GetSingle(CImguiMgr)->Update(TimeDelta);


	return UPDATENONE;
}

_int CLevel_Tool::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;
}

HRESULT CLevel_Tool::Render()
{
	FAILED_CHECK(__super::Render());

	GetSingle(CImguiMgr)->Render();
	


#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("Toll Level"));
#endif

	return S_OK;
}

HRESULT CLevel_Tool::ReadyTools()
{
	// IMGUI 생성
//	GetSingle(CImguiMgr)->Add_IMGUI(mIMGUI_DEMO = CImgui_MyDemo::Create(m_pDevice, m_pDeviceContext));
	GetSingle(CImguiMgr)->Add_IMGUI(mIMGUI_UI = CImgui_UI::Create(m_pDevice, m_pDeviceContext));

//	Safe_AddRef(mIMGUI_DEMO);
	Safe_AddRef(mIMGUI_UI);

	// 오브젝트 생성

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Light()
{
	// 빛 세팅
	// 싱글턴이라 삭제안됨 조심
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

HRESULT CLevel_Tool::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0, 5.0f, -5.0f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.SpeedPersec = 10.f;
	CameraDesc.TransformDesc.RotPersec = XMConvertToRadians(90.0f);


	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_CAMERA), &CameraDesc));
	return S_OK;

}

HRESULT CLevel_Tool::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	UIDESC desc = { g_iWinCX*0.5f,g_iWinCY *0.5f,
		 g_iWinCX,g_iWinCY,
		_float2(0.5f, 0.5f),
	};

	// CGameObject* obj = GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_2D));
	// CGameObject_2D* uiobj = static_cast<CGameObject_2D*>(obj);
	// uiobj->Set_LoadUIDesc(desc);
	// mIMGUI_UI->Set_UIObject(uiobj);

//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_SKY)));
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_MYTERRAIN)));
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mLevelIndex, pLayerTag, TAGOBJ(GAMEOBJECT_TEST)));


	return S_OK;
}



CLevel_Tool * CLevel_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Tool*	pInstance = NEW CLevel_Tool(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool::Free()
{
	__super::Free();
	Safe_Release(mIMGUI_DEMO);
	Safe_Release(mIMGUI_UI);
}