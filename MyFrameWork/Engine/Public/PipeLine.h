#pragma once

#include "Base.h"


// 여러 행렬을 

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:
	enum TRANSFORMSTATETYPE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

public:
	CPipeLine() = default;
	virtual ~CPipeLine() = default;

public:

	HRESULT Set_Transform(TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix)
	{
		XMStoreFloat4x4(&mMatrix[eStateType], TransformMatrix);		
	}

	_fmatrix Get_TransformMatrix(TRANSFORMSTATETYPE eStateType)
	{
		XMLoadFloat4x4(&mMatrix[eStateType]);
	}

	_float4x4 Get_TransformFloat4x4(TRANSFORMSTATETYPE eStateType)
	{
		return mMatrix[eStateType];
	}

	// 전치 행렬
	_float4x4 Get_TransformFloat4x4_TP(TRANSFORMSTATETYPE eStateType)
	{
		return mMatrix[eStateType];
	}

public:
//	void Update();

private:
	_float4x4				mMatrix[D3DTS_END];

	_float3					m_vCamPosition;

public:
	virtual void Free() override;
};

END