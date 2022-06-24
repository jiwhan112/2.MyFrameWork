#pragma once

#include "Base.h"

BEGIN(Engine)

// 절두체로 그려질 오브젝트 판단.
// 절두체 충돌로 판단한다.

class CFrustum final :
	public CBase
{
	DECLARE_SINGLETON(CFrustum)

private:
	explicit CFrustum();
	virtual ~CFrustum() = default;


public:
	HRESULT Initialize();
	void Tick();

	// 같은 로컬로 이동하기
	HRESULT Transform_ToLocalSpace(class CTransform* pTransform);

public:
	_bool IsIn_WorldSpace(_fvector vPoint, _float fRange=0);
	_bool IsIn_LocalSpace(_fvector vPoint, _float fRange=0);

private:
	// 점 8개와 각 공간에서 처리 변수 선언
	_float3				m_vPoint[8];
	_float3				m_vPointInWorld[8];
	_float3				m_vPointInLocal[8];

	// 평면 
	_float4				m_PlaneInWorld[6];
	_float4				m_PlaneInLocal[6];

private:

	// 평면을 만들어 준다.
	void Compute_Plane(const _float3* pPoints, _float4* pPlanes);


public:
	virtual void Free() override;
};

END