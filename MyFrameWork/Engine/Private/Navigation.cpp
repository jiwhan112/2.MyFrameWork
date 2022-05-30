#include "..\Public\Navigation.h"
#include "..\Public\Picking.h"
#include "..\Public\VIBuffer_Terrain.h"

#include "PipeLine.h"
#include "Transform.h"
#include "DebugDraw.h"
#include "GameInstance.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, mVecCells(rhs.mVecCells)
	, mCurrentIndex(rhs.mCurrentIndex)

#ifdef _DEBUG
	, mBaseEffect(rhs.mBaseEffect)
	, mBatch(rhs.mBatch)
	, mInputLayout(rhs.mInputLayout)
#endif // _DEBUG

{
#ifdef _DEBUG
	Safe_AddRef(mInputLayout);
#endif // _DEBUG

	for (auto& pCell : mVecCells)
		Safe_AddRef(pCell);
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar * pNaviDataFilePath)
{
	if (pNaviDataFilePath != nullptr)
		Load_NaviMeshData(pNaviDataFilePath);

#ifdef _DEBUG

	// 셰이더 정점 초기화
	mBaseEffect = NEW BasicEffect(m_pDevice);
	mBaseEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	mBaseEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &mInputLayout)))
		return E_FAIL;

	mBatch = NEW PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);

#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	mCurrentIndex = 0;
	return S_OK;
}

bool CNavigation::Pick(const _float4x4 & WorldMatrixInverse, _float3 * pOut)
{
	CPicking*		pPicking = GetSingle(CPicking);
	pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
	_bool		isPick = false;

	// 네비 메시 정점이랑 충돌
	for (auto cell : mVecCells)
	{
		_float3		vPoint[3] = {
			cell->Get_Point(CCell::POINT_A),
			cell->Get_Point(CCell::POINT_B),
			cell->Get_Point(CCell::POINT_C)
		};

		if (isPick = pPicking->isPick(vPoint, pOut))
		{
			_float4x4		WorldMatrix;
			WorldMatrix = WorldMatrixInverse.Invert();
			_float3::Transform(*pOut, WorldMatrix, *pOut);
			return isPick;
		}
	}

	return false;
}

_bool CNavigation::Get_PickPosForIndex(_float3 PickPos , _uint* outIndex )
{

	// 네비 메시 정점이랑 충돌
	for (auto cell : mVecCells)
	{

		if (nullptr != cell->isIn_Cell(PickPos))
		{
			*outIndex = cell->Get_Index();
			return true;
		}	
	}
	return false;
}


void CNavigation::Pick_ChangeCellOption(const _float4x4 & WorldMatrixInverse,CCell::E_CELLTYPE type)
{
	// 피킹시 해당 인덱스 반환
	CPicking*		pPicking = GetSingle(CPicking);
	pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
	_bool		isPick = false;

	// 네비 메시 정점이랑 충돌
	for (auto cell : mVecCells)
	{
		_float3		vPoint[3] = {
			cell->Get_Point(CCell::POINT_A),
			cell->Get_Point(CCell::POINT_B),
			cell->Get_Point(CCell::POINT_C)
		};

		_float3 fOut;
		if (isPick = pPicking->isPick(vPoint, &fOut))
		{		
			cell->Set_TileType(type);
			return;
		}
	}

	return;
}

HRESULT CNavigation::SetUp_NewNaviMesh(list<_float3*>& vpointlist)
{
	if (mVecCells.empty() == false)
	{
		Remove_NaviMeshData();
	}

	// 새로 초기화
	FAILED_CHECK(ReadyNaviMeshForListData(vpointlist));
	FAILED_CHECK(SetUp_Neighbor());

	return S_OK;
}

