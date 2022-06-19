#include "..\Public\Level.h"

CLevel::CLevel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CLevel::NativeConstruct()
{
	return S_OK;
}

_int CLevel::Tick(_double TimeDelta)
{
	return _int();
}

_int CLevel::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CLevel::Render()
{
	return S_OK;
}

_float2 CLevel::Get_ViewSize() const
{
	_uint		iNumViewports = 1;
	D3D11_VIEWPORT		Viewport;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);
	return _float2(Viewport.Width, Viewport.Height);
}

void CLevel::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}