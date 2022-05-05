#include "stdafx.h"
#include "GameObject/GameObject_2D_Particle.h"

CGameObject_2D_Particle::CGameObject_2D_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_2D;
}

CGameObject_2D_Particle::CGameObject_2D_Particle(const CGameObject_2D_Particle& rhs)
	: CGameObject_Base(rhs)
{

}

HRESULT CGameObject_2D_Particle::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_2D_Particle::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));


	return S_OK;
}

_int CGameObject_2D_Particle::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	

	return UPDATENONE;
}

_int CGameObject_2D_Particle::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return UPDATENONE;
}

HRESULT CGameObject_2D_Particle::Render()
{
	FAILED_CHECK(__super::Render());


	return S_OK;
}


HRESULT CGameObject_2D_Particle::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));

	// if (mComVIBuffer == nullptr)
	// 	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	// 
	// if (mComTexture == nullptr)
	// 	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	return S_OK;
}



CGameObject_2D_Particle * CGameObject_2D_Particle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_2D_Particle*	pInstance = NEW CGameObject_2D_Particle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_2D_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_2D_Particle* CGameObject_2D_Particle::Clone(void* pArg)
{
	CGameObject_2D_Particle*	pInstance = NEW CGameObject_2D_Particle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_2D_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_2D_Particle::Free()
{
	__super::Free();
}