#include "stdafx.h"
#include "GameObject/GameObject_2D_Particle_Buffer.h"

GameObject_2D_Particle_Buffer::GameObject_2D_Particle_Buffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_2D_PARTICLE_RECT;
}

GameObject_2D_Particle_Buffer::GameObject_2D_Particle_Buffer(const GameObject_2D_Particle_Buffer& rhs)
	: CGameObject_Base(rhs)
{

}

HRESULT GameObject_2D_Particle_Buffer::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	return S_OK;
}

HRESULT GameObject_2D_Particle_Buffer::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	mComTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0, 0.f, 1.f));

	if (strlen(mTexStrDESC.mTextureKey_Diffuse) < 2)
	{
		string str("Snow.png");
		strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str.c_str());
	}
	mCurrentShaderPass = 0;
	
	return S_OK;
}

_int GameObject_2D_Particle_Buffer::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return UPDATENONE;
}

_int GameObject_2D_Particle_Buffer::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	return UPDATENONE;
}

HRESULT GameObject_2D_Particle_Buffer::Render()
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


HRESULT GameObject_2D_Particle_Buffer::Set_Component()
{
	if (mComRenderer == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	}

	if (mComShader == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_INSTANCE_POINT), TEXT("Com_Shader"), (CComponent**)&mComShader));
	}
	if (mComVIBuffer == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
	}

	//if (mComTexture == nullptr)
	//{
	//
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	//}

	return S_OK;
}

HRESULT GameObject_2D_Particle_Buffer::Set_ConstantTable_Texture()
{
//	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
	return S_OK;
}



GameObject_2D_Particle_Buffer * GameObject_2D_Particle_Buffer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	GameObject_2D_Particle_Buffer*	pInstance = NEW GameObject_2D_Particle_Buffer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating GameObject_2D_Particle_Buffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}
GameObject_2D_Particle_Buffer* GameObject_2D_Particle_Buffer::Clone(void* pArg)
{
	GameObject_2D_Particle_Buffer*	pInstance = NEW GameObject_2D_Particle_Buffer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating GameObject_2D_Particle_Buffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void GameObject_2D_Particle_Buffer::Free()
{
	__super::Free();
	Safe_Release(mComVIBuffer);
	
}