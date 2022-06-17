
cbuffer	RenderingPipeLine
{
	matrix			g_WorldMatrix;
	matrix			g_ViewMatrix;
	matrix			g_ProjMatrix;
};


vector		g_vColor;

struct VS_IN
{
	float3		vPosition : POSITION;
	float4		vColor : COLOR0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vColor : COLOR0;
};

VS_OUT VS_MAIN_RECT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vColor = In.vColor;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vColor : COLOR0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_RECT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_vColor;

	return Out;
}

BlendState	NonBlending
{
	BlendEnable[0] = false;
};

DepthStencilState  ZTestAndWriteState
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

RasterizerState FillMode_Solid
{
	FillMode = Solid;
	CullMode = back;
	FrontCounterClockwise = false;
};


technique11		DefaultTechnique
{
	pass Rect
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(FillMode_Solid);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}	
}