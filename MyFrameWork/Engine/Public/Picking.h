#pragma once
#include "Base.h"

BEGIN(Engine)
class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	explicit CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context, HWND hWnd, void * pArg=nullptr);

	 HRESULT Transform_ToWorldSpace();
	// HRESULT Transform_ToWorldSpace(POINT mousePos);
	 HRESULT Transform_ToLocalSpace(_fmatrix WorldMatrixinverse);
	 _bool isPick(_float3 * pLocalPoint, _float3 * pOut);
//	 _bool	is_On_Rect(const _rect* TargetRect);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	HWND					m_hWnd;

public:
	_ray					mRayWorld;
	_ray					mRayLocal;

	virtual void Free() override;

};

END