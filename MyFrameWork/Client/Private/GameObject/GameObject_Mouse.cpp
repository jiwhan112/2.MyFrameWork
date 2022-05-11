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
#ifdef _DEBUG
	, mBaseEffect(rhs.mBaseEffect)
	, mBatch(rhs.mBatch)
	, mInputLayout(rhs.mInputLayout)
#endif // _DEBUG
{


#ifdef _DEBUG
	Safe_AddRef(mInputLayout);
#endif // _DEBUG
}

HRESULT CGameObject_Mouse::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

#ifdef _DEBUG
	mBaseEffect = new BasicEffect(m_pDevice);
	mBaseEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	mBaseEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	FAILED_CHECK(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &mInputLayout));

	mBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);


#endif // _DEBUG


	return S_OK;
}

HRESULT CGameObject_Mouse::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	string str("GUI_Attribute_Damage.png");
	strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str.c_str());
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);

	mUiDesc.mDepth = 99;
	mViewPort = _viewport(0, 0, g_iWinCX, g_iWinCY);

	return S_OK;
}

_int CGameObject_Mouse::Tick(_double TimeDelta)
{
	//	FAILED_UPDATE(__super::Tick(TimeDelta));

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	mMousePos.x = (float)ptMouse.x - (g_iWinCX * 0.5f);
	mMousePos.y = (g_iWinCY * 0.5f) - (float)ptMouse.y;

	mComTransform->Scaled(XMVectorSet(50, 50, 1, 0.0f));
	mComTransform->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(mMousePos.x, mMousePos.y, 0.0f, 1.f));

	if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON))
	{
		mIsRender = !mIsRender;
		mRay = GetSingle(CGameInstance)->Get_Ray_World();

	}
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
	//	FAILED_CHECK(__super::Render());

	FAILED_CHECK(Set_ConstantTable_UI());
	FAILED_CHECK(Set_ConstantTable_Tex());

	mComVIBuffer->Render(mComShader, mCurrentShaderPass);

#ifdef _DEBUG
	if (mIsRender)
	{
		// 랜더링시 처리
		m_pDeviceContext->IASetInputLayout(mInputLayout);
		//	mBaseEffect->SetWorld(trans->GetWorldFloat4x4());

		
		mBaseEffect->SetWorld(_float4x4::Identity);
		mBaseEffect->SetView(XMLoadFloat4x4(&GetSingle(CGameInstance)->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW)));
		mBaseEffect->SetProjection(XMLoadFloat4x4(&GetSingle(CGameInstance)->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ)));

		mBaseEffect->Apply(m_pDeviceContext);

		// 배치 클래스의 Begin ~ End 까지 물체를 그린다.
		mBatch->Begin();
		DX::DrawRay(mBatch, mRay.position, mRay.direction, false, DirectX::Colors::Blue);


		mBatch->End();
	}
#endif // _DEBUG

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

#ifdef _DEBUG
	Safe_Release(mInputLayout);
	if (mIsClone == false)
	{
		Safe_Delete(mBaseEffect);
		Safe_Delete(mBatch);
	}
#endif // _DEBUG
}