#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class ENGINE_DLL CMeshContainer final : public CVIBuffer
{
public:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(CModel::E_MODEL_TYPE eMeshtype, aiMesh* pAIMesh, _fmatrix defaultmatrix);
	virtual HRESULT NativeConstruct(void* pArg);


private:
	HRESULT Ready_NonAnimMeshContainer(aiMesh* pAIMesh, _fmatrix defaultmatrix);
	HRESULT Ready_AnimMeshContainer(aiMesh* pAIMesh);


public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
		CModel::E_MODEL_TYPE type, aiMesh* mesh,_fmatrix defaultmatrix);

	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END