#include "Shader_Defines.hpp"

// Terrain 전용 셰이더
texture2D		g_DiffuseTexture;
texture2D		g_SourDiffuseTexture1;
texture2D		g_SourDiffuseTexture2;
texture2D		g_SourDiffuseTexture3;
texture2D		g_SourDiffuseTexture4;


// 필터 / 브러쉬 텍스처 
texture2D		g_FilterTexture;
texture2D		g_BrushTexture;


// float			g_BrushSize = 10;
cbuffer BrushDesc
{
	float4		g_vBrushPos = float4(10.0f, 0.0f, 10.f, 1.f);
	float		g_fRadius = 0.5f;
};

uint	g_TextureSize;

// VS
struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vNormal = mul(vector(In.vNormal, 0.0f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	return Out;
}

// PS
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_TERRAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	// 텍스처 색상
	float4	Albedo = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV*g_TextureSize);

	// 노말
	float	Nomal = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));

	// 조명이 적용된 텍스처
	float4  Diffuse = g_vLightDiffuse * Albedo * saturate(Nomal + (g_vLightAmbient * g_vMtrlAmbient));

	// 스펙큘러
	// 반사각 구하기
	// 빛 -> 노말에 맞아 반사 벡터 구하기
	float3 PN = In.vNormal* dot(normalize(g_vLightDir) * -1.f, In.vNormal);
//	float3 Reflection = normalize(g_vLightDir) + 2 * PN; // R = P+2n(dot(-P,n)) // 1이라면 슬라이드 벡터 2라면 반사벡터
	float3 Reflection = normalize(reflect(g_vLightDir,In.vNormal));
	float3 LookVec = normalize(g_CameraPosition - In.vWorldPos);

	float  SpecularPow = pow(saturate(dot(normalize(Reflection), LookVec)), 30.0f);
	float4 Specular = g_vLightSpecular * g_vMtrlSpecular* SpecularPow;

	Out.vColor = Diffuse + Specular;
	return Out;
}

PS_OUT PS_MAIN_TERRAIN_FITER(PS_IN In)
{

	PS_OUT			Out = (PS_OUT)0;

	// 스플라인 기법으로 텍스처 함치기

	// 기본 텍스처 로드
	vector	vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * g_TextureSize);

	
	// 4개의 텍스처 로드
	vector	vSource1 = g_SourDiffuseTexture1.Sample(DefaultSampler, In.vTexUV * g_TextureSize);
	vector	vSource2 = g_SourDiffuseTexture2.Sample(DefaultSampler, In.vTexUV * g_TextureSize);
	vector	vSource3 = g_SourDiffuseTexture3.Sample(DefaultSampler, In.vTexUV * g_TextureSize);
	vector	vSource4 = g_SourDiffuseTexture4.Sample(DefaultSampler, In.vTexUV * g_TextureSize);

	vector	vFilterColor = g_FilterTexture.Sample(PointSampler, In.vTexUV);

	// 브러쉬 텍스처
	vector	vBrushColor = (vector)0.f;

	if (g_vBrushPos.x - g_fRadius < In.vWorldPos.x &&  g_vBrushPos.x + g_fRadius >= In.vWorldPos.x &&
		g_vBrushPos.z - g_fRadius < In.vWorldPos.z &&  g_vBrushPos.z + g_fRadius >= In.vWorldPos.z)
	{
		float2		vBrushUV;
		vBrushUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fRadius)) / (2.f * g_fRadius);
		vBrushUV.y = (In.vWorldPos.z - (g_vBrushPos.z + g_fRadius)) / (2.f * g_fRadius);

		vBrushColor = g_BrushTexture.Sample(PointSampler, vBrushUV);
	}

	// Diffuse를 구할 떄 필터와 브러쉬 텍스처도 같이 구한다.
	// diffuse = (ATex * Fiter) + ((1 - Fiter) * BTex)
	
	float4	vMtrlDiffuse0 = vDiffuse;
	float4	vMtrlDiffuse1 = vSource1 * vFilterColor.a + vMtrlDiffuse0 * (1.f - vFilterColor.a);
	float4	vMtrlDiffuse2 = vSource2 * vFilterColor.r + vMtrlDiffuse1 * (1.f - vFilterColor.r);
	float4	vMtrlDiffuse3 = vSource3 * vFilterColor.g + vMtrlDiffuse2 * (1.f - vFilterColor.g);
	float4	vMtrlDiffuse4 = vSource4 * vFilterColor.b + vMtrlDiffuse3 * (1.f - vFilterColor.b);

	float4	vMtrlDiffuse = vMtrlDiffuse4; // 잘 섞인다

	float	Nomal = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));

	float4	Diffuse = g_vLightDiffuse * vMtrlDiffuse * saturate(Nomal + (g_vLightAmbient * g_vMtrlAmbient)); //+ vBrushColor;

	float3	PN = In.vNormal* dot(normalize(g_vLightDir) * -1.f, In.vNormal);
	float3	Reflection = normalize(g_vLightDir) + 2 * PN;
	float3	LookVec = normalize(g_CameraPosition - In.vWorldPos);

	float  SpecularPow = pow(saturate(dot(normalize(Reflection), LookVec)), 30.0f);
	float4 Specular = g_vLightSpecular * g_vMtrlSpecular* SpecularPow;

	if (vBrushColor.a > 0.1f)
		Diffuse *= 1.5f;

	Out.vColor = Diffuse + Specular;

	return Out;
}

//PS_OUT PS_MAIN_TOON(PS_IN In)
//{
//	PS_OUT			Out = (PS_OUT)0;
//
//	// Diffuse
//	float4	Albedo = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 30);
//	float	Nomal = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));
//	float4  Diifuse = g_vLightDiffuse * Albedo * Nomal + (g_vLightAmbient * g_vMtrlAmbient);
//
//	// TOON
//	Diifuse = ceil(Diifuse * 5) / 5.0f;
//
//	Out.vColor = Diifuse;
//
//	return Out;
//}

technique11		DefaultTechnique
{
	pass Terrain
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}

	pass Terrain_FITER
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_FITER();
	}
	//pass TOON
	//{
	//	SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	//	SetDepthStencilState(ZTestAndWriteState, 0);
	//	SetRasterizerState(CullMode_ccw);

	//	VertexShader = compile vs_5_0 VS_MAIN();
	//	GeometryShader = NULL;
	//	PixelShader = compile ps_5_0 PS_MAIN_TOON();
	//}
}