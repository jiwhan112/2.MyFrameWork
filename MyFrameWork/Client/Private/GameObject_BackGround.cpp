#include "stdafx.h"
#include "GameObject_BackGround.h"

CGameObject_BackGround::CGameObject_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CGameObject_BackGround::CGameObject_BackGround(const CGameObject_BackGround& rhs)
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

HRESULT CGameObject_BackGround::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_BackGround::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	mSize = { (float)g_iWinCX,(float)g_iWinCY };
	mPos = { g_iWinCX*0.5f,g_iWinCY *0.5f };

	XMStoreFloat4x4(&mProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	return S_OK;
}

_int CGameObject_BackGround::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return _int();
}

_int CGameObject_BackGround::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComTransform->Scaled(XMVectorSet(mSize.x, mSize.y, 1.f, 0.0f));
	mComTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(mPos.x - (g_iWinCX * 0.5f), -mPos.y + (g_iWinCY * 0.5f), 0.f, 1.f));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return _int();
}

HRESULT CGameObject_BackGround::Render()
{
	if (nullptr == mComVIBuffer)
		return E_FAIL;
	mComTransform->Bind_OnShader(mComShader, "g_WorldMatrix");
	//mComShader->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4));

	mComShader->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	mComShader->Set_RawValue("g_ProjMatrix", &mProjMatrix, sizeof(_float4x4));

	// 텍스처 넘기기
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, "g_DiffuseTexture", 0));

	FAILED_CHECK(mComVIBuffer->Render(mComShader, 0));
	return S_OK;
}

HRESULT CGameObject_BackGround::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_DEFAULT), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	return S_OK;
}

CGameObject_BackGround * CGameObject_BackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_BackGround*	pInstance = NEW CGameObject_BackGround(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject_BackGround* CGameObject_BackGround::Clone(void* pArg)
{
	CGameObject_BackGround*	pInstance = NEW CGameObject_BackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_BackGround::Free()
{
	__super::Free();
	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComVIBuffer);
	Safe_Release(mComTexture);
}