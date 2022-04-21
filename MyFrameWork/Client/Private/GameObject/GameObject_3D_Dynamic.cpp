#include "stdafx.h"
#include "GameObject/GameObject_3D_Dynamic.h"

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_3D;

}

CGameObject_3D_Dynamic::CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs)
	: CGameObject_Base(rhs)
	, mComModel(rhs.mComModel)
{
	Safe_AddRef(mComModel);
}

HRESULT CGameObject_3D_Dynamic::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	string str("GUI_Menu_Main_Curtain.png");	
	strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str.c_str());
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);

	return S_OK;
}

_int CGameObject_3D_Dynamic::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));
	return UPDATENONE;
}

_int CGameObject_3D_Dynamic::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	return UPDATENONE;
}

HRESULT CGameObject_3D_Dynamic::Render()
{
	

	FAILED_CHECK(Set_ConstantTable_Tex());
	FAILED_CHECK(mComVIBuffer->Render(mComShader, 0));
	return S_OK;
}


HRESULT CGameObject_3D_Dynamic::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	
	// �� Ÿ�Կ� ���� ������ ������ ó��
	if (mComModel == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_MODEL_ANI), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXANIMODEL), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComVIBuffer == nullptr)
		mComVIBuffer = nullptr;

	if (mComTexture == nullptr)
		mComTexture = nullptr;
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_ConstantTable_World()
{
	return S_OK;
}

HRESULT CGameObject_3D_Dynamic::Set_ConstantTable_Tex(_uint texid)
{
	// �ؽ�ó �ѱ��
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
	return S_OK;
}


CGameObject_3D_Dynamic * CGameObject_3D_Dynamic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_3D_Dynamic*	pInstance = NEW CGameObject_3D_Dynamic(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_3D_Dynamic* CGameObject_3D_Dynamic::Clone(void* pArg)
{
	CGameObject_3D_Dynamic*	pInstance = NEW CGameObject_3D_Dynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_3D_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_3D_Dynamic::Free()
{
	__super::Free();

	Safe_Release(mComModel);


}