#include "stdafx.h"
#include "GameObject/GameObject_3D.h"

CGameObject_3D::CGameObject_3D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D;

}

CGameObject_3D::CGameObject_3D(const CGameObject_3D& rhs)
	: CGameObject_Base(rhs)
{
}

HRESULT CGameObject_3D::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_3D::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	string str("GUI_Menu_Main_Curtain.png");	
	strcpy_s(mTexDESC.mTextureKey_Diffuse, str.c_str());
	mComTexture->Set_TextureMap(mTexDESC.mTextureKey_Diffuse);
	return S_OK;
}

_int CGameObject_3D::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return UPDATENONE;
}

_int CGameObject_3D::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return UPDATENONE;
}

HRESULT CGameObject_3D::Render()
{
	

	FAILED_CHECK(Set_ConstantTable_Tex());
	FAILED_CHECK(mComVIBuffer->Render(mComShader, 0));
	return S_OK;
}


HRESULT CGameObject_3D::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_MODEL), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	return S_OK;
}

HRESULT CGameObject_3D::Set_ConstantTable_World()
{
	return S_OK;
}

HRESULT CGameObject_3D::Set_ConstantTable_Tex(_uint texid)
{
	// 텍스처 넘기기
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
	return S_OK;
}


CGameObject_3D * CGameObject_3D::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D*	pInstance = new CGameObject_3D(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D* CGameObject_3D::Clone(void* pArg)
{
	CGameObject_3D*	pInstance = new CGameObject_3D(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D::Free()
{
	__super::Free();

}
