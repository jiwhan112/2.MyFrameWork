#include "..\Public\VIBuffer.h"
#include "Shader.h"
#include "Picking.h"

CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_VBDesc(rhs.m_VBDesc)
	, m_VBSubResourceData(rhs.m_VBSubResourceData)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_IBDesc(rhs.m_IBDesc)
	, m_IBSubResourceData(rhs.m_IBSubResourceData)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iNumIndicesPerPrimitive(rhs.m_iNumIndicesPerPrimitive)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eTopology(rhs.m_eTopology)
	, mpVertexPos(rhs.mpVertexPos)
	, mIndeces(rhs.mIndeces)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render(CShader* shader, _uint passindex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	// ���� �������� �������� ���۸� �ѹ��� �׸� �� �ֱ� ������ �迭�� ���õȴ�.
	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB
	};

	_uint			iStrides[] = {
		m_VBDesc.StructureByteStride
	};

	_uint			iOffsets[] = {
		0,
	};

	// ���ؽ� �ε��� Topology Ÿ���� context�� ���ε��ؼ� ����Ѵ�.
	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

	// ���̴� ������
	FAILED_CHECK(shader->Set_InputLayout(passindex));
	FAILED_CHECK(shader->Apply(passindex));

	m_pDeviceContext->DrawIndexed(m_iNumIndicesPerPrimitive * m_iNumPrimitive, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	// DESC�� ������ DESC�� �����Ѵ�.
	return m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubResourceData, &m_pVB);
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	// DESC�� ������ DESC�� �����Ѵ�.
	return m_pDevice->CreateBuffer(&m_IBDesc, &m_IBSubResourceData, &m_pIB);
}

_bool CVIBuffer::Pick(const _float4x4& WorldMatrixInverse, _float3 * pOut)
{
	// #TODO: ���۷� �浹���� ���� �ݶ��̴� / �׺�޽��浹�� �����غ���.

	CPicking*		pPicking = GetSingle(CPicking);

	pPicking->Transform_ToLocalSpace(WorldMatrixInverse);

	_uint		iIndexByte = 0;
	_uint		iIndexSize = 0;

	if (m_eIndexFormat == DXGI_FORMAT_R16_UINT)
	{
		iIndexByte = 2;
		iIndexSize = sizeof(FACEINDICES16);
	}
	else
	{
		iIndexByte = 4;
		iIndexSize = sizeof(FACEINDICES32);

	}
	_bool		isPick = false;

	_uint	iIndices[3] = { 0 };

	
	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		_uint	iIndices[3] = { 0 };

		/* void*�� _byte*�� ĳ������ ���� : �ּҿ� ����� �������� �� �������Ʈũ�⸸ŭ �̵��� �� �ֵ��� ������ֱ�����. */
		/* _short*�������������� 1�����ϸ� 2byte�� �̵��Ѵ�.  */
		/* _uint*�������������� 1�����ϸ� 4byte�� �̵��Ѵ�.  */
		/* _byte*�������������� 1�����ϸ� 1byte�� �̵��Ѵ�.  */

		for (_uint j = 0; j < 3; ++j)
			memcpy(&iIndices[j], (((_byte*)mIndeces + iIndexSize * i) + iIndexByte * j), iIndexByte);

		_float3		vPoint[3] = {
			mpVertexPos[iIndices[0]],
			mpVertexPos[iIndices[1]],
			mpVertexPos[iIndices[2]]
		};

		if (isPick = pPicking->isPick(vPoint, pOut))
		{
			_float4x4		WorldMatrix;
			WorldMatrix = WorldMatrixInverse.Invert();
			_float3::Transform(*pOut, WorldMatrix, *pOut);
			return isPick;
		}
	}

	return isPick;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (m_isCloned == false)
	{
		Safe_Delete_Array(mpVertexPos);
		Safe_Delete_Array(mIndeces);

	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);



}