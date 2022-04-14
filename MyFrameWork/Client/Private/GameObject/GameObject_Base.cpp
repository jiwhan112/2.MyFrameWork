#include "stdafx.h"
#include "GameObject/GameObject_Base.h"

CGameObject_Base::CGameObject_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_END;

}

CGameObject_Base::CGameObject_Base(const CGameObject_Base& rhs)
	: CGameObject(rhs)
	, mTexDESC(rhs.mTexDESC)
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

HRESULT CGameObject_Base::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_Base::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	return S_OK;
}

_int CGameObject_Base::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return UPDATENONE;
}

_int CGameObject_Base::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;

}

HRESULT CGameObject_Base::Render()
{
	return S_OK;
}

void CGameObject_Base::Set_LoadTexDesc(const TEXTUREDESC & desc)
{
	memcpy(&mTexDESC, &desc, sizeof(TEXTUREDESC));
	mComTexture->Set_TextureMap(mTexDESC.mTextureKey_Diffuse);
}

HRESULT CGameObject_Base::Set_ConstantTable_World()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);
	// WVP
	FAILED_CHECK(mComTransform->Bind_OnShader(mComShader, STR_MAT_WORLD));
	FAILED_CHECK(mComShader->Set_RawValue(STR_MAT_VIEW, &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_MAT_PROJ, &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_PROJ), sizeof(_float4x4)));
	return S_OK;
}

HRESULT CGameObject_Base::Set_ConstantTable_Tex()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
	return S_OK;
}

HRESULT CGameObject_Base::Set_ConstantTable_Light(_uint lightid)
{
	// 카메라 빛 세팅
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(lightid);
	if (pLightDesc == nullptr)
		return S_FALSE;

	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_DIR, &pLightDesc->vDirection, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_DIFFUSE, &pLightDesc->vDiffuse, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_AMBIENT, &pLightDesc->vAmbient, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_SPECULAR, &pLightDesc->vSpecular, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_CAMPOS, &pGameInstance->GetCameraPosition_vec(), sizeof(_float4)));

	return S_OK;
}

void CGameObject_Base::Free()
{
	__super::Free();
	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComVIBuffer);
	Safe_Release(mComTexture);

}
