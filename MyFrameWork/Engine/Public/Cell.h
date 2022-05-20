#pragma once

#include "Base.h"

BEGIN(Engine)

// �׺���̼� �޽��� �ﰢ�� ����� ��
class CCell final : public CBase
{
public:
	// �� 3���� �̿��ϴ� ������ ����.
	enum E_POINTS { POINT_A, POINT_B, POINT_C, POINT_END };
	enum E_LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum E_CELLTYPE { CELLTYPE_NONE, CELLTYPE_DEBUG, CELLTYPE_STOP, CELLTYPE_END };

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;

public:
	void Set_ParentIndex(int idx)
	{
		mParentIndex = idx;
	}
	_int Get_ParentIndex()
	{
		return mParentIndex;
	}
	
	_uint Get_Index()
	{
		return mIndex;
	}

	const _float3& Get_Point(E_POINTS ePoint) const
	{
		return mPoints[ePoint];
	}

	void Set_NeighborIndex(E_LINES eLine, _int iNeighborIndex)
	{
		mNeighborIndex[eLine] = iNeighborIndex;
	}
	void Set_TileType(E_CELLTYPE type)
	{
		meCellType = type;
	}

	const _int& Get_NeighborIndex(E_LINES eLine) const
	{
		return mNeighborIndex[eLine];
	}

	const E_CELLTYPE& Get_CellType() const
	{
		return meCellType;
	}
	const _int* Get_ArrayNeighborIndex() const
	{
		return mNeighborIndex;
	}
	const _float3& Get_CenterPoint() const
	{
		return mCenterPoint;
	}
	

public:
	HRESULT NativeConstruct(const _float3* pPoints, _uint iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	// ���� �ε���
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);
	CCell* isIn_Cell(_fvector vPosition);

	_float3 Get_MeshCenter();


private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private:
	// ���� �Ӽ�
	E_CELLTYPE		meCellType = CELLTYPE_NONE;

	// ���� �ε���
	_uint			mIndex = 0;

	// ���� �ε���
	_int			mParentIndex = -1;

	// �̿��ϴ� �ε���
	_int			mNeighborIndex[LINE_END] = { -1, -1, -1 };

	// ���� ��ġ
	_float3			mPoints[POINT_END];
	_float3			mCenterPoint;

	// ������ ��ġ
	_float3			mLineDir[LINE_END];
	

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END