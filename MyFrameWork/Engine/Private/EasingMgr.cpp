#include "..\Public\EasingMgr.h"


IMPLEMENT_SINGLETON(CEasingMgr)

#define PI 3.1415

CEasingMgr::CEasingMgr()
{
}

_float CEasingMgr::Easing(EasingTypeID eEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if (eEasingType >= TYPE_End)
		return 0;

	switch (eEasingType)
	{
	case TYPE_Linear:
		return Linear(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuadIn:
		return QuadIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuadOut:
		return  QuadOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuadInOut:
		return  QuadInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_CubicIn:
		return  CubicIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_CubicOut:
		return  CubicOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_CubicInOut:
		return  CubicInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuarticIn:
		return  QuarticIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuarticOut:
		return  QuarticOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuarticInOut:
		return QuarticInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuinticIn:
		return  QuinticIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuinticOut:
		return QuinticOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_QuinticInOut:
		return QuinticInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_SinIn:
		return SinIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_SinOut:
		return  sinfOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_SinInOut:
		return sinfInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_ExpoIn:
		return  ExpoIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_ExpoOut:
		return  ExpoOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_ExpoInOut:
		return  ExpoInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_CircularIn:
		return  CircularIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_CircularOut:
		return  CircularOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_CircularInOut:
		return  CircularInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_ElasticIn:
		return  ElasticEaseIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_ElasticOut:
		return  ElasticEaseOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_ElasticInOut:
		return  ElasticEaseInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_BounceIn:
		return  BounceEaseIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case TYPE_BounceOut:
		return  BounceEaseOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	default:
		break;
	}

	return 0;
}

_float3 CEasingMgr::Easing3(EasingTypeID eEasingType, _float3 fStartPoint, _float3 fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	_float3 returnFloat3;

	if (eEasingType >= TYPE_End)
		return _float3();

	switch (eEasingType)
	{
	case TYPE_Linear:
	
		returnFloat3.x = Linear(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = Linear(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = Linear(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;
	
	case TYPE_QuadIn:
	
		returnFloat3.x = QuadIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuadIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuadIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;
	
	case TYPE_QuadOut:
		returnFloat3.x = QuadOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuadOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuadOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_QuadInOut:
		returnFloat3.x = QuadInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuadInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuadInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_CubicIn:
		returnFloat3.x = CubicIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = CubicIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = CubicIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_CubicOut:
		returnFloat3.x = CubicOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = CubicOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = CubicOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_CubicInOut:
		returnFloat3.x = CubicInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = CubicInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = CubicInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_QuarticIn:
		returnFloat3.x = QuarticIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuarticIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuarticIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_QuarticOut:
		returnFloat3.x = QuarticOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuarticOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuarticOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_QuarticInOut:
		returnFloat3.x = QuarticInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuarticInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuarticInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_QuinticIn:
		returnFloat3.x = QuinticIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuinticIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuinticIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_QuinticOut:
		returnFloat3.x = QuinticOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuinticOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuinticOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_QuinticInOut:
		returnFloat3.x = QuinticInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = QuinticInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = QuinticInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_SinIn:
		returnFloat3.x = SinIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = SinIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = SinIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_SinOut:
		returnFloat3.x = sinfOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = sinfOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = sinfOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_SinInOut:
		returnFloat3.x = sinfInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = sinfInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = sinfInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_ExpoIn:
		returnFloat3.x = ExpoIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = ExpoIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = ExpoIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_ExpoOut:
		returnFloat3.x = ExpoOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = ExpoOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = ExpoOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_ExpoInOut:
		returnFloat3.x = ExpoInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = ExpoInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = ExpoInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_CircularIn:
		returnFloat3.x = CircularIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = CircularIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = CircularIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_CircularOut:
		returnFloat3.x = CircularOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = CircularOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = CircularOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_CircularInOut:
		returnFloat3.x = CircularInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = CircularInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = CircularInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_ElasticIn:
		returnFloat3.x = ElasticEaseIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = ElasticEaseIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = ElasticEaseIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_ElasticOut:
		returnFloat3.x = ElasticEaseOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = ElasticEaseOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = ElasticEaseOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_ElasticInOut:
		returnFloat3.x = ElasticEaseInOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = ElasticEaseInOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = ElasticEaseInOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_BounceIn:
		returnFloat3.x = BounceEaseIn(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = BounceEaseIn(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = BounceEaseIn(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	case TYPE_BounceOut:
		returnFloat3.x = BounceEaseOut(fStartPoint.x, fTargetPoint.x, fPassedTime, fTotalTime);
		returnFloat3.y = BounceEaseOut(fStartPoint.y, fTargetPoint.y, fPassedTime, fTotalTime);
		returnFloat3.z = BounceEaseOut(fStartPoint.z, fTargetPoint.z, fPassedTime, fTotalTime);
		return returnFloat3;

	default:
		break;
	}

	return _float3();
}


_float CEasingMgr::Linear(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float((fTargetPoint - fStartPoint) * fPassedTime / fTotalTime + fStartPoint);
}

_float CEasingMgr::QuadIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float((fTargetPoint - fStartPoint) * fPassedTime*fPassedTime + fStartPoint);
}

_float CEasingMgr::QuadOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float(-(fTargetPoint - fStartPoint) * fPassedTime*(fPassedTime - 2) + fStartPoint);
}

_float CEasingMgr::QuadInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float((fTargetPoint - fStartPoint) / 2 * fPassedTime*fPassedTime + fStartPoint);
	fPassedTime--;
	return _float(-(fTargetPoint - fStartPoint) / 2 * (fPassedTime*(fPassedTime - 2) - 1) + fStartPoint);
}

_float CEasingMgr::CubicIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float((fTargetPoint - fStartPoint) * fPassedTime*fPassedTime*fPassedTime + fStartPoint);
}

_float CEasingMgr::CubicOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float((fTargetPoint - fStartPoint) * (fPassedTime*fPassedTime*fPassedTime + 1) + fStartPoint);
}

_float CEasingMgr::CubicInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return (fTargetPoint - fStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime + fStartPoint;
	fPassedTime -= 2;
	return _float((fTargetPoint - fStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime + 2) + fStartPoint);
}

_float CEasingMgr::QuarticIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float((fTargetPoint - fStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime + fStartPoint);
}

_float CEasingMgr::QuarticOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float(-(fTargetPoint - fStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 1) + fStartPoint);
}

_float CEasingMgr::QuarticInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float((fTargetPoint - fStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime + fStartPoint);
	fPassedTime -= 2;
	return _float(-(fTargetPoint - fStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 2) + fStartPoint);
}

_float CEasingMgr::QuinticIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float((fTargetPoint - fStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + fStartPoint);
}

_float CEasingMgr::QuinticOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float((fTargetPoint - fStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 1) + fStartPoint);
}

