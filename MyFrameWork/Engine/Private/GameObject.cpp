#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"
#include "Transform.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
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
	return S_OK;
}

_int CGameObject::Tick(_float fTimeDelta)
{
	return _int();
}

_int CGameObject::LateTick(_float fTimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;	
}

HRESULT CGameObject::Compute_CamDistance(CTransform * pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vCamPosition = *(_float3*)&ViewMatrix.m[3][0];

	_float3		vPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	_float3		vDir = vPosition - vCamPosition;

	m_fCamDistance = D3DXVec3Length(&vDir);

	return S_OK;
}

void CGameObject::Free()
{
	Safe_Release(m_pGraphic_Device);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}
