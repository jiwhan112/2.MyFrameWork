#include "Shader_Defines.hpp"

// 인스턴스용 셰이더
texture2D			g_DiffuseTexture;

struct VS_IN
{
	float3		vPosition :		POSITION;
	float2		vTexUV :		TEXCOORD0;

	float4		vRight :		TEXCOORD1;
	float4		vUp :			TEXCOORD2;
	float4		vLook :			TEXCOORD3;
	float4		vTranslation:	TEXCOORD4;


};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	// 로컬 위치 행렬
	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matrix			matWV, matWVP;

	// 로컬에 먼저 곱한다.
	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

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

	if (Out.vColor.a < 0.9f)
		discard;
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

	//pass UI
	//{
	//	SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	//	SetDepthStencilState(ZTestAndWriteState, 0);
	//	SetRasterizerState(CullMode_None);

	//	VertexShader = compile vs_5_0 VS_MAIN();
	//	GeometryShader = NULL;
	//	PixelShader = compile ps_5_0 PS_MAIN_UI();
	//}
}