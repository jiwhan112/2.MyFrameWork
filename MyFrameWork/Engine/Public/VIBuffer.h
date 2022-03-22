#pragma once

#include "Component.h"

BEGIN(Engine)

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
	HRESULT Render();

protected:
	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_VBDesc;
	D3D11_SUBRESOURCE_DATA		m_VBSubResourceData;
	_uint						m_iNumVertices = 0;
	_uint						m_iNumVertexBuffers = 0;

protected:
	ID3D11Buffer*				m_pIB = nullptr;
	D3D11_BUFFER_DESC			m_IBDesc;
	D3D11_SUBRESOURCE_DATA		m_IBSubResourceData;
	_uint						m_iNumPrimitive = 0;
	_uint						m_iNumIndicesPerPrimitive = 0;
	DXGI_FORMAT					m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;


protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END