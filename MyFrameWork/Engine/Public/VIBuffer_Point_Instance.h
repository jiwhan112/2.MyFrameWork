#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer
{
public:
	typedef struct tagParticleDesc
	{
		_float		fMinSpeed, fMaxSpeed;
		_float3		vMoveDir;
	}PARTICLEDESC;

public:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint NumInstance);
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual HRESULT Render(CShader* shader, _uint passindex)override;


public:
	void Update(_double TimeDelta);

private:
	// 인스턴스용 버퍼
	ID3D11Buffer*				m_pVBInstance;
	D3D11_BUFFER_DESC			m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA		m_VBInstSubResource;
	_uint						m_iNumInstance = 0; // 인스턴스 개수

	PARTICLEDESC				m_ParticleDesc;
	_float*						m_pSpeeds = nullptr;

public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint NumInstance);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END