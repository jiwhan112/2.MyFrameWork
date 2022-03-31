
#include "Shader_Defines.hpp"

texture2D			g_DiffuseTexture;

// ��
// ���� 3���� ������ ���ǵǰ� �Ϲ������� Diffuse�� �⺻ ���̴�.
cbuffer LightDesc
{
	float3		g_vLightDir = float3(1.f, -1.f, 1.f);
	float3		g_vLightPos = float3(1.f, -1.f, 1.f);
	float4		g_vLightDiffuse = (float4)1.f;
	float4		g_vLightAmbient = (float4)1.f;
	float4		g_vLightSpecular = (float4)1.f;
};

// ����
// ��ü�� ������ ���̰� �ؽ�ó diffuse���� �⺻ ���̴�.
cbuffer MtrlDesc
{
	float4		g_vMtrlAmbient = float4(0.1f, 0.1f, 0.1f, 1.f);
	float4		g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);
};


// ī�޶� 
cbuffer CameraDesc
{
	float3 g_CameraPosition;
};


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
	float3		vViewDir: TEXCOORD1;

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

VS_OUT VS_MAIN_LIGHT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);


	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 1.f), g_WorldMatrix));

	
	// ������ġ -> ī�޶� ���� ���ϱ�
	Out.vViewDir = normalize(g_CameraPosition - mul(vector(In.vPosition, 1.f), g_WorldMatrix));
	return Out;
}

// PS
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vViewDir: TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV*30);

	return Out;
}

PS_OUT PS_MAIN_LIGHT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	// �ؽ�ó ����
	float4 Albedo = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV*30);


	// ������ ����� �ؽ�ó
	float4  Diffuse = g_vLightDiffuse * Albedo;
	
	// �븻
	float	Nomal = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
	
	// ����ŧ�� 
	// g_CameraPosition

	// �ݻ簢 ���ϱ�
	// �� -> �븻�� �¾� �ݻ� ���� ���ϱ�
	float3 PN = In.vNormal* dot(normalize(g_vLightDir) * -1.f, In.vNormal);
	float3 Reflection = normalize(g_vLightDir) + 2 * PN; // R = P+2n(dot(-P,n)) // 1�̶�� �����̵� ���� 2��� �ݻ纤��

	float4 Specular = (float4)0.0f;
	if (Diffuse.x)
	{
		Specular = max(dot(normalize(Reflection), normalize(In.vViewDir)), 0.f);
		Specular = pow(Specular, 20.0f);
	}
	
	Out.vColor = (Diffuse)* Nomal + Specular + (g_vLightAmbient * g_vMtrlAmbient);


	
	return Out;
}


technique11		DefaultTechnique
{
	pass Terrain
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Terrain_Light
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_LIGHT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT();
	}

}