#pragma once

#include "Base.h"

BEGIN(Engine)

// ����ü�� �׷��� ������Ʈ �Ǵ�.
// ����ü �浹�� �Ǵ��Ѵ�.

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

	// ���� ���÷� �̵��ϱ�
	HRESULT Transform_ToLocalSpace(class CTransform* pTransform);

public:
	_bool IsIn_WorldSpace(_fvector vPoint, _float fRange=0);
	_bool IsIn_LocalSpace(_fvector vPoint, _float fRange=0);

private:
	// �� 8���� �� �������� ó�� ���� ����
	_float3				m_vPoint[8];
	_float3				m_vPointInWorld[8];
	_float3				m_vPointInLocal[8];

	// ��� 
	_float4				m_PlaneInWorld[6];
	_float4				m_PlaneInLocal[6];

private:

	// ����� ����� �ش�.
	void Compute_Plane(const _float3* pPoints, _float4* pPlanes);


public:
	virtual void Free() override;
};

END