#pragma once

// 씬이름 정의
enum E_LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_MYGAMEPLAY, LEVEL_TOOL, LEVEL_END };

// 레이어 이름 정의
enum E_TAYLAY
{
	LAY_NONE,
	LAY_BACKGROUND,
	LAY_TERRAIN_DUNGEON,
	LAY_TERRAIN_WORLD,
	LAY_CUBETILE,
	LAY_CAMERA,
	LAY_MOUSE,
	LAY_PLAYER,
	LAY_OBJECT,
	LAY_OBJECT_UNIT,
	LAY_OBJECT_D,
	LAY_OBJECT_W,
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
	case LAY_TERRAIN_DUNGEON:
		return L"LAY_TERRAIN_DUNGEON";
	case LAY_TERRAIN_WORLD:
		return L"LAY_TERRAIN_WORLD";
	case LAY_CUBETILE:
		return L"LAY_CUBETILE";
	case LAY_CAMERA:
		return L"LAY_CAMERA";
	case LAY_MOUSE:
		return L"LAY_MOUSE";
	case LAY_PLAYER:
		return L"LAY_PLAYER";
	case LAY_OBJECT:
		return L"LAY_OBJECT";
	case LAY_OBJECT_UNIT:
		return L"LAY_OBJECT_UNIT";
	case LAY_OBJECT_D:
		return L"LAY_OBJECT_D";
	case LAY_OBJECT_W:
		return L"LAY_OBJECT_W";
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
	// AI
	COMPONENT_BEHAVIORTREE,

	// Model
	COMPONENT_MODEL,
	COMPONENT_MODEL_ANI,

	// COllider
	COMPONENT_COLLIDER_AABB,
	COMPONENT_COLLIDER_OBB,
	COMPONENT_COLLIDER_SPHERE,

	// Navi
	COMPONENT_NAVIMESH,

	// VIBuffer
	COMPONENT_VIBUFFER_RECT,
	COMPONENT_VIBUFFER_CUBE,
	COMPONENT_VIBUFFER_TERRAIN,
	COMPONENT_VIBUFFER_TERRAIN_16,
	COMPONENT_VIBUFFER_TERRAIN_32,
	COMPONENT_VIBUFFER_TERRAIN_64,
	COMPONENT_VIBUFFER_TERRAIN_128,
	COMPONENT_VIBUFFER_MODEL,
	COMPONENT_VIBUFFER_INSTANCE_POINT,

	// Shsader
	COMPONENT_SHADER_VTXTEX,
	COMPONENT_SHADER_VTXTEXCUBE,
	COMPONENT_SHADER_TERRAIN,
	COMPONENT_SHADER_VTXMODEL,
	COMPONENT_SHADER_VTXANIMODEL,
	COMPONENT_SHADER_INSTANCE_POINT,

	// Texture
	COMPONENT_TEXTURE_DEFAULT,
	COMPONENT_TEXTURE_SKY,
	COMPONENT_TEXTURE_DEFAULT_FLOOR,

	// FiterTest Texture
	//COMPONENT_TEXTURE_FITER1,
	//COMPONENT_TEXTURE_FITER2,
	//COMPONENT_TEXTURE_FITER3,
	//COMPONENT_TEXTURE_FITER4,
	//COMPONENT_TEXTURE_FITER_XYZW,
	COMPONENT_TEXTURE_HEIGHT,

	//COMPONENT_TEXTURE_BRUSH,


	// TextureMap
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
	case COMPONENT_BEHAVIORTREE:
		return L"COMPONENT_BEHAVIORTREE";
		
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

	case COMPONENT_NAVIMESH:
		return L"COMPONENT_NAVIMESH";

	case COMPONENT_VIBUFFER_RECT:
		return L"COMPONENT_VIBUFFER_RECT";
	case COMPONENT_VIBUFFER_CUBE:
		return L"COMPONENT_VIBUFFER_CUBE";
	case COMPONENT_VIBUFFER_TERRAIN:
		return L"COMPONENT_VIBUFFER_TERRAIN";
	case COMPONENT_VIBUFFER_TERRAIN_16:
		return L"COMPONENT_VIBUFFER_TERRAIN_16";
	case COMPONENT_VIBUFFER_TERRAIN_32:
		return L"COMPONENT_VIBUFFER_TERRAIN_32";
	case COMPONENT_VIBUFFER_TERRAIN_64:
		return L"COMPONENT_VIBUFFER_TERRAIN_64";
	case COMPONENT_VIBUFFER_TERRAIN_128:
		return L"COMPONENT_VIBUFFER_TERRAIN_128";

