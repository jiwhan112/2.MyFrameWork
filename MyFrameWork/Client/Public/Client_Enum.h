#pragma once

// 씬이름 정의
enum E_LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_TOOL, LEVEL_END };




// 레이어 이름 정의
enum E_TAYLAY
{
	LAY_NONE,
	LAY_BACKGROUND,
	LAY_CAMERA,
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
	case LAY_CAMERA:
		return L"LAY_CAMERA";
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

	// VIBuffer
	COMPONENT_VIBUFFER_RECT,
	COMPONENT_VIBUFFER_CUBE,
	COMPONENT_VIBUFFER_TERRAIN,
	// Shsader
	COMPONENT_SHADER_VTXTEX,
	COMPONENT_SHADER_VTXTEXCUBE,
	COMPONENT_SHADER_VTXNORTEX,


	// Texture
	COMPONENT_TEXTURE_DEFAULT,
	COMPONENT_TEXTURE_SKY,
	COMPONENT_TEXTURE_TERRAIN,
	COMPONENT_TEXTURE_GRASS,
	COMPONENT_TEXTURE_BRUSH,
	COMPONENT_TEXTURE_FITER,
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

	case COMPONENT_VIBUFFER_RECT:
		return L"COMPONENT_VIBUFFER_RECT";
	case COMPONENT_VIBUFFER_CUBE:
		return L"COMPONENT_VIBUFFER_CUBE";	
	case COMPONENT_VIBUFFER_TERRAIN:
		return L"COMPONENT_VIBUFFER_TERRAIN";

	case COMPONENT_SHADER_VTXTEX:
		return L"COMPONENT_SHADER_VTXTEX";
	case COMPONENT_SHADER_VTXTEXCUBE:
		return L"COMPONENT_SHADER_VTXTEXCUBE";
	case COMPONENT_SHADER_VTXNORTEX:
		return L"COMPONENT_SHADER_VTXNORTEX";
	
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
	default:
		return L"COMPONENT";
	}
}



// 게임오브젝트 프로토 이름 정의
enum E_TAYGAMEOBJECT
{
	GAMEOBJECT_NONE,
	GAMEOBJECT_BACKGROUND,
	GAMEOBJECT_SKY,
	GAMEOBJECT_CAMERA,
	GAMEOBJECT_TERRAIN,
	GAMEOBJECT_PLAYER,
	GAMEOBJECT_END,
};

static const wchar_t* STR_TAYOBJ(E_TAYGAMEOBJECT obj)
{
	switch (obj)
	{
	case GAMEOBJECT_BACKGROUND:
		return L"GAMEOBJECT_BACKGROUND";
	case GAMEOBJECT_CAMERA:
		return L"GAMEOBJECT_CAMERA";
	case GAMEOBJECT_PLAYER:
		return L"GAMEOBJECT_PLAYER";
	case GAMEOBJECT_TERRAIN:
		return L"GAMEOBJECT_TERRAIN";

	case GAMEOBJECT_SKY:
		return L"GAMEOBJECT_SKY";
		
	default:
		return L"GAMEOBJECT";
	}
}

#define  TAGLAY STR_TAYLAY
#define  TAGCOM STR_TAYCOM
#define  TAGOBJ STR_TAYOBJ