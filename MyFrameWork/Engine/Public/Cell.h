#pragma once

#include "Base.h"

BEGIN(Engine)

// �׺���̼� �޽��� �ﰢ�� ����� ��
class CCell final : public CBase
{
public:
	// �� 3���� �̿��ϴ� ������ ����.
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;

public:
	_uint Get_Index() {
		return mIndex;
	}

	const _float3& Get_Point(POINTS ePoint) const {
		return mPoints[ePoint];
	}

	void Set_NeighborIndex(LINES eLine, _int iNeighborIndex) {
		mNeighborIndex[eLine] = iNeighborIndex;
	}

public:
	HRESULT NativeConstruct(const _float3* pPoints, _uint iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG
	

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private:
	// �̿��ϴ� �ε����� �� 3���� ���´�.
	// �̿��ϴ� �޽ô� �ִ� 3���̴�.
	_uint			mIndex = 0;
	_int			mNeighborIndex[LINE_END] = { -1, -1, -1 };
	_float3			mPoints[POINT_END];

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END