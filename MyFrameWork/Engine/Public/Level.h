#pragma once

#include "Base.h"

/* Ŭ���̾�Ʈ���� ������ �������� �θ� �Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
public:
	explicit CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
	_float2 Get_ViewSize()const;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	_uint					mLevelIndex = -1;

public:
	virtual void Free() override;
};

END