	case COMPONENT_VIBUFFER_MODEL:
		return L"COMPONENT_VIBUFFER_MODEL";
	case COMPONENT_VIBUFFER_INSTANCE_POINT:
		return L"COMPONENT_VIBUFFER_INSTANCE_POINT";


	case COMPONENT_SHADER_VTXTEX:
		return L"COMPONENT_SHADER_VTXTEX";
	case COMPONENT_SHADER_VTXTEXCUBE:
		return L"COMPONENT_SHADER_VTXTEXCUBE";
	case COMPONENT_SHADER_TERRAIN:
		return L"COMPONENT_SHADER_VTXNORTEX";
	case COMPONENT_SHADER_VTXMODEL:
		return L"COMPONENT_SHADER_VTXMODEL";
	case COMPONENT_SHADER_VTXANIMODEL:
		return L"COMPONENT_SHADER_VTXANIMODEL";
	case COMPONENT_SHADER_INSTANCE_POINT:
		return L"COMPONENT_SHADER_INSTANCE_POINT";

	case COMPONENT_TEXTURE_DEFAULT:
		return L"COMPONENT_TEXTURE_DEFAULT";
	case COMPONENT_TEXTURE_SKY:
		return L"COMPONENT_TEXTURE_SKY";
			   
	case COMPONENT_TEXTURE_DEFAULT_FLOOR:
		return L"COMPONENT_TEXTURE_DEFAULT_FLOOR";
	//case COMPONENT_TEXTURE_FITER1:
	//	return L"COMPONENT_TEXTURE_FITER1";
	//case COMPONENT_TEXTURE_FITER2:
	//	return L"COMPONENT_TEXTURE_FITER2";
	//case COMPONENT_TEXTURE_FITER3:
	//	return L"COMPONENT_TEXTURE_FITER3";
	//case COMPONENT_TEXTURE_FITER4:
	//	return L"COMPONENT_TEXTURE_FITER4";
	//case COMPONENT_TEXTURE_FITER_XYZW:
	//	return L"COMPONENT_TEXTURE_FITER_XYZW";
	//case COMPONENT_TEXTURE_BRUSH:
	//	return L"COMPONENT_TEXTURE_BRUSH";
	case COMPONENT_TEXTURE_HEIGHT:
		return L"COMPONENT_TEXTURE_HEIGHT";	
		
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
	GAMEOBJECT_3D_STATIC2,
	GAMEOBJECT_3D_DYNAMIC,

	GAMEOBJECT_3D_DYNAMIC_MINE,
	GAMEOBJECT_3D_DYNAMIC_GOBLIN,
	GAMEOBJECT_3D_DYNAMIC_ORC,
	GAMEOBJECT_3D_DYNAMIC_ENEMY,
	GAMEOBJECT_3D_DYNAMIC_BOSS,

	GAMEOBJECT_3D_TILE,
	GAMEOBJECT_3D_SOCKET,
	GAMEOBJECT_3D_ENVIRONMENT,


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
	case GAMEOBJECT_3D_STATIC2:
		return L"GAMEOBJECT_3D_STATIC2";
	case GAMEOBJECT_3D_TILE:
		return L"GAMEOBJECT_3D_TILE";
	case GAMEOBJECT_3D_SOCKET:
		return L"GAMEOBJECT_3D_SOCKET";
	case GAMEOBJECT_3D_ENVIRONMENT:
		return L"GAMEOBJECT_3D_ENVIRONMENT";
		

	case GAMEOBJECT_3D_DYNAMIC:
		return L"GAMEOBJECT_3D_DYNAMIC";
	case GAMEOBJECT_3D_DYNAMIC_MINE:
		return L"GAMEOBJECT_3D_DYNAMIC_MINE";
	case GAMEOBJECT_3D_DYNAMIC_GOBLIN:
		return L"GAMEOBJECT_3D_DYNAMIC_GOBLIN";
	case GAMEOBJECT_3D_DYNAMIC_ORC:
		return L"GAMEOBJECT_3D_DYNAMIC_ORC";
	case GAMEOBJECT_3D_DYNAMIC_ENEMY:
		return L"GAMEOBJECT_3D_DYNAMIC_ENEMY";
	case GAMEOBJECT_3D_DYNAMIC_BOSS:
		return L"GAMEOBJECT_3D_DYNAMIC_BOSS";

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


// 오브젝트 별 파일 정보
// 로드로 만드는 오브젝트는 모두 깡통오브젝트이다.
enum E_OBJECT_TYPE
{
	OBJECT_TYPE_NONE=0,

