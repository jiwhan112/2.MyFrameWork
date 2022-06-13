#include "stdafx.h"
#include "GameObject/GameObject_3D_Particle.h"

CGameObject_3D_Particle::CGameObject_3D_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_PARTICLE;
}

CGameObject_3D_Particle::CGameObject_3D_Particle(const CGameObject_3D_Particle& rhs)
	: CGameObject_Base(rhs)
{

}

HRESULT CGameObject_3D_Particle::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	return S_OK;
}

HRESULT CGameObject_3D_Particle::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));


	if (mModelName.length() < 2)
	{
		mModelName = "aaa.fbx";
	}
	mCurrentShaderPass = 0;

	return S_OK;
}

_int CGameObject_3D_Particle::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return UPDATENONE;
}

_int CGameObject_3D_Particle::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	return UPDATENONE;
}

HRESULT CGameObject_3D_Particle::Render()
{
	FAILED_CHECK(__super::Render());
	if (
		mComShader == nullptr ||
		mComModel == nullptr
		)
		return E_FAIL;

	//FAILED_CHECK(Set_ConstantTable_World());
	//FAILED_CHECK(Set_ConstantTable_Texture());
	//FAILED_CHECK(Set_ConstantTable_OnlyCameraPos());

	//mComModel->Render(mComShader, mCurrentShaderPass);


	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());
	FAILED_CHECK(Set_ConstantTable_Model());

	if (mComModel != nullptr)
	{
		_uint iNumMaterials = mComModel->Get_NumMaterials();

		// 재질 개수만큼 루프
		for (int i = 0; i < iNumMaterials; ++i)
		{
			mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
			mComModel->Render(mComShader, mCurrentShaderPass, i, nullptr);
		}
	}


	return S_OK;
}


HRESULT CGameObject_3D_Particle::Set_Component()
{
	//if (mComRenderer == nullptr)
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	//if (mComShader == nullptr)
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_INSTANCE_POINT), TEXT("Com_Shader"), (CComponent**)&mComShader));

	//if (mComVIBuffer == nullptr)
	//{
	//	CVIBuffer_Point_Instance::PARTICLEDESC  ParticleDesc;

	//	ParticleDesc.fMinSpeed = 2.f;
	//	ParticleDesc.fMaxSpeed = 10.f;
	//	ParticleDesc.vMoveDir = _float3(0.f, -1.f, 0.f);
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_INSTANCE_POINT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer,&ParticleDesc));
	//}

	//if (mComTexture == nullptr)
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	return S_OK;
}

HRESULT CGameObject_3D_Particle::Set_ConstantTable_Model()
{
	// 이외 텍스처 바인딩
	if (mComTextureMap == nullptr)
		return S_OK;

	//auto tex1 = mComTexture->Get_MapTexture(mTexture_Model_DESC.mTextureKey_Normal);
	//auto tex2 = mComTexture->Get_MapTexture(mTexture_Model_DESC.mTextureKey_Hieght);

	//if (tex1)
	//	mComShader->Set_Texture(STR_TEX_NOMAL, tex1);

	//if (tex2)
	//	mComShader->Set_Texture(STR_TEX_HEIGHT, tex2);

	return S_OK;
}

CGameObject_3D_Particle * CGameObject_3D_Particle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Particle*	pInstance = NEW CGameObject_3D_Particle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Particle* CGameObject_3D_Particle::Clone(void* pArg)
{
	CGameObject_3D_Particle*	pInstance = NEW CGameObject_3D_Particle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Particle::Free()
{
	__super::Free();
}