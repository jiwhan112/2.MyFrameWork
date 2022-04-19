#pragma 


// UI 정보
typedef struct tag_UIDESC
{
	_int mPosX;
	_int mPosY;
	_int mSizeX;
	_int mSizeY;
	_float2 mPivot;

}UIDESC;


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
}TEXTUREDESC;


