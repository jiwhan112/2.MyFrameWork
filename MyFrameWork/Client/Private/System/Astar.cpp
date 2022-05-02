#include "stdafx.h"
#include "System/Astar.h"

//CAstar::CAstar(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
//{
//}

CAstar::CAstar()
{
}

CAstar::CAstar(const CAstar & rhs)
{
}

HRESULT CAstar::NativeConstruct_Prototype()
{
	return S_OK;
}

CAstar * CAstar::Create()
{
	CAstar*	pInstance = NEW CAstar();

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_MyTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAstar::Free()
{
	if (mListBest.empty() == false)
	{
		for (auto tile : mListBest)
		{
			Safe_Delete(tile);
		}

		mListBest.clear();
	}
}


void CAstar::Start_AStar(const _float3& vStart, const _float3& vGoal)
{
	mListOpen.clear();
	mListClose.clear();
	mListBest.clear();

	CGameObject_MyTerrain* pterrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
	if (pterrain == nullptr)
		return;

	auto vecTile = pterrain->Get_VecTile();
	if (vecTile == nullptr)
		return;

	// 시작 지점의 타일 인덱스를 구한다.
	miStartIndex = pterrain->Get_TileIndex(vStart);

	int	iGoalIndex = pterrain->Get_TileIndex(vGoal);


	if (0 > miStartIndex || 0 > iGoalIndex)
		return;

	if (miStartIndex == iGoalIndex)
		return;

	// 목표 지점이 못가는 곳이면 astar 탐색 중지
	if ((*vecTile)[iGoalIndex]->mTileMode == CGameObject_MyTerrain::TILEMODE_XXX || 
		(*vecTile)[iGoalIndex]->mTileMode == CGameObject_MyTerrain::TILEMODE_END)
		return;

	// 경로 찾기
	//if (true == MakeRoute(miStartIndex, iGoalIndex))
	//	MakeBestList(iGoalIndex);

}

void CAstar::MakeBestList(int iGoalIndex)
{
	// 역 추적진행
	CGameObject_MyTerrain* pterrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
	if (pterrain == nullptr)
		return;

	auto vecTile = pterrain->Get_VecTile();
	if (vecTile == nullptr)
		return;

	mListBest.push_front((*vecTile)[iGoalIndex]);

	int	iRouteIndex = (*vecTile)[iGoalIndex]->mTilePre;

	while (true)
	{
		if (iRouteIndex == miStartIndex)
			break;

		// 부모 타일을 역추적하면서 앞에서부터 bestlist 값을 채워 나간다.
		mListBest.push_front((*vecTile)[iRouteIndex]);
		iRouteIndex = (*vecTile)[iRouteIndex]->mTilePre;
	}
}


//bool CAstar::MakeRoute(int iStartIndex, int iGoalIndex)
//{
//	// 
//
//	CGameObject_MyTerrain* pterrain = (CGameObject_MyTerrain*)GetSingle(CGameManager)->Get_LevelObject_LayerTag(TAGLAY(LAY_TERRAIN));
//	if (pterrain == nullptr)
//		return;
//
//	auto vecTile = pterrain->Get_VecTile();
//	if (vecTile == nullptr)
//		return;
//
////	vector<list<TILE*>>&	vecAdj   = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecAdj();
//	// 큐의 형식으로 동작하는 너비 우선탐색의 특성 상 가장 먼저 들어온 노드를 오픈 리스트에 제거
//
//	if (!mListOpen.empty())
//		mListOpen.pop_front();
//
//	mListClose.push_back(iStartIndex); // 시작지점은 도착지점이 아니기 때문에 바로 검색대상에서 제거
//
//	for (auto& pTile : vecAdj[iStartIndex])
//	{
//		// 찾은 경로가 골 지점에 도달했을 경우
//		if (iGoalIndex == pTile->iIndex)
//		{
//			pTile->iParentIndex = iStartIndex;
//			return true;
//		}
//
//		// 찾은 인접한 타일이 close 또는 open 있는지 조사
//		// 현재 조건은 두 리스트에 모두 포함되어 있지 않아 이동할 수 있는 가능성을 가진 노드 임을 조사
//		if (false == CheckClose(pTile->iIndex) &&
//			false == CheckOpen(pTile->iIndex))
//		{
//			pTile->iParentIndex = iStartIndex;
//			// 갈수 있는 녀석으로 판단했기 때문에 open 편성
//			m_OpenList.push_back(pTile->iIndex);
//		}
//	}
//
//	// 더 이상 갈 수 있는 길이 없다는 것
//	if (m_OpenList.empty())
//		return false;
//
//		//	Cost		PCost							GCost
//		// 휴리스틱 = 최초 시작 시점과의 길이(깊이 값) + 골 지점까지의 거리
//		// 오름차순
//	int	iOriginStart = m_iStartIndex;
//
//	m_OpenList.sort([&](int iDest, int iSour)
//	{
//		D3DXVECTOR3 vPCost1 = vecTile[iDest]->vPos - vecTile[m_iStartIndex]->vPos;
//		D3DXVECTOR3 vGCost1 = vecTile[iDest]->vPos - vecTile[iGoalIndex]->vPos;
//		float fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
//
//		D3DXVECTOR3 vPCost2 = vecTile[iSour]->vPos - vecTile[m_iStartIndex]->vPos;
//		D3DXVECTOR3 vGCost2 = vecTile[iSour]->vPos - vecTile[iGoalIndex]->vPos;
//		float fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);
//
//		return fCost1 < fCost2;
//	});	
//
//	// 깊이 우선탐색처럼 도착지점을 찾을 때까지 계속 탐색
//	return MakeRoute(m_OpenList.front(), iGoalIndex);
//}

//
//int CAStar::GetTileIndex(const D3DXVECTOR3& vPos)
//{
//	CObj* pTerrain = CObjMgr::GetInstance()->Get_Terrain();
//	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();
//
//	if (vecTile.empty())
//		return -1;
//
//	for (size_t iIndex = 0; iIndex < vecTile.size(); ++iIndex)
//	{
//		if (Picking(vPos, iIndex))
//		{
//			return iIndex;
//		}
//	}
//
//	return -1;
//}
//
//bool CAStar::Picking(const D3DXVECTOR3& vPos, int iIndex)
//{
//	CObj* pTerrain = CObjMgr::GetInstance()->Get_Terrain();
//	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();
//
//	if (vecTile.empty())
//		return false;
//
//	// 12시 방향을 기준으로 점들의 위치 값 저장
//	D3DXVECTOR3		vPoint[4] =
//	{
//		D3DXVECTOR3(vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f),
//		D3DXVECTOR3(vecTile[iIndex]->vPos.x + (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f),
//		D3DXVECTOR3(vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f),
//		D3DXVECTOR3(vecTile[iIndex]->vPos.x - (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f),
//	};
//
//	D3DXVECTOR3		vDir[4] = {
//
//		vPoint[1] - vPoint[0],
//		vPoint[2] - vPoint[1],
//		vPoint[3] - vPoint[2],
//		vPoint[0] - vPoint[3]
//	};
//
//	// 각 방향 벡터의 법선 벡터를 만들어줘야 한다.
//	// 법선벡터는 각 벡터와 직각을 이루는 방향 벡터로 크기는 반드시 1이어야 한다.
//
//	D3DXVECTOR3	vNormal[4] = {
//
//		D3DXVECTOR3(-vDir[0].y, vDir[0].x, 0.f),
//		D3DXVECTOR3(-vDir[1].y, vDir[1].x, 0.f),
//		D3DXVECTOR3(-vDir[2].y, vDir[2].x, 0.f),
//		D3DXVECTOR3(-vDir[3].y, vDir[3].x, 0.f)
//	};
//
//	// 타일의 네 점에서 마우스 위치를 향하는 방향 벡터를 구한다.
//
//	D3DXVECTOR3	vMouseDir[4] = {
//
//		vPos - vPoint[0],
//		vPos - vPoint[1],
//		vPos - vPoint[2],
//		vPos - vPoint[3]
//	};
//	for (int i = 0; i < 4; ++i)
//	{
//		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
//		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
//	}
//
//	for (int i = 0; i < 4; ++i)
//	{
//		// 기울기가 양수인 경우(예각)
//		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
//			return false;
//	}
//
//	return true;
//}
//
//bool CAStar::CheckClose(int iIndex)
//{
//	for (int& iCloseIndex : m_CloseList)
//	{
//		if (iIndex == iCloseIndex)
//			return true;
//	}
//
//	return false;
//}
//
//
//bool CAStar::CheckOpen(int iIndex)
//{
//	for (int& iOpenIndex : m_OpenList)
//	{
//		if (iIndex == iOpenIndex)
//			return true;
//	}
//
//	return false;
//}
//
