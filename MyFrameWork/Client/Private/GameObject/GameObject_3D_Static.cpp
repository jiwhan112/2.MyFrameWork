#include "stdafx.h"
#include "GameObject/GameObject_3D_Static.h"

// #TODO 3D깡통오브젝트 생성
// 

CGameObject_3D_Static::CGameObject_3D_Static(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D;

}

CGameObject_3D_Static::CGameObject_3D_Static(const CGameObject_3D_Static& rhs)
	: CGameObject_Base(rhs)
	, mComModel(rhs.mComModel)
{
	Safe_AddRef(mComModel);
}

HRESULT CGameObject_3D_Static::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

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

	for (int i = 0; i < 1; ++i)
	{
		FAILED_CHECK(mComModel->Render(mComShader, 0, 0));
	}
	return S_OK;
}


HRESULT CGameObject_3D_Static::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

		// 모델 타입에 따라 정적모델 동적모델 처리

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	string strModel = mModelDesc.mModelName;
	wstring ModelName;
	ModelName.assign(strModel.begin(), strModel.end());

	if (mComModel == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, ModelName.c_str(), TEXT("Com_Shader"), (CComponent**)&mComShader));

	return S_OK;
}

HRESULT CGameObject_3D_Static::Set_ConstantTable_Model()
{
	return S_OK;
}

//HRESULT CGameObject_3D_Static::Set_ConstantTable_Light(_uint lightid)
//{
//	return S_OK;
//}

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