	OBJECT_TYPE_2D=1,
	OBJECT_TYPE_2D_PARTICLE,
	OBJECT_TYPE_2D_PARTICLE_POINT,
	OBJECT_TYPE_2D_PARTICLE_RECT,

	OBJECT_TYPE_3D_STATIC,
	OBJECT_TYPE_3D_STATIC_PARENT,
	OBJECT_TYPE_3D_STATIC_TILES,
	OBJECT_TYPE_3D_SOCKET,


	OBJECT_TYPE_3D_DYNAMIC,
	OBJECT_TYPE_3D_DYNAMIC_MINE,
	OBJECT_TYPE_3D_DYNAMIC_GOBLIN,
	OBJECT_TYPE_3D_DYNAMIC_ORC,
	OBJECT_TYPE_3D_DYNAMIC_ENEMY,
	OBJECT_TYPE_3D_DYNAMIC_BOSS,

	
	OBJECT_TYPE_MOUSE,
	OBJECT_TYPE_TERRAIN,
	OBJECT_TYPE_CAMERA,
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
	case OBJECT_TYPE_3D_STATIC_PARENT:
		return "OBJECT_TYPE_3D_STATIC_PARENT";
	case OBJECT_TYPE_3D_STATIC_TILES:
		return "OBJECT_TYPE_3D_STATIC_TILES";
	case OBJECT_TYPE_3D_SOCKET:
		return "OBJECT_TYPE_3D_SOCKET";


	case OBJECT_TYPE_3D_DYNAMIC:
		return "OBJECT_TYPE_3D_DYNAMIC";
	case OBJECT_TYPE_3D_DYNAMIC_MINE:
		return "OBJECT_TYPE_3D_DYNAMIC_MINE";
	case OBJECT_TYPE_3D_DYNAMIC_GOBLIN:
		return "OBJECT_TYPE_3D_DYNAMIC_GOBLIN";
	case OBJECT_TYPE_3D_DYNAMIC_ORC:
		return "OBJECT_TYPE_3D_DYNAMIC_ORC";
	case OBJECT_TYPE_3D_DYNAMIC_ENEMY:
		return "OBJECT_TYPE_3D_DYNAMIC_ENEMY";
	case OBJECT_TYPE_3D_DYNAMIC_BOSS:
		return "OBJECT_TYPE_3D_DYNAMIC_BOSS";


	case OBJECT_TYPE_TERRAIN:
		return "OBJECT_TYPE_TERRAIN";
	case OBJECT_TYPE_MOUSE:
		return "OBJECT_TYPE_MOUSE";
		
	case OBJECT_TYPE_CAMERA:
		return "OBJECT_TYPE_CAMERA";
	case OBJECT_TYPE_END:
		return "";
	default:
		return "";
	}
}

// FONT
enum E_FONT_TYPE
{
	FONT_ARIAL,
	FONT_BRID,
	FONT_CALIBRI,
	FONT_END,
};

static const wchar_t* STR_TAYFONT(E_FONT_TYPE str)
{
	switch (str)
	{
		case FONT_ARIAL:
			return L"FONT_ARIAL";
			break;
		case FONT_BRID:
			return L"FONT_BRID";
			break;
		case FONT_CALIBRI:
			return L"FONT_CALIBRI";
			break;
		case FONT_END:
			return L"FONT_END";
			break;
		default:
			break;
	}

}


// 랜더링 패스
enum E_SHADERPASS_STATICMODEL
{
	E_SHADERPASS_STATICMODEL_DEFAULT = 0,
	E_SHADERPASS_STATICMODEL_RED = 1,
	E_SHADERPASS_STATICMODEL_END = 1,
};

// AI 타입
enum E_AI_TYPE
{
	AI_NONE = 0,
	AI_DEALY, 
	AI_MOVE, 
	AI_MOVETARGET,
	AI_FUNCTION,
	AI_END, 
};

static const char* STR_TAYAI(E_AI_TYPE str)
{
	switch (str)
	{
	case AI_NONE:
		return "AI_NONE";
		break;
	case AI_DEALY:
		return "AI_DEALY";
		break;
	case AI_MOVE:
		return "AI_MOVE";
		break;
	case AI_MOVETARGET:
		return "AI_MOVETARGET";
		break;
	case AI_FUNCTION:
		return "AI_FUNCTION";
	default:
		return "";
		break;
	}

}
enum E_DECO_TYPE
{
	DECO_NONE = 0,
	DECO_MINUS,
	DECO_END,
};
static const char* STR_TAYDECO(E_DECO_TYPE str)
{
	switch (str)
	{
	case DECO_NONE:
		return "DECO_NONE";
		break;
	case DECO_MINUS:
		return "DECO_MINUS";
		break;
	case DECO_END:
		break;
	default:
		return "";
		break;
	}

}


