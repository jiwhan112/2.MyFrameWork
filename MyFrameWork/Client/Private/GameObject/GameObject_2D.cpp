#include "stdafx.h"
#include "GameObject/GameObject_2D.h"

CGameObject_2D::CGameObject_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_2D;

}

CGameObject_2D::CGameObject_2D(const CGameObject_2D& rhs)
	: CGameObject_Base(rhs)
	,mUiDesc(rhs.mUiDesc)
	,mTexStrDESC(rhs.mTexStrDESC)
{

	mComVIBuffer = rhs.mComVIBuffer;
	mComTexture = rhs.mComTexture;

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
	if (strlen(mTexStrDESC.mTextureKey_Diffuse) < 2)
	{
		string str("GUI_Menu_Main_Curtain.png");
		strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str.c_str());

	}
	mCurrentShaderPass = 1;
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);

	return S_OK;
}

_int CGameObject_2D::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	mComTransform->Scaled(XMVectorSet(mUiDesc.mSizeX, mUiDesc.mSizeY, 1.f, 0.0f));
	mComTransform->SetState(CTransform::STATE_POSITION, XMVectorSet(mUiDesc.mPosX - (g_iWinCX * mUiDesc.mPivot.x), -mUiDesc.mPosY + (g_iWinCY * mUiDesc.mPivot.y), 0, 1.f));


	return UPDATENONE;
}

_int CGameObject_2D::LateTick(_double TimeDelta)
{

	FAILED_UPDATE(__super::LateTick(TimeDelta));


	mComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);


	return UPDATENONE;

}

HRESULT CGameObject_2D::Render()
{
	FAILED_CHECK(__super::Render());

	FAILED_CHECK(Set_ConstantTable_UI());
	FAILED_CHECK(Set_ConstantTable_Tex());

	
	mComVIBuffer->Render(mComShader, mCurrentShaderPass);

	return S_OK;
}


void CGameObject_2D::Set_LoadTexDesc(const TEXTUREDESC & desc)
{
	memcpy(&mTexStrDESC, &desc, sizeof(TEXTUREDESC));
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);
}

HRESULT CGameObject_2D::Set_Component()
{

	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComVIBuffer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));

	if (mComTexture == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	return S_OK;
}

HRESULT CGameObject_2D::Set_ConstantTable_UI()
{
	if (nullptr == mComVIBuffer)
		return E_FAIL;

	_float4x4 view;
	XMStoreFloat4x4(&view, XMMatrixIdentity());

	_float4x4 projori;
	XMStoreFloat4x4(&projori, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0, 1)));

	// WVP
	mComTransform->Bind_OnShader(mComShader, STR_MAT_WORLD);
	mComShader->Set_RawValue(STR_MAT_VIEW, &view, sizeof(_float4x4));
	mComShader->Set_RawValue(STR_MAT_PROJ, &projori, sizeof(_float4x4));
	return S_OK;
}



HRESULT CGameObject_2D::Set_ConstantTable_Tex()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
	return S_OK;
}


CGameObject_2D * CGameObject_2D::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_2D*	pInstance = NEW CGameObject_2D(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_2D");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_2D* CGameObject_2D::Clone(void* pArg)
{
	CGameObject_2D*	pInstance = NEW CGameObject_2D(*this);

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
	Safe_Release(mComVIBuffer);;
	Safe_Release(mComTexture);;

}
