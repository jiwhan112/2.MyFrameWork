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
	virtual HRESULT NativeConstruct_Prototype(aiMesh* pAIMesh);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	_uint Get_NumBones() const { return mNumBones; }
	aiMesh* Get_AIMesh() { return mpAiMesh; }

public:	
	HRESULT Ready_VertexIndexBuffer(CModel::E_MODEL_TYPE eMeshtype, _fmatrix defaultmatrix);
	HRESULT Add_HierarchyNode(class CHierarchyNode* pNode);
	HRESULT SetUp_Matrices(_float4x4* pBoneMatrices);

private:
	HRESULT Ready_NonAnimMeshContainer(aiMesh* pAIMesh, _fmatrix defaultmatrix);
	HRESULT Ready_AnimMeshContainer(aiMesh* pAIMesh);
	HRESULT Ready_SkinnedInfo(VTXANIMODEL* pVertices);

private:
	_uint			mIndexMaterial = 0;
	_uint			mNumBones= 0;
	aiMesh*			mpAiMesh = nullptr;

	// 현재 메시에 영향을 주는 메시 저장
	vector<class CHierarchyNode*>		mHierarchyNodes;

public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,aiMesh* mesh);

	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END