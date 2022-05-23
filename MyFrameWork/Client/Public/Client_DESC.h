#pragma

// OTHER Data
enum E_TERRAINSIZE
{
	TERRAINSIZE_16,
	TERRAINSIZE_32,
	TERRAINSIZE_64,
	TERRAINSIZE_128,
	TERRAINSIZE_END,
};

// DESC DATA

// DESC ���� ����
enum E_DESC_DATA
{
	// ���� ������ Ÿ��
	DESC_DATA_WORLD, // ���� ���
	DESC_DATA_TEXTURENAME, // �ؽ�ó�̸� ����
	DESC_DATA_STRNAME, // �� �̸�
	DESC_DATA_COLLIDER, // Collider 

	// �����Ͱ� �ʿ��� Ÿ��
	DESC_DATA_TERRAIN, // Terrain
	DESC_DATA_END,
};

// Ȯ���� ��ȯ
static const _tchar* STR_DATADESC_EXE(E_DESC_DATA type)
{
	switch (type)
	{
	case DESC_DATA_WORLD:
		return L".matdat";
		break;
	case DESC_DATA_TEXTURENAME:
		return L".texdat";;
	case DESC_DATA_STRNAME:
		return L".strdat";;
	case DESC_DATA_COLLIDER:
		return L".coldat";;
	case DESC_DATA_TERRAIN:
		return L".terrdat";
	default:
		return L"";
		break;

	}
	return L"";
}


#define  DESCEXE		STR_DATADESC_EXE


//typedef struct tag_BaseDesc
//{
//	virtual tag_BaseDesc* Clone()PURE;
//}BASE_DESC;


// UI ����
typedef struct tag_UIDESC
{
	 tag_UIDESC()
	{
		mPosX = g_iWinCX * 0.5f;
		mPosY = g_iWinCY * 0.5f;
		mSizeX = g_iWinCX;
		mSizeY = g_iWinCY;
		mDepth = 0;
		mPivot = { 0.5f,0.5f };
	};

	//virtual tag_UIDESC* Clone()override
	//{
	//	tag_UIDESC* desc = NEW tag_UIDESC;

	//	desc->mPosX = mPosX;
	//	desc->mPosY = mPosY;

	//	desc->mSizeX = mSizeX;
	//	desc->mSizeY = mSizeY;

	//	desc->mDepth = mDepth;
	//	desc->mPivot = mPivot;
	//	return desc;

	//}


	_int	mPosX = g_iWinCX * 0.5f;
	_int	mPosY = g_iWinCY * 0.5f;
	_int	mSizeX = g_iWinCX;
	_int	mSizeY = g_iWinCY;
	_int	mDepth = 0;
	_float2 mPivot = { 0.5f,0.5f };
}UI_DESC;

// �ؽ�ó �̸� 8�� ����
// �ؽ�ó �ʿ��� �̸����� ������ �� �ְ� �Ѵ�.
typedef struct tag_TEXTUREDESC_UI
{
	char mTextureKey_Diffuse[MAX_STR] = "";
	char mTextureKey_01[MAX_STR] = "";
	char mTextureKey_02[MAX_STR] = "";
	char mTextureKey_03[MAX_STR] = "";
	char mTextureKey_04[MAX_STR] = "";
	char mTextureKey_05[MAX_STR] = "";
	char mTextureKey_06[MAX_STR] = "";
	char mTextureKey_07[MAX_STR] = "";

}TEXTURE_UI_DESC;

typedef struct tag_TEXTUREDESC_MODEL
{
	char mTextureKey_Diffuse[MAX_STR] = "";
	char mTextureKey_Normal[MAX_STR] = "";
	char mTextureKey_Hieght[MAX_STR] = "";
	char mTextureKey_AA[MAX_STR] = "";

	/* tag_TEXTUREDESC_MODEL() = default;

	 tag_TEXTUREDESC_MODEL(const tag_TEXTUREDESC_MODEL& rhs)
	{
		strcpy_s(mTextureKey_Diffuse, rhs.mTextureKey_Diffuse);
		strcpy_s(mTextureKey_Normal, rhs.mTextureKey_Normal);
		strcpy_s(mTextureKey_Hieght, rhs.mTextureKey_Hieght);
		strcpy_s(mTextureKey_AA, rhs.mTextureKey_AA);
	}*/

}TEXTURE_MODEL_DESC;