HRESULT CNavigation::SetUp_AutoMesh(CVIBuffer_Terrain * terrain)
{
	if (mVecCells.empty() == false)
	{
		Remove_NaviMeshData();
	}

	// 네비메시 데이터 자동 세팅
	CVIBuffer_Terrain* terrainbuffer = terrain;

	// 1. 정점을 가져온다.
	_float3* vertexes = terrainbuffer->Get_VerrtexPosition();
	_uint vertexSize = terrainbuffer->Get_VerrtexSize();
	_uint numX = terrainbuffer->Get_XZ()[0];
	_uint numZ = terrainbuffer->Get_XZ()[1];

	// 버텍스 그대로 네비에 넘김
	list<_float3*> NewFloat3ArrayList;
	_float3 vPoints[3];

	for (_uint z = 0; z < numZ - 1; z++)
	{
		for (_uint x = 0; x < numX - 1; x++)
		{
			_uint		iIndex = z * numX + x;

			_uint		iIndices[4] = {
				iIndex + numX,
				iIndex + numX + 1,
				iIndex + 1,
				iIndex
			};

			vPoints[0] = vertexes[iIndices[0]];
			vPoints[1] = vertexes[iIndices[1]];
			vPoints[2] = vertexes[iIndices[2]];

			_float3* newPoint1 = NEW _float3[3];
			newPoint1[0] = vPoints[0];
			newPoint1[1] = vPoints[1];
			newPoint1[2] = vPoints[2];
			NewFloat3ArrayList.push_back(newPoint1);

			vPoints[0] = vertexes[iIndices[0]];
			vPoints[1] = vertexes[iIndices[2]];
			vPoints[2] = vertexes[iIndices[3]];

			_float3* newPoint2 = NEW _float3[3];
			newPoint2[0] = vPoints[0];
			newPoint2[1] = vPoints[1];
			newPoint2[2] = vPoints[2];
			NewFloat3ArrayList.push_back(newPoint2);
		}
	}

	SetUp_NewNaviMesh(NewFloat3ArrayList);
	for (auto& p : NewFloat3ArrayList)
	{
		Safe_Delete_Array(p);
	}
	NewFloat3ArrayList.clear();

	FAILED_CHECK(SetUp_Neighbor());

	return S_OK;
}



_bool CNavigation::Move_OnNavigation(_fvector vPosition)
{
	_int		iNeighborIndex = -1;

	// 움직인 Position에 현재 셀을 비교해서 다음 연산을 수행한다.
	// 1. 인점한 인덱스 체크
	if (false == mVecCells[mCurrentIndex]->isIn(vPosition, &iNeighborIndex))
	{
		// 이웃이 있다면 그 지점으로 인덱스를 변경한다.
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				_int	iCurrentNeighborIndex = -1;

				if (true == mVecCells[iNeighborIndex]->isIn(vPosition, &iCurrentNeighborIndex))
				{
					mCurrentIndex = iNeighborIndex;
					break;
				}
				else
				{
					if (-1 == iCurrentNeighborIndex)
						return false;
				}
				iNeighborIndex = iCurrentNeighborIndex;
			}
			return true;
		}

		// 이웃이 없다면 이동을 막는다.
		else
		{
			// 슬라이딩 벡터로 구현
			// 반사 벡터를 구하는 것처럼 구한다.
			return false;
		}
	}
	else
		return true;
}

