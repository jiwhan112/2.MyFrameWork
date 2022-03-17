#include "..\Public\Renderer.h"

#include "GameObject.h"


CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
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

	m_RenderObjects[eRenderGroup].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Render()
{

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pRenderObject)
		{
			if (FAILED(pRenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	for (auto& pRenderObject : m_RenderObjects[RENDER_NONALPHA])
	{
		if (nullptr != pRenderObject)
		{
			if (FAILED(pRenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RENDER_NONALPHA].clear();

	return S_OK;
}



HRESULT CRenderer::Render_Alpha()
{
	m_RenderObjects[RENDER_ALPHA].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	for (auto& pRenderObject : m_RenderObjects[RENDER_ALPHA])
	{
		if (nullptr != pRenderObject)
		{
			if (FAILED(pRenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RENDER_ALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& pRenderObject : m_RenderObjects[RENDER_UI])
	{
		if (nullptr != pRenderObject)
		{
			if (FAILED(pRenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RENDER_UI].clear();

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

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

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);
		RenderObjects.clear();
	}
}
