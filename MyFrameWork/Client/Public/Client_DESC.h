#pragma 


// UI ����
typedef struct tag_UIDESC
{
	_int mPosX = g_iWinCX * 0.5f;
	_int mPosY = g_iWinCY *0.5f;

	_int mSizeX = g_iWinCX;
	_int mSizeY = g_iWinCY;
	_int mDepth = 0;
	_float2 mPivot = {0.5f,0.5f};

}UIDESC;


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
}TEXTUREDESC;


