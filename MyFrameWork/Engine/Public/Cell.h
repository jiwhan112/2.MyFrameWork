#pragma once

#include "Base.h"

BEGIN(Engine)

// 네비게이션 메시의 삼각형 모양의 셀
class CCell final : public CBase
{
public:
	// 점 3개와 이웃하는 선분을 정의.
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
	// 현재 인덱스 
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private:
	
	// 현재 인덱스
	_uint			mIndex = 0;
	
	// 이웃하는 인덱스
	_int			mNeighborIndex[LINE_END] = { -1, -1, -1 };
	
	// 점의 위치
	_float3			mPoints[POINT_END];
	// 라인의 위치
	_float3			mLineDir[LINE_END];

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END