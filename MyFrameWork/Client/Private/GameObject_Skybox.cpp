#include "stdafx.h"
#include "GameObject_Skybox.h"

CGameObject_Skybox::CGameObject_Skybox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CGameObject_Skybox::CGameObject_Skybox(const CGameObject_Skybox& rhs)
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

HRESULT CGameObject_Skybox::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_Skybox::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}

_int CGameObject_Skybox::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return _int();
}

_int CGameObject_Skybox::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	mComTransform->SetState(CTransform::STATE_POSITION, pGameInstance->GetCameraPosition_vec());

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CGameObject_Skybox::Render()
{
	FAILED_CHECK(__super::Render());

	FAILED_CHECK(Set_ConstantTable());
	FAILED_CHECK(mComVIBuffer->Render(mComShader, 0));
	return S_OK;
}

HRESULT CGameObject_Skybox::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEXCUBE), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_CUBE), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_SKY), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	return S_OK;
}

HRESULT CGameObject_Skybox::Set_ConstantTable()
{
	// 셰이더 관련 내용 정의

	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(mComTransform->Bind_OnShader(mComShader, "g_WorldMatrix"));
	FAILED_CHECK(mComShader->Set_RawValue("g_ViewMatrix", &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_ProjMatrix", &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_PROJ), sizeof(_float4x4)));

	// 텍스처 넘기기
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, "g_DiffuseTexture", 2));
	return S_OK;
}

CGameObject_Skybox * CGameObject_Skybox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Skybox*	pInstance = DBG_NEW CGameObject_Skybox(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Skybox");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Skybox* CGameObject_Skybox::Clone(void* pArg)
{
	CGameObject_Skybox*	pInstance = DBG_NEW CGameObject_Skybox(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Skybox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Skybox::Free()
{
	__super::Free();

	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComVIBuffer);
	Safe_Release(mComTexture);
}