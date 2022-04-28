#include "stdafx.h"
#include "..\Public\MainApp.h"


#include "Level_Logo.h"
#include "Level_Loader.h"

#include "Camera_Client.h"
#include "GameObject/GameObject_2D.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/Camera_Game.h"
#include "GameObject/GameObject_Mouse.h"
#include "GameObject/GameObject_3D_Static.h"
#include "GameObject/GameObject_3D_Dynamic.h"


#include "GameObject_Skybox.h"
#include "GameObject_Terrain.h"
#include "GameObject_FBX.h"
#include "GameObject_FBX_Ani.h"



CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	m_pGameManager =  GetSingle(CGameManager)->GetInstance();
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	FAILED_CHECK(Ready_Initialize());
	m_pGameManager->Initialize(m_pDevice, m_pDeviceContext);

	FAILED_CHECK(Ready_Prototype_Components());
	FAILED_CHECK(Ready_Prototype_GameObject());
	FAILED_CHECK(Open_Level(LEVEL_LOGO));	
	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	m_pGameInstance->Tick_Engine(TimeDelta);

	m_pGameManager->Update(TimeDelta);
	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	// 백버퍼 / 깊이버퍼 클리어
	m_pGameInstance->Clear_BackBuffer_View(_float4(_float3(0, 0, 1), 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Render();
	m_pGameInstance->Render_Level();

	m_pGameManager->Render();

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

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	// #Tag 컴포넌트 원형 초기화

	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext)));
	Safe_AddRef(m_pRenderer);

	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TRANSFORM),
		CTransform::Create(m_pDevice, m_pDeviceContext)));


	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_AABB),
		CCollider::Create(m_pDevice, m_pDeviceContext,CCollider::COL_AABB)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_OBB),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::COL_OBB)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::COL_SPHERE)));


	// 버퍼 컴포넌트
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT),
		CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_CUBE),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext)));

	// 높이맵으로 처리
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp"))));
	
	// 지형 컴포넌트
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext,129,129)));

	// 정적 오브젝트
	// _float4x4		DefaultTransform;	
	// DefaultTransform = _float4x4::CreateScale(1) * _float4x4::CreateRotationY(XMConvertToRadians(180));
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_MODEL),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODEL_NOANI, "../Bin/Resources/TestFBX/", "crea_Snot_a.fbx", DefaultTransform)));

	// 동적 오브젝트
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_MODEL_ANI),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODEL_ANI, "../Bin/Resources/TestFBX/", "crea_Snot_a.fbx", DefaultTransform)));


	// 텍스처 컴포넌트
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_DEFAULT),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Logo.png"), 1)));
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

	// 모델 / 텍스처 맵 / 셰이더 / 
	FAILED_CHECK(Ready_Prototype_Components_Model());
	FAILED_CHECK(Ready_Prototype_Components_AniModel());
	FAILED_CHECK(Ready_Prototype_Components_Texture());
	FAILED_CHECK(Ready_Prototype_Components_Shader());

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	// #Tag 수업용 오브젝트 원형 초기화
	//==================================================================================================================
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_CAMERA),
		CCamera_Client::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_SKY),
		CGameObject_Skybox::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_TERRAIN),
		CGameObject_Terrain::Create(m_pDevice, m_pDeviceContext)));
	 
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_CAMERA_GAME),
		CCamera_Game::Create(m_pDevice, m_pDeviceContext)));

	// 마우스
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_MOUSE),
		CGameObject_Mouse::Create(m_pDevice, m_pDeviceContext)));

	//FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_FBXTEST),
	//	CGameObject_FBX::Create(m_pDevice, m_pDeviceContext)));
	//
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_FBXTEST_ANI),
	//	 CGameObject_FBX_Ani::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(Ready_Prototype_GameObject_Emptyobject());

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components_Model()
{
	// 정적 모델 컴포넌트 생성

	const list<MYFILEPATH*>* listFBXpath_Static = m_pGameManager->Get_PathList(CGameManager::PATHTYPE_FBX_STATIC);


	_float4x4		DefaultTransform;
	DefaultTransform = _float4x4::CreateScale(1) * _float4x4::CreateRotationY(XMConvertToRadians(180));

	for (auto& path : *listFBXpath_Static)
	{
		wstring wpath = GetSingle(CGameInstance)->Get_PathData(path->FullPath);
		wstring wName = path->FileName;

		string pathstr;
		string namestr;
		CHelperClass::Convert_string_wstring(wpath, pathstr, true);
		CHelperClass::Convert_string_wstring(wName, namestr, true);		
		FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, path->FileName,
			CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODEL_NOANI, pathstr.c_str(), namestr.c_str(), DefaultTransform)));
		
	}

	return S_OK;
}
HRESULT CMainApp::Ready_Prototype_Components_AniModel()
{
	// 애니메이션 모델 컴포넌트 생성

	const list<MYFILEPATH*>* listFBXpath_Dynamic = m_pGameManager->Get_PathList(CGameManager::PATHTYPE_FBX_DYNAMIC);


	_float4x4		DefaultTransform;
	DefaultTransform = _float4x4::CreateScale(1) * _float4x4::CreateRotationY(XMConvertToRadians(180));

	for (auto& path : *listFBXpath_Dynamic)
	{
		wstring wpath = GetSingle(CGameInstance)->Get_PathData(path->FullPath);
		wstring wName = path->FileName;

		string pathstr;
		string namestr;
		CHelperClass::Convert_string_wstring(wpath, pathstr, true);
		CHelperClass::Convert_string_wstring(wName, namestr, true);
		FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, path->FileName,
			CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODEL_ANI, pathstr.c_str(), namestr.c_str(), DefaultTransform)));

	}

	return S_OK;
}




HRESULT CMainApp::Ready_Prototype_Components_Texture()
{
	// 2D텍스처 맵 컴포넌트
	const list<MYFILEPATH*>* listpngpath = m_pGameManager->Get_PathList(CGameManager::PATHTYPE_SPRITE);
	FAILED_CHECK(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP),
		CTexture_map::Create(m_pDevice, m_pDeviceContext, *listpngpath)));


	// 모델 텍스처 맵
//	const list<MYFILEPATH*>* listFBXpngpath = m_pGameManager->Get_PathList(CGameManager::PATHTYPE_FBXTEX);
//	FAILED_CHECK(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP_FBX),
//		CTexture_map::Create(m_pDevice, m_pDeviceContext, *listFBXpngpath)));

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components_Shader()
{
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
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"),
			VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements)));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXANIMODEL),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxAni.hlsl"),
			VTXANIMODEL_DECLARATION::Elements, VTXANIMODEL_DECLARATION::iNumElements)));

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_Emptyobject()
{
	//==================================================================================================================
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_MYTERRAIN),
	//	CGameObject_MyTerrain::Create(m_pDevice, m_pDeviceContext)));

	// #Tag 깡통 오브젝트들 초기화

	// 깡통: 컴포넌트 기능으로 여러가지 오브젝트를 만들 수 있게 설계된 오브젝트를 뜻함
	
	// 2D 깡통
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_2D),
		CGameObject_2D::Create(m_pDevice, m_pDeviceContext)));

	// 3D 깡통
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_3D_STATIC),
		CGameObject_3D_Static::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_3D_DYNAMIC),
		CGameObject_3D_Dynamic::Create(m_pDevice, m_pDeviceContext)));


	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = NEW CMainApp();

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

	m_pGameManager->DestroyInstance();
	Safe_Release(m_pGameManager);

	GetSingle(CGameInstance)->DestroyInstance();

	
	CGameInstance::Release_Engine();
}