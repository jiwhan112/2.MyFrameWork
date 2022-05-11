#include "stdafx.h"
#include "GameObject/GameObject_3D_Static2.h"

CGameObject_3D_Static2::CGameObject_3D_Static2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D_STATIC_PARENT;
}

CGameObject_3D_Static2::CGameObject_3D_Static2(const CGameObject_3D_Static2& rhs)
	: CGameObject_Base(rhs)
	, meUpdateType(rhs.meUpdateType)
{
	// ���� ���� ó��
	for (auto& obj : rhs.mVecChildObject)
	{
		CGameObject_3D_Static* newchildobj = obj->Clone(nullptr);
		Add_StaticObejct(newchildobj);
	}

}

HRESULT CGameObject_3D_Static2::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	// ������ ����Ʈ ����

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

	// �ڽ���ġ ������Ʈ
	// Update_Hier();

	if (meUpdateType == CGameObject_3D_Static2::E_UPDATETYPE_NONE)
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_DEFAULT;

	}
	else if (meUpdateType == CGameObject_3D_Static2::E_UPDATETYPE_PICK)
	{
		mCurrentShaderPass = (int)E_SHADERPASS_STATICMODEL_RED;
	}

	FAILED_CHECK_NONERETURN(Tick_Child(TimeDelta));

	return UPDATENONE;
}

_int CGameObject_3D_Static2::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	FAILED_CHECK_NONERETURN(LateTick_Child(TimeDelta));


	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);

	return UPDATENONE;
}

HRESULT CGameObject_3D_Static2::Render()
{
	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Light());

	FAILED_CHECK(Render_Child());

	return S_OK;
}

HRESULT CGameObject_3D_Static2::Add_StaticObejct(CGameObject_3D_Static* obj)
{
	// �ڽ� ������Ʈ �߰�
	mVecChildObject.push_back(obj);
	return S_OK;
}

HRESULT CGameObject_3D_Static2::Delefe_StaticObejct(int index)
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

	// �� Ÿ�Կ� ���� ������ ������ ó��

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));


	return S_OK;
}

HRESULT CGameObject_3D_Static2::Render_Child()
{
	if (mVecChildObject.empty())
		return S_FALSE;


	// �θ� ������Ʈ
	/*_uint iNumMaterials = mComModel->Get_NumMaterials();

	for (int i = 0; i < iNumMaterials; ++i)
	{
		mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
		FAILED_CHECK(mComModel->Render(mComShader, mCurrentShaderPass, 0));
	}*/

	// �ڽ� ������Ʈ
	for (auto& model : mVecChildObject)
	{
		model->Render();
	}
	return S_OK;
}

HRESULT CGameObject_3D_Static2::Tick_Child(_double time)
{
	// �ڽ� ������Ʈ
	for (auto& model : mVecChildObject)
	{
		if (0 > model->Tick(time))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CGameObject_3D_Static2::LateTick_Child(_double time)
{
	// �ڽ� ������Ʈ
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