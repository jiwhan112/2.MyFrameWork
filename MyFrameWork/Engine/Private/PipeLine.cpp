#include "..\Public\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

HRESULT CPipeLine::Tick()
{
	// ViewMat�� �ִٸ� ������� ���� CameraPosition�� �־����
	_matrix ViewMat = XMLoadFloat4x4(&mMatrix[D3DTS_VIEW]);
	ViewMat = XMMatrixInverse(nullptr, ViewMat);

	XMStoreFloat4(&mCameraPostion, ViewMat.r[3]);
	return S_OK;
}

HRESULT CPipeLine::SetTransform(E_TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&mMatrix[eStateType], TransformMatrix);
	return S_OK;
}

_matrix CPipeLine::GetTransformMatrix(E_TRANSFORMSTATETYPE eStateType)
{
	return XMLoadFloat4x4(&mMatrix[eStateType]);
}

_float4x4 CPipeLine::GetTransformFloat4x4(E_TRANSFORMSTATETYPE eStateType)
{
	return mMatrix[eStateType];
}

_float4x4 CPipeLine::GetTransformFloat4x4_TP(E_TRANSFORMSTATETYPE eStateType)
{
	// float->mat->��ġ->float�� ���� �� ��ȯ
	_float4x4 TransposeMat;
	XMStoreFloat4x4(&TransposeMat, XMMatrixTranspose(XMLoadFloat4x4(&mMatrix[eStateType])));
	return TransposeMat;
}

void CPipeLine::Free()
{
}