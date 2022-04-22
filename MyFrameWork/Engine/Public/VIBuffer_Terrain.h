#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
public:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMap);
	virtual HRESULT NativeConstruct_Prototype(_uint x,_uint z);
	virtual HRESULT NativeConstruct(void* pArg) override;

	// #Tag ����Ÿ��
	_float4 Get_Height(_float4 TargetPos);


private:
	_uint			miNumX = 0;
	_uint			miNumZ = 0;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* HeightMap);
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint x, _uint z);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END