#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

const _tchar* CGameObject::mComTag_Transform = TEXT("Com_Tranform");
CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

}


CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
	, mIsLife(rhs.mIsLife)
	, mIsRenderer(rhs.mIsRenderer)
	, mObjectTypeid(rhs.mObjectTypeid)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);


}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void * pArg)
{
	// Transform 컴포넌트는 자동 추가
	mComTransform = CTransform::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == mComTransform)
		return E_FAIL;

	if (nullptr != pArg)
		mComTransform->SetTransformDesc(*(CTransform::TRANSFORMDESC*)pArg);

	if (nullptr != Find_Component(mComTag_Transform))
		return E_FAIL;

	CGameInstance*		pGameInstance = GetSingle(CGameInstance);
	m_Components.emplace(mComTag_Transform, mComTransform);

	FAILED_CHECK(Set_Component());

	return S_OK;
}

_int CGameObject::Tick(_double TimeDelta)
{



	return UPDATENONE;
}

_int CGameObject::LateTick(_double TimeDelta)
{



	return UPDATENONE;
}

HRESULT CGameObject::Render()
{


	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}