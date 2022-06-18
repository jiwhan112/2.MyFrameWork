#pragma once

namespace Engine
{
	enum EasingTypeID
	{
		TYPE_Linear = 0,

		TYPE_QuadIn = 1,
		TYPE_QuadOut,
		TYPE_QuadInOut,

		TYPE_CubicIn = 4,
		TYPE_CubicOut,
		TYPE_CubicInOut,

		TYPE_QuarticIn = 7,
		TYPE_QuarticOut,
		TYPE_QuarticInOut,

		TYPE_QuinticIn = 10,
		TYPE_QuinticOut,
		TYPE_QuinticInOut,

		TYPE_SinIn = 13,
		TYPE_SinOut,
		TYPE_SinInOut,

		TYPE_ExpoIn = 16,
		TYPE_ExpoOut,
		TYPE_ExpoInOut,

		TYPE_CircularIn = 19,
		TYPE_CircularOut,
		TYPE_CircularInOut,

		TYPE_ElasticIn = 22,
		TYPE_ElasticOut,
		TYPE_ElasticInOut,

		TYPE_BounceIn = 25,
		TYPE_BounceOut,


		TYPE_End
	};
	

	// 랜더 타겟 이름 정의
	enum E_RENDERTARGET
	{
		RENDERTARGET_DIFFUSE,
		RENDERTARGET_NOMAL,
		RENDERTARGET_DEPTH,
		RENDERTARGET_SPECULAR,
		RENDERTARGET_SHADE,
		RENDERTARGET_END

	};
	enum E_MRT
	{
		MRT_DEFERRED,
		MRT_LIGHTACC,
		MRT_END,
	};

	// 멀티 랜더 타겟 이름 정의

	static const wchar_t* STR_TAYRENDERTARGET(E_RENDERTARGET e)
	{
		switch (e)
		{
		case RENDERTARGET_DIFFUSE:
			return L"RENDERTARGET_DIFFUSE";
			break;
		case RENDERTARGET_NOMAL:
			return L"RENDERTARGET_NOMAL";
			break;
		case RENDERTARGET_DEPTH:
			return L"RENDERTARGET_DEPTH";

		case RENDERTARGET_SPECULAR:
			return L"RENDERTARGET_SPECULAR";
			break;
		case RENDERTARGET_SHADE:
			return L"RENDERTARGET_SHADE";
			break;
		case RENDERTARGET_END:
			break;
		default:
			break;
		}
	}

	static const wchar_t* STR_TAYMRT(E_MRT e)
	{
		switch (e)
		{
		case MRT_DEFERRED:
			return L"MRT_DEFERRED";
			break;
		case MRT_LIGHTACC:
			return L"MRT_LIGHTACC";
			break;
		case MRT_END:
			break;
		default:
			break;
		}
	}


#define  TAGTARGET		STR_TAYRENDERTARGET
#define  TAGMRT			STR_TAYMRT



}