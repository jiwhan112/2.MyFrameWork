#pragma 


// UI ����
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


// �ؽ�ó �̸� 8�� ����
// �ؽ�ó �ʿ��� �̸����� ������ �� �ְ� �Ѵ�.
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
	// FBX �̸�
	char mModelName[MAX_STR] = "";

}MODEL_STATIC_DESC;


typedef struct tag_MODEL_DYNAMIC_DESC
{
	// FBX �̸�
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





