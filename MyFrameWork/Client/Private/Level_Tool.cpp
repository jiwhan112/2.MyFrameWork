#include "stdafx.h"
#include "Level_Tool.h"
#include "Camera_Client.h"
#include "GameObject/GameObject_2D.h"

#include "Tool/Imgui_MyDemo.h"
#include "Tool/Imgui_CommonUI.h"
#include "Tool/Imgui_UI.h"
#include "Tool/Imgui_Model.h"
#include "Tool/IMGUI_Terrain.h"
#include "Tool/Imgui_DESC.h"

CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
	mLevelIndex = LEVEL_TOOL;

	//mIMGUI_DEMO = nullptr;
	//mIMGUI_UI = nullptr;
}

HRESULT CLevel_Tool::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());

#ifdef _DEBUG
	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_Tools());
	FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));
	FAILED_CHECK(Ready_Layer_BackGround(TAGLAY(LAY_BACKGROUND)));
#endif

	// 파일 원령객체 초기화
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	Create_Manager->LoaderDatFile_For_PrototypeObject();


	return S_OK;
}

_int CLevel_Tool::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
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

#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("Tool Level"));
#endif

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Tools()
{
#ifdef _DEBUG
	// IMGUI 생성
	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_UI::Create(m_pDevice, m_pDeviceContext));
	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_Model::Create(m_pDevice, m_pDeviceContext));
	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_Terrain::Create(m_pDevice, m_pDeviceContext));
	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_DESC::Create(m_pDevice, m_pDeviceContext));
#endif

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

	//	Safe_Release(mIMGUI_DEMO);
	//	Safe_Release(mIMGUI_UI);
}