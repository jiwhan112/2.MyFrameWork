#pragma once

// 씬이름 정의
enum E_LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_MYGAMEPLAY,LEVEL_TOOL, LEVEL_END };




// 레이어 이름 정의
enum E_TAYLAY
{
	LAY_NONE,
	LAY_BACKGROUND,
	LAY_TERRAIN,
	LAY_CAMERA,
	LAY_MOUSE,
	LAY_PLAYER,
	LAY_OBJECT,
	LAY_EFFECT,
	LAY_UI,
	LAY_END
};




static const wchar_t* STR_TAYLAY(E_TAYLAY lay)
{
	switch (lay)
	{
	case LAY_NONE:
		return L"LAY_NONE";
	case LAY_BACKGROUND:
		return L"LAY_BACKGROUND";
	case LAY_TERRAIN:
		return L"LAY_TERRAIN";
	case LAY_CAMERA:
		return L"LAY_CAMERA";
	case LAY_MOUSE:
		return L"LAY_MOUSE";		
	case LAY_PLAYER:
		return L"LAY_PLAYER";
	case LAY_OBJECT:
		return L"LAY_OBJECT";
	case LAY_EFFECT:
		return L"LAY_EFFECT";
	case LAY_UI:
		return L"LAY_UI";
	case LAY_END:
		return L"LAY_END";
	default:
		return L"LAY";
	}
}


// 컴포넌트 프로토 이름 정의
enum E_TAYCOMPONENT
{
	COMPONENT_NONE,
	// Transform
	COMPONENT_TRANSFORM,

	// Renderer
	COMPONENT_RENDERER,

	// Model
	COMPONENT_MODEL,
	COMPONENT_MODEL_ANI,
	
	// COllider
	COMPONENT_COLLIDER_AABB,
	COMPONENT_COLLIDER_OBB,
	COMPONENT_COLLIDER_SPHERE,


	// VIBuffer
	COMPONENT_VIBUFFER_RECT,
	COMPONENT_VIBUFFER_CUBE,
	COMPONENT_VIBUFFER_TERRAIN,
	COMPONENT_VIBUFFER_MODEL,

	// Shsader
	COMPONENT_SHADER_VTXTEX,
	COMPONENT_SHADER_VTXTEXCUBE,
	COMPONENT_SHADER_VTXNORTEX,
	COMPONENT_SHADER_VTXMODEL,
	COMPONENT_SHADER_VTXANIMODEL,


	// Texture
	COMPONENT_TEXTURE_DEFAULT,
	COMPONENT_TEXTURE_SKY,
	COMPONENT_TEXTURE_TERRAIN,
	COMPONENT_TEXTURE_GRASS,
	COMPONENT_TEXTURE_BRUSH,
	COMPONENT_TEXTURE_FITER,
	COMPONENT_TEXTURE_MAP,
	COMPONENT_TEXTURE_MAP_FBX,
	COMPONENT_END,
};

static const wchar_t* STR_TAYCOM(E_TAYCOMPONENT com)
{
	switch (com)
	{
	case COMPONENT_TRANSFORM:
		return L"COMPONENT_TRANSFORM";
	case COMPONENT_RENDERER:
		return L"COMPONENT_RENDERER"; 

	case COMPONENT_MODEL:
		return L"COMPONENT_MODEL"; 
	case COMPONENT_MODEL_ANI:
		return L"COMPONENT_MODEL_ANI";

	case COMPONENT_COLLIDER_AABB:
		return L"COMPONENT_COLLIDER_AABB";
	case COMPONENT_COLLIDER_OBB:
		return L"COMPONENT_COLLIDER_OBB";
	case COMPONENT_COLLIDER_SPHERE:
		return L"COMPONENT_COLLIDER_SPHERE";

	case COMPONENT_VIBUFFER_RECT:
		return L"COMPONENT_VIBUFFER_RECT";
	case COMPONENT_VIBUFFER_CUBE:
		return L"COMPONENT_VIBUFFER_CUBE";	
	case COMPONENT_VIBUFFER_TERRAIN:
		return L"COMPONENT_VIBUFFER_TERRAIN";
	case COMPONENT_VIBUFFER_MODEL:
		return L"COMPONENT_VIBUFFER_MODEL";

	case COMPONENT_SHADER_VTXTEX:
		return L"COMPONENT_SHADER_VTXTEX";
	case COMPONENT_SHADER_VTXTEXCUBE:
		return L"COMPONENT_SHADER_VTXTEXCUBE";
	case COMPONENT_SHADER_VTXNORTEX:
		return L"COMPONENT_SHADER_VTXNORTEX";
	case COMPONENT_SHADER_VTXMODEL:
		return L"COMPONENT_SHADER_VTXMODEL";
	case COMPONENT_SHADER_VTXANIMODEL:
		return L"COMPONENT_SHADER_VTXANIMODEL";
	
	case COMPONENT_TEXTURE_DEFAULT:
		return L"COMPONENT_TEXTURE_DEFAULT";
	case COMPONENT_TEXTURE_SKY:
		return L"COMPONENT_TEXTURE_SKY";
	case COMPONENT_TEXTURE_TERRAIN:
		return L"COMPONENT_TEXTURE_TERRAIN";
	case COMPONENT_TEXTURE_GRASS:
		return L"COMPONENT_TEXTURE_GRASS";
	case COMPONENT_TEXTURE_BRUSH:
		return L"COMPONENT_TEXTURE_BRUSH";
	case COMPONENT_TEXTURE_FITER:
		return L"COMPONENT_TEXTURE_FITER";
	case COMPONENT_TEXTURE_MAP:
		return L"COMPONENT_TEXTURE_MAP";
	case COMPONENT_TEXTURE_MAP_FBX:
		return L"COMPONENT_TEXTURE_MAP_FBX";
		
	default:
		return L"COMPONENT";
	}
}



