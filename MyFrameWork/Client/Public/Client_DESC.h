#pragma 


// UI ����
typedef struct tag_UIDESC
{
	_int mPosX;
	_int mPosY;
	_int mSizeX;
	_int mSizeY;
	_float2 mPivot;

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


