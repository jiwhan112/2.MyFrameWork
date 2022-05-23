#pragma once

#include "Component.h"
#include "Animatior.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	// 소켓에 해당 뼈 행렬 넘길떄 사용
	typedef struct tagBoneMatrixPtr
	{
		_float4x4*			pOffsetMatrix = nullptr;
		_float4x4*			pCombinedMatrix = nullptr;
	}BONEMATRIX_PTR;

	enum E_MODEL_TYPE { MODEL_NOANI, MODEL_ANI, MODEL_END };
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMaterials() const;
	const vector<class CHierarchyNode*>* Get_VecHierarchy()const { return &m_HierarchyNodes; }
	class CAnimatior* Get_Animaitor() const
	{
		return mAnimator;
	}
	const _float4x4& Get_DefaultMatrix() const { return mDefaultMatrix; }
	BONEMATRIX_PTR Get_BoneMatrixPtr(string bonename);
	

public:
	virtual HRESULT NativeConstruct_Prototype(E_MODEL_TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT SetUp_AnimIndex(_uint iAnimIndex);
	HRESULT SetUp_AnimName(string tag);
	HRESULT SetUp_AnimEnum(CAnimatior::E_COMMON_ANINAME e);

	HRESULT Update_CombinedTransformationMatrices(_double TimeDelta);
	HRESULT Bind_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pValueName);
	HRESULT Render(class CShader* pShader, _uint iPassIndex, _uint iMaterialIndex, const char* pBoneValueName = nullptr);

private:
	const aiScene*				m_pScene = nullptr;
	Importer					m_Importer;

private: // 메시
	_uint									m_iNumMeshContainers = 0;
	vector<class CMeshContainer*>*			m_pMeshContainers = nullptr;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	E_MODEL_TYPE							m_eModelType = MODEL_END;
	_float4x4								mDefaultMatrix;


	// 계층 // 영향을 주는 뼈정보 세팅 // 메시프로토 생성시 필요
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

private: // 텍스처
	_uint									m_iNumMaterials = 0;
	vector<MESHMATERIALDESC*>				m_Materials;
	typedef vector<MESHMATERIALDESC*>		MATERIALS;


private: // 에니메이터
	class CAnimatior*						mAnimator = nullptr;

private:
	HRESULT Ready_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth);

private:
	CHierarchyNode* Find_HierarchyNode(const char* pName, _uint* pOut = nullptr);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, E_MODEL_TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END