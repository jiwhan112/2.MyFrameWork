#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"

#include "ImguiMgr.h"

#include "Level_Logo.h"
#include "Level_Loader.h"

#include "Camera_Client.h"
#include "GameObject_BackGround.h"
#include "GameObject_Skybox.h"
#include "GameObject_Terrain.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	FAILED_CHECK(Ready_Initialize());
	FAILED_CHECK(Ready_Prototype_Components());
	FAILED_CHECK(Ready_Prototype_GameObject());
	FAILED_CHECK(Open_Level(LEVEL_LOGO));

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	m_pGameInstance->Tick_Engine(TimeDelta);
	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	// 백버퍼 / 깊이버퍼 클리어

	m_pGameInstance->Clear_BackBuffer_View(_float4(_float3(0, 0, 1), 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Render_Level();
	m_pRenderer->Render();

	// 스왑체인
	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(E_LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	HRESULT		hr = 0;

	switch (eLevelIndex)
	{
	case LEVEL_LOGO:
		hr = m_pGameInstance->OpenLevel(eLevelIndex, CLevel_Logo::Create(m_pDevice, m_pDeviceContext));
		break;
	default:
		hr = m_pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loader::Create(m_pDevice, m_pDeviceContext, eLevelIndex));
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Initialize()
{
	CGraphic_Device::GRAPHICDESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	// IMGUI INIT
	GetSingle(CImguiMgr)->InitImGUI(GraphicDesc.hWnd, m_pDevice, m_pDeviceContext);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	// #Tag 컴포넌트 원형 초기화

	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TRANSFORM),
		CTransform::Create(m_pDevice, m_pDeviceContext)));

	// 버퍼 컴포넌트
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT),
		CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_CUBE),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp"))));

	// 텍스처 컴포넌트
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_DEFAULT),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Default%d.dds"), 2)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_SKY),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/burger%d.dds"), 4)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_TERRAIN),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds"), 1)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_GRASS),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_BRUSH),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_FITER),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1)));

	// 셰이더 컴포넌트
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEX),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"),
			VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEXCUBE),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxTexCube.hlsl"),
			VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXNORTEX),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"),
			VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements)));

	Safe_AddRef(m_pRenderer);
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	// #Tag 오브젝트 원형 초기화

	/* For.Prototype_GameObject_BackGround */
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_BACKGROUND),
		CGameObject_BackGround::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_CAMERA),
		CCamera_Client::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_SKY),
		CGameObject_Skybox::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_TERRAIN),
		CGameObject_Terrain::Create(m_pDevice, m_pDeviceContext)));

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = DBG_NEW CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

	GetSingle(CImguiMgr)->DestroyInstance();

	CGameInstance::Release_Engine();
}