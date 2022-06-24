#include "stdafx.h"
#include "GameObject/GameObject_Mouse.h"
#include "DebugDraw.h"

CGameObject_Mouse::CGameObject_Mouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_2D(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_MOUSE;
}

CGameObject_Mouse::CGameObject_Mouse(const CGameObject_Mouse& rhs)
	: CGameObject_2D(rhs)
{

}

HRESULT CGameObject_Mouse::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());



	return S_OK;
}

HRESULT CGameObject_Mouse::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));



	_rect rect = _rect(500, 500, 50, 50);
	mUiDesc.mUIRECT = rect;
	mUiDesc.mDepth = 99;
	Setup_UIPosition(mUiDesc);

	Setup_UIType(CGameObject_2D::UITYPE_IMAGE);
	strcpy_s(mTexStrDESC.mTextureKey_Diffuse, "GUI_Attribute_Damage.png");
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);


	return S_OK;
}

_int CGameObject_Mouse::Tick(_double TimeDelta)
{
	//	FAILED_UPDATE(__super::Tick(TimeDelta));

	mMousePos = GetSingle(CGameInstance)->Get_WindowPos();

	mUiDesc.mUIRECT.x = mMousePos.x;
	mUiDesc.mUIRECT.y = mMousePos.y;

	Setup_UIPosition();
	return UPDATENONE;
}

_int CGameObject_Mouse::LateTick(_double TimeDelta)
{
	//	FAILED_UPDATE(__super::LateTick(TimeDelta));
	
	mComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return UPDATENONE; 
}

HRESULT CGameObject_Mouse::Render()
{
	FAILED_CHECK(__super::Render());

	//FAILED_CHECK(Set_ConstantTable_UI());
	//FAILED_CHECK(Set_ConstantTable_Tex());
	//
	//mComVIBuffer->Render(mComShader, mCurrentShaderPass);

	return S_OK;
}

//HRESULT CGameObject_Mouse::Set_Component()
//{
//
//	if (mComRenderer == nullptr)
//		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));
//
//	if (mComShader == nullptr)
//		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));
//
//	if (mComVIBuffer == nullptr)
//		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
//
//	if (mComTexture == nullptr)
//		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));
//
//
//	return S_OK;
//}



CGameObject_Mouse * CGameObject_Mouse::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_Mouse*	pInstance = NEW CGameObject_Mouse(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_Mouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_Mouse* CGameObject_Mouse::Clone(void* pArg)
{
	CGameObject_Mouse*	pInstance = NEW CGameObject_Mouse(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_Mouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_Mouse::Free()
{
	__super::Free();
}