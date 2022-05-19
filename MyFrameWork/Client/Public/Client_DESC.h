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

// DESC 정보 구분
enum E_DESC_DATA
{
	DESC_DATA_WORLD, // 월드 행렬
	DESC_DATA_TEXTURENAME, // 텍스처이름 모음
	DESC_DATA_STRNAME, // 모델 이름
	DESC_DATA_COLLIDER, // Collider 
	DESC_DATA_TERRAIN, // Terrain
	DESC_DATA_NONE1,
	DESC_DATA_NONE2,
	DESC_DATA_END,
};

// 확장자 반환
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




// UI 정보
typedef struct tag_UIDESC
{
	explicit tag_UIDESC()
	{
		mPosX = g_iWinCX * 0.5f;
		mPosY = g_iWinCY * 0.5f;
		mSizeX = g_iWinCX;
		mSizeY = g_iWinCY;
		mDepth = 0;
		mPivot = { 0.5f,0.5f };
	};

	_int	mPosX = g_iWinCX * 0.5f;
	_int	mPosY = g_iWinCY * 0.5f;
	_int	mSizeX = g_iWinCX;
	_int	mSizeY = g_iWinCY;
	_int	mDepth = 0;
	_float2 mPivot = { 0.5f,0.5f };
}UI_DESC;

// 텍스처 이름 8개 저장
// 텍스처 맵에서 이름으로 매핑할 수 있게 한다.
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
}TEXTURE_MODEL_DESC;


typedef struct tag_MODEL_STATIC_DESC
{
	// FBX 이름
	char mModelName[MAX_STR] = "";
}MODEL_STATIC_DESC;

typedef struct tag_MODEL_DYNAMIC_DESC
{
	// FBX 이름
	char mModelName[MAX_STR] = "";
}MODEL_DYNAMIC_DESC;

typedef struct tag_COLLIDERODEL_DESC
{
	tag_COLLIDERODEL_DESC()
	{
		meColliderType = CCollider::E_COLLIDER_TYPE::COL_SPHERE;
		mSize = _float3(1, 1, 1);
	}
	CCollider::E_COLLIDER_TYPE meColliderType;
	_float3 mSize;
}COLLIDER_DESC;



typedef struct tag_MODELNAME_WORLD_DESC
{
	char mProtoName[MAX_STR] = "";
	_float4x4 mWorldMat;

}MODEL_WORLD_DESC;

typedef struct tag_TERRAIN_DESC
{
	explicit tag_TERRAIN_DESC()
	{
		meTerrainSize = TERRAINSIZE_16;
		mTextureMultiSize = 30;
		mObjectSize = 0;
		mModelObjects = nullptr;
	}

	E_TERRAINSIZE			meTerrainSize;
	_uint					mTextureMultiSize;

	_uint					mObjectSize;
	MODEL_WORLD_DESC*		mModelObjects;

}TERRAIN_DESC;
