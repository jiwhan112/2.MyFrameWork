#include "stdafx.h"
#include "GameObject/GameObject_3D_Static.h"


CGameObject_3D_Static::CGameObject_3D_Static(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_STATIC;

}

CGameObject_3D_Static::CGameObject_3D_Static(const CGameObject_3D_Static& rhs)
	: CGameObject_Base(rhs)
	, mComModel(rhs.mComModel)
	, mModelDesc(rhs.mModelDesc)
{
	Safe_AddRef(mComModel);
}

HRESULT CGameObject_3D_Static::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	// 데이터 디폴트 세팅
	if (strlen(mModelDesc.mModelName) < 2)
	{
		string str("room_Prison_FloorTile.fbx");
		strcpy_s(mModelDesc.mModelName, str.c_str());
	}


	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_3D_Static::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	
	return S_OK;
}

_int CGameObject_3D_Static::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return UPDATENONE;
}

_int CGameObject_3D_Static::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	return UPDATENONE;
}

HRESULT CGameObject_3D_Static::Render()
{

	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());
	FAILED_CHECK(Set_ConstantTable_Model());

	if (mComModel != nullptr)
	{
		_uint iNumMaterials = mComModel->Get_NumMaterials();

		// 재질 개수만큼 루프
		for (int i = 0; i < iNumMaterials; ++i)
		{
			// 1. 텍스처 설정
			mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
			// 2. 랜더링
			// 여기서 뼈를 넘긴다.
			FAILED_CHECK(mComModel->Render(mComShader, mCurrentShaderPass, 0));
		}
	}	
	
	return S_OK;
}


HRESULT CGameObject_3D_Static::Set_LoadModelDESC(const MODEL_STATIC_DESC & desc)
{
	memcpy(&mModelDesc, &desc, sizeof(MODEL_STATIC_DESC));

	// 해당 모델 컴포넌트로 변경
	if (mComModel != nullptr)
	{
		Safe_Release(mComModel);
		mComModel = nullptr;
	}

	string strModel = mModelDesc.mModelName;
	wstring ModelName = CHelperClass::Convert_str2wstr(strModel);

	FAILED_CHECK(__super::Release_Component(TEXT("Com_Model")));

	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));

	return S_OK;
}

HRESULT CGameObject_3D_Static::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

		// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	

	if (mComModel == nullptr)
	{
		string strModel = mModelDesc.mModelName;
		wstring ModelName = CHelperClass::Convert_str2wstr(strModel);
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Model"), (CComponent**)&mComModel));
	}
	return S_OK;
}

HRESULT CGameObject_3D_Static::Set_ConstantTable_Model()
{
	return S_OK;
}


CGameObject_3D_Static * CGameObject_3D_Static::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Static*	pInstance = NEW CGameObject_3D_Static(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Static");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Static* CGameObject_3D_Static::Clone(void* pArg)
{
	CGameObject_3D_Static*	pInstance = NEW CGameObject_3D_Static(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Static");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Static::Free()
{
	__super::Free();

	Safe_Release(mComModel);
}
