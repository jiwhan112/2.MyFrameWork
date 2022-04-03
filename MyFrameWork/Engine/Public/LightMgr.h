#pragma once

#include "Base.h"

// ����Ʈ���� �����ϴ� ����Ʈ �Ŵ���

BEGIN(Engine)

class CLightMgr final : public CBase
{
	DECLARE_SINGLETON(CLightMgr)
public:
	CLightMgr() = default;
	virtual ~CLightMgr() = default;

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex) const;
	HRESULT Add_Light(ID3D11Device* device, ID3D11DeviceContext* context, const LIGHTDESC& desc);

public:
	list<class CLight*>		mLights;
	typedef list<class CLight*>		LIGHTS;

public:
	virtual void Free() override;
};

END