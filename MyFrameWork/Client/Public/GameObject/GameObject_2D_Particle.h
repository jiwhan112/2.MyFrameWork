#pragma once

#include "GameObject_Base.h"

BEGIN(Client)

// 인스턴스로 랜더링될 파티클 오브젝트 나중에 구현

class CGameObject_2D_Particle :
	public CGameObject_Base
{
protected:
	explicit CGameObject_2D_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_2D_Particle(const CGameObject_2D_Particle& rhs);
	virtual ~CGameObject_2D_Particle() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:


protected:
	virtual HRESULT Set_Component()override;


protected:
	CVIBuffer*		mComVIBuffer = nullptr;
	CTexture_map*	mComTexture = nullptr;

	TEXTURE_NAMES_DESC		mTexStrDESC;
public:

	static CGameObject_2D_Particle* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D_Particle* Clone(void* pArg);
	virtual void Free() override;
};

END