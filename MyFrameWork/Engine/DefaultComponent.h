#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);


public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CModel* Clone(void* pArg);
	virtual void Free() override;
};

END