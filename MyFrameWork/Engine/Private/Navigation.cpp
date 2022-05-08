#include "..\Public\Navigation.h"
#include "..\Public\Picking.h"
#include "..\Public\VIBuffer_Terrain.h"

#include "Cell.h"
#include "PipeLine.h"
#include "Transform.h"
#include "DebugDraw.h"

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
	//if (pNaviDataFilePath == nullptr)
	//{
	//	// ���Ƿ� �׺�޽� ����
	//	ReadyDefault();
	//}

	//else
	//{
	//	// ���Ͽ� ����� ���� �����´�.
	//	_ulong			dwByte = 0;
	//	HANDLE			hFile = CreateFile(pNaviDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//	if (0 == hFile)
	//		return E_FAIL;

	//	_float3			vPoints[3];

	//	while (true)
	//	{
	//		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	//		if (0 == dwByte)
	//			break;

	//		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
	//		if (nullptr == pCell)
	//			return E_FAIL;

	//		mVecCells.push_back(pCell);
	//	}

	//	CloseHandle(hFile);
	//}

	//
	//if (FAILED(SetUp_Neighbor()))
	//	return E_FAIL;

#ifdef _DEBUG

	// ���̴� ���� �ʱ�ȭ
	mBaseEffect = new BasicEffect(m_pDevice);
	mBaseEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	mBaseEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &mInputLayout)))
		return E_FAIL;

	mBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);

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

HRESULT CNavigation::SetUp_NewNaviMesh(list<_float3*>& vpointlist)
{
	if (mVecCells.empty() == false)
	{
		for (auto& pCell : mVecCells)
			Safe_Release(pCell);
	}
	mVecCells.clear();

	// ���� �ʱ�ȭ
	FAILED_CHECK(ReadyNaviMeshForListData(vpointlist));
	FAILED_CHECK(SetUp_Neighbor());

	return S_OK;
}

HRESULT CNavigation::SetUp_AutoMesh(CVIBuffer_Terrain * terrain)
{
	if (mVecCells.empty() == false)
	{
		for (auto& pCell : mVecCells)
			Safe_Release(pCell);
	}
	mVecCells.clear();

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
	HANDLE			hFile = CreateFile(wpath.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];

	for (_uint i=0; i<mVecCells.size();++i)
	{



	}
	while (true)
	{

		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
		if (nullptr == pCell)
			return E_FAIL;

		mVecCells.push_back(pCell);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CNavigation::Load_NaviMeshData(wstring wpath)
{
	 
	// ���Ͽ� ����� ���� �����´�.
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(wpath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];

	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
		if (nullptr == pCell)
			return E_FAIL;

		mVecCells.push_back(pCell);
	}

	CloseHandle(hFile);

	return S_OK;
}

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
			newPoint.y += newY;
			vPoints[i] = newPoint;
		}

		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
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
			debugColor = DirectX::Colors::Red;
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

void CNavigation::ReadyDefault()
{
	_float3			vPoints[3];
	CCell*		pCell = nullptr;
	_float newY = 0.1f;

	vPoints[0] = _float3(0.f, newY, 5.f);
	vPoints[1] = _float3(5.f, newY, 0.f);
	vPoints[2] = _float3(0.f, newY, 0.f);

	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
	if (nullptr == pCell)
		return;
	mVecCells.push_back(pCell);

	vPoints[0] = _float3(0.f, newY, 5.f);
	vPoints[1] = _float3(5.f, newY, 5.f);
	vPoints[2] = _float3(5.f, newY, 0.f);

	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
	if (nullptr == pCell)
		return;
	mVecCells.push_back(pCell);

	vPoints[0] = _float3(0.f, newY, 10.f);
	vPoints[1] = _float3(5.f, newY, 5.f);
	vPoints[2] = _float3(0.f, newY, 5.f);

	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
	if (nullptr == pCell)
		return;
	mVecCells.push_back(pCell);

	vPoints[0] = _float3(5.f, newY, 5.f);
	vPoints[1] = _float3(10.f, newY, 0.f);
	vPoints[2] = _float3(5.f, newY, 0.f);

	pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, mVecCells.size());
	if (nullptr == pCell)
		return;
	mVecCells.push_back(pCell);
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pNaviDataFilePath)
{
	CNavigation*	pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pNaviDataFilePath)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);

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