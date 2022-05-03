#include "..\Public\Cell.h"
#include "Shader.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CCell::NativeConstruct(const _float3 * pPoints, _uint iIndex)
{
	// 점 3개와 인덱스 초기화 
	memcpy(mPoints, pPoints, sizeof(_float3) * POINT_END);
	mIndex = iIndex;

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	// 점에 따라 선분을 비교한다.
	if (mPoints[POINT_A] == vSourPoint)
	{
		if (mPoints[POINT_B] == vDestPoint)
			return true;
		if (mPoints[POINT_C] == vDestPoint)
			return true;	
	}

	if (mPoints[POINT_B] == vSourPoint)
	{
		if (mPoints[POINT_A] == vDestPoint)
			return true;
		if (mPoints[POINT_C] == vDestPoint)
			return true;
	}

	if (mPoints[POINT_C] == vSourPoint)
	{
		if (mPoints[POINT_A] == vDestPoint)
			return true;
		if (mPoints[POINT_B] == vDestPoint)
			return true;
	}

	return false;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{



	return S_OK;
}
#endif // _DEBUG

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _float3 * pPoints, _uint iIndex)
{
	CCell*	pInstance = new CCell(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
	{
		MSGBOX("Failed to Creating CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
