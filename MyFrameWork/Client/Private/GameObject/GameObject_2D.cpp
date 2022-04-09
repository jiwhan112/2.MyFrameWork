#include "stdafx.h"
#include "GameObject/GameObject_2D.h"

CGameObject_2D::CGameObject_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CGameObject_2D::CGameObject_2D(const CGameObject_2D& rhs)
	: CGameObject(rhs)
{
	mComShader = rhs.mComShader;
	mComRenderer = rhs.mComRenderer;
	mComVIBuffer = rhs.mComVIBuffer;
	mComTexture = rhs.mComTexture;

	Safe_AddRef(mComShader);
	Safe_AddRef(mComRenderer);
	Safe_AddRef(mComVIBuffer);
	Safe_AddRef(mComTexture);
}

HRESULT CGameObject_2D::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_2D::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	string str("GUI_Menu_Main_Curtain.png");

	mComTexture->Set_TextureMap(str);

	return S_OK;
}

_int CGameObject_2D::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return UPDATENONE;
}

_int CGameObject_2D::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));


	mComTransform->Scaled(XMVectorSet(mUiDesc.mSizeX, mUiDesc.mSizeY, 1.f, 0.0f));
	mComTransform->SetState(CTransform::STATE_POSITION, XMVectorSet(mUiDesc.mPosX - (g_iWinCX * 0.5f), -mUiDesc.mPosY + (g_iWinCY * 0.5f), 0, 1.f));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return _int();

	return UPDATENONE;

}

HRESULT CGameObject_2D::Render()
{
	FAILED_CHECK(__super::Render());
	
	FAILED_CHECK(Set_ConstantTable_UI());
	FAILED_CHECK(Set_ConstantTable_Tex(0));
	FAILED_CHECK(mComVIBuffer->Render(mComShader, 0));


	return S_OK;
}

HRESULT CGameObject_2D::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	return S_OK;
}

HRESULT CGameObject_2D::Set_ConstantTable_UI()
{
	if (nullptr == mComVIBuffer)
		return E_FAIL;

	_float4x4 view;
	XMStoreFloat4x4(&view, XMMatrixIdentity());



//	_float4x4 projori = GetSingle(CGameInstance)->GetTransformFloat4x4_TP(CPipeLine::D3DTS_PROJ_ORI);
	_float4x4 projori;// = GetSingle(CGameInstance)->GetTransformFloat4x4_TP(CPipeLine::D3DTS_PROJ_ORI);
	XMStoreFloat4x4(&projori, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0, 1)));

	// WVP
	mComTransform->Bind_OnShader(mComShader, STR_MAT_WORLD);
	mComShader->Set_RawValue(STR_MAT_VIEW, &view, sizeof(_float4x4));
	mComShader->Set_RawValue(STR_MAT_PROJ, &projori, sizeof(_float4x4));
	return S_OK;
}

HRESULT CGameObject_2D::Set_ConstantTable_World()
{
	return S_OK;
}

HRESULT CGameObject_2D::Set_ConstantTable_Tex(_uint texid)
{
	// 텍스처 넘기기
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
	return S_OK;
}


CGameObject_2D * CGameObject_2D::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_2D*	pInstance = new CGameObject_2D(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_2D");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_2D* CGameObject_2D::Clone(void* pArg)
{
	CGameObject_2D*	pInstance = new CGameObject_2D(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_2D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_2D::Free()
{
	__super::Free();
	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComVIBuffer);
	Safe_Release(mComTexture);

}
