#include "stdafx.h"
#include "GameObject/GameObject_3D_Particle.h"

CGameObject_3D_Particle::CGameObject_3D_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_PARTICLE;
}

CGameObject_3D_Particle::CGameObject_3D_Particle(const CGameObject_3D_Particle& rhs)
	: CGameObject_Base(rhs)
	, mModelName(rhs.mModelName)
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
		mModelName = "skfx_Meteor_big_00.fbx";
	}


	return S_OK;
}

_int CGameObject_3D_Particle::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	// 파티클 움직임
	mParticleDESC.Timer += TimeDelta;

	mComTransform->MovetoDir(mParticleDESC.Dir, 0.3f,TimeDelta);

	if (mParticleDESC.TimeMax < mParticleDESC.Timer)
	{
		// Dead;
		Set_Dead();
	}

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
		return S_FALSE;

	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());

//	FAILED_CHECK(Set_ConstantTable_Texture());
//	FAILED_CHECK(Set_ConstantTable_OnlyCameraPos());

	mCurrentShaderPass = 0;
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


HRESULT CGameObject_3D_Particle::Set_LoadParticleDesc(PARTICLEDESC desc)
{
	mParticleDESC = desc;
	return S_OK;
}

HRESULT CGameObject_3D_Particle::Set_LoadModelDesc(string str)
{

	mModelName = str.c_str();

	// 해당 모델 컴포넌트로 변경
	if (mComModel != nullptr)
	{
		Safe_Release(mComModel);
		mComModel = nullptr;
	}

	wstring ModelName = CHelperClass::Convert_str2wstr(mModelName);

	FAILED_CHECK(__super::Release_Component(TEXT("Com_Model")));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));

	return S_OK;
}

HRESULT CGameObject_3D_Particle::Set_Component()
{
	if (mComRenderer == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	}
	if (mComShader == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_PARTICLE3D), TEXT("Com_Shader"), (CComponent**)&mComShader));
	}

	mComModel = nullptr;
	if (mComTextureMap == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTextureMap));

	}
	return S_OK;
}

HRESULT CGameObject_3D_Particle::Set_ConstantTable_Model()
{
	// 이외 텍스처 바인딩
	if (mComTextureMap == nullptr)
		return S_OK;

	// 텍스처 

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