#include "stdafx.h"
#include "GameObject_FBX.h"

CGameObject_FBX::CGameObject_FBX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CGameObject_FBX::CGameObject_FBX(const CGameObject_FBX& rhs)
	: CGameObject(rhs)
	, mComShader(rhs.mComShader)
	, mComRenderer(rhs.mComRenderer)
	, mComModel(rhs.mComModel)
{
	Safe_AddRef(mComShader);
	Safe_AddRef(mComRenderer);
	Safe_AddRef(mComModel);
}

HRESULT CGameObject_FBX::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_FBX::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	CTransform::TRANSFORMDESC desc;
	desc.RotPersec = XMConvertToRadians(180);
	desc.SpeedPersec = 5.0f;

	mComTransform->SetTransformDesc(desc);
	return S_OK;
}

_int CGameObject_FBX::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	CGameInstance* pGameInstance = GetSingle(CGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_UPARROW) & 0x80)
	{
		mComTransform->GO_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_DOWNARROW) & 0x80)
	{
		mComTransform->GO_Backward(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFTARROW) & 0x80)
	{
		mComTransform->Turn(XMVectorSet(0,1,0,0), TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_RIGHTARROW) & 0x80)
	{
		mComTransform->Turn(XMVectorSet(0, -1, 0, 0), TimeDelta);
	}

	return UPDATENONE;
}

_int CGameObject_FBX::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return UPDATENONE;
}

HRESULT CGameObject_FBX::Render()
{
	FAILED_CHECK(__super::Render());
	FAILED_CHECK(Set_ConstantTable());
	FAILED_CHECK(mComModel->Render(mComShader, 0));

	return S_OK;
}


CGameObject_FBX * CGameObject_FBX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_FBX*	pInstance = NEW CGameObject_FBX(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_FBX");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_FBX* CGameObject_FBX::Clone(void* pArg)
{
	CGameObject_FBX*	pInstance = NEW CGameObject_FBX(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_FBX");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGameObject_FBX::Set_Component()
{
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_MODEL), TEXT("Com_Model"), (CComponent**)&mComModel));
	return S_OK;
}

HRESULT CGameObject_FBX::Set_ConstantTable()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	FAILED_CHECK(mComTransform->Bind_OnShader(mComShader, "g_WorldMatrix"));
	FAILED_CHECK(mComShader->Set_RawValue("g_ViewMatrix", &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_ProjMatrix", &pGameInstance->GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE::D3DTS_PROJ), sizeof(_float4x4)));

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (pLightDesc == nullptr)
		return E_FAIL;

	//	FAILED_CHECK(mComShader->Set_RawValue("g_vLightPos", &pLightDesc->vDiffuse, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)));


	return S_OK;
}


void CGameObject_FBX::Free()
{
	Safe_Release(mComShader);
	Safe_Release(mComRenderer);
	Safe_Release(mComModel);

	__super::Free();

}
