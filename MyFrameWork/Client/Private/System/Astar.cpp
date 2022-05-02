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

	// ���� ������ Ÿ�� �ε����� ���Ѵ�.
	miStartIndex = pterrain->Get_TileIndex(vStart);

	int	iGoalIndex = pterrain->Get_TileIndex(vGoal);


	if (0 > miStartIndex || 0 > iGoalIndex)
		return;

	if (miStartIndex == iGoalIndex)
		return;

	// ��ǥ ������ ������ ���̸� astar Ž�� ����
	if ((*vecTile)[iGoalIndex]->mTileMode == CGameObject_MyTerrain::TILEMODE_XXX || 
		(*vecTile)[iGoalIndex]->mTileMode == CGameObject_MyTerrain::TILEMODE_END)
		return;

	// ��� ã��
	//if (true == MakeRoute(miStartIndex, iGoalIndex))
	//	MakeBestList(iGoalIndex);

}

void CAstar::MakeBestList(int iGoalIndex)
{
	// �� ��������
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

		// �θ� Ÿ���� �������ϸ鼭 �տ������� bestlist ���� ä�� ������.
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
//	// ť�� �������� �����ϴ� �ʺ� �켱Ž���� Ư�� �� ���� ���� ���� ��带 ���� ����Ʈ�� ����
//
//	if (!mListOpen.empty())
//		mListOpen.pop_front();
//
//	mListClose.push_back(iStartIndex); // ���������� ���������� �ƴϱ� ������ �ٷ� �˻���󿡼� ����
//
//	for (auto& pTile : vecAdj[iStartIndex])
//	{
//		// ã�� ��ΰ� �� ������ �������� ���
//		if (iGoalIndex == pTile->iIndex)
//		{
//			pTile->iParentIndex = iStartIndex;
//			return true;
//		}
//
//		// ã�� ������ Ÿ���� close �Ǵ� open �ִ��� ����
//		// ���� ������ �� ����Ʈ�� ��� ���ԵǾ� ���� �ʾ� �̵��� �� �ִ� ���ɼ��� ���� ��� ���� ����
//		if (false == CheckClose(pTile->iIndex) &&
//			false == CheckOpen(pTile->iIndex))
//		{
//			pTile->iParentIndex = iStartIndex;
//			// ���� �ִ� �༮���� �Ǵ��߱� ������ open ��
//			m_OpenList.push_back(pTile->iIndex);
//		}
//	}
//
//	// �� �̻� �� �� �ִ� ���� ���ٴ� ��
//	if (m_OpenList.empty())
//		return false;
//
//		//	Cost		PCost							GCost
//		// �޸���ƽ = ���� ���� �������� ����(���� ��) + �� ���������� �Ÿ�
//		// ��������
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
//	// ���� �켱Ž��ó�� ���������� ã�� ������ ��� Ž��
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
//	// 12�� ������ �������� ������ ��ġ �� ����
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
//	// �� ���� ������ ���� ���͸� �������� �Ѵ�.
//	// �������ʹ� �� ���Ϳ� ������ �̷�� ���� ���ͷ� ũ��� �ݵ�� 1�̾�� �Ѵ�.
//
//	D3DXVECTOR3	vNormal[4] = {
//
//		D3DXVECTOR3(-vDir[0].y, vDir[0].x, 0.f),
//		D3DXVECTOR3(-vDir[1].y, vDir[1].x, 0.f),
//		D3DXVECTOR3(-vDir[2].y, vDir[2].x, 0.f),
//		D3DXVECTOR3(-vDir[3].y, vDir[3].x, 0.f)
//	};
//
//	// Ÿ���� �� ������ ���콺 ��ġ�� ���ϴ� ���� ���͸� ���Ѵ�.
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
//		// ���Ⱑ ����� ���(����)
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
