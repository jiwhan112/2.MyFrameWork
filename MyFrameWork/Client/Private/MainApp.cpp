#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "Tool/ImguiMgr.h"

#include "Level_Logo.h"
#include "Level_Loader.h"

#include "Camera_Client.h"
#include "GameObject/GameObject_2D.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/Camera_Game.h"
#include "GameObject/GameObject_Mouse.h"

#include "GameObject_Skybox.h"
#include "GameObject_Terrain.h"
#include "GameObject_FBX.h"
#include "GameObject_FBX_Ani.h"



CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	GetSingle(CImguiMgr)->GetInstance();
	GetSingle(CGameObject_Creater)->GetInstance();

	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	FAILED_CHECK(Ready_Initialize());
	GetSingle(CGameObject_Creater)->Set_Device(m_pDevice,m_pDeviceContext);
	FAILED_CHECK(Ready_Prototype_Components());
	FAILED_CHECK(Ready_Prototype_GameObject());
	FAILED_CHECK(Open_Level(LEVEL_LOGO));	
	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	m_pGameInstance->Tick_Engine(TimeDelta);
	GetSingle(CImguiMgr)->Update(TimeDelta);

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

	GetSingle(CImguiMgr)->Render();
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
	_float4x4		DefaultTransform;	
	DefaultTransform = _float4x4::CreateScale(1) * _float4x4::CreateRotationY(XMConvertToRadians(180));
	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TAGCOM(COMPONENT_MODEL),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODEL_NOANI, "../Bin/Resources/TestFBX/", "crea_Snot_a.fbx", DefaultTransform)));

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

	// 텍스처 맵 컴포넌트
	
	list<MYFILEPATH*> listpngpath = m_pGameInstance->Load_ExtensionList(STR_FILEPATH_RESOURCE_SPRITETXT_L,"png");

	FAILED_CHECK(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP),
		CTexture_map::Create(m_pDevice, m_pDeviceContext, listpngpath)));

	// Path 데이터 삭제
	for (auto s : listpngpath)
	{
		Safe_Delete(s);
	}
	listpngpath.clear();

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


	// 모델 컴포넌트 생성

	// FBX 파일 이름으로 모델 생성
	list<MYFILEPATH*> listFBXpath = m_pGameInstance->Load_ExtensionList(STR_FILEPATH_RESOURCE_3DPATHTXT_L, "fbx");

	MYFILEPATH* TestPathData = (*listFBXpath.begin());
	wstring wpath = GetSingle(CGameInstance)->Get_PathData(TestPathData->FullPath);
	wstring wName = TestPathData->FileName;

	string pathstr;
	string namestr;
	CHelperClass::Convert_string_wstring(wpath, pathstr, true);
	CHelperClass::Convert_string_wstring(wName, namestr, true);

	_float4x4		DefaultTransform2;
	DefaultTransform2 = _float4x4::CreateScale(1) * _float4x4::CreateRotationY(XMConvertToRadians(180));

	

	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, TestPathData->FileName,
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODEL_NOANI, pathstr.c_str(), namestr.c_str(), DefaultTransform)));

	
	//for (auto& path : listFBXpath)
	//{
	//	FAILED_CHECK(m_pGameInstance->Add_Prototype(E_LEVEL::LEVEL_STATIC, path->FileName,
	//		CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODEL_NOANI, path->FullPath, path->FileName, DefaultTransform)));
	//}


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
	 
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_FBXTEST),
		CGameObject_FBX::Create(m_pDevice, m_pDeviceContext)));
	
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_FBXTEST_ANI),
	//	 CGameObject_FBX_Ani::Create(m_pDevice, m_pDeviceContext)));

	//==================================================================================================================

	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_CAMERA_GAME),
		CCamera_Game::Create(m_pDevice, m_pDeviceContext)));

	//FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_MYTERRAIN),
	//	CGameObject_MyTerrain::Create(m_pDevice, m_pDeviceContext)));

	// #Tag 깡통 오브젝트들 초기화
	// 깡통: 컴포넌트 기능으로 여러가지 오브젝트를 만들 수 있게 설계된 오브젝트를 뜻함
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_2D),
		CGameObject_2D::Create(m_pDevice, m_pDeviceContext)));

	// Creater 클래스에서 데이터를 읽는다고 가정하고 2D 깡통 클래스를 가지는 
	// 부모 / 자식이 완성된 프로토 오브젝트를 만든다.
	//FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_TEST),
	//	CGameObject_2D::Create(m_pDevice, m_pDeviceContext)));


	// 마우스
	FAILED_CHECK(m_pGameInstance->Add_Prototype(TAGOBJ(GAMEOBJECT_MOUSE),
		CGameObject_Mouse::Create(m_pDevice, m_pDeviceContext)));

	

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

	GetSingle(CImguiMgr)->DestroyInstance();
	GetSingle(CGameObject_Creater)->DestroyInstance();

	
	CGameInstance::Release_Engine();
}