HRESULT CNavigation::Save_NaviMeshData(wstring wpath)
{

	if (mVecCells.empty())
		return E_FAIL;

	// 파일 저장
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(wpath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	/*
	
	// 셀의 속성
	E_CELLTYPE		mCellType = CELLTYPE_NONE;

	// 현재 인덱스
	_uint			mIndex = 0;

	// 이웃하는 인덱스
	_int			mNeighborIndex[LINE_END] = { -1, -1, -1 };

	// 점의 위치
	_float3			mPoints[POINT_END];

	*/

	// 저장될 정보
	_uint					CellIndexndex = 0;
	_float3					vPoints[3];
	_int					vNeighborIndex[3];
	CCell::E_CELLTYPE		CellType;

	for (_uint index = 0; index < mVecCells.size(); ++index)
	{

		// 인덱스		
		CellIndexndex =  mVecCells[index]->Get_Index();
		WriteFile(hFile, &CellIndexndex, sizeof(_uint), &dwByte, nullptr);
		// 점
		for (_uint i = 0; i < 3; ++i)
		{
			vPoints[i] = mVecCells[index]->Get_Point((CCell::E_POINTS)i);
		}
		WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

		// 인접점
		for (_uint i = 0; i < 3; ++i)
		{
			vNeighborIndex[i] = mVecCells[index]->Get_NeighborIndex((CCell::E_LINES)i);

		}
		WriteFile(hFile, vNeighborIndex, sizeof(_uint) * 3, &dwByte, nullptr);

		CellType = mVecCells[index]->Get_CellType();
		WriteFile(hFile, &CellType, sizeof(CCell::E_CELLTYPE), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CNavigation::Load_NaviMeshData(wstring wpath)
{
	if (mVecCells.empty() == false)
		Remove_NaviMeshData();

	// 파일에 저장된 점을 가져온다.
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(wpath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;


	_uint					CellIndexndex = 0;
	_float3					vPoints[3];
	_int					vNeighborIndex[3];
	CCell::E_CELLTYPE		CellType;
	CCell* pCell = nullptr;

	while (true)
	{

		ReadFile(hFile, &CellIndexndex, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		ReadFile(hFile, vNeighborIndex, sizeof(_int) * 3, &dwByte, nullptr);
		ReadFile(hFile, &CellType, sizeof(CCell::E_CELLTYPE) , &dwByte, nullptr);

		if (0 == dwByte)
			break;

		pCell = CCell::Create(vPoints, CellIndexndex);
		
		for (int i =0; i <3; ++i)
		{
			pCell->Set_NeighborIndex((CCell::E_LINES)i, vNeighborIndex[i]);
		}

		pCell->Set_TileType(CellType);



		if (nullptr == pCell)
			return E_FAIL;

		mVecCells.push_back(pCell);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CNavigation::Remove_NaviMeshData()
{
	for (auto& pCell : mVecCells)
		Safe_Release(pCell);
	mVecCells.clear();

	return S_OK;
}

HRESULT CNavigation::SetUp_CurrentPoint(CVIBuffer_Terrain * terrainBuffer)
{
	// 버퍼의 위치를 다시 얻어와서 셀에 정의해준다.
	if (terrainBuffer == nullptr)
		return E_FAIL;
	if (mVecCells.empty())
		return E_FAIL;

	_uint* XZ = terrainBuffer->Get_XZ();
	_uint sizeX = XZ[0];
	_uint sizeZ = XZ[1];

	_float3* vertexPostitions = terrainBuffer->Get_VerrtexPosition();
	
	_uint numface = 0;
	for (_uint  z = 0; z< sizeZ-1;++z)
	{
		for (_uint x = 0; x < sizeX-1; ++x)
		{
			_uint iIndex = z * sizeX + x;
			_uint iIndeics[4] = {
				iIndex + sizeX,iIndex + sizeX + 1,
				 iIndex + 1,iIndex
			};

			mVecCells[numface]->Set_NewPoint( CCell::POINT_A, vertexPostitions[iIndeics[0]]);
			mVecCells[numface]->Set_NewPoint(CCell::POINT_B, vertexPostitions[iIndeics[1]]);
			mVecCells[numface]->Set_NewPoint(CCell::POINT_C, vertexPostitions[iIndeics[2]]);
			numface++;
			mVecCells[numface]->Set_NewPoint(CCell::POINT_A, vertexPostitions[iIndeics[0]]);
			mVecCells[numface]->Set_NewPoint(CCell::POINT_B, vertexPostitions[iIndeics[2]]);
			mVecCells[numface]->Set_NewPoint(CCell::POINT_C, vertexPostitions[iIndeics[3]]);
			numface++;
		}
	}


	return S_OK;
}

const list<CCell*>& CNavigation::AstartPathFind(_uint StartTileIndex, _uint GoalTileIndex)
{
	// 사용한 것 초기화....
	mListPathList.clear();
	mListOpen.clear();
	mListClose.clear();
	if (MakeRoute_INDEX(StartTileIndex, GoalTileIndex, Get_TileForIndex(StartTileIndex)))
	{
		CCell* GoalCell = mVecCells[GoalTileIndex];
		if (GoalCell == nullptr)
			return mListPathList;

		CCell* ParentCell = GoalCell;
	

		while(true)
		{
			mListPathList.push_front(ParentCell);
			_int parentIndex = ParentCell->Get_ParentIndex();
			ParentCell->Set_ParentIndex(-1);
			if (parentIndex == -1 || parentIndex == StartTileIndex)
				break;

			ParentCell = mVecCells[parentIndex];
		}
	}
	return mListPathList;
}

bool CNavigation::MakeRoute_INDEX(_uint StartTileIndex, _uint GoalTileIndex, CCell* oriStartCell)
{
	// TODO: 경로 정보 변경
	if (mVecCells.empty())
		return false;
	if (oriStartCell == nullptr)
		return false;


	if (!mListOpen.empty())
		mListOpen.pop_front();

	mListClose.push_back(StartTileIndex);


	const CCell* cell = mVecCells[StartTileIndex];
	if (cell == nullptr)
		return false;

	const _int* VecNeighors = cell->Get_ArrayNeighborIndex();
	if (VecNeighors != nullptr)
	{
		for (int i = 0; i < 3; ++i)
		{
			_int NearIndex = VecNeighors[i];

			if (NearIndex < 0) 
				continue;

			if (GoalTileIndex == NearIndex)
			{
				// 찾은 경로가 골 지점에 도달했을 경우
				mVecCells[GoalTileIndex]->Set_ParentIndex(StartTileIndex);
				return true;
			}

			// 인접한 타일 조사
			// Close 이미 조사한 노드
			// Open 조사할 대상 노드
			if (false == Check_Close(NearIndex) &&
				false == Check_Open(NearIndex))
			{
				// 갈 수 있는 길인지 판단
				CCell* NearCell = mVecCells[NearIndex];

				if (NearCell)
				{
					if (NearCell->Get_CellType() == CCell::CELLTYPE_STOP)
					{
						// 갈 수 없다.
						mListClose.push_back(NearIndex);
					}
					else
					{
						// 간다.
						NearCell->Set_ParentIndex(StartTileIndex);
						mListOpen.push_back(NearIndex);
					}

				}
			}
		}
	}

	// 갈 수 없음
	if (mListOpen.empty())
		return false;

	if (mListClose.size()>1000)
		return false;
	// 오름차순

	_float3 OriStartCenter = mVecCells[oriStartCell->Get_Index()]->Get_CenterPoint();
//	_float3 newStartCenter = mVecCells[StartTileIndex]->Get_CenterPoint();
	_float3 GoalCenter = mVecCells[GoalTileIndex]->Get_CenterPoint();

	mListOpen.sort([&](int iDest, int iSour)
	{
		// 현재 위치
		_float3 DestCenter = mVecCells[iDest]->Get_CenterPoint();
		_float3 SourCenter = mVecCells[iSour]->Get_CenterPoint();


		// 시작 ~ 현재
		_float3 vSCost1 = DestCenter - OriStartCenter;
		// 현재 ~ 골
		_float3 vGCost1 = GoalCenter - DestCenter;
		_float Cost1 = vSCost1.Length() + vGCost1.Length();

		// 비교대상
		_float3 vSCost2 = SourCenter - OriStartCenter;
		_float3 vGCost2 = GoalCenter - SourCenter;
		_float Cost2 = vSCost2.Length() + vGCost2.Length();

		// 오름차순 가까운 지점부터 정렬
		return Cost1 < Cost2;
	});

	// 깊이 우선탐색처럼 도착지점을 찾을 때까지 계속 탐색
	return MakeRoute_INDEX(mListOpen.front(), GoalTileIndex, oriStartCell);

}

//bool CNavigation::MakeRoute_LINE(_uint StartTileIndex, _uint GoalTileIndex, CCell* oriStartCell, CCell* oriGoalCell)
//{
//	// 인접한 선을 중심으로 탐색해 나간다.
//
//	if (mVecCells.empty())
//		return false;
//	if (oriStartCell == nullptr || oriGoalCell == nullptr)
//		return false;
//
//
//
//	// 큐의 형식으로 동작하는 너비 우선탐색의 특성 상 가장 먼저 들어온 노드를 오픈 리스트에 제거
//	if (!mListOpen.empty())
//		mListOpen.pop_front();
//
//	// 시작지점은 바로 넣는다.
//	mListClose.push_back(StartTileIndex);
//
//	// 인접한 타일 정보로 가까운 경로 찾기
//
//	const CCell* cell = Get_TileForIndex(StartTileIndex);
//	if (cell == nullptr)
//		return false;
//
//	// 인접 인덱스 가져오기
//	const _int* VecNeighors = cell->Get_ArrayNeighborIndex();
//	if (VecNeighors == nullptr)
//	{
//
//	}
//
//	else
//	{
//		for (int i = 0; i < 3; ++i)
//		{
//			// 찾은 경로가 골 지점에 도달했을 경우
//			_int NearIndex = VecNeighors[i];
//			if (GoalTileIndex == NearIndex)
//			{
//				// 도달할 경우 이전 인덱스 저장
//				Get_TileForIndex(GoalTileIndex)->Set_ParentIndex(StartTileIndex);
//				return true;
//			}
//
//			// 인접한 타일 조사
//			// Close 이미 조사한 노드
//			// Open 조사할 대상 노드
//			if (false == Check_Close(NearIndex) &&
//				false == Check_Open(NearIndex))
//			{
//				// 갈 수 있는 길인지 판단
//				CCell* NearCell = Get_TileForIndex(NearIndex);
//				if (NearCell)
//				{
//					if (NearCell->Get_CellType() == CCell::CELLTYPE_STOP)
//					{
//						// 갈 수 없다.
//						mListClose.push_back(NearIndex);
//					}
//					else
//					{
//						// 간다.
//						NearCell->Set_ParentIndex(StartTileIndex);
//						mListOpen.push_back(NearIndex);
//					}
//
//				}
//			}
//		}
//	}
//
//	// 갈 수 없음
//	if (mListOpen.empty())
//		return false;
//
//	// OpenList의 가중치 비교해서 다음 지점 선택
//	// F = g+h
//	// 가중치는 목표지점과의 거리로 결정된다.
//	// 가중치 = (시작->현재지점) + (현재지점->목표지점)
//
//	// 오름차순
//	_uint	iOriginStart = StartTileIndex;
//
//	mListOpen.sort([&](int iDest, int iSour)
//	{
//		CCell::E_POINTS ePointA = CCell::E_POINTS::POINT_A;
//		CCell* destCel = Get_TileForIndex(iDest);
//		CCell* sourCel = Get_TileForIndex(iSour);
//
//
//		// 시작 ~ 현재
//		_float3 vSCost1 = destCel->Get_Point(ePointA) - oriStartCell->Get_Point(ePointA);
//		// 현재 ~ 골
//		_float3 vGCost1 = destCel->Get_Point(ePointA) - oriGoalCell->Get_Point(ePointA);
//		_float Cost1 = vSCost1.Length() + vGCost1.Length();
//
//		// 비교대상
//		_float3 vSCost2 = sourCel->Get_Point(ePointA) - oriStartCell->Get_Point(ePointA);
//		_float3 vGCost2 = sourCel->Get_Point(ePointA) - oriGoalCell->Get_Point(ePointA);
//		_float Cost2 = vSCost2.Length() + vGCost2.Length();
//
//
//		return Cost1 < Cost2;
//	});
//
//	// 깊이 우선탐색처럼 도착지점을 찾을 때까지 계속 탐색
//
//
//	return MakeRoute_LINE(mListOpen.front(), GoalTileIndex, oriStartCell, oriGoalCell);
//}


HRESULT CNavigation::ReadyNaviMeshForListData(list<_float3*>& vpointlist)
{
	_float newY = 0.01f;
	for (auto point : vpointlist)
	{
		_float3			vPoints[3];
		CCell*		pCell = nullptr;
		for (int i = 0; i < 3; ++i)
		{
			_float3 newPoint = point[i];
			newPoint.y = newY;
			vPoints[i] = newPoint;
		}

		pCell = CCell::Create(vPoints, mVecCells.size());
		if (nullptr == pCell)
			return E_FAIL;
		mVecCells.push_back(pCell);
	}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CNavigation::Render(CTransform* pTransform)
{
	if (mVecCells.empty())
		return S_FALSE;
	if (GetSingle(CGameInstance)->Get_IsColliderRender() == false)
		return S_OK;
	// 랜더링시 처리
	m_pDeviceContext->IASetInputLayout(mInputLayout);
	//	mBaseEffect->SetWorld(trans->GetWorldFloat4x4());

	CPipeLine*		pPipeLine = GetSingle(CPipeLine);
	mBaseEffect->SetWorld(pTransform->GetWorldMatrix());
	mBaseEffect->SetView(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW)));
	mBaseEffect->SetProjection(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ)));

	mBaseEffect->Apply(m_pDeviceContext);

	// 배치 클래스의 Begin ~ End 까지 물체를 그린다.
	mBatch->Begin();
	_color debugColor = DirectX::Colors::White;

	if (meNaviType == CNavigation::NAVI_OBJTYPE_NONE)
	{
		for (auto cell : mVecCells)
		{
			if(cell->Get_CellType() == CCell::E_CELLTYPE::CELLTYPE_STOP)
				debugColor = DirectX::Colors::Red;
			else if (cell->Get_CellType() == CCell::E_CELLTYPE::CELLTYPE_DEBUG)
				debugColor = DirectX::Colors::Blue;
			else
				debugColor = DirectX::Colors::White;

			DX::DrawTriangle(mBatch,
				cell->Get_Point(CCell::POINT_A),
				cell->Get_Point(CCell::POINT_B),
				cell->Get_Point(CCell::POINT_C), debugColor);
		}
	}
	else
	{
		switch (meNaviType)
		{
		case Engine::CNavigation::NAVI_OBJTYPE_PLAYER:
			debugColor = DirectX::Colors::Yellow;
			break;
		case Engine::CNavigation::NAVI_OBJTYPE_STATIC:
			debugColor = DirectX::Colors::Blue;
			break;

		case Engine::CNavigation::NAVI_OBJTYPE_DYNAMIC:
			debugColor = DirectX::Colors::Magenta;
			break;
		default:
			debugColor = DirectX::Colors::White;
			break;
		}

		DX::DrawTriangle(mBatch,
			mVecCells[mCurrentIndex]->Get_Point(CCell::POINT_A),
			mVecCells[mCurrentIndex]->Get_Point(CCell::POINT_B),
			mVecCells[mCurrentIndex]->Get_Point(CCell::POINT_C), debugColor);
	}
	mBatch->End();

	return S_OK;
}

#endif // _DEBUG

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : mVecCells)
	{
		for (auto& pDestCell : mVecCells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_A),
				pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_AB, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_B),
				pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_BC, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_C),
				pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_CA, pDestCell->Get_Index());
			}
		}
	}

	return S_OK;
}

