#pragma once

#include "Component.h"

BEGIN(Engine)
// class CShader;
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

public:
	virtual HRESULT Render(class CShader* pShader, _uint iPassIndex);


public:
	_bool Pick(const _float4x4& WorldMatrixInverse, _float3 * pOut);
	_float3*		Get_VerrtexPosition() const { return mpVertexPos; }
	_uint		Get_VerrtexSize() const { return m_iNumVertices; }

protected:
	// DX11�� VB ����
	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_VBDesc;				// DESC
	D3D11_SUBRESOURCE_DATA		m_VBSubResourceData;	// ���� ������
	_uint						m_iNumVertices = 0;		// ���ؽ� ����
	_uint						m_iNumVertexBuffers = 0;// ���ε��� �� ����

	// DX11�� IB ����
	ID3D11Buffer*				m_pIB = nullptr;				//
	D3D11_BUFFER_DESC			m_IBDesc;						// DESC
	D3D11_SUBRESOURCE_DATA		m_IBSubResourceData;			// ���� ������
	_uint						m_iNumPrimitive = 0;			//
	_uint						m_iNumIndicesPerPrimitive = 0;	//
	DXGI_FORMAT					m_eIndexFormat;					// �ε��� ����
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;					//

	_float3*		mpVertexPos = nullptr;
	void*			mIndeces = nullptr;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END