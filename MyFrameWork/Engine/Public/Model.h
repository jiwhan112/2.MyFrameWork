#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum E_MODEL_TYPE { MODEL_ANI,MODEL_NOANI,MODEL_END };

public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(E_MODEL_TYPE type, const char* ModelPath, const char* ModelName, _fmatrix defaultMatrix);
	virtual HRESULT NativeConstruct(void* pArg);
	HRESULT Render(class CShader* shader,_uint pass);


private:
	const aiScene*				mScene = nullptr;
	Importer					mImporter;

private:
	_uint									mNumMeshContainer = 0;
	vector<class CMeshContainer*>			mVectorMeshContainer;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;

	E_MODEL_TYPE							meModelType = MODEL_END;
	_float4x4								mTransformMatrix;

	_uint									mNumMaterials = 0;
	vector<MESHMATERIALDESC*>				mVectorMaterials;
	typedef vector<MESHMATERIALDESC*>		MATERIALS;

private:
	HRESULT Ready_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
		E_MODEL_TYPE eModelType, const char* modelPath, const char* modelName, _fmatrix defaultMatrix);
	virtual CModel* Clone(void* pArg) override;
	virtual void Free() override;
};

END