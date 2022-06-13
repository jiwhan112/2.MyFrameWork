#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

// 인스턴스로 랜더링될 파티클 오브젝트 나중에 구현

class CGameObject_2D_Particle_Point:
	public CGameObject_Base
{
public:

	// 인스턴싱에 넘길 구조
	//typedef struct tagParticleDesc
	//{
	//	_float		fMinSpeed, fMaxSpeed;
	//	_float3		vMoveDir;
	//}PARTICLEDESC;

protected:
	explicit CGameObject_2D_Particle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_2D_Particle_Point(const CGameObject_2D_Particle_Point& rhs);
	virtual ~CGameObject_2D_Particle_Point() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Texture();


protected:
	// 인스턴싱 버퍼
	// 인스터싱 셰이더

	CVIBuffer_Point_Instance*		mComVIBuffer = nullptr;
	CTexture_map*					mComTexture = nullptr;

	TEXTURE_NAMES_DESC				mTexStrDESC;
//	PARTICLE_DESC					mParticleDESC;


public:
	static CGameObject_2D_Particle_Point* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D_Particle_Point* Clone(void* pArg);
	virtual void Free() override;
};

END