_float CEasingMgr::QuinticInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float((fTargetPoint - fStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + fStartPoint);
	fPassedTime -= 2;
	return _float((fTargetPoint - fStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 2) + fStartPoint);
}

_float CEasingMgr::SinIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float(-(fTargetPoint - fStartPoint) * cosf(fPassedTime / fTotalTime * (PI / 2)) + (fTargetPoint - fStartPoint) + fStartPoint);
}

_float CEasingMgr::sinfOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float((fTargetPoint - fStartPoint) * sinf(fPassedTime / fTotalTime * (PI / 2)) + fStartPoint);
}

_float CEasingMgr::sinfInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float(-(fTargetPoint - fStartPoint) / 2 * (cosf(PI*fPassedTime / fTotalTime) - 1) + fStartPoint);
}

_float CEasingMgr::ExpoIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float((fTargetPoint - fStartPoint) * pow(2, 10.0f * (fPassedTime / fTotalTime - 1)) + fStartPoint);
}

_float CEasingMgr::ExpoOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float((fTargetPoint - fStartPoint) * (-pow(2, -10 * fPassedTime / fTotalTime) + 1) + fStartPoint);
}

_float CEasingMgr::ExpoInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float((fTargetPoint - fStartPoint) / 2 * pow(2, 10 * (fPassedTime - 1)) + fStartPoint);
	fPassedTime--;
	return _float((fTargetPoint - fStartPoint) / 2 * (-pow(2, -10 * fPassedTime) + 2) + fStartPoint);
}

