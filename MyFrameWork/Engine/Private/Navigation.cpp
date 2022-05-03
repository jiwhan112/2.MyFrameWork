#include "..\Public\Navigation.h"
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
	, m_iCurrentIndex(rhs.m_iCurrentIndex)	

	, mBaseEffect(rhs.mBaseEffect)
	, mBatch(rhs.mBatch)
	, mInputLayout(rhs.mInputLayout)

{
	Safe_AddRef(mInputLayout);

	for (auto& pCell : mVecCells)
		Safe_AddRef(pCell);

}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar * pNaviDataFilePath)
{
	if (pNaviDataFilePath == nullptr)
	{
		// 임의로 네비메시 생성
		ReadyDefault();

	}

	else
	{
		// 파일에 저장된 점을 가져온다.
		_ulong			dwByte = 0;
		HANDLE			hFile = CreateFile(pNaviDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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
	}

	
	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

#ifdef _DEBUG

	// 셰이더 정점 초기화
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
	return S_OK;
}

#ifdef _DEBUG
HRESULT CNavigation::Render(CTransform* pTransform)
{

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
	for (auto cell : mVecCells)
	{
		DX::DrawTriangle(mBatch, 
			cell->Get_Point(CCell::POINT_A),
			cell->Get_Point(CCell::POINT_B),
			cell->Get_Point(CCell::POINT_C));

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

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_AB, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_BC, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
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

