#include "..\Public\EasingMgr.h"

IMPLEMENT_SINGLETON(CEasingMgr);

_float CEasingMgr::Easing(_uint eEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if (eEasingType >= TYPE_End)
		return 0;

	switch (eEasingType)
	{
	case Engine::CEasingMgr::TYPE_Linear:
		return Linear(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuadIn:
		return QuadIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuadOut:
		return  QuadOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuadInOut:
		return  QuadInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_CubicIn:
		return  CubicIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_CubicOut:
		return  CubicOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_CubicInOut:
		return  CubicInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuarticIn:
		return  QuarticIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuarticOut:
		return  QuarticOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuarticInOut:
		return QuarticInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuinticIn:
		return  QuinticIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuinticOut:
		return QuinticOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_QuinticInOut:
		return QuinticInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_SinIn:
		return SinIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_SinOut:
		return  sinfOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_SinInOut:
		return sinfInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_ExpoIn:
		return  ExpoIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_ExpoOut:
		return  ExpoOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_ExpoInOut:
		return  ExpoInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_CircularIn:
		return  CircularIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_CircularOut:
		return  CircularOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_CircularInOut:
		return  CircularInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_ElasticIn:
		return  ElasticEaseIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_ElasticOut:
		return  ElasticEaseOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_ElasticInOut:
		return  ElasticEaseInOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_BounceIn:
		return  BounceEaseIn(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	case Engine::CEasingMgr::TYPE_BounceOut:
		return  BounceEaseOut(fStartPoint, fTargetPoint, fPassedTime, fTotalTime);

	default:
		break;
	}

	return 0;
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

void Engine::CEasingMgr::Free()
{

}

_float CEasingMgr::BounceEaseIn(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float(BounceEaseOut(fTargetPoint, fStartPoint, fTotalTime - fPassedTime, fTotalTime));
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