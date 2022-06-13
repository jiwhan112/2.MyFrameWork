#include "Shader_Defines.hpp"

texture2D g_DiffuseTexture;
// 파티클 메쉬용 셰이더


// VS
struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.0f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	return Out;
}



// PS
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	// 텍스처 색상
	float4	DiffuseMap = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	// 노말
	// float3	NormalMap = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	//
	// NormalMap = normalize(NormalMap*2-1);
	 // float3x3 TBN = float3x3(In.vTangent, cross(In.vTangent, In.vNormal), In.vNormal);
	// float3x3 TBN = float3x3(In.vTangent, cross(In.vNormal, In.vTangent), In.vNormal);
	//
	//
	// float3	WorldNormal = mul(TBN, NormalMap);
	//

	// 노말2

	float	Noraml = saturate(dot(normalize(In.vNormal), normalize(g_vLightDir) * -1));

	// 스펙큘러
	float4		vReflect = reflect(normalize(g_vLightDir), In.vNormal);
	float4		vLook = normalize(g_CameraPosition - In.vWorldPos);
	float		fSpecular = pow(saturate(dot(normalize(vReflect), vLook)), 30.f);

	float4  Diffuse = g_vLightDiffuse * DiffuseMap;
	float4  Shade = saturate(Noraml + (g_vLightAmbient * g_vMtrlAmbient));
	float4  Specular = 0;//(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	float4 color = Diffuse * Shade + Specular;
	color.a = DiffuseMap.a;
	if (color.a < 0.5f)
		discard;

	Out.vColor = color;
	return Out;
}

technique11		DefaultTechnique
{
	pass Default // 0
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}

	
}