#include "stdafx.h"
#include "GameObject/GameObject_MyTerrain.h"

CGameObject_MyTerrain::CGameObject_MyTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D;

}

CGameObject_MyTerrain::CGameObject_MyTerrain(const CGameObject_MyTerrain& rhs)
	: CGameObject_Base(rhs)
	, mComVIBuffer(rhs.mComVIBuffer)
{
}

HRESULT CGameObject_MyTerrain::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_MyTerrain::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	//string str("GUI_Menu_Main_Curtain.png");
	//strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str.c_str());

	//mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);
	return S_OK;
}

_int CGameObject_MyTerrain::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return UPDATENONE;
}

_int CGameObject_MyTerrain::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return UPDATENONE;
}

HRESULT CGameObject_MyTerrain::Render()
{
	if (__super::Render() == S_FALSE)
	{
		return S_FALSE;
	}

	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Tex());
	FAILED_CHECK(Set_ConstantTable_Light());
//	mComVIBuffer->Render(mComShader, mCurrentShaderPass);
	return S_OK;
}


HRESULT CGameObject_MyTerrain::Set_Component()
{
//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXNORTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));
//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_ConstantTable_Tex()
{
	return S_OK;
}



CGameObject_MyTerrain * CGameObject_MyTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_MyTerrain*	pInstance = NEW CGameObject_MyTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_MyTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_MyTerrain* CGameObject_MyTerrain::Clone(void* pArg)
{
	CGameObject_MyTerrain*	pInstance = NEW CGameObject_MyTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_MyTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_MyTerrain::Free()
{
	__super::Free();
	Safe_Release(mComVIBuffer);

}
