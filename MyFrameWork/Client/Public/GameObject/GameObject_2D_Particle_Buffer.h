#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CVIBuffer_Rect;

END

BEGIN(Client)

// Rect 버퍼 사용  

class CGameObject_2D_Particle_Buffer:
	public CGameObject_Base
{
public:
	enum E_RECTPARTILCE_INDEX
	{
		RECTPARTILCE_INDEX_01,
		RECTPARTILCE_INDEX_02,
		RECTPARTILCE_INDEX_03,
		RECTPARTILCE_INDEX_04,
		RECTPARTILCE_INDEX_END,
	};

	// 인스턴싱에 넘길 구조
	//typedef struct tagParticleDesc
	//{
	//	_float		fMinSpeed, fMaxSpeed;
	//	_float3		vMoveDir;
	//}PARTICLEDESC;

protected:
	explicit CGameObject_2D_Particle_Buffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_2D_Particle_Buffer(const CGameObject_2D_Particle_Buffer& rhs);
	virtual ~CGameObject_2D_Particle_Buffer() = default;

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

	CVIBuffer_Rect*					mComVIBuffer = nullptr;
	CTexture_map*					mComTexture[E_RECTPARTILCE_INDEX::RECTPARTILCE_INDEX_END] = { nullptr ,nullptr ,nullptr ,nullptr };

	TEXTURE_NAMES_DESC				mTexStrDESC;
//	PARTICLE_DESC					mParticleDESC;

public:
	static CGameObject_2D_Particle_Buffer* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D_Particle_Buffer* Clone(void* pArg);
	virtual void Free() override;
};

END