typedef struct tag_MODEL_STATIC_DESC
{
	// FBX �̸�
	char mModelName[MAX_STR] = "";
	 tag_MODEL_STATIC_DESC() = default;

	 /* tag_MODEL_STATIC_DESC(const tag_MODEL_STATIC_DESC& rhs)
	 {
		 strcpy_s(mModelName, rhs.mModelName);
	 }*/

}MODEL_STATIC_DESC;

typedef struct tag_MODEL_DYNAMIC_DESC
{
	// FBX �̸�
	char mModelName[MAX_STR] = "";
	 tag_MODEL_DYNAMIC_DESC() = default;

	// tag_MODEL_DYNAMIC_DESC(const tag_MODEL_DYNAMIC_DESC& rhs)
	//{
	//	strcpy_s(mModelName, rhs.mModelName);
	//}

}MODEL_DYNAMIC_DESC;

typedef struct tag_COLLIDERODEL_DESC
{
	tag_COLLIDERODEL_DESC()
	{
		meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
		mSize = _float3(1, 1, 1);
		mOffset = _float3(0,0,0);
	}

	CCollider::E_COLLIDER_TYPE meColliderType;
	_float3 mOffset;
	_float3 mSize;

}COLLIDER_DESC;



typedef struct tag_MODELNAME_WORLD_DESC
{
	char mProtoName[MAX_STR] = "";
	_float4x4 mWorldMat;


	/* tag_MODELNAME_WORLD_DESC(const tag_MODELNAME_WORLD_DESC& rhs)
	{
		strcpy_s(mProtoName, rhs.mProtoName);
		mWorldMat = rhs.mWorldMat;
	}*/


}MODEL_WORLD_DESC;

typedef struct tag_TERRAIN_DESC // : public BASE_DESC
{
	tag_TERRAIN_DESC()
	{
		meTerrainSize = TERRAINSIZE_32;
		mTextureMultiSize = 1;

		mNoTileSize = 0;
		mArrayIndes = nullptr;

		mObjectSize = 0;
		mArrayModelObjects = nullptr;
	}

	// tag_BaseDesc��(��) ���� ��ӵ�
	//virtual tag_TERRAIN_DESC* Clone()override
	//{
	//	tag_TERRAIN_DESC* desc = NEW tag_TERRAIN_DESC;

	//	desc->meTerrainSize = meTerrainSize;
	//	desc->mTextureMultiSize = mTextureMultiSize;

	//	if (mArrayIndes)
	//	{
	//		desc->mNoTileSize = mNoTileSize;
	//		desc->mArrayIndes = NEW _uint[mNoTileSize];

	//		for (int i = 0; i < mNoTileSize; ++i)
	//		{
	//			_uint data = mArrayIndes[i];
	//			desc->mArrayIndes[i] = data;
	//		}
	//	}

	//	if (mArrayModelObjects)
	//	{
	//		desc->mObjectSize = mObjectSize;
	//		desc->mArrayModelObjects = NEW MODEL_WORLD_DESC[mObjectSize];

	//		for (int i = 0; i < mObjectSize; ++i)
	//		{
	//			desc->mArrayModelObjects[i] = mArrayModelObjects[i];
	//		}
	//	}
	//	return desc;
	//}

	~tag_TERRAIN_DESC();

	E_TERRAINSIZE			meTerrainSize;
	_uint					mTextureMultiSize;

	// Ÿ�� ������Ʈ ���� / ���� Ÿ���� �ε����� �����Ѵ�.
	_uint					mNoTileSize;
	_uint*					mArrayIndes = nullptr;

	// Ÿ�� �̿��� ������Ʈ
	_uint					mObjectSize;
	MODEL_WORLD_DESC*		mArrayModelObjects = nullptr;

}TERRAIN_DESC;

