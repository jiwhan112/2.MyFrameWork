#include "stdafx.h"
#include "GameObject/GameObject_MyTerrain.h"

CGameObject_MyTerrain::CGameObject_MyTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_TERRAIN;

}

CGameObject_MyTerrain::CGameObject_MyTerrain(const CGameObject_MyTerrain& rhs)
	: CGameObject_Base(rhs)
	, mComVIBuffer(rhs.mComVIBuffer)
	, mComTexture(rhs.mComTexture)
{
	Safe_AddRef(mComVIBuffer);
	Safe_AddRef(mComTexture);

}

HRESULT CGameObject_MyTerrain::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_MyTerrain::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	FAILED_CHECK(Set_Component());

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

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
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

	mComTexture->SetUp_OnShader(mComShader,STR_TEX_DIFFUSE,0);
	mComVIBuffer->Render(mComShader, 0);
	return S_OK;
}

bool CGameObject_MyTerrain::PickObject()
{
	_float3 pick;
	if (true == mComVIBuffer->Pick(mComTransform->GetWorldFloat4x4().Invert(), &pick))
	{
		int breakPoint = 5;
		return true;
	}
	return false;
}


HRESULT CGameObject_MyTerrain::Set_Component()
{

	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리
	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComVIBuffer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));

	if (mComTexture == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_GRASS), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_ConstantTable_Tex()
{
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE, 0));

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
	Safe_Release(mComTexture);
	

}
