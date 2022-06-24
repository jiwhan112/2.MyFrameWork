#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer
{
public:
	typedef struct tagParticleDesc
	{
		_float		fMinSpeed, fMaxSpeed;
		_float3		vMoveDir;
	}PARTICLEDESC;

public:
	CVIBuffer_Rect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint NumInstance);
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual HRESULT Render(CShader* shader, _uint passindex)override;


public:
//	void Update(_double TimeDelta);

private:
	// 인스턴싱에서는 정점정보와 위치행렬정보(float4 *4)가 추가로 들어간다. 
	// 인스턴스하나당 표현되어야할 행렬을 정점으로서 인스턴스의 갯수만큼 가지고 있는 정점 버퍼.

	ID3D11Buffer*				m_pVBInstance;
	D3D11_BUFFER_DESC			m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA		m_VBInstSubResource;
	_uint						m_iNumInstance = 0; // 인스턴스 개수

	PARTICLEDESC				m_ParticleDesc;
	_float*						m_pSpeeds = nullptr;

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint NumInstance);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END