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

	// 각 선분의 벡터
	mLineDir[LINE_AB] = mPoints[POINT_B] - mPoints[POINT_A];
	mLineDir[LINE_BC] = mPoints[POINT_C] - mPoints[POINT_B];
	mLineDir[LINE_CA] = mPoints[POINT_A] - mPoints[POINT_C];

	mCenterPoint = Get_MeshCenter();

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

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	// 현재위치가 해당 인덱스에 있는지 확인한다.
	// 라인을 전부 비교한다.
	for (_uint i = 0; i < LINE_END; ++i)
	{
		// 현재 인덱스에 있는지 3변과 현재 위치를 내적해서 판단.
		// 현재 위치와 각 포인트 방향벡터를 구한다.
		_float3		dest;
		(_float3(vPosition) - mPoints[i]).Normalize(dest);

		_float3 sour;
		// 2차원에서 수직인 벡터 벡터구하는 식 (x,y) = (-y,x)
		// 수직인 방향벡터를 구한다.
		_float3(mLineDir[i].z * -1.f, 0.0f, mLineDir[i].x).Normalize(sour);

		// 서로의 노말벡터 내적이 0보다크면 밖에 있다.
		if (0 < dest.Dot(sour))
		{
			// 이웃하는 메시가 있다면 이동
			if (0 <= mNeighborIndex[i])
				*pNeighborIndex = mNeighborIndex[i];

			// 밖에 있음
			return false;
		}
	}
	// 안에있음
	return true;
}

CCell* CCell::isIn_Cell(_fvector vPosition)
{
	// 현재위치가 해당 인덱스에 있는지 확인한다.
		// 라인을 전부 비교한다.
	for (_uint i = 0; i < LINE_END; ++i)
	{
		// 현재 인덱스에 있는지 3변과 현재 위치를 내적해서 판단.
		// 현재 위치와 각 포인트 방향벡터를 구한다.
		_float3		dest;
		(_float3(vPosition) - mPoints[i]).Normalize(dest);

		_float3 sour;
		// 2차원에서 수직인 벡터 벡터구하는 식 (x,y) = (-y,x)
		// 수직인 방향벡터를 구한다.
		_float3(mLineDir[i].z * -1.f, 0.0f, mLineDir[i].x).Normalize(sour);

		// 서로의 노말벡터 내적이 0보다크면 밖에 있다.
		if (0 < dest.Dot(sour))
		{
			return nullptr;
		}
	}
	// 안에있음
	return this;
}

_float3 CCell::Get_MeshCenter()
{
	_float3 centerPoint = mPoints[0] + mPoints[1] + mPoints[2];
	centerPoint /= 3;

	return centerPoint;
}

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _float3 * pPoints, _uint iIndex)
{
	CCell*	pInstance = NEW CCell(pDevice, pDeviceContext);

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