// 게임오브젝트 프로토 이름 정의
enum E_TAYGAMEOBJECT
{
	GAMEOBJECT_NONE,
	GAMEOBJECT_TEST,// 테스트 오브젝트 이름
	GAMEOBJECT_BACKGROUND,
	GAMEOBJECT_2D,
	GAMEOBJECT_3D_STATIC,
	GAMEOBJECT_3D_DYNAMIC,

	GAMEOBJECT_MOUSE,
	GAMEOBJECT_SKY,
	GAMEOBJECT_CAMERA,
	GAMEOBJECT_CAMERA_GAME,
	GAMEOBJECT_TERRAIN,
	GAMEOBJECT_MYTERRAIN,
	GAMEOBJECT_PLAYER,
	GAMEOBJECT_FBXTEST,
	GAMEOBJECT_FBXTEST_ANI,
	GAMEOBJECT_END,
};

static const wchar_t* STR_TAYOBJ(E_TAYGAMEOBJECT obj)
{
	switch (obj)
	{
	case GAMEOBJECT_TEST:
		return L"GAMEOBJECT_TEST";
	case GAMEOBJECT_2D:
		return L"GAMEOBJECT_2D";
	case GAMEOBJECT_3D_STATIC:
		return L"GAMEOBJECT_3D_STATIC";
	case GAMEOBJECT_3D_DYNAMIC:
		return L"GAMEOBJECT_3D_DYNAMIC";


	case GAMEOBJECT_MOUSE:
		return L"GAMEOBJECT_MOUSE";
		
		
	case GAMEOBJECT_BACKGROUND:
		return L"GAMEOBJECT_BACKGROUND";
	case GAMEOBJECT_CAMERA:
		return L"GAMEOBJECT_CAMERA";
	case GAMEOBJECT_CAMERA_GAME:
		return L"GAMEOBJECT_CAMERA_GAME";
	case GAMEOBJECT_PLAYER:
		return L"GAMEOBJECT_PLAYER";
	case GAMEOBJECT_TERRAIN:
		return L"GAMEOBJECT_TERRAIN";
	case GAMEOBJECT_MYTERRAIN:
		return L"GAMEOBJECT_MYTERRAIN";
	case GAMEOBJECT_FBXTEST:
		return L"GAMEOBJECT_FBXTEST";
	case GAMEOBJECT_FBXTEST_ANI:
		return L"GAMEOBJECT_FBXTEST_ANI";
	case GAMEOBJECT_SKY:
		return L"GAMEOBJECT_SKY";
		
	default:
		return L"GAMEOBJECT";
	}
}

// 데이터 타입 별 정보
enum E_OBJECT_DATA_TYPE
{
	OBJECT_TYPE_DATA_OBJECT,
	OBJECT_TYPE_DATA_UIDESC,
	OBJECT_TYPE_DATA_TEXTUREDESC,
	OBJECT_TYPE_DATA_END,
};


// 오브젝트 별 파일 정보
// 로드로 만드는 오브젝트는 모두 깡통오브젝트이다.
enum E_OBJECT_TYPE
{
	OBJECT_TYPE_2D,
	OBJECT_TYPE_3D_STATIC,
	OBJECT_TYPE_3D_ANI,
	OBJECT_TYPE_MOUSE,
	OBJECT_TYPE_TERRAIN,
	OBJECT_TYPE_END,

};

static const char* STR_TAGOBJ_TYPE(E_OBJECT_TYPE type)
{
	switch (type)
	{
	
	case OBJECT_TYPE_2D:
		return "OBJECT_TYPE_2D";
	case OBJECT_TYPE_3D_STATIC:
		return "OBJECT_TYPE_3D_STATIC";
	case OBJECT_TYPE_3D_ANI:
		return "OBJECT_TYPE_3D_ANI";
	case OBJECT_TYPE_TERRAIN:
		return "OBJECT_TYPE_TERRAIN";
	case OBJECT_TYPE_MOUSE:
		return "OBJECT_TYPE_MOUSE";
		
	case OBJECT_TYPE_END:
		return "";
	default:
		return "";
	}

}


#define  TAGLAY			STR_TAYLAY
#define  TAGCOM			STR_TAYCOM
#define  TAGOBJ			STR_TAYOBJ
#define  TAGOBJTYPE		STR_TAGOBJ_TYPE

