#include "..\Public\Renderer.h"

#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
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
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha_First()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha_Second()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

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

	for (auto& RenderObjects : mRenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);
		RenderObjects.clear();
	}
}