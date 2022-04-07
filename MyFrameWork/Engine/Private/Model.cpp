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
	, mpVectorMeshContainers(rhs.mpVectorMeshContainers)
	, meModelType(rhs.meModelType)
	, mNumMaterials(rhs.mNumMaterials)
	, mVectorMaterials(rhs.mVectorMaterials)

{
	// 재질 벡터 복사
	for (auto& mmatrial : mVectorMaterials)
	{
		for (auto& tex : mmatrial->pTexture)
		{
			Safe_AddRef(tex);
		}
	}

	// 메시 벡터들 복사
	if(mpVectorMeshContainers != nullptr)
	{
		for (_uint i = 0; i<mNumMaterials;++i)
		{
			for (auto& mesh : mpVectorMeshContainers[i])
			{
				Safe_AddRef(mesh);
			}
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
	if (mScene == nullptr)
		return E_FAIL;

	// Scene 정보로 한 모델의 메쉬 파츠를 저장한다.
	FAILED_CHECK(Ready_Materials(ModelPath));
	FAILED_CHECK(Ready_MeshContainers());

	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	return S_OK;
}



HRESULT CModel::Bind_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pValueName)
{
	if (iMaterialIndex >= mNumMaterials)
		return E_FAIL;

	// 각 텍스처 바인딩
	CTexture*		pTexture = mVectorMaterials[iMaterialIndex]->pTexture[eTextureType];
	if (nullptr == pTexture)
		return E_FAIL;
	
	return pTexture->SetUp_OnShader(pShader, pValueName);
}

// 재질마다 다른 셰이더로 그려줄 수 있음.
HRESULT CModel::Render(CShader* shader,_uint pass, _uint iMaterialIndex)
{
	// 바인딩이 끝나면 메쉬의 개수가 아닌 재질의 개수로 루프를 돈다.
	if (iMaterialIndex >= mNumMaterials ||
		mpVectorMeshContainers == nullptr)
		return E_FAIL;


	// 메시컨테이너들을 저장하는 컨테이너를 재질 개수만큼 만들고
	// 메시컨테이너를 재질 개수만큼 호출시킨다.
	for (auto& pMeshContainer : mpVectorMeshContainers[iMaterialIndex])
	{
		// 기존 같이 메시버퍼 랜더링
		if (pMeshContainer != nullptr)
			pMeshContainer->Render(shader, pass);
	}
	return S_OK;
}



HRESULT CModel::Ready_MeshContainers()
{
	// 메쉬 파츠 초기화

	if (nullptr == mScene)
		return E_FAIL;

	// 메시 컨테이너 벡터를 재질의 총 개수만큼 동적할당.
	mNumMeshContainer = mScene->mNumMeshes;
	mpVectorMeshContainers = NEW MESHCONTAINERS[mNumMaterials];

	// 메시 개수 만큼
	for (_uint i = 0; i < mNumMeshContainer; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, meModelType, mScene->mMeshes[i], XMLoadFloat4x4(&mTransformMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		// 각 메시가 가지고 있는 재질 번호로 메시를 넣는다.
		// 즉 같은 번호의 텍스처를 사용해 그리는 객체는 같은 벡터[ID]에 저장된다.
		mpVectorMeshContainers[mScene->mMeshes[i]->mMaterialIndex].push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	// 재질 초기화
	if (mScene == nullptr)
		return E_FAIL;
	
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

	// 재질 해제
	for (auto& mat : mVectorMaterials)
	{
		for (auto& tex : mat->pTexture)
			Safe_Release(tex);

		if (m_isCloned == false)
			Safe_Delete(mat);	
	}
	mVectorMaterials.clear();


	// 메쉬 해제
	if (mpVectorMeshContainers != nullptr)
	{
		for (_uint i = 0; i < mNumMaterials; ++i)
		{
			for (auto& mesh : mpVectorMeshContainers[i])
				Safe_Release(mesh);

			if (m_isCloned == false)
				mpVectorMeshContainers[i].clear();
		}
	}

	if (false == m_isCloned)
	{
		Safe_Delete_Array(mpVectorMeshContainers);
	}

	// 하이어라키 해제

	mImporter.FreeScene();

}
