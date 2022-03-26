#pragma once

namespace Engine
{
	// 기본타입
	typedef unsigned char				_ubyte;
	typedef signed char					_byte;

	typedef unsigned short				_ushort;
	typedef signed short				_short;

	typedef unsigned int				_uint;
	typedef signed int					_int;

	typedef unsigned long				_ulong;
	typedef signed long					_long;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;

	typedef wchar_t						_tchar;
	   
	// 벡터 타입
	typedef XMFLOAT2					_float2;
	typedef XMFLOAT3					_float3;
	typedef XMFLOAT4					_float4;

	// 연산용 벡터 타입
	typedef XMVECTOR					_vector;
	
	// 인자로 들어올때 지정해줘야하는 타입 const int& 같은 타입이다. 
	typedef FXMVECTOR					_fvector; 

	// 행렬 타입
	typedef XMFLOAT4X4					_float4x4;
	// 연산용 행렬 타입
	typedef XMMATRIX					_matrix;
	// 인자용 행렬 타입 1~3 4~5 ...
	typedef FXMMATRIX					_fmatrix;
	typedef CXMMATRIX					_cmatrix;

	// SIMD 연산
	// 데이터덩어리로 비트 연산을 해서 좀더 빠르게 행렬 / 벡터연산을 수행할 수 있게 한다.

	// 단 스택에서만 사용해야되고 동적할당 및 힙에서 사용하지 말자 
	// 클래스에서 선언 X
}