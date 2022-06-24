#include "Shader_Defines.hpp"

// 정적 모델 셰이더

texture2D			g_DiffuseTexture;
texture2D			g_NormalTexture;

// 소켓
cbuffer SocketMatrix
{
	matrix		g_SocketMatrix;
};

cbuffer Timer
{
	float		g_Timer;
	

};

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
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;


};

VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.0f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));

	return Out;
}

VS_OUT VS_MAIN_SOCKET(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matrix			WorldMatrix = g_SocketMatrix;

	matWV = mul(WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), WorldMatrix);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.0f), WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

// PS
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;

};

struct PS_OUT
{
	// vector		vColor : SV_TARGET0;

	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
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

	// float	Noraml = saturate(dot(normalize(In.vNormal), normalize(g_vLightDir) * -1));

	// 스펙큘러
	//float4		vReflect = reflect(normalize(g_vLightDir), In.vNormal);
	//float4		vLook = normalize(g_CameraPosition - In.vWorldPos);
	//float		fSpecular = pow(saturate(dot(normalize(vReflect), vLook)), 30.f);

	float4  Diffuse = g_vLightDiffuse * DiffuseMap;
	//float4  Shade = saturate(Noraml + (g_vLightAmbient * g_vMtrlAmbient));
	//float4  Specular = 0;//(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	Diffuse.a = DiffuseMap.a;
	if (Diffuse.a < 0.5f)
		discard;


	Out.vDiffuse = Diffuse;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_NOMAL(PS_IN In)
{
	//Albedo = ceil(Albedo * 5) / 5.0f; // Ceil

	PS_OUT			Out = (PS_OUT)0;

	// 노말 맵으로 노말 연산
	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{

	PS_OUT			Out = (PS_OUT)0;
	// 텍스처 색상
	float4	DiffuseMap = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float4 Diffuse = DiffuseMap;
	Diffuse.a = 1;


	Out.vDiffuse = saturate(Diffuse * 1.3f);
	Out.vNormal = float4(1, 1, 1, 1);
	Out.vDepth = float4(1, 1, 1, 1);
	return Out;
}
PS_OUT PS_MAIN_GREEN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	// 텍스처 색상
	float4	DiffuseMap = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float4 Diffuse = DiffuseMap;
	Diffuse.a = 1;


	Out.vDiffuse = saturate(Diffuse * 1.5f);
	Out.vNormal = float4(1, 1, 1, 1);
	Out.vDepth = float4(1, 1, 1, 1);
	return Out;
}

// 던전 하트용 
PS_OUT PS_MAIN_EMSIVE_HEART(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	// 텍스처 색상
	float4	DiffuseMap = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float4 Diffuse = DiffuseMap;
	Diffuse.a = 1;


	Out.vDiffuse = Diffuse;
	Out.vNormal = float4(1, 1, 1, 1);
	Out.vDepth = float4(1, 1, 1, 1);

	return Out;
}



technique11		DefaultTechnique
{
	pass Default // 0
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		//	SetRasterizerState(CullMode_ccw);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}

	pass Red // 1
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		//	SetRasterizerState(CullMode_ccw);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RED();
	}

	pass SOCKET // 2
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		//	SetRasterizerState(CullMode_ccw);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_SOCKET();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}

	pass Green // 3
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		//	SetRasterizerState(CullMode_ccw);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GREEN();
	}

	pass DUNGEONHEART_EMSV // 4
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EMSIVE_HEART();
	}

	//pass DUNGEONHEART // 5
	//{
	//	SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	//	SetDepthStencilState(ZTestAndWriteState, 0);
	//	SetRasterizerState(CullMode_ccw);

	//	VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
	//	GeometryShader = NULL;
	//	PixelShader = compile ps_5_0 PS_MAIN_GREEN();
	//}
	//pass DUNGEONDOOR // 6
	//{
	//	SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	//	SetDepthStencilState(ZTestAndWriteState, 0);
	//	SetRasterizerState(CullMode_ccw);

	//	VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
	//	GeometryShader = NULL;
	//	PixelShader = compile ps_5_0 PS_MAIN_GREEN();
	//}

}