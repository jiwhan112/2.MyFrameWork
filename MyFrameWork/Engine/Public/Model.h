#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:

	enum E_MODEL_TYPE { MODEL_NOANI, MODEL_ANI, MODEL_END };
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMaterials() const;
	_uint Get_NumAnimations()const { return m_iNumAnimations; }

	const vector<class CAnimation*>* Get_VecAnimations()const { return &m_Animations; }
	HRESULT Set_AniString(string AniName);


	
public:
	virtual HRESULT NativeConstruct_Prototype(E_MODEL_TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT SetUp_AnimIndex(_uint iAnimIndex);
	HRESULT Update_CombinedTransformationMatrices(_double TimeDelta);
	HRESULT Bind_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pValueName);
	HRESULT Render(class CShader* pShader, _uint iPassIndex, _uint iMaterialIndex, const char* pBoneValueName = nullptr);

private:
	const aiScene*				m_pScene = nullptr;
	Importer					m_Importer;

private:
	_uint									m_iNumMeshContainers = 0;
	vector<class CMeshContainer*>*			m_pMeshContainers = nullptr;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	E_MODEL_TYPE							m_eModelType = MODEL_END;
	_float4x4								m_TransformMatrix;

private:
	_uint									m_iNumMaterials = 0;
	vector<MESHMATERIALDESC*>				m_Materials;
	typedef vector<MESHMATERIALDESC*>		MATERIALS;

	// 애니메이션 영역
private:
	// 계층
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;	

	// 애니메이션
	_uint									m_iCurrentAnim = 0;
	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

private:
	HRESULT Ready_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth);	
	HRESULT Ready_OffsetMatrices();
	HRESULT Ready_Animation();
	HRESULT Link_ChannelToNode();


private:
	CHierarchyNode* Find_HierarchyNode(const char* pName, _uint* pOut = nullptr);
	_uint Find_Animation(string AniName)const; // 애니메이션 찾기

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, E_MODEL_TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END