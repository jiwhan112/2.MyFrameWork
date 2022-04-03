#pragma once

#include "Base.h"

// 조명의 형태를 저장한다.

BEGIN(Engine)

class CLight final : public CBase
{
public:
	CLight(ID3D11Device* device, ID3D11DeviceContext* devicecontext);
	virtual ~CLight() = default;

	const LIGHTDESC* GetLightDesc() const { return &mLightDesc; }

public:
	HRESULT NativeConstruct(const LIGHTDESC& LightDesc);

private:
	ID3D11Device*				mpDevice = nullptr;
	ID3D11DeviceContext*		mpDeviceContext = nullptr;
	LIGHTDESC					mLightDesc;

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END