#include "stdafx.h"
#include "GameObject_Terrain.h"

CGameObject_Terrain::CGameObject_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CGameObject_Terrain::CGameObject_Terrain(const CGameObject_Terrain& rhs)
	: CGameObject(rhs)
	, mComShader(rhs.mComShader)
	, mComRenderer(rhs.mComRenderer)
	, mComVIBuffer(rhs.mComVIBuffer)
{
	Safe_AddRef(mComShader);
	Safe_AddRef(mComRenderer);
	Safe_AddRef(mComVIBuffer);

}

HRESULT CGameObject_Terrain::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_Terrain::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
//	Create_FilterTexture();
	return S_OK;
}

_int CGameObject_Terrain::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));


	/*if (PickObject())
	{
		int a = 5;
	}*/

	return _int();
}

_int CGameObject_Terrain::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return _int();
}

bool CGameObject_Terrain::PickObject()
{
	_float3 pick;
	if (true == mComVIBuffer->Pick(mComTransform->GetWorldFloat4x4().Invert(), &pick))
	{
		int a = 5;
		return true;
	}

	return false;
}


HRESULT CGameObject_Terrain::Render()
{
	if (nullptr == mComVIBuffer)
		return E_FAIL;

	FAILED_CHECK(Set_ConstantTable());
	FAILED_CHECK(mComVIBuffer->Render(mComShader, 1));
	return S_OK;
}

HRESULT CGameObject_Terrain::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXNORTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));

	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_GRASS), TEXT("Com_Texture1"), (CComponent**)&mComTexture[TYPE_DIFFUSE]));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_FITER), TEXT("Com_Texture2"), (CComponent**)&mComTexture[TYPE_FILTER]));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_BRUSH), TEXT("Com_Texture3"), (CComponent**)&mComTexture[TYPE_BRUSH]));
	return S_OK;
}

HRESULT CGameObject_Terrain::Set_ConstantTable()
{
	// 셰이더 관련 내용 정의

	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(mComTransform->Bind_OnShader(mComShader, "g_WorldMatrix"));
	FAILED_CHECK(mComShader->Set_RawValue("g_ViewMatrix", &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_ProjMatrix", &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_PROJ), sizeof(_float4x4)));

	// 텍스처 넘기기
	FAILED_CHECK(mComTexture[TYPE_DIFFUSE]->SetUp_OnShader(mComShader, "g_DiffuseTexture"));
	FAILED_CHECK(mComTexture[TYPE_DIFFUSE]->SetUp_OnShader(mComShader, "g_SourDiffuseTexture",0));
	FAILED_CHECK(mComTexture[TYPE_DIFFUSE]->SetUp_OnShader(mComShader, "g_DestDiffuseTexture",1));
//	FAILED_CHECK(mComTexture[TYPE_FILTER]->SetUp_OnShader(mComShader, "g_FilterTexture"));
	FAILED_CHECK(mComTexture[TYPE_BRUSH]->SetUp_OnShader(mComShader, "g_BrushTexture"));
	mComShader->Set_Texture("g_FilterTexture", mRSV);

	// 카메라 빛 세팅
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (pLightDesc == nullptr)
		return E_FAIL;

	//	FAILED_CHECK(mComShader->Set_RawValue("g_vLightPos", &pLightDesc->vDiffuse, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)));

	FAILED_CHECK(mComShader->Set_RawValue("g_CameraPosition", &pGameInstance->GetCameraPosition_vec(), sizeof(_float4)));

	return S_OK;
}

HRESULT CGameObject_Terrain::Create_FilterTexture()
{
	// 동적 텍스처 제작
	// 버퍼를 만드는 과정과 비슷하다.

	// 1. 텍스처2D 생성
	ID3D11Texture2D*			pTexture;

	// 2. 텍스처 타입 결정
	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 129;
	TextureDesc.Height = 129;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 0~1을 갖는 BGRA텍스처
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DYNAMIC; // 수정을 위해 DYNAMIC으로 제작
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // 셰이더 리소스라고 명시
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU에서 수정할 수 있게 (Write) 
	
	// D3D11_SUBRESOURCE_DATA 리소스 생성
	D3D11_SUBRESOURCE_DATA			SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	// 픽셀의 크기 == 지형의 버텍스 크기로 설정
	_uint NumX = 129;
	_uint NumY = 129;

	_ulong*		pPixel = NEW _ulong[NumX * NumY];
	ZeroMemory(pPixel, sizeof(_ulong) * NumX * NumY);

	for (_uint y = 0; y < NumY; ++y)
	{
		for (_uint x = 0; x < NumX; ++x)
		{
			_uint iIndex = y * NumX + x;

			if (y < 65)
				pPixel[iIndex] = D3D11COLOR_ARGB(255, 255, 255, 255);
			else
				pPixel[iIndex] = D3D11COLOR_ARGB(255, 0, 0, 0);
		}
	}

	pPixel[0] = D3D11COLOR_ARGB(255, 255, 255, 255);

	// D3D11_SUBRESOURCE_DATA에 픽셀 데이터 대입
	SubResourceData.pSysMem = pPixel;
	// 텍스처는 Pitch 설정 필수
	SubResourceData.SysMemPitch = sizeof(_ulong) * NumX;

	// 텍스처 생성 -> 리소스뷰형태로 변환 (텍스처 사용) 
	FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture));
	FAILED_CHECK(m_pDevice->CreateShaderResourceView(pTexture, nullptr, &mRSV));

	// 텍스처 생성 ->리소스뷰형태로 저장 (텍스처 저장) 
	FAILED_CHECK(SaveWICTextureToFile(m_pDeviceContext, pTexture,
		GUID_ContainerFormatPng, TEXT("../Bin/Resources/Textures/Test.png"), &GUID_WICPixelFormat32bppBGRA));

	Safe_Delete_Array(pPixel);
	Safe_Release(pTexture);

	return S_OK;
}

CGameObject_Terrain * CGameObject_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Terrain*	pInstance = NEW CGameObject_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject_Terrain* CGameObject_Terrain::Clone(void* pArg)
{
	CGameObject_Terrain*	pInstance = NEW CGameObject_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Terrain::Free()
{
	__super::Free();
	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComVIBuffer);

	Safe_Release(mRSV);
	for (auto& pTexture : mComTexture)
		Safe_Release(pTexture);
}