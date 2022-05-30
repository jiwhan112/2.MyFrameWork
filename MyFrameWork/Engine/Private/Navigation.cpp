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

	// ���̴� ���� �ʱ�ȭ
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

	// �׺� �޽� �����̶� �浹
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

	// �׺� �޽� �����̶� �浹
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
	// ��ŷ�� �ش� �ε��� ��ȯ
	CPicking*		pPicking = GetSingle(CPicking);
	pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
	_bool		isPick = false;

	// �׺� �޽� �����̶� �浹
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

	// ���� �ʱ�ȭ
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

	// �׺�޽� ������ �ڵ� ����
	CVIBuffer_Terrain* terrainbuffer = terrain;

	// 1. ������ �����´�.
	_float3* vertexes = terrainbuffer->Get_VerrtexPosition();
	_uint vertexSize = terrainbuffer->Get_VerrtexSize();
	_uint numX = terrainbuffer->Get_XZ()[0];
	_uint numZ = terrainbuffer->Get_XZ()[1];

	// ���ؽ� �״�� �׺� �ѱ�
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

	// ������ Position�� ���� ���� ���ؼ� ���� ������ �����Ѵ�.
	// 1. ������ �ε��� üũ
	if (false == mVecCells[mCurrentIndex]->isIn(vPosition, &iNeighborIndex))
	{
		// �̿��� �ִٸ� �� �������� �ε����� �����Ѵ�.
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

		// �̿��� ���ٸ� �̵��� ���´�.
		else
		{
			// �����̵� ���ͷ� ����
			// �ݻ� ���͸� ���ϴ� ��ó�� ���Ѵ�.
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

	// ���� ����
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(wpath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	/*
	
	// ���� �Ӽ�
	E_CELLTYPE		mCellType = CELLTYPE_NONE;

	// ���� �ε���
	_uint			mIndex = 0;

	// �̿��ϴ� �ε���
	_int			mNeighborIndex[LINE_END] = { -1, -1, -1 };

	// ���� ��ġ
	_float3			mPoints[POINT_END];

	*/

	// ����� ����
	_uint					CellIndexndex = 0;
	_float3					vPoints[3];
	_int					vNeighborIndex[3];
	CCell::E_CELLTYPE		CellType;

	for (_uint index = 0; index < mVecCells.size(); ++index)
	{

		// �ε���		
		CellIndexndex =  mVecCells[index]->Get_Index();
		WriteFile(hFile, &CellIndexndex, sizeof(_uint), &dwByte, nullptr);
		// ��
		for (_uint i = 0; i < 3; ++i)
		{
			vPoints[i] = mVecCells[index]->Get_Point((CCell::E_POINTS)i);
		}
		WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

		// ������
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

	// ���Ͽ� ����� ���� �����´�.
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
	// ������ ��ġ�� �ٽ� ���ͼ� ���� �������ش�.
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
	// ����� �� �ʱ�ȭ....
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
	// TODO: ��� ���� ����
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
				// ã�� ��ΰ� �� ������ �������� ���
				mVecCells[GoalTileIndex]->Set_ParentIndex(StartTileIndex);
				return true;
			}

			// ������ Ÿ�� ����
			// Close �̹� ������ ���
			// Open ������ ��� ���
			if (false == Check_Close(NearIndex) &&
				false == Check_Open(NearIndex))
			{
				// �� �� �ִ� ������ �Ǵ�
				CCell* NearCell = mVecCells[NearIndex];

				if (NearCell)
				{
					if (NearCell->Get_CellType() == CCell::CELLTYPE_STOP)
					{
						// �� �� ����.
						mListClose.push_back(NearIndex);
					}
					else
					{
						// ����.
						NearCell->Set_ParentIndex(StartTileIndex);
						mListOpen.push_back(NearIndex);
					}

				}
			}
		}
	}

	// �� �� ����
	if (mListOpen.empty())
		return false;

	if (mListClose.size()>1000)
		return false;
	// ��������

	_float3 OriStartCenter = mVecCells[oriStartCell->Get_Index()]->Get_CenterPoint();
//	_float3 newStartCenter = mVecCells[StartTileIndex]->Get_CenterPoint();
	_float3 GoalCenter = mVecCells[GoalTileIndex]->Get_CenterPoint();

	mListOpen.sort([&](int iDest, int iSour)
	{
		// ���� ��ġ
		_float3 DestCenter = mVecCells[iDest]->Get_CenterPoint();
		_float3 SourCenter = mVecCells[iSour]->Get_CenterPoint();


		// ���� ~ ����
		_float3 vSCost1 = DestCenter - OriStartCenter;
		// ���� ~ ��
		_float3 vGCost1 = GoalCenter - DestCenter;
		_float Cost1 = vSCost1.Length() + vGCost1.Length();

		// �񱳴��
		_float3 vSCost2 = SourCenter - OriStartCenter;
		_float3 vGCost2 = GoalCenter - SourCenter;
		_float Cost2 = vSCost2.Length() + vGCost2.Length();

		// �������� ����� �������� ����
		return Cost1 < Cost2;
	});

	// ���� �켱Ž��ó�� ���������� ã�� ������ ��� Ž��
	return MakeRoute_INDEX(mListOpen.front(), GoalTileIndex, oriStartCell);

}

