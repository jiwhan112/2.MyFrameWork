#include "stdafx.h"
#include "GameObject/GameObject_2D.h"

CGameObject_2D::CGameObject_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_2D;
}

CGameObject_2D::CGameObject_2D(const CGameObject_2D& rhs)
	: CGameObject_Base(rhs)
	, mUiDesc(rhs.mUiDesc)
	, mTexStrDESC(rhs.mTexStrDESC)
{
	mComVIBuffer = rhs.mComVIBuffer;
	mComTexture = rhs.mComTexture;

	Safe_AddRef(mComVIBuffer);
	Safe_AddRef(mComTexture);
}

HRESULT CGameObject_2D::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());
	return S_OK;
}

HRESULT CGameObject_2D::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	if (strlen(mTexStrDESC.mTextureKey_Diffuse) < 2)
	{
		string str("GUI_Menu_Main_Curtain.png");
		strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str.c_str());
	}
	mCurrentShaderPass = 1;
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);

	mIsActive = true;


	return S_OK;
}

_int CGameObject_2D::Tick(_double TimeDelta)
{
	mIsOverrMouse = false;

	FAILED_UPDATE(__super::Tick(TimeDelta));
	GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::E_COLLIDEROBJ_TYPE::COLLIDEROBJ_UI, this);

	return UPDATENONE;
}

_int CGameObject_2D::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);

	if (meUIType == CGameObject_2D::UITYPE_BUTTON1)
	{
		LateTick_Button1();

	}
	else if (meUIType == CGameObject_2D::UITYPE_BUTTON2_UNIT)
	{
		LateTick_Button2_Unit();

	}

	

	return UPDATENONE;
}

HRESULT CGameObject_2D::Render()
{
	FAILED_CHECK(__super::Render());


	FAILED_CHECK(Set_ConstantTable_UI());
	FAILED_CHECK(Set_ConstantTable_Tex());

	mComVIBuffer->Render(mComShader, mCurrentShaderPass);

	return S_OK;
}

HRESULT CGameObject_2D::CollisionFunc(CGameObject_Base * object)
{
	if (meUIType == CGameObject_2D::UITYPE_BUTTON1)
	{
		mIsOverrMouse = true;
		if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)&DIS_Down)
		{
			CallFuntion();
		}
	}

	else if (meUIType == CGameObject_2D::UITYPE_BUTTON2_UNIT)
	{
		if (mIsActive == false)
		{

		}
		else
		{
			mIsOverrMouse = true;

			if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)&DIS_Down)
			{
				CallFuntion();
			}
		}
	}



	return S_OK;
}

void CGameObject_2D::Set_LoadTexDesc(const TEXTURE_NAMES_DESC & desc)
{
	memcpy(&mTexStrDESC, &desc, sizeof(TEXTURE_NAMES_DESC));
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);
}

void CGameObject_2D::Set_LoadTexDiffuse(const char* str)
{
	strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str);
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);
}

void CGameObject_2D::Set_LoadTexButton(const char* str1, const char* str2)
{
	strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str1);
	strcpy_s(mTexStrDESC.mTextureKey_01, str2);
	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);
}

void CGameObject_2D::Set_LoadTexButton2(const char * str1, const char * str2, const char * str3)
{
	strcpy_s(mTexStrDESC.mTextureKey_Diffuse, str1);
	strcpy_s(mTexStrDESC.mTextureKey_01, str2);
	strcpy_s(mTexStrDESC.mTextureKey_02, str3);

	mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);

}


HRESULT CGameObject_2D::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComVIBuffer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_RECT), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));

	if (mComTexture == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComTexture));
	//if(mComCollider_NULL == nullptr)
	//	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_COLLIDER_AABB), TEXT("Com_Collider"), (CComponent**)&mComCollider_NULL));


	return S_OK;
}



HRESULT CGameObject_2D::Set_ConstantTable_UI()
{
	if (nullptr == mComVIBuffer)
		return E_FAIL;

	_float4x4 worldmat, projmat;
	mWorldmat.Transpose(worldmat);
	mProjmat.Transpose(projmat);

	mComShader->Set_RawValue(STR_MAT_WORLD, &worldmat, sizeof(_float4x4));
	mComShader->Set_RawValue(STR_MAT_VIEW, &mViewmat, sizeof(_float4x4));
	mComShader->Set_RawValue(STR_MAT_PROJ, &projmat, sizeof(_float4x4));


	return S_OK;
}

