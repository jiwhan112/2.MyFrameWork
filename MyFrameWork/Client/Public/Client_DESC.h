#pragma 


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
	_int	mPosY = g_iWinCY *0.5f;
	_int	mSizeX = g_iWinCX;
	_int	mSizeY = g_iWinCY;
	_int	mDepth = 0;
	_float2 mPivot = {0.5f,0.5f};

}UI_DESC;


// 텍스처 이름 8개 저장
// 텍스처 맵에서 이름으로 매핑할 수 있게 한다.
typedef struct tag_TEXTUREDESC
{
	char mTextureKey_Diffuse[MAX_STR] = "";
	char mTextureKey_01[MAX_STR] = "";
	char mTextureKey_02[MAX_STR] = "";
	char mTextureKey_03[MAX_STR] = "";
	char mTextureKey_04[MAX_STR] = "";
	char mTextureKey_05[MAX_STR] = "";
	char mTextureKey_06[MAX_STR] = "";
	char mTextureKey_07[MAX_STR] = "";
}TEXTURE_DESC;


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

typedef struct tag_TERRAIN_DESC
{
	explicit tag_TERRAIN_DESC()
	{
		NumX = 129;
		NumZ = 129;
	}
	int NumX;
	int NumZ;

}TERRAIN_DESC;