// 소켓 타입
enum E_SOKET_TYPE
{
	SOCKET_NONE = 0,

	// 무기 뼈 
	SOCKET_WEAPON_1,
	SOCKET_WEAPON_2,
	SOCKET_WEAPON_3,
	SOCKET_WEAPON_4,
	SOCKET_WEAPON_5,
	SOCKET_WEAPON_6,
	SOCKET_WEAPON_7,
	SOCKET_WEAPON_8,

	// 파티클 뼈
	SOCKET_PARTILCE_1,
	SOCKET_PARTILCE_2,
	SOCKET_PARTILCE_3,
	SOCKET_PARTILCE_4,
	SOCKET_PARTILCE_5,
	SOCKET_PARTILCE_6,
	SOCKET_PARTILCE_7,
	SOCKET_PARTILCE_8,
	SOCKET_PARTILCE_9,
	SOCKET_PARTILCE_10,

	// 충돌용 뼈
	SOCKET_ANY_1,
	SOCKET_ANY_2,
	SOCKET_ANY_3,
	SOCKET_ANY_4,
	SOCKET_ANY_5,

	SOCKET_END,

};

static const char* STR_TAYSOCKET(E_SOKET_TYPE str)
{
	switch (str)
	{
	case SOCKET_NONE:
		return "SOCKET_NONE";

	case SOCKET_WEAPON_1:
		return "SOCKET_WEAPON_1";
		break;
	case SOCKET_WEAPON_2:
		return "SOCKET_WEAPON_2";
		break;
	case SOCKET_WEAPON_3:
		return "SOCKET_WEAPON_3";
		break;
	case SOCKET_WEAPON_4:
		return "SOCKET_WEAPON_4";
		break;
	case SOCKET_WEAPON_5:
		return "SOCKET_WEAPON_5";
		break;
	case SOCKET_WEAPON_6:
		return "SOCKET_WEAPON_6";
		break;
	case SOCKET_WEAPON_7:
		return "SOCKET_WEAPON_7";
		break;
	case SOCKET_WEAPON_8:
		return "SOCKET_WEAPON_8";
		break;
	case SOCKET_PARTILCE_1:
		return "SOCKET_PARTILCE_1";
		break;
	case SOCKET_PARTILCE_2:
		return "SOCKET_PARTILCE_2";
		break;
	case SOCKET_PARTILCE_3:
		return "SOCKET_PARTILCE_3";
		break;
	case SOCKET_PARTILCE_4:
		return "SOCKET_PARTILCE_4";
		break;
	case SOCKET_PARTILCE_5:
		return "SOCKET_PARTILCE_5";
		break;
	case SOCKET_PARTILCE_6:
		return "SOCKET_PARTILCE_6";
		break;
	case SOCKET_PARTILCE_7:
		return "SOCKET_PARTILCE_7";
		break;
	case SOCKET_PARTILCE_8:
		return "SOCKET_PARTILCE_8";
		break;
	case SOCKET_PARTILCE_9:
		return "SOCKET_PARTILCE_9";
		break;
	case SOCKET_PARTILCE_10:
		return "SOCKET_PARTILCE_10";
		break;
	case SOCKET_ANY_1:
		return "SOCKET_ANY_1";
		break;
	case SOCKET_ANY_2:
		return "SOCKET_ANY_2";
		break;
	case SOCKET_ANY_3:
		return "SOCKET_ANY_3";
		break;
	case SOCKET_ANY_4:
		return "SOCKET_ANY_4";
		break;
	case SOCKET_ANY_5:
		return "SOCKET_ANY_5";
		break;
	case SOCKET_END:
		return "SOCKET_END";
		break;
	default:
		return "";
		break;
	

	}
}

#define  TAGLAY			STR_TAYLAY
#define  TAGCOM			STR_TAYCOM
#define  TAGOBJ			STR_TAYOBJ
#define  TAGOBJTYPE		STR_TAGOBJ_TYPE
#define  TAGFONT		STR_TAYFONT
#define  TAGAI			STR_TAYAI
#define  TAGSOCKET		STR_TAYSOCKET
#define  TAGDECO		STR_TAYDECO
