#include "stdafx.h"
#include "GameObject/ParticleManager.h"
#include "GameObject/GameObject_3D_Particle.h"
#include "GameObject/GameObject_2D_Particle_Point.h"
#include "GameObject/GameObject_2D_Particle_Buffer.h"

CParticleManager::CParticleManager(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mpDevice = pDevice;
	mpDeviceContext = pDeviceContext;

	Safe_AddRef(mpDevice);
	Safe_AddRef(mpDeviceContext);

	mMapParticle_3D.clear();
	mMapParticle_2D_Instance.clear();
	mMapParticle_2D_Buffer.clear();

}

HRESULT CParticleManager::NativeConstruct_Prototype()
{
	// 정보에 따라 파티클 생성 
	// Init_PartilceDesc():

	return S_OK;
}

HRESULT CParticleManager::Create_Partilce_3D(string tag, PARTICLECREATEDESC desc)
{

	return S_OK;
}

HRESULT CParticleManager::Create_Partilce_Rect2D(string tag, PARTICLECREATEDESC des)
{
	return S_OK;
}

HRESULT CParticleManager::Create_Partilce_Instance2D(string tag, PARTICLECREATEDESC des)
{
	return S_OK;
}

HRESULT CParticleManager::Create_Partilce_3D_Tool(PARTICLECREATEDESC createDesc, PARTICLEDESC particleDesc )
{
	for (int i=0;i<createDesc.Count;++i)
	{
		CGameObject_Base* particle = GetSingle(CGameManager)->Get_CreaterManager()->CreateEmptyObject(GAMEOBJECT_3D_PARTICLE);
	//	static_cast<CGameObject_3D_Particle*>(particle)->Set_Desc(particleDesc);
	//	CGameObject_Base* particle = GetSingle(CGameManager)->Get_CreaterManager()->CreateEmptyObject(GAMEOBJECT_3D_PARTICLE);

	}

	return S_OK;
}

HRESULT CParticleManager::Create_Partilce_Rect2D_Tool(PARTICLECREATEDESC createDesc, PARTICLEDESC particleDesc )
{
//	GetSingle(CGameManager)->Get_CreaterManager()->CreateEmptyObject(GAMEOBJECT_2D_PARTICLE_BUFFER);

	return S_OK;
}

HRESULT CParticleManager::Create_Partilce_Instance2D_Tool(PARTICLECREATEDESC createDesc, PARTICLEDESC particleDesc)
{
//	GetSingle(CGameManager)->Get_CreaterManager()->CreateEmptyObject(GAMEOBJECT_2D_PARTICLE_POINT);

	return S_OK;
}

HRESULT CParticleManager::Delete_Particle()
{
	GetSingle(CGameManager)->Set_DeadLayer(TAGLAY(LAY_EFFECT));
	return S_OK;
}

CGameObject_3D_Particle * CParticleManager::Find_3DPartilce(string tag)
{
	auto iter = mMapParticle_3D.find(tag);
	if (iter == mMapParticle_3D.end())
		return nullptr;
	return iter->second;
}

CGameObject_2D_Particle_Point * CParticleManager::Find_2DPartilce_Instance(string tag)
{

	auto iter = mMapParticle_2D_Instance.find(tag);
	if (iter == mMapParticle_2D_Instance.end())
		return nullptr;
	return iter->second;
}

CGameObject_2D_Particle_Buffer * CParticleManager::Find_2DPartilce_Buffer(string tag)
{
	auto iter = mMapParticle_2D_Buffer.find(tag);
	if (iter == mMapParticle_2D_Buffer.end())
		return nullptr;
	return iter->second;

}


CParticleManager * CParticleManager::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CParticleManager*	pInstance = NEW CParticleManager(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CParticleManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleManager::Free()
{
	Safe_Release(mpDevice);
	Safe_Release(mpDeviceContext);


	for (auto pair : mMapParticle_3D)
	{
		Safe_Release(pair.second);
	}
	for (auto pair : mMapParticle_2D_Instance)
	{
		Safe_Release(pair.second);
	}
	for (auto pair : mMapParticle_2D_Buffer)
	{
		Safe_Release(pair.second);
	}
	mMapParticle_3D.clear();
	mMapParticle_2D_Instance.clear();
	mMapParticle_2D_Buffer.clear();

}