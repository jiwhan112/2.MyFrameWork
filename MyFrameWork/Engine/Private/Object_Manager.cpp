#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);
}

const list<class CGameObject*>* CObject_Manager::Get_GameObjectLayerList(_uint iLevelIndex, const _tchar* pLayerTag)
{
	const CLayer* layer = Find_Layer(iLevelIndex, pLayerTag);
	if (layer != nullptr)
		return layer->Get_GameObjectList();
	
	return nullptr;
}

const map<wstring, class CLayer*>* CObject_Manager::Get_All_GameObjectLayerMap(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	return &m_pLayers[iLevelIndex];
}

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = NEW LAYERS[m_iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype ||
		nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));

	return S_OK;
}

CGameObject* CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	NULL_CHECK_BREAK(pPrototype);

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	NULL_CHECK_BREAK(pGameObject);

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return nullptr;

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject),nullptr);

		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
	{
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), nullptr);

	}
	return pGameObject;
}

HRESULT  CObject_Manager::Push_GameObject(_uint iLevelIndex, const _tchar * pLayerTag,CGameObject* cloneObj)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	if (cloneObj == nullptr)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(cloneObj)))
			return E_FAIL;

		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(cloneObj)))
			return E_FAIL;
	}

	return S_OK;
}

CGameObject * CObject_Manager::Create_GameObject(const _tchar * pPrototypeTag, void * pArg)
{
	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;
	return pGameObject;
}

HRESULT CObject_Manager::Clear_LevelObject(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();

	return S_OK;
}

_int CObject_Manager::Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (0 > Pair.second->Tick(TimeDelta))
				return -1;
		}
	}

	return _int();
}

_int CObject_Manager::LateTick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (0 > Pair.second->LateTick(TimeDelta))
				return -1;
		}
	}

	return _int();
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
//	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));

	auto	iter = m_Prototypes.find(pPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{

//	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	auto	iter = m_pLayers[iLevelIndex].find(pLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}