#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "Shader.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, mNumMeshContainer(rhs.mNumMeshContainer)
	, mVectorMeshContainer(rhs.mVectorMeshContainer)
	, meModelType(rhs.meModelType)
	, mNumMaterials(rhs.mNumMaterials)
	, mVectorMaterials(rhs.mVectorMaterials)
	, mScene(rhs.mScene)


{
	for (auto& mesh : mVectorMeshContainer)
	{
		Safe_AddRef(mesh);
	}

	for (auto& mmatrial : mVectorMaterials)
	{
		for (auto& tex : mmatrial->pTexture)
		{
			Safe_AddRef(tex);
		}
	}	
}



HRESULT CModel::NativeConstruct_Prototype(E_MODEL_TYPE type, const char * ModelPath, const char * ModelName, _fmatrix defaultMatrix)
{
	char	szFullPath[MAX_PATH] = "";

	if (type == CModel::MODEL_END)
		return E_FAIL;
	meModelType = type;
	XMStoreFloat4x4(&mTransformMatrix, defaultMatrix);


	strcpy_s(szFullPath, ModelPath);
	strcat_s(szFullPath, ModelName);

	// 모델을 불러올 때 애니메이션 / 애니가 없는 모델을 분리해서 불러온다.
	_uint		iFlag = 0;

	// 좌표계 업데이트 / 삼각형으로 그림 / 탄젠트공간 계산 / 
	// 애니메이션이 아니면 파츠를 본에 같이 붙여줌
	
	if (meModelType == CModel::MODEL_NOANI)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	
	// aiScene 객체는 모든 메쉬 데이터를 저장한다. 
	// 모델 생성자는 aiScene을 초기화하는 것과 다름 없다.
	mScene = mImporter.ReadFile(szFullPath, iFlag);

	if (nullptr == mScene)
		return E_FAIL;

	// 이렇게 메쉬에 접근한다.
//	mScene->mNumMeshes;
//	mScene->mMeshes[0]->mNumVertices[];

	// Scene 정보로 한 모델의 메쉬 파츠를 저장한다.
	Ready_MeshContainers();
	Ready_Materials(ModelPath);
	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	return S_OK;
}



HRESULT CModel::Render(CShader* shader,_uint pass)
{
	if (mScene == nullptr)
		return E_FAIL;

	int count = 0;
	for (auto& mesh : mVectorMeshContainer)
	{
		// 텍스처를 넘기고 랜더링
		_uint matindex = mScene->mMeshes[count]->mMaterialIndex;
		mVectorMaterials[matindex]->pTexture[aiTextureType_DIFFUSE]->SetUp_OnShader(shader, SHADER_TEX_DIFFUSE, 0);
	//	mVectorMaterials[matindex]->pTexture[aiTextureType_NORMALS]->SetUp_OnShader(shader, SHADER_TEX_NOMAL, 0);
		mVectorMaterials[matindex]->pTexture[aiTextureType_HEIGHT]->SetUp_OnShader(shader, SHADER_TEX_NOMAL, 0);
		count++;
		mesh->Render(shader, pass);
	}
	return S_OK;
}



HRESULT CModel::Ready_MeshContainers()
{
	// 메쉬 파츠 생성

	if (nullptr == mScene)
		return E_FAIL;

	mNumMeshContainer = mScene->mNumMeshes;

	mVectorMeshContainer.reserve(mNumMeshContainer);

	for (_uint i = 0; i < mNumMeshContainer; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, meModelType, mScene->mMeshes[i], XMLoadFloat4x4(&mTransformMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		mVectorMeshContainer.push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	if (mScene == nullptr)
		return E_FAIL;
	
	// 재질의 총 개수 반환
	mNumMaterials = mScene->mNumMaterials;
	for (_uint i = 0; i < mNumMaterials; ++i)
	{
		MESHMATERIALDESC* meshDesc = NEW MESHMATERIALDESC;
		ZeroMemory(meshDesc, sizeof(MESHMATERIALDESC));

		// 재질 총 타입을 순회하면서 맞는 재질 업데이트
		for (_uint k = 0; k < AI_TEXTURE_TYPE_MAX; ++k)
		{
			// mseh의 텍스처 경로 반환
			aiString texturepath;
			if (FAILED(mScene->mMaterials[i]->GetTexture(aiTextureType(k), 0, &texturepath)))
				continue;

			// 텍스처 이름 추출
			char szFullPath[MAX_PATH] = "";
			char szFileName[MAX_PATH] = "";
			char szEXE[MAX_PATH] = "";

			_splitpath_s(texturepath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXE, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXE);

			// 유니코드 변경
			_tchar		szTextureFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFullPath, MAX_PATH);

			meshDesc->pTexture[k] = CTexture::Create(m_pDevice, m_pDeviceContext,szTextureFullPath,1);
			if (nullptr == meshDesc->pTexture[k])
				return E_FAIL;

		}
		mVectorMaterials.push_back(meshDesc);

	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, E_MODEL_TYPE eModelType, const char * modelPath, const char * modelName, _fmatrix defaultMatrix)
{
	CModel*	pInstance = NEW CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eModelType, modelPath, modelName, defaultMatrix)))
	{
		MSGBOX("Failed to Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel * CModel::Clone(void * pArg)
{
	CModel*	pInstance = NEW CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& mesh : mVectorMeshContainer)
	{
		Safe_Release(mesh);
	}

	for (auto& material : mVectorMaterials)
	{
		for (auto& tex : material->pTexture)
			Safe_Release(tex);

	}

	// 메쉬 해체
	if (m_isCloned == false)
	{

		for (auto& material : mVectorMaterials)
		{			
			Safe_Release(*material->pTexture);
			Safe_Delete(material);
		}

		mVectorMaterials.clear();
		mImporter.FreeScene();
	}
}
