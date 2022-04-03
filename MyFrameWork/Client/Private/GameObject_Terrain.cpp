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
	, mComTexture(rhs.mComTexture)
{
	Safe_AddRef(mComShader);
	Safe_AddRef(mComRenderer);
	Safe_AddRef(mComVIBuffer);
	Safe_AddRef(mComTexture);
}

HRESULT CGameObject_Terrain::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_Terrain::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}

_int CGameObject_Terrain::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return _int();
}

_int CGameObject_Terrain::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return _int();
}

HRESULT CGameObject_Terrain::Render()
{
	if (nullptr == mComVIBuffer)
		return E_FAIL;

	FAILED_CHECK(Set_ConstantTable());
	FAILED_CHECK(mComVIBuffer->Render(mComShader, 0));
	return S_OK;
}

HRESULT CGameObject_Terrain::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXNORTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_TERRAIN), TEXT("Com_Texture"), (CComponent**)&mComTexture));
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
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, "g_DiffuseTexture"));

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

CGameObject_Terrain * CGameObject_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Terrain*	pInstance = DBG_NEW CGameObject_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject_Terrain* CGameObject_Terrain::Clone(void* pArg)
{
	CGameObject_Terrain*	pInstance = DBG_NEW CGameObject_Terrain(*this);

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
	Safe_Release(mComTexture);
}