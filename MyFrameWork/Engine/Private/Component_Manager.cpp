#include "..\Public\Component_Manager.h"
#include "Component.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Container(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	mMapProtos = NEW PROTOTYPES[m_iNumLevels];

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent * pPrototype)
{
	if (iLevelIndex >= m_iNumLevels ||
		nullptr == mMapProtos)
		return S_FALSE;

	if (nullptr != Find_Component(iLevelIndex, pPrototypeTag))
		return S_FALSE;

	mMapProtos[iLevelIndex].emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (iLevelIndex >= m_iNumLevels ||
		nullptr == mMapProtos)
		return nullptr;

	CComponent*		pPrototype = Find_Component(iLevelIndex, pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CComponent*		pComponent = pPrototype->Clone(pArg);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

HRESULT CComponent_Manager::Clear_LevelObject(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : mMapProtos[iLevelIndex])
		Safe_Release(Pair.second);

	mMapProtos[iLevelIndex].clear();

	return S_OK;
}

CComponent * CComponent_Manager::Find_Component(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	auto	iter = find_if(mMapProtos[iLevelIndex].begin(), mMapProtos[iLevelIndex].end(), CTagFinder(pPrototypeTag));

	if (iter == mMapProtos[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : mMapProtos[i])
			Safe_Release(Pair.second);
		mMapProtos[i].clear();
	}

	Safe_Delete_Array(mMapProtos);
}