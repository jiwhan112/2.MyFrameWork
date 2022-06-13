#include "..\Public/VIBuffer_Point_Instance.h"
#include "Shader.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubResource(rhs.m_VBInstSubResource)
	, m_iNumInstance(rhs.m_iNumInstance)
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Point_Instance::NativeConstruct_Prototype(_uint NumInstance)
{
	// ���ؽ� ���� �ʱ�ȭ
#pragma region VERTEX_BUFFER
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumIndicesPerPrimitive = 1;
	m_iNumVertices = 1;
	m_iNumVertexBuffers = 2; // ������ ������ 2��

	// �⺻ ���ؽ� ���ۼ��ð� �����ϴ�.
	m_VBDesc.ByteWidth = sizeof(VTXMATRIX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;				
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		
	m_VBDesc.StructureByteStride = sizeof(VTXPOINT);

	// ���� ���� ������ LOCK UNLOCK�� ���� �ʰ� ���ش�.
	VTXPOINT*		pVertices = NEW VTXPOINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0, 0, 0.f);
	pVertices[0].fPSize = 1.f;

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	// m_pVB�� ���ؽ� ���� ����
	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	// ���� �μ��� DX9�� ����Ѵ�.
	m_iNumInstance = NumInstance;
	m_iNumPrimitive = m_iNumInstance;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(_ushort) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	_ushort*	pIndices = NEW _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	// �ν��Ͻ� ������ŭ �ε��� ���� ����
	// ���̱� ������ ������.
	_uint iNumFaces = 0;
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pIndices[i] = 0;
	}

	// DX11������ ���������� ��� ���� ������ �־� �������ش�.
	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	mIndeces = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;
	Safe_Delete_Array(pIndices);

#pragma endregion

// �ν��Ͻ� ���� �ʱ�ȭ
#pragma region INSTANCE_BUFFER
	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBInstDesc.ByteWidth = sizeof(VTXMATRIX) * m_iNumInstance;
	// �ν��Ͻ� ���۴� �������� ����
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.StructureByteStride = sizeof(VTXMATRIX); // RULP�� ������ȴ�.
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VTXMATRIX*		pInstanceMatrix = NEW VTXMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceMatrix, sizeof(VTXMATRIX) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceMatrix[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstanceMatrix[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstanceMatrix[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		// ��ġ���� ���߿��� ���ڰ�����
		pInstanceMatrix[i].vTranslation = _float4(rand() % 15, 0.f, rand() % 15, 1.f); 
	}

	ZeroMemory(&m_VBInstSubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResource.pSysMem = pInstanceMatrix;

	// �ν��Ͻ� ���� ����
	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResource, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceMatrix);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_ParticleDesc, pArg, sizeof(m_ParticleDesc));

	// �ӵ� ����
	m_pSpeeds =  NEW _float[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
		m_pSpeeds[i] = rand() % _uint(m_ParticleDesc.fMaxSpeed - m_ParticleDesc.fMinSpeed + 1) + m_ParticleDesc.fMinSpeed;


	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Render(CShader * shader, _uint passindex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	// ���� 2��
	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	// ���� 2��
	_uint			iStrides[] = {
		m_VBDesc.StructureByteStride,
		m_VBInstDesc.StructureByteStride
	};

	_uint			iOffsets[] = {
		0,
		0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

	if (FAILED(shader->Set_InputLayout(passindex)))
		return E_FAIL;
	if (FAILED(shader->Apply(passindex)))
		return E_FAIL;

	m_pDeviceContext->DrawIndexedInstanced(1, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Point_Instance::Update(_double TimeDelta)
{
	if (nullptr == m_pDeviceContext)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_vector		vPosition = XMLoadFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vTranslation));

		vPosition += XMVector3Normalize(XMLoadFloat3(&m_ParticleDesc.vMoveDir)) * m_pSpeeds[i] * TimeDelta;

		if (XMVectorGetY(vPosition) < -10.f)
			vPosition = XMVectorSetY(vPosition, 0.f);

		XMStoreFloat4(&(((VTXMATRIX*)SubResource.pData)[i].vTranslation), vPosition);
	}



	m_pDeviceContext->Unmap(m_pVBInstance, 0);

}

CVIBuffer_Point_Instance * CVIBuffer_Point_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint NumInstance)
{
	CVIBuffer_Point_Instance*	pInstance = NEW CVIBuffer_Point_Instance(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(NumInstance)))
	{
		MSGBOX("Failed to Created CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Point_Instance::Clone(void * pArg)
{
	CVIBuffer_Point_Instance*	pInstance = NEW CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pSpeeds);
	Safe_Release(m_pVBInstance);

}