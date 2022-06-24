// 셰이더 전역 변수 모음

// Matrix
cbuffer	RenderingPipeLine
{
	matrix			g_WorldMatrix;
	matrix			g_ViewMatrix;
	matrix			g_ProjMatrix;
};

// Texture Type
sampler DefaultSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_point;
	AddressU = wrap;
	AddressV = wrap;
};

// Light
// 빛은 3가지 색으로 정의되고 일반적으로 Diffuse가 기본 색이다.
cbuffer LightDesc
{
	float4		g_vLightDir;
	float4		g_vLightPos;
	float4		g_vLightDiffuse;
	float4		g_vLightAmbient = (float4)1.f;
	float4		g_vLightSpecular = (float4)1.f;
	float		g_fLightRange = 1.f;
};

// Matrial
// 물체가 가지는 색이고 텍스처 diffuse맵이 기본 색이다.
cbuffer MtrlDesc
{
	float4		g_vMtrlAmbient = float4(0.4f, 0.4f, 0.4f, 1.f);
	float4		g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);
};

// Camera
cbuffer CameraDesc
{
	float4 g_CameraPosition;
};

// Blends
/*
typedef struct D3D11_RENDER_TARGET_BLEND_DESC
	{
	BOOL BlendEnable;
	D3D11_BLEND SrcBlend;
	D3D11_BLEND DestBlend;
	D3D11_BLEND_OP BlendOp;
	D3D11_BLEND SrcBlendAlpha;
	D3D11_BLEND DestBlendAlpha;
	D3D11_BLEND_OP BlendOpAlpha;
	UINT8 RenderTargetWriteMask;
	} 	D3D11_RENDER_TARGET_BLEND_DESC;

typedef struct D3D11_BLEND_DESC
	{
	BOOL AlphaToCoverageEnable;
	BOOL IndependentBlendEnable;
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[ 8 ];
	} 	D3D11_BLEND_DESC;
*/

BlendState	AlphaBlending
{
	BlendEnable[0] = true;
	BlendOp = Add;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
};

BlendState	NonBlending
{
	BlendEnable[0] = false;
};

/*
BOOL DepthEnable;
D3D11_DEPTH_WRITE_MASK DepthWriteMask;
D3D11_COMPARISON_FUNC DepthFunc;
BOOL StencilEnable;
UINT8 StencilReadMask;
UINT8 StencilWriteMask;
D3D11_DEPTH_STENCILOP_DESC FrontFace;
D3D11_DEPTH_STENCILOP_DESC BackFace;
*/

DepthStencilState  ZTestAndWriteState
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState  NonZTestAndWriteState
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

/*
D3D11_FILL_MODE FillMode;
D3D11_CULL_MODE CullMode;
BOOL FrontCounterClockwise;
INT DepthBias;
FLOAT DepthBiasClamp;
FLOAT SlopeScaledDepthBias;
BOOL DepthClipEnable;
BOOL ScissorEnable;
BOOL MultisampleEnable;
BOOL AntialiasedLineEnable;
*/

RasterizerState FillMode_Wireframe
{
	FillMode = wireframe;
	CullMode = back;
	FrontCounterClockwise = false;
};
RasterizerState FillMode_Solid
{
	FillMode = Solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_cw
{
	FillMode = Solid;
	CullMode = front;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_ccw
{
	FillMode = Solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_None
{
	FillMode = Solid;
	CullMode = none;
	FrontCounterClockwise = false;
};