HRESULT CGameObject_2D::Setup_UIPosition(_float fX, _float fY, _float fSizeX, _float fSizeY,_float depth)
{
	mUiDesc.mUIRECT = _rect(fX,fY,fSizeX,fSizeY);
	mUiDesc.mDepth = depth;
	Setup_UIPosition(mUiDesc);
	return S_OK;
}

HRESULT CGameObject_2D::Setup_UIPosition(UI_DESC desc)
{
	mUiDesc = desc;
	Setup_UIPosition();
	return S_OK;
}

HRESULT CGameObject_2D::Setup_UIPosition()
{
	_uint		iNumViewports = 1;
	D3D11_VIEWPORT		_Viewport;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &_Viewport);
	_float2 Viewport = _float2(_Viewport.Width, _Viewport.Height);

	// 월드 세팅
	mWorldmat = mWorldmat.Identity;
	mWorldmat._11 = mUiDesc.mUIRECT.width;
	mWorldmat._22 = mUiDesc.mUIRECT.height;
	mWorldmat._33 = 1.f;

	// 윈도우 좌표로 위치함 
	memcpy(&mWorldmat.m[3][0], &_float4(mUiDesc.mUIRECT.x - (Viewport.x * 0.5f), -mUiDesc.mUIRECT.y + (Viewport.y * 0.5f),
		0.f, 1.f), sizeof(_float4));

	XMStoreFloat4x4(&mViewmat, XMMatrixIdentity());
	XMStoreFloat4x4(&mProjmat, XMMatrixOrthographicLH(Viewport.x, Viewport.y, 0.f, 1.f));
	return S_OK;
}

HRESULT CGameObject_2D::Setup_UIType(E_UI_TYPE e)
{
	meUIType = e;

	switch (meUIType)
	{
	case Client::CGameObject_2D::UITYPE_LOGO:
		Set_ViewportSize();
		break;
	case Client::CGameObject_2D::UITYPE_IMAGE:
		Setup_UIPosition();
		break;
	case Client::CGameObject_2D::UITYPE_BUTTON1:
		Setup_UIPosition();
		break;
	case Client::CGameObject_2D::UITYPE_BUTTON2_UNIT:
		Setup_UIPosition();
		break;
	case Client::CGameObject_2D::UITYPE_TEXT:
		break;
	case Client::CGameObject_2D::UITYPE_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CGameObject_2D::Set_ConstantTable_Tex()
{
	switch (meUIType)
	{
	case Client::CGameObject_2D::UITYPE_LOGO:
		FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
		break;
	case Client::CGameObject_2D::UITYPE_IMAGE:
		FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
		break;
	case Client::CGameObject_2D::UITYPE_BUTTON1:
		FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
		break;
	case Client::CGameObject_2D::UITYPE_BUTTON2_UNIT:
		FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
		break;
	case Client::CGameObject_2D::UITYPE_TEXT:
		return E_FAIL;
		break;
	default:
		return E_FAIL;
		break;

	}

	return S_OK;
}

void CGameObject_2D::Set_ViewportSize()
{
	// 화면사이즈로 초기화
	_uint		iNumViewports = 1;
	D3D11_VIEWPORT		Viewport;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);
	mUiDesc.mUIRECT = _rect(Viewport.Width*0.5f, Viewport.Height*0.5f,Viewport.Width, Viewport.Height);
	Setup_UIPosition();

}

void CGameObject_2D::LateTick_Button1()
{
	if (mIsOverrMouse)
	{
		mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_01);
	}
	else
	{
		mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);
	}
}

void CGameObject_2D::LateTick_Button2_Unit()
{
	if (mIsActive)
	{
		if (mIsOverrMouse)
		{
			mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_01);
		}
		else
		{
			mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_Diffuse);
		}
	}
	else
		mComTexture->Set_TextureMap(mTexStrDESC.mTextureKey_02);
	

}

CGameObject_2D * CGameObject_2D::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_2D*	pInstance = NEW CGameObject_2D(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_2D");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_2D* CGameObject_2D::Clone(void* pArg)
{
	CGameObject_2D*	pInstance = NEW CGameObject_2D(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_2D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_2D::Free()
{
	__super::Free();
	Safe_Release(mComVIBuffer);;
	Safe_Release(mComTexture);;
}