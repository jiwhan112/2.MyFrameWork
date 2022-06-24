#include "stdafx.h"
#include "GameObject/GameObject_3D_Static2.h"
#include "GameObject/GameObject_MyTerrain.h"

CGameObject_3D_Static2::CGameObject_3D_Static2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_STATIC_PARENT;
}

CGameObject_3D_Static2::CGameObject_3D_Static2(const CGameObject_3D_Static2& rhs)
	: CGameObject_Base(rhs)
	, meUpdateType(rhs.meUpdateType)
{
	// 깊은 복사 처리
	for (auto& obj : rhs.mVecChildObject)
	{
		CGameObject_3D_Static* newchildobj = obj->Clone(nullptr);
		Add_StaticObejct(newchildobj);
	}
}

HRESULT CGameObject_3D_Static2::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	// 데이터 디폴트 세팅

	mCurrentShaderPass = 0;
	return S_OK;
}

HRESULT CGameObject_3D_Static2::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}

_int CGameObject_3D_Static2::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	if (meUpdateType == CGameObject_3D_Static2::E_UPDATETYPE_NONE)
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_DEFAULT;
	}

	else if (meUpdateType == CGameObject_3D_Static2::E_UPDATETYPE_PICK)
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_RED;

		// 피킹오브젝트는 해당 지형에 매핑된다.
		CGameObject_MyTerrain* terrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN_DUNGEON));
		if (terrain != nullptr)
		{
				_float3 worldPos = GetSingle(CGameManager)->Get_PickPos();
				int index = terrain->Get_TileIndex(worldPos);
				_float3 pickTilePos = terrain->Get_TileWorld(index);			
				_float4x4 transmat = _float4x4::CreateTranslation(pickTilePos);
				mComTransform->Set_State(CTransform::STATE_POSITION, transmat.Translation());
		}
	}

	FAILED_CHECK_NONERETURN(Tick_Child(TimeDelta));

	return UPDATENONE;
}

_int CGameObject_3D_Static2::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	FAILED_CHECK_NONERETURN(LateTick_Child(TimeDelta));
	return UPDATENONE;
}

HRESULT CGameObject_3D_Static2::Render()
{
	//FAILED_CHECK(Set_ConstantTable_World());
	//FAILED_CHECK(Set_ConstantTable_Light());



	return S_OK;
}

HRESULT CGameObject_3D_Static2::Add_StaticObejct(CGameObject_3D_Static* obj)
{
	// 자식 오브젝트 추가
	mVecChildObject.push_back(obj);
	return S_OK;
}

HRESULT CGameObject_3D_Static2::Delete_StaticObejct(int index)
{
	if (index >= mVecChildObject.size())
		return E_FAIL;

	vector<CGameObject_3D_Static*>::iterator iter;
	iter = mVecChildObject.begin();

	for (int i = 0; i < index; ++i, ++iter);
	Safe_Release(*iter);
	mVecChildObject.erase(iter);

	return S_OK;

}


CGameObject_3D_Static * CGameObject_3D_Static2::Get_ChildOfIndex(_uint index) const
{
	if (mVecChildObject.empty())
		return nullptr;

	return mVecChildObject[index];
}

HRESULT CGameObject_3D_Static2::ReleaseModelVec()
{
	if (mVecChildObject.empty() == false)
	{
		for (auto& model : mVecChildObject)
		{
			Safe_Release(model);
		}
	}
	mVecChildObject.clear();

	return S_OK;
}

HRESULT CGameObject_3D_Static2::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));


	return S_OK;
}

HRESULT CGameObject_3D_Static2::Render_Child()
{
	if (mVecChildObject.empty())
		return S_FALSE;


	// 부모 업데이트
	//_uint iNumMaterials = mComModel->Get_NumMaterials();

	//for (int i = 0; i < iNumMaterials; ++i)
	//{
	//	mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
	//	FAILED_CHECK(mComModel->Render(mComShader, mCurrentShaderPass, 0));
	//}

	// 자식 업데이트
//	for (auto& model : mVecChildObject)
//	{
//		model->Set_RenderPass(mCurrentShaderPass);
//		model->Render();
//	}
	return S_OK;
}

HRESULT CGameObject_3D_Static2::Tick_Child(_double time)
{
	// 자식 업데이트
	_matrix parentMat = mComTransform->GetWorldMatrix();
	for (auto& childObj : mVecChildObject)
	{
		childObj->Update_CombinedTransformationMatrix(parentMat);
		if (0 > childObj->Tick(time))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CGameObject_3D_Static2::LateTick_Child(_double time)
{
	// 자식 업데이트
	for (auto& model : mVecChildObject)
	{
		if (0 > model->LateTick(time))
			return E_FAIL;
	}
	return S_OK;
}


CGameObject_3D_Static2 * CGameObject_3D_Static2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Static2*	pInstance = NEW CGameObject_3D_Static2(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Static2");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Static2* CGameObject_3D_Static2::Clone(void* pArg)
{
	CGameObject_3D_Static2*	pInstance = NEW CGameObject_3D_Static2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Static2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Static2::Free()
{
	__super::Free();
	ReleaseModelVec();

	
}