#include "stdafx.h"
#include "GameObject_FBX_Ani.h"

CGameObject_FBX_Ani::CGameObject_FBX_Ani(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CGameObject_FBX_Ani::CGameObject_FBX_Ani(const CGameObject_FBX_Ani& rhs)
	: CGameObject(rhs)
	, mComShader(rhs.mComShader)
	, mComRenderer(rhs.mComRenderer)
	, mComModel(rhs.mComModel)
{
	Safe_AddRef(mComShader);
	Safe_AddRef(mComRenderer);
	Safe_AddRef(mComModel);
}

HRESULT CGameObject_FBX_Ani::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_FBX_Ani::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	CTransform::TRANSFORMDESC desc;
	desc.RotPersec = XMConvertToRadians(180);
	desc.SpeedPersec = 5.0f;

	mComTransform->SetTransformDesc(desc);
	return S_OK;
}

_int CGameObject_FBX_Ani::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	CGameInstance* pGameInstance = GetSingle(CGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_UPARROW) & DIS_Press)
	{
		mComTransform->GO_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_DOWNARROW) & DIS_Press)
	{
		mComTransform->GO_Backward(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFTARROW) & DIS_Press)
	{
		mComTransform->Turn(XMVectorSet(0,1,0,0), TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_RIGHTARROW) & DIS_Press)
	{
		mComTransform->Turn(XMVectorSet(0, -1, 0, 0), TimeDelta);
	}
	static int Index = 0;

	if (pGameInstance->Get_DIKeyState(DIK_F) & DIS_Down)
	{
		int max = mComModel->Get_NumAnimations();
		Index++;
		Index %= max;
	}

	mComModel->SetUp_AnimIndex(Index);

	return UPDATENONE;
}

_int CGameObject_FBX_Ani::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	mComModel->Update_CombinedTransformationMatrices(TimeDelta);

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
	return UPDATENONE;
}

HRESULT CGameObject_FBX_Ani::Render()
{
	FAILED_CHECK(__super::Render());
	FAILED_CHECK(Set_ConstantTable());

	_uint iNumMaterials = mComModel->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{

		mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
	//	mComModel->Bind_OnShader(mComShader, i, aiTextureType_DIFFUSE, STR_TEX_DIFFUSE);
		mComModel->Render(mComShader, 0, i,STR_BONES);
	}


	return S_OK;
}


CGameObject_FBX_Ani * CGameObject_FBX_Ani::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_FBX_Ani*	pInstance = NEW CGameObject_FBX_Ani(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_FBX_Ani");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_FBX_Ani* CGameObject_FBX_Ani::Clone(void* pArg)
{
	CGameObject_FBX_Ani*	pInstance = NEW CGameObject_FBX_Ani(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_FBX_Ani");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGameObject_FBX_Ani::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXANIMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_MODEL_ANI), TEXT("Com_Model"), (CComponent**)&mComModel));
	return S_OK;
}

HRESULT CGameObject_FBX_Ani::Set_ConstantTable()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(mComTransform->Bind_OnShader(mComShader, STR_MAT_WORLD));
	FAILED_CHECK(mComShader->Set_RawValue(STR_MAT_VIEW, &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue(STR_MAT_PROJ, &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_PROJ), sizeof(_float4x4)));

	//const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	//if (pLightDesc == nullptr)
	//	return E_FAIL;

	//	FAILED_CHECK(mComShader->Set_RawValue("g_vLightPos", &pLightDesc->vDiffuse, sizeof(_float4)));
	//FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_DIR, &pLightDesc->vDirection, sizeof(_float4)));
	//FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_DIFFUSE, &pLightDesc->vDiffuse, sizeof(_float4)));
	//FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_AMBIENT, &pLightDesc->vAmbient, sizeof(_float4)));
	//FAILED_CHECK(mComShader->Set_RawValue(STR_LIGHT_SPECULAR, &pLightDesc->vSpecular, sizeof(_float4)));


	return S_OK;
}


void CGameObject_FBX_Ani::Free()
{
	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComModel);

	__super::Free();

}