//bool CNavigation::MakeRoute_LINE(_uint StartTileIndex, _uint GoalTileIndex, CCell* oriStartCell, CCell* oriGoalCell)
//{
//	// ������ ���� �߽����� Ž���� ������.
//
//	if (mVecCells.empty())
//		return false;
//	if (oriStartCell == nullptr || oriGoalCell == nullptr)
//		return false;
//
//
//
//	// ť�� �������� �����ϴ� �ʺ� �켱Ž���� Ư�� �� ���� ���� ���� ��带 ���� ����Ʈ�� ����
//	if (!mListOpen.empty())
//		mListOpen.pop_front();
//
//	// ���������� �ٷ� �ִ´�.
//	mListClose.push_back(StartTileIndex);
//
//	// ������ Ÿ�� ������ ����� ��� ã��
//
//	const CCell* cell = Get_TileForIndex(StartTileIndex);
//	if (cell == nullptr)
//		return false;
//
//	// ���� �ε��� ��������
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
//			// ã�� ��ΰ� �� ������ �������� ���
//			_int NearIndex = VecNeighors[i];
//			if (GoalTileIndex == NearIndex)
//			{
//				// ������ ��� ���� �ε��� ����
//				Get_TileForIndex(GoalTileIndex)->Set_ParentIndex(StartTileIndex);
//				return true;
//			}
//
//			// ������ Ÿ�� ����
//			// Close �̹� ������ ���
//			// Open ������ ��� ���
//			if (false == Check_Close(NearIndex) &&
//				false == Check_Open(NearIndex))
//			{
//				// �� �� �ִ� ������ �Ǵ�
//				CCell* NearCell = Get_TileForIndex(NearIndex);
//				if (NearCell)
//				{
//					if (NearCell->Get_CellType() == CCell::CELLTYPE_STOP)
//					{
//						// �� �� ����.
//						mListClose.push_back(NearIndex);
//					}
//					else
//					{
//						// ����.
//						NearCell->Set_ParentIndex(StartTileIndex);
//						mListOpen.push_back(NearIndex);
//					}
//
//				}
//			}
//		}
//	}
//
//	// �� �� ����
//	if (mListOpen.empty())
//		return false;
//
//	// OpenList�� ����ġ ���ؼ� ���� ���� ����
//	// F = g+h
//	// ����ġ�� ��ǥ�������� �Ÿ��� �����ȴ�.
//	// ����ġ = (����->��������) + (��������->��ǥ����)
//
//	// ��������
//	_uint	iOriginStart = StartTileIndex;
//
//	mListOpen.sort([&](int iDest, int iSour)
//	{
//		CCell::E_POINTS ePointA = CCell::E_POINTS::POINT_A;
//		CCell* destCel = Get_TileForIndex(iDest);
//		CCell* sourCel = Get_TileForIndex(iSour);
//
//
//		// ���� ~ ����
//		_float3 vSCost1 = destCel->Get_Point(ePointA) - oriStartCell->Get_Point(ePointA);
//		// ���� ~ ��
//		_float3 vGCost1 = destCel->Get_Point(ePointA) - oriGoalCell->Get_Point(ePointA);
//		_float Cost1 = vSCost1.Length() + vGCost1.Length();
//
//		// �񱳴��
//		_float3 vSCost2 = sourCel->Get_Point(ePointA) - oriStartCell->Get_Point(ePointA);
//		_float3 vGCost2 = sourCel->Get_Point(ePointA) - oriGoalCell->Get_Point(ePointA);
//		_float Cost2 = vSCost2.Length() + vGCost2.Length();
//
//
//		return Cost1 < Cost2;
//	});
//
//	// ���� �켱Ž��ó�� ���������� ã�� ������ ��� Ž��
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
	// �������� ó��
	m_pDeviceContext->IASetInputLayout(mInputLayout);
	//	mBaseEffect->SetWorld(trans->GetWorldFloat4x4());

	CPipeLine*		pPipeLine = GetSingle(CPipeLine);
	mBaseEffect->SetWorld(pTransform->GetWorldMatrix());
	mBaseEffect->SetView(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW)));
	mBaseEffect->SetProjection(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ)));

	mBaseEffect->Apply(m_pDeviceContext);

	// ��ġ Ŭ������ Begin ~ End ���� ��ü�� �׸���.
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