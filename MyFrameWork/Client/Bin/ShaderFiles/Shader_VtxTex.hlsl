#include "Shader_Defines.hpp"

// 나중에 소프트 셰이더에서 사용
texture2D			g_DiffuseTexture;
texture2D			g_SourTexture;
texture2D			g_DepthTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct VS_OUT_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1; // uv 좌표를 찾기 위해 Proj 추가
};

VS_OUT_SOFT VS_MAIN_SOFTRECT(VS_IN In)
{
	VS_OUT_SOFT			Out;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}



struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_MAIN_UI(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);


	if (Out.vColor.a < 0.5f)
	{
		discard;
	}
	return Out;
}

struct PS_IN_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};


PS_OUT PS_MAIN_SOFTRECT(PS_IN_SOFT In)
{
	PS_OUT		Out = (PS_OUT)0;

	// UV 좌표 변환
	float2		vUV = In.vProjPos.xy / In.vProjPos.w;

	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);

	float		fViewZ = vDepthDesc.x * 300.f;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a = Out.vColor.a * saturate((fViewZ - In.vProjPos.w) * 2.f);

	return Out;

}


technique11		DefaultTechnique
{
	pass Rect
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UI
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_UI();
	}

	pass SOFT
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_SOFTRECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFTRECT();
	}

}