#include "..\Public\Renderer.h"

#include "GameObject.h"

#include "RenderTargetMgr.h"
#include "LightMgr.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, mRenderTargetManager(CRenderTargetMgr::GetInstance())
	, mLightManager(CLightMgr::GetInstance())

{
	Safe_AddRef(mRenderTargetManager);
	Safe_AddRef(mLightManager);
}

HRESULT CRenderer::NativeConstruct_Prototype()
{

	// 랜더 타겟 초기화
	FAILED_CHECK(RenderTargetSetting());

	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (eRenderGroup >= RENDER_END ||
		nullptr == pRenderObject)
		return E_FAIL;

	mRenderObjects[eRenderGroup].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Render()
{
	FAILED_CHECK(Render_Priority());
	FAILED_CHECK(Render_NonAlpha_First());
	FAILED_CHECK(Render_NonAlpha_Second());
	FAILED_CHECK(Render_Alpha());
	FAILED_CHECK(Render_UI());


#ifdef _DEBUG

//	FAILED_CHECK(Render_Debug());
	FAILED_CHECK(mRenderTargetManager->Render_DebugBuffer(TAGMRT(MRT_DEFERRED)));
//	FAILED_CHECK(mRenderTargetManager->Render_DebugBuffer(TEXT("MRT_LightAcc")));

#endif


	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pRenderObject : mRenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pRenderObject)
		{
			if (pRenderObject->Get_IsRenderer())
			{
				if (FAILED(pRenderObject->Render()))
					return E_FAIL;
			}
		}
		Safe_Release(pRenderObject);
	}
	mRenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha_First()
{
	for (auto& pRenderObject : mRenderObjects[RENDER_NONBLEND_FIRST])
	{
		if (nullptr != pRenderObject)
		{
			if (pRenderObject->Get_IsRenderer())
			{
				if (FAILED(pRenderObject->Render()))
					return E_FAIL;
			}
		}
		Safe_Release(pRenderObject);
	}
	mRenderObjects[RENDER_NONBLEND_FIRST].clear();

	return S_OK;
}
HRESULT CRenderer::Render_NonAlpha_Second()
{
	for (auto& pRenderObject : mRenderObjects[RENDER_NONBLEND_SECOND])
	{
		if (nullptr != pRenderObject)
		{
			if (pRenderObject->Get_IsRenderer())
			{
				if (FAILED(pRenderObject->Render()))
					return E_FAIL;
			}
		}
		Safe_Release(pRenderObject);
	}
	mRenderObjects[RENDER_NONBLEND_SECOND].clear();

	return S_OK;
}
HRESULT CRenderer::Render_Alpha()
{
	//mRenderObjects[RENDER_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	//{
	//	return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	//});
	
	for (auto& pRenderObject : mRenderObjects[RENDER_BLEND])
	{
		if (nullptr != pRenderObject)
		{
			if (pRenderObject->Get_IsRenderer())
			{
				if (FAILED(pRenderObject->Render()))
					return E_FAIL;
			}
		}
		Safe_Release(pRenderObject);
	}
	mRenderObjects[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	if (nullptr == m_pDevice ||
		nullptr == m_pDeviceContext)
		return E_FAIL;

	mRenderObjects[RENDER_UI].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_Depth() > pDest->Get_Depth();
	});

	for (auto& pRenderObject : mRenderObjects[RENDER_UI])
	{
		if (nullptr != pRenderObject)
		{
			if (pRenderObject->Get_IsRenderer())
			{
				if (FAILED(pRenderObject->Render()))
					return E_FAIL;
			}
		}
		Safe_Release(pRenderObject);
	}
	mRenderObjects[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::RenderTargetSetting()
{

	// 랜더 타겟들 세팅
	if (nullptr == mRenderTargetManager)
		return E_FAIL;

	_uint		iNumViewports = 1;

	D3D11_VIEWPORT		Viewport;

	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	/* For.Target_Diffuse */
	// 디퓨즈
	FAILED_CHECK(mRenderTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TAGTARGET(RENDERTARGET_DIFFUSE),
		(_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	// 노말
	FAILED_CHECK(mRenderTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TAGTARGET(RENDERTARGET_NOMAL),
		(_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));

	// 명암
	FAILED_CHECK(mRenderTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TAGTARGET(RENDERTARGET_SHADE),
		(_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));

	// 깊이
	FAILED_CHECK(mRenderTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TAGTARGET(RENDERTARGET_DEPTH),
		(_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));

	// SPEC
	FAILED_CHECK(mRenderTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TAGTARGET(RENDERTARGET_SPECULAR),
		(_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));


	
	/* For.MRT_Deferred : 객체들을 그릴때 바인드. */
	FAILED_CHECK(mRenderTargetManager->Add_MRT(TAGMRT(MRT_DEFERRED), TAGTARGET(RENDERTARGET_DIFFUSE)));
	FAILED_CHECK(mRenderTargetManager->Add_MRT(TAGMRT(MRT_DEFERRED), TAGTARGET(RENDERTARGET_NOMAL)));
	FAILED_CHECK(mRenderTargetManager->Add_MRT(TAGMRT(MRT_DEFERRED), TAGTARGET(RENDERTARGET_DEPTH)));

	/* For.MRT_LightAcc : 빛을 그릴때 바인드 */
	FAILED_CHECK(mRenderTargetManager->Add_MRT(TAGMRT(MRT_LIGHTACC), TAGTARGET(RENDERTARGET_SHADE)));
	FAILED_CHECK(mRenderTargetManager->Add_MRT(TAGMRT(MRT_LIGHTACC), TAGTARGET(RENDERTARGET_SPECULAR)));


	// Render
	// Rect 텍스처 생성
	mComVIRECT = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == mComVIRECT)
		return E_FAIL;

	mComShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == mComShader)
		return E_FAIL;



	mWorldMat = _float4x4::Identity;


	mWorldMat._11 = Viewport.Width;
	mWorldMat._22 = Viewport.Height;
	mWorldMat._33 = 1.f;
	memcpy(&mWorldMat.m[3][0], &_float4(Viewport.Width * 0.5f - (Viewport.Width * 0.5f), -Viewport.Height * 0.5f + (Viewport.Height * 0.5f), 0.f, 1.f), sizeof(_float4));

	XMStoreFloat4x4(&mWorldMat, XMMatrixTranspose(XMLoadFloat4x4(&mWorldMat)));
	XMStoreFloat4x4(&mViewdMat, XMMatrixIdentity());
	XMStoreFloat4x4(&mProjMat, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));
	
#ifdef _DEBUG
	FAILED_CHECK(mRenderTargetManager->Ready_DebugDesc(TAGTARGET(RENDERTARGET_DIFFUSE), 100, 100, 200, 200));
	FAILED_CHECK(mRenderTargetManager->Ready_DebugDesc(TAGTARGET(RENDERTARGET_NOMAL), 100, 300, 200, 200));
	FAILED_CHECK(mRenderTargetManager->Ready_DebugDesc(TAGTARGET(RENDERTARGET_DEPTH), 100, 500, 200, 200));
	FAILED_CHECK(mRenderTargetManager->Ready_DebugDesc(TAGTARGET(RENDERTARGET_SHADE), 300, 100, 200, 200));
	FAILED_CHECK(mRenderTargetManager->Ready_DebugDesc(TAGTARGET(RENDERTARGET_SPECULAR), 300, 300, 200, 200));

#endif

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CRenderer*	pInstance = NEW CRenderer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();
	Safe_Release(mComVIRECT);
	Safe_Release(mComShader);

	Safe_Release(mRenderTargetManager);
	Safe_Release(mLightManager);

	for (auto& RenderObjects : mRenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);
		RenderObjects.clear();
	}
}