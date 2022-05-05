#pragma once

namespace Engine
{
	// �⺻Ÿ��
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

	// ���� Ÿ��
//	typedef XMFLOAT2					_float2;
//	typedef XMFLOAT3					_float3;

	// �⺻ Ÿ���� ��ӹ޾Ƽ� �������� ����� ����Ѵ�.
	// ����3�� 4�� �����ϴ� �� ������
	//typedef struct tagFloat4_Derived final : public XMFLOAT4
	//{
	//	tagFloat4_Derived(float _x, float _y, float _z, float _w) :XMFLOAT4(_x, _y, _z, _w) {}
	//	tagFloat4_Derived() = default;
	//	tagFloat4_Derived(_float3 vFloat3, _float fW) :
	//		XMFLOAT4(vFloat3.x, vFloat3.y, vFloat3.z, fW)
	//	{
	//	}
	//}_float4;

	// ����� ���� Ÿ��
	typedef XMVECTOR					_vector;

	// ���ڷ� ���ö� ����������ϴ� Ÿ�� const int& ���� Ÿ���̴�.
	typedef FXMVECTOR					_fvector;

	// ��� Ÿ��
	// typedef XMFLOAT4X4					_float4x4;
	// ����� ��� Ÿ��
	typedef XMMATRIX					_matrix;
	// ���ڿ� ��� Ÿ�� 1~3 4~5 ...
	typedef FXMMATRIX					_fmatrix;
	typedef CXMMATRIX					_cmatrix;

	// SIMD ����
	// �����͵���� ��Ʈ ������ �ؼ� ���� ������ ��� / ���Ϳ����� ������ �� �ְ� �Ѵ�.

	// �� ���ÿ����� ����ؾߵǰ� �����Ҵ� �� ������ ������� ����
	// Ŭ�������� ���� X

	// DXSimpleMath ���� ��ķ� ����
	typedef DirectX::SimpleMath::Matrix		_float4x4;
	typedef DirectX::SimpleMath::Vector2	_float2;
	typedef DirectX::SimpleMath::Vector3	_float3;

	typedef struct tagFloat4_Derived final : public DirectX::SimpleMath::Vector4
	{
		tagFloat4_Derived(float _x, float _y, float _z, float _w) :Vector4(_x, _y, _z, _w) {}
		tagFloat4_Derived() = default;
		tagFloat4_Derived(_float3 vFloat3, _float fW) :
			Vector4(vFloat3.x, vFloat3.y, vFloat3.z, fW)
		{
		}
		_float3 Get_Float3()
		{
			return _float3(x, y, z);
		}
		tagFloat4_Derived Get_Float4(float _w)
		{
			return tagFloat4_Derived(x, y, z, _w);
		}
		Vector4& operator=(const _float3& f)
		{
			x = f.x;
			y = f.y;
			z = f.z;
			w = 0;
			return *this;
		}
	}_float4;

	typedef DirectX::SimpleMath::Plane		_plane;
	typedef DirectX::SimpleMath::Quaternion _quaterion;
	typedef DirectX::SimpleMath::Rectangle	_rect;
	typedef DirectX::SimpleMath::Ray		_ray;
	typedef DirectX::SimpleMath::Color		_color;
	typedef DirectX::SimpleMath::Viewport	_viewport;
}