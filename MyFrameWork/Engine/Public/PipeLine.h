#pragma once

#include "Base.h"

// ���� ����� �����ϰ� �̱��Ͽ���

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
	// ��� ���ε�
	HRESULT SetTransform(E_TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix);

	// ����� ��ȯ
	_matrix GetTransformMatrix(E_TRANSFORMSTATETYPE eStateType);

	// ���Կ� ��ȯ
	_float4x4 GetTransformFloat4x4(E_TRANSFORMSTATETYPE eStateType);

	// ��ġ ��� ��ȯ
	_float4x4 GetTransformFloat4x4_TP(E_TRANSFORMSTATETYPE eStateType);

	_float4 GetCameraPosition_float() const { return mCameraPostion; }
	_vector GetCameraPosition_vec() const { return XMLoadFloat4(&mCameraPostion); }

private:
	_float4x4				mMatrix[D3DTS_END];

	// ī�޶� ���ϴ� �۾��� ������ ���� ����.
	_float4					mCameraPostion;

public:
	virtual void Free() override;
};
END