#pragma once

#include "Base.h"

// 여러 행렬을 저장하고 싱글턴에서

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:
	enum E_TRANSFORMSTATETYPE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_PROJ_ORI, D3DTS_END };

public:
	CPipeLine() = default;
	virtual ~CPipeLine() = default;

public:
	HRESULT Tick();

public:
	// 행렬 바인딩
	HRESULT SetTransform(E_TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix);

	// 연산용 반환
	_matrix GetTransformMatrix(E_TRANSFORMSTATETYPE eStateType);

	// 대입용 반환
	_float4x4 GetTransformFloat4x4(E_TRANSFORMSTATETYPE eStateType);

	// 전치 행렬 반환
	_float4x4 GetTransformFloat4x4_TP(E_TRANSFORMSTATETYPE eStateType);

	_float4 GetCameraPosition_float() const { return mCameraPostion; }
	_vector GetCameraPosition_vec() const { return XMLoadFloat4(&mCameraPostion); }

private:
	_float4x4				mMatrix[D3DTS_END];

	// 카메라 구하는 작업을 여러번 하지 말자.
	_float4					mCameraPostion;

public:
	virtual void Free() override;
};
END