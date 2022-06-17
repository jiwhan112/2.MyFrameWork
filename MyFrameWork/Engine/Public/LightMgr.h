#pragma once

#include "Base.h"

// 라이트들을 관리하는 라이트 매니저

BEGIN(Engine)

class CLightMgr final : public CBase
{
	DECLARE_SINGLETON(CLightMgr)
public:
	CLightMgr() = default;
	virtual ~CLightMgr() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Add_Light(ID3D11Device* device, ID3D11DeviceContext* context, const LIGHTDESC& desc);
	HRESULT Render();

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex) const;


private:
	list<class CLight*>		mLights;

private:
	class CVIBuffer_Rect*		mComVIRect = nullptr;
	class CShader*				mComShader= nullptr;
	ID3D11ShaderResourceView*	m_pNormalSRV = nullptr;

private:
	_float4x4					m_WorldMatrix;
	_float4x4					m_ViewMatrix;
	_float4x4					m_ProjMatrix;

public:
	virtual void Free() override;



};


END