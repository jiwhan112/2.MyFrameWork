#include "stdafx.h"
#include "GameObject/GameObject_2D_Particle_Point.h"

CGameObject_2D_Particle_Point::CGameObject_2D_Particle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_2D_PARTICLE_POINT;
}

CGameObject_2D_Particle_Point::CGameObject_2D_Particle_Point(const CGameObject_2D_Particle_Point& rhs)
	: CGameObject_Base(rhs)
{

}

HRESULT CGameObject_2D_Particle_Point::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	return S_OK;
}

HRESULT CGameObject_2D_Particle_Point::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	if (strlen(mTexStrDESC.mTextureKey_Diffuse) < 2)
	{
		string str("Snow.png");
		strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str.c_str());
	}
	mCurrentShaderPass = 0;
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);

	return S_OK;
}

_int CGameObject_2D_Particle_Point::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return UPDATENONE;
}

_int CGameObject_2D_Particle_Point::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	CVIBuffer_Point_Instance::PARTICLE_INSTANCEDESC desc;
	mComVIBuffer->Tick_Client(TimeDelta, desc);
	mComRenderer->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	return UPDATENONE;
}

HRESULT CGameObject_2D_Particle_Point::Render()
{
	FAILED_CHECK(__super::Render());
	if (
		mComShader == nullptr ||
		mComVIBuffer == nullptr
		)
		return E_FAIL;

	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Texture());
	FAILED_CHECK(Set_ConstantTable_OnlyCameraPos());

	mComVIBuffer->Render(mComShader, mCurrentShaderPass);

	return S_OK;
}


HRESULT CGameObject_2D_Particle_Point::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_INSTANCE_POINT), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComVIBuffer == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_INSTANCE_POINT_20), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_INSTANCE_POINT_100), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_INSTANCE_POINT_300), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	}

	if (mComTexture == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	return S_OK;
}

HRESULT CGameObject_2D_Particle_Point::Set_ConstantTable_Texture()
{
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));

	return S_OK;
}



CGameObject_2D_Particle_Point * CGameObject_2D_Particle_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_2D_Particle_Point*	pInstance = NEW CGameObject_2D_Particle_Point(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_2D_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_2D_Particle_Point* CGameObject_2D_Particle_Point::Clone(void* pArg)
{
	CGameObject_2D_Particle_Point*	pInstance = NEW CGameObject_2D_Particle_Point(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_2D_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_2D_Particle_Point::Free()
{
	__super::Free();
}