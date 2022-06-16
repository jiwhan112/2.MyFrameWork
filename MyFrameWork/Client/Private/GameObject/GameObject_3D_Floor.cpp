#include "stdafx.h"
#include "GameObject/GameObject_3D_Floor.h"

CGameObject_3D_Floor::CGameObject_3D_Floor(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject_3D_Static(pDevice,pDeviceContext)
{

}

CGameObject_3D_Floor::CGameObject_3D_Floor(const CGameObject_3D_Floor & rhs)
	: CGameObject_3D_Static(rhs)
{
}

HRESULT CGameObject_3D_Floor::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	mComModel = nullptr;

	if (mComCollider == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_SPHERE), TEXT("Com_Collider"), (CComponent**)&mComCollider));
	}

	/*if (mComTexture == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP_FBX), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	}*/

	return S_OK;
}

HRESULT CGameObject_3D_Floor::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	mCurrentShaderPass = 0;

	return S_OK;
}

HRESULT CGameObject_3D_Floor::NativeConstruct(void * pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	Set_LoadNewFBX(CGameObject_3D_Floor::FLOOR_FLOOR);
	return S_OK;
}

_int CGameObject_3D_Floor::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	if (meTickType == CGameObject_3D_Floor::TICKTYPE_NONE)
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_DEFAULT;
	}
	else
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_RED;
	}

	if (mComCollider)
		mComCollider->Update_Transform(mComTransform->GetWorldFloat4x4());
	return UPDATENONE;
}

HRESULT CGameObject_3D_Floor::Render()
{
	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());


	if (mComModel != nullptr)
	{
		_uint iNumMaterials = mComModel->Get_NumMaterials();

		// 재질 개수만큼 루프
		for (int i = 0; i < iNumMaterials; ++i)
		{
			// 1. Diffuse 텍스처 설정
			mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);

			// 2. 랜더링
			// 여기서 뼈를 넘긴다.
			mComModel->Render(mComShader, mCurrentShaderPass, i, "g_BoneMatrices");
		}
	}
#ifdef _DEBUG
	if (mComCollider)
		mComCollider->Render();
#endif // _DEBUG
	return S_OK;
}

HRESULT CGameObject_3D_Floor::Set_LoadNewFBX(E_FLOORTYPE type)
{
	MODEL_STATIC_DESC desc;

	switch (type)
	{
	case CGameObject_3D_Floor::FLOOR_FLOOR:
		strcpy_s(desc.mModelName, FloorFbxName);
		Set_LoadModelDESC(desc);
		break;

	case CGameObject_3D_Floor::FLOOR_BUILD:
		strcpy_s(desc.mModelName, BuildFbxName);
		Set_LoadModelDESC(desc);
		break;

	}
	return S_OK;

}

CGameObject_3D_Floor * CGameObject_3D_Floor::Create(ID3D11Device * d, ID3D11DeviceContext * c)
{
	CGameObject_3D_Floor*	pInstance = NEW CGameObject_3D_Floor(d, c);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Floor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject_3D_Floor * CGameObject_3D_Floor::Clone(void * pArg)
{
	CGameObject_3D_Floor*	pInstance = NEW CGameObject_3D_Floor(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Floor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Floor::Free()
{
	__super::Free();

}