_float CEasingMgr::CircularIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float(-(fTargetPoint - fStartPoint) * (sqrtf(1 - fPassedTime * fPassedTime) - 1) + fStartPoint);
}

_float CEasingMgr::CircularOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float((fTargetPoint - fStartPoint) * sqrtf(1 - fPassedTime * fPassedTime) + fStartPoint);
}

_float CEasingMgr::CircularInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float(-(fTargetPoint - fStartPoint) / 2 * (sqrtf(1 - fPassedTime * fPassedTime) - 1) + fStartPoint);
	fPassedTime -= 2;
	return _float((fTargetPoint - fStartPoint) / 2 * (sqrtf(1 - fPassedTime * fPassedTime) + 1) + fStartPoint);
}

_float CEasingMgr::ElasticEaseOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{

	if ((fPassedTime /= fTotalTime) == 1)
		return fStartPoint + (fTargetPoint - fStartPoint);

	_float p = fTotalTime * .3f;
	_float s = p / 4;

	return _float((fTargetPoint - fStartPoint) * pow(2, -10 * fPassedTime) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) + (fTargetPoint - fStartPoint) + fStartPoint);
}

_float CEasingMgr::ElasticEaseIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime) == 1)
		return fStartPoint + (fTargetPoint - fStartPoint);

	_float p = fTotalTime * .3f;
	_float s = p / 4.f;

	return _float(-((fTargetPoint - fStartPoint) * pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + fStartPoint);
}

_float CEasingMgr::ElasticEaseInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime / 2) == 2)
		return _float(fStartPoint + (fTargetPoint - fStartPoint));

	_float p = fTotalTime * (.3f * 1.5f);
	_float s = p / 4;

	if (fPassedTime < 1)
		return _float(-.5f * ((fTargetPoint - fStartPoint) * pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + fStartPoint);
	return _float((fTargetPoint - fStartPoint) * pow(2, -10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) * .5f + (fTargetPoint - fStartPoint) + fStartPoint);
}

_float CEasingMgr::BounceEaseOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime) < (1 / 2.75f))
		return _float((fTargetPoint - fStartPoint) * (7.5625f * fPassedTime * fPassedTime) + fStartPoint);
	else if (fPassedTime < (2 / 2.75f))
		return _float((fTargetPoint - fStartPoint) * (7.5625f * (fPassedTime -= (1.5f / 2.75f)) * fPassedTime + .75f) + fStartPoint);
	else if (fPassedTime < (2.5f / 2.75f))
		return _float((fTargetPoint - fStartPoint) * (7.5625f * (fPassedTime -= (2.25f / 2.75f)) * fPassedTime + .9375f) + fStartPoint);
	else
		return _float((fTargetPoint - fStartPoint) * (7.5625f * (fPassedTime -= (2.625f / 2.75f)) * fPassedTime + .984375f) + fStartPoint);
}


_float CEasingMgr::BounceEaseIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float(BounceEaseOut(fTargetPoint, fStartPoint, fTotalTime - fPassedTime, fTotalTime) );
}
//_float CEasingMgr::BounceEaseInOut(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
//{
//	if (fPassedTime < fTotalTime * 0.5f)
//		return BounceEaseIn(fStartPoint, fTargetPoint * 0.5f, fPassedTime , fTotalTime * 0.5f);
//	else
//		return BounceEaseOut(fTargetPoint * 0.5f, fTargetPoint , fPassedTime-(fTotalTime * 0.5f) , fTotalTime * 0.5f);
//}
//
//_float CEasingMgr::BounceEaseOutIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
//{
//
//	if (fPassedTime < fTotalTime * 0.5f )
//		return BounceEaseOut(fStartPoint, fTargetPoint * 0.5f, fPassedTime * 0.5f, fTotalTime * 0.5f);
//	return BounceEaseIn(fTargetPoint * 0.5f, fTargetPoint, fPassedTime * 0.5f, fTotalTime * 0.5f);
//}

void CEasingMgr::Free()
{
}
