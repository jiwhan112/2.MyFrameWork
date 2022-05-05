#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMeshContainer final : public CVIBuffer
{
private:
	explicit CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	_uint Get_NumBones() const
	{
		return m_iNumBones;
	}

	aiMesh* Get_AIMesh()
	{
		return m_pAIMesh;
	}

public:
	virtual HRESULT NativeConstruct_Prototype(aiMesh* pAIMesh);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT Ready_VertexIndexBuffer(CModel::E_MODEL_TYPE eMeshtype, _fmatrix TransformMatrix);
	HRESULT Add_HierarchyNode(_uint iNodeIndex);
	HRESULT SetUp_Matrices(_float4x4* pBoneMatrices, const vector<CHierarchyNode*>* pNodes, _fmatrix TransformMatrix);

private:
	HRESULT Ready_NonAnimMeshContainer(aiMesh* pAIMesh, _fmatrix TransformMatrix);
	HRESULT Ready_AnimMeshContainer(aiMesh* pAIMesh);
	HRESULT Ready_SkinnedInfo(VTXANIMODEL* pVertices);

private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iMaterialIndex = 0;

	_uint			m_iNumBones = 0;
	aiMesh*			m_pAIMesh = nullptr;

	/* 현재 메시에 영향을 주는 노드들. */
	vector<_uint>		m_HierarchyNodeIndices;

public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, aiMesh* pAIMesh);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END