#pragma once

#include "Base.h"

BEGIN(Engine)
END

BEGIN(Client)

// 파티클 매니저
class CGameObject_3D_Particle;
class CGameObject_2D_Particle_Point;
class CGameObject_2D_Particle_Buffer;

class CParticleManager:
	public CBase
{
public:


protected:
	explicit CParticleManager(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CParticleManager(const CParticleManager& rhs) = default;
	virtual ~CParticleManager() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();

	//virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	//virtual HRESULT Render();

public:
	// 파티클 생성
	HRESULT Create_Partilce_3D(string tag, PARTICLECREATEDESC desc);
	HRESULT Create_Partilce_Rect2D(string tag, PARTICLECREATEDESC des);
	HRESULT Create_Partilce_Instance2D(string tag, PARTICLECREATEDESC des);

	// 툴용 생성
	HRESULT Create_Partilce_3D_Tool(PARTICLECREATEDESC createDesc,PARTICLEDESC particleDesc);
	HRESULT Create_Partilce_Rect2D_Tool(PARTICLECREATEDESC createDesc, PARTICLEDESC particleDesc);
	HRESULT Create_Partilce_Instance2D_Tool(PARTICLECREATEDESC createDesc, PARTICLEDESC particleDesc);
	HRESULT Delete_Particle();

protected:
	CGameObject_3D_Particle*		Find_3DPartilce(string tag);
	CGameObject_2D_Particle_Point*	Find_2DPartilce_Instance(string tag);
	CGameObject_2D_Particle_Buffer*	Find_2DPartilce_Buffer(string tag);



private:
	ID3D11Device* mpDevice = nullptr;
	ID3D11DeviceContext* mpDeviceContext = nullptr;
	   
	map<string, CGameObject_3D_Particle*>			mMapParticle_3D;
	map<string, CGameObject_2D_Particle_Point*>		mMapParticle_2D_Instance;
	map<string, CGameObject_2D_Particle_Buffer*>	mMapParticle_2D_Buffer;

public:
	static CParticleManager* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual void Free() override;
};

END