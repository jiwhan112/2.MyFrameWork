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
	// �� 3���� �ε��� �ʱ�ȭ
	memcpy(mPoints, pPoints, sizeof(_float3) * POINT_END);
	mIndex = iIndex;

	// �� ������ ����
	mLineDir[LINE_AB] = mPoints[POINT_B] - mPoints[POINT_A];
	mLineDir[LINE_BC] = mPoints[POINT_C] - mPoints[POINT_B];
	mLineDir[LINE_CA] = mPoints[POINT_A] - mPoints[POINT_C];

	mCenterPoint = Get_MeshCenter();

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	// ���� ���� ������ ���Ѵ�.
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
	// ������ġ�� �ش� �ε����� �ִ��� Ȯ���Ѵ�.
	// ������ ���� ���Ѵ�.
	for (_uint i = 0; i < LINE_END; ++i)
	{
		// ���� �ε����� �ִ��� 3���� ���� ��ġ�� �����ؼ� �Ǵ�.
		// ���� ��ġ�� �� ����Ʈ ���⺤�͸� ���Ѵ�.
		_float3		dest;
		(_float3(vPosition) - mPoints[i]).Normalize(dest);

		_float3 sour;
		// 2�������� ������ ���� ���ͱ��ϴ� �� (x,y) = (-y,x)
		// ������ ���⺤�͸� ���Ѵ�.
		_float3(mLineDir[i].z * -1.f, 0.0f, mLineDir[i].x).Normalize(sour);

		// ������ �븻���� ������ 0����ũ�� �ۿ� �ִ�.
		if (0 < dest.Dot(sour))
		{
			// �̿��ϴ� �޽ð� �ִٸ� �̵�
			if (0 <= mNeighborIndex[i])
				*pNeighborIndex = mNeighborIndex[i];

			// �ۿ� ����
			return false;
		}
	}
	// �ȿ�����
	return true;
}

CCell* CCell::isIn_Cell(_fvector vPosition)
{
	// ������ġ�� �ش� �ε����� �ִ��� Ȯ���Ѵ�.
		// ������ ���� ���Ѵ�.
	for (_uint i = 0; i < LINE_END; ++i)
	{
		// ���� �ε����� �ִ��� 3���� ���� ��ġ�� �����ؼ� �Ǵ�.
		// ���� ��ġ�� �� ����Ʈ ���⺤�͸� ���Ѵ�.
		_float3		dest;
		(_float3(vPosition) - mPoints[i]).Normalize(dest);

		_float3 sour;
		// 2�������� ������ ���� ���ͱ��ϴ� �� (x,y) = (-y,x)
		// ������ ���⺤�͸� ���Ѵ�.
		_float3(mLineDir[i].z * -1.f, 0.0f, mLineDir[i].x).Normalize(sour);

		// ������ �븻���� ������ 0����ũ�� �ۿ� �ִ�.
		if (0 < dest.Dot(sour))
		{
			return nullptr;
		}
	}
	// �ȿ�����
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