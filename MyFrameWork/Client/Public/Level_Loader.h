#pragma once

#include "Level.h"

BEGIN(Client)
class CLevel_Loader final:
	public CLevel
{
private:
	explicit CLevel_Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Loader() = default;

public:
	virtual HRESULT NativeConstruct(E_LEVEL eNextLevel);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	ID3D11Device*			mpDevice;
	ID3D11DeviceContext*	mpDeviceContext;

private:
	E_LEVEL					mNextLevel;
	class CLoader*			mpLoader;

public:
	static CLevel_Loader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, E_LEVEL nextLevel);
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};

END