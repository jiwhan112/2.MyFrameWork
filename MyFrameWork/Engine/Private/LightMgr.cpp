#include "..\Public\LightMgr.h"
#include "..\Public\Light.h"
#include "RenderTargetMgr.h"
#include "PipeLine.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

IMPLEMENT_SINGLETON(CLightMgr)

HRESULT CLightMgr::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	mComVIRect = CVIBuffer_Rect::Create(pDevice, pDeviceContext);
	if (nullptr == mComVIRect)
		return E_FAIL;

	mComShader = CShader::Create(pDevice, pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == mComShader)
		return E_FAIL;

	  
	// 랜더타겟 랜더링처럼 행렬 저장
	// 뷰포트 행렬 저장
	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		Viewport;

	pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = Viewport.Width;
	m_WorldMatrix._22 = Viewport.Height;
	m_WorldMatrix._33 = 1.f;
	memcpy(&m_WorldMatrix.m[3][0], &_float4(Viewport.Width * 0.5f - (Viewport.Width * 0.5f), -Viewport.Height * 0.5f + (Viewport.Height * 0.5f), 0.f, 1.f), sizeof(_float4));

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));
	return S_OK;
}

const LIGHTDESC* CLightMgr::Get_LightDesc(_uint iIndex) const
{
	if (iIndex >= mLights.size())
		return nullptr;

	auto	iter = mLights.begin();
	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->GetLightDesc();
}

HRESULT CLightMgr::Add_Light(ID3D11Device * device, ID3D11DeviceContext * context, const LIGHTDESC & desc)
{
	CLight* light = CLight::Create(device, context, desc);
	if (light == nullptr)
		return E_FAIL;

	mLights.push_back(light);
	return S_OK;
}

HRESULT CLightMgr::Render()
{
	// 빛 셰이더로 넘기기


	if (nullptr == mComShader ||
		nullptr == mComVIRect)
		return E_FAIL;

	CRenderTargetMgr*	 RenderTargetMgr = GetSingle(CRenderTargetMgr);

	// 노말과 Depth를 넘긴다.
	
	FAILED_CHECK(mComShader->Set_Texture("g_NormalTexture", RenderTargetMgr->Get_SRV(TEXT("Target_Normal"))));
	FAILED_CHECK(mComShader->Set_Texture("g_DepthTexture", RenderTargetMgr->Get_SRV(TEXT("Target_Depth"))));

	// 월드 뷰 투영 행렬을 넘긴다.
	FAILED_CHECK(mComShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	CPipeLine*		pPipeLine = GetSingle(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv;
	_float4			vCamPosition;

	// 각 역행렬과 카메라위치를 넘긴다.
	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ))));
	XMStoreFloat4(&vCamPosition, pPipeLine->GetCameraPosition_float());

	FAILED_CHECK(mComShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(mComShader->Set_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4)));

	for (auto& pLight : mLights)
	{
		// 랜더 타겟과 셰이더를 넘긴다.
		if (nullptr != pLight)
			pLight->Render(mComShader, mComVIRect);
	}

	return S_OK;
}

void CLightMgr::Free()
{
	Safe_Release(m_pNormalSRV);
	Safe_Release(mComShader);
	Safe_Release(mComVIRect);

	for (auto light : mLights)
	{
		Safe_Release(light);
	}
}