CCell * CNavigation::Get_TileForIndex(_uint index) const
{
	if (mVecCells.empty())
		return nullptr;

	for (auto cell: mVecCells)
	{
		if (index == cell->Get_Index())
			return cell;
	}
	return nullptr;
}

bool CNavigation::Check_Close(_uint index)
{	
	for (_uint& iCloseIndex : mListClose)
	{
		if (index == iCloseIndex)
			return true;
	}
	return false;
}


bool CNavigation::Check_Open(_uint index)
{
	for (_uint& iOpenIndex : mListOpen)
	{
		if (index == iOpenIndex)
			return true;
	}
	return false;
}

//void CNavigation::ReadyDefault()
//{
//	_float3			vPoints[3];
//	CCell*		pCell = nullptr;
//	_float newY = 0.1f;
//
//	vPoints[0] = _float3(0.f, newY, 5.f);
//	vPoints[1] = _float3(5.f, newY, 0.f);
//	vPoints[2] = _float3(0.f, newY, 0.f);
//
//	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
//	if (nullptr == pCell)
//		return;
//	mVecCells.push_back(pCell);
//
//	vPoints[0] = _float3(0.f, newY, 5.f);
//	vPoints[1] = _float3(5.f, newY, 5.f);
//	vPoints[2] = _float3(5.f, newY, 0.f);
//
//	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
//	if (nullptr == pCell)
//		return;
//	mVecCells.push_back(pCell);
//
//	vPoints[0] = _float3(0.f, newY, 10.f);
//	vPoints[1] = _float3(5.f, newY, 5.f);
//	vPoints[2] = _float3(0.f, newY, 5.f);
//
//	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
//	if (nullptr == pCell)
//		return;
//	mVecCells.push_back(pCell);
//
//	vPoints[0] = _float3(5.f, newY, 5.f);
//	vPoints[1] = _float3(10.f, newY, 0.f);
//	vPoints[2] = _float3(5.f, newY, 0.f);
//
//	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
//	if (nullptr == pCell)
//		return;
//	mVecCells.push_back(pCell);
//}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pNaviDataFilePath)
{
	CNavigation*	pInstance = NEW CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pNaviDataFilePath)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = NEW CNavigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : mVecCells)
		Safe_Release(pCell);

	mVecCells.clear();

#ifdef _DEBUG
	Safe_Release(mInputLayout);

	if (false == m_isCloned)
	{
		Safe_Delete(mBaseEffect);
		Safe_Delete(mBatch);
	}
#endif // _DEBUG
}