
/*
Depth Ÿ�� �߰�
 ��ü ������ �׸��� ������ ��� Depth SV_TARGET �߰�
 proj = WorldPos�� �޴´�. // �� 0��°�� Position�� ��� ��ȯ�� �Ϸ�� �����̱� ������ ���� �����ؾ��Ѵ�.

 ������� ������ ���� ���̱� ������ (�佺���̽��� Z, 0~Far,0,0)�� �����Ѵ�.
				(proj.w,proj.z) �̷��� 2���� Z�� �ѱ��.
				(proj.w/Far) // �ؼ� ~1�� ����ȴ�.
				(proj.z/proj.w) // 0~1�� ����ȴ�.
				������ġ�� ���� �� �ִ�.

*/

// ������
cbuffer LightDesc
{
	vector			g_vLightDir;
	vector			g_vLightPos;
	float			g_fLightRange;
	vector			g_vLightDiffuse;
	vector			g_vLightAmbient;
	vector			g_vLightSpecular;
	vector			g_vCamPosition;
};

cbuffer MtrlDesc
{
	vector			g_vMtrlDiffuse = (vector)1.f;
	vector			g_vMtrlAmbient = (vector)1.f;
	vector			g_vMtrlSpecular = (vector)1.f;
};

cbuffer Matrices
{
	matrix			g_ProjMatrixInv;
	matrix			g_ViewMatrixInv;
};

// ����� �ؽ�ó
texture2D			g_TargetTexture; 

// ���� ���� �ؽ�ó
texture2D			g_DiffuseTexture; 
texture2D			g_NormalTexture;
texture2D			g_DepthTexture;
texture2D			g_ShadeTexture;
texture2D			g_SpecularTexture;

// �׸���

// ����Ʈ ���μ���
texture2D			g_RenderTexture;

const float TexX = float(1 / 1280);
const float TexY = float(1 / 720);

cbuffer FiterDesc
{
	float3x3 Kx = {
	-1.f,0.f,1.f,
	-2.f,0.f,2.f,
	-1.f,0.f,1.f
	};

	float3x3 Ky = {
		1.f,2.f,1.f,
		0.f,0.f,0.f,
		-1.f,-2.f,-1.f
	};

}


sampler DefaultSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer	RenderingPipeLine
{
	matrix			g_WorldMatrix;
	matrix			g_ViewMatrix;
	matrix			g_ProjMatrix;
};

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

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_RECT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);
	return Out;
}

struct PS_OUT_LIGHT
{
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	// ����Ʈ ���̴� 

	PS_OUT_LIGHT	Out = (PS_OUT_LIGHT)0;

	// �븻�� ���̸� ���� �����´�.
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	// ���������� Z���� �����Ѵ�.
	float		fViewZ = vDepthDesc.x * 300.f;

	// 0~1�� ����� �븻�� -1~1�� �ٲ۴�.
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	// �븻�� ���� ������ ������ shade�� ���Ѵ�.
	Out.vShade = g_vLightDiffuse * saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient);
	Out.vShade.a = 1.f;

	// ����ŧ���� ���ϱ� ���� �ݻ纤�͸� ���Ѵ�.
	vector		vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector		vWorldPos;

	// ���� ������ ������ġ�� ���Ѵ�.

	/* ������ġ * ������� * ����� * ������� * w������. */
	//vWorldPos.x = (In.vTexUV.x * 2.f - 1.f);
	//vWorldPos.y = (In.vTexUV.y * -2.f + 1.f);
	//vWorldPos.z = vDepthDesc.y; /* 0 ~ 1 */
	//vWorldPos.w = 1.f;

	/* ������ġ * ������� * ����� * ������� */
	//	������ġ�� ���� �� �ִ�.

	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.y * fViewZ; /* 0 ~ f */
	vWorldPos.w = 1.f * fViewZ;

	// ���⼭ ����� ����� �ܺο��� ��������.
	/* ������ġ * ������� * ����� */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ������ġ * ������� */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f);
	Out.vSpecular.a = 0.f;


	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT	Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	float		fViewZ = vDepthDesc.x * 300.f;

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vWorldPos;

	/* ������ġ * ������� * ����� * ������� * w������. */
	//vWorldPos.x = (In.vTexUV.x * 2.f - 1.f);
	//vWorldPos.y = (In.vTexUV.y * -2.f + 1.f);
	//vWorldPos.z = vDepthDesc.y; /* 0 ~ 1 */
	//vWorldPos.w = 1.f;

	/* ������ġ * ������� * ����� * ������� */
	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.y * fViewZ; /* 0 ~ f */
	vWorldPos.w = 1.f * fViewZ;

	/* ������ġ * ������� * ����� */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ������ġ * ������� */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);
	
	if (fDistance > g_fLightRange)
		discard;

	// ����
	float		fAtt = max(g_fLightRange - fDistance, 0.f) / g_fLightRange;

	Out.vShade = (g_vLightDiffuse * saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vLightDir), vNormal);


	vector		vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f) * fAtt;
	Out.vSpecular.a = 0.f;

	return Out;
}


PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vShadeDesc = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vSpecularDesc = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);

	// Diffuse �ؽ�ó�� Shader / Specular�� ������Ŀ� �°� �׷��ش�.
	//Out.vColor = vDiffuseDesc * vShadeDesc + vSpecularDesc;
	Out.vColor = vDiffuseDesc * vShadeDesc;

	if (Out.vColor.a == 0.0f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_POST_REVERSE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4		vRenderTarget = g_RenderTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = 1 - vRenderTarget;

	return Out;
}
PS_OUT PS_MAIN_POST_EDGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	float Lx = 0;
	float Ly = 0;

	for (int y = -1; y <=1 ; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			float2		offset = float2(x, y) * 0.001f;
			float3		vRenderTarget = g_RenderTexture.Sample(DefaultSampler, In.vTexUV + offset).rgb;
			float		Gray = dot(vRenderTarget, float3(0.3f, 0.59f, 0.11f));


			Lx += Gray * Kx[y+1][x+1];
			Ly += Gray * Ky[y+1][x+1];
		}
	}

	float Outline = sqrt((Lx*Lx) + (Ly*Ly));
	Out.vColor = float4(Outline.xxx, 1.f);
	return Out;
}

PS_OUT PS_MAIN_POST_EDGE_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vEdge = g_RenderTexture.Sample(DefaultSampler, In.vTexUV);
	float		Edge = vEdge.x;

	//if (Edge > 0.3f)
	//	Edge = 1.f;
	//else
	//	Edge = 0.0f;

	Out.vColor = vDiffuseDesc * (1-Edge);

	Out.vColor.a = 1;
	return Out;
}



PS_OUT PS_MAIN_POST_SHADOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4		vRenderTarget = g_RenderTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = 1 - vRenderTarget;
	return Out;
}



BlendState	NonBlending
{
	BlendEnable[0] = false;
};

BlendState	Blending_One
{
	// ���� �ɼ� �ѱ�
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	// add�� �ΰ��� �ؽ�ó�� ���Ѵ�.
	// ����Ÿ�ٿ� �������� ���� ������ ���Ѵ�.
	BlendOp = Add;
	SrcBlend = one;
	DestBlend = one;


};

DepthStencilState  NonZTestAndWriteState
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

RasterizerState CullMode_ccw
{
	FillMode = Solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

technique11		DefaultTechnique
{
	pass DebugBuffer
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}

	pass Light_Directional
	{
		SetBlendState(Blending_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetBlendState(Blending_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}

	// ����Ÿ�ٿ� �׷��ش�.
	pass Blend
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	// ����Ʈ ���μ���
	// ���� 4
	pass REVERSE
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POST_REVERSE();
	}

	// �ܰ���
	pass EDGE
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POST_EDGE();
	}

	// �ܰ��� ����
	pass EDGE_BLEND
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POST_EDGE_BLEND();
	}
	// �׸���
	pass SHADOW
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POST_SHADOW();
	}

	
}