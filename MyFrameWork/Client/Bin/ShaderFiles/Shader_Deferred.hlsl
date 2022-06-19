
/*
Depth 타겟 추가
 객체 들한테 그리기 때문에 모두 Depth SV_TARGET 추가
 proj = WorldPos를 받는다. // 단 0번째의 Position은 모든 변환이 완료된 상태이기 떄문에 새로 저장해야한다.

 투영행렬 까지만 곱한 값이기 때문에 (뷰스페이스의 Z, 0~Far,0,0)을 저장한다.
				(proj.w,proj.z) 이렇게 2개의 Z를 넘긴다.
				(proj.w/Far) // 해서 ~1로 저장된다.
				(proj.z/proj.w) // 0~1로 저장된다.
				월드위치를 구할 수 있다.

*/

// 라이팅
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

// 디버깅 텍스처
texture2D			g_TargetTexture; 

// 조명 연산 텍스처
texture2D			g_DiffuseTexture; 
texture2D			g_NormalTexture;
texture2D			g_DepthTexture;
texture2D			g_ShadeTexture;
texture2D			g_SpecularTexture;

// 그림자

// 포스트 프로세싱
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
	// 라이트 셰이더 

	PS_OUT_LIGHT	Out = (PS_OUT_LIGHT)0;

	// 노말과 깊이를 맵을 가져온다.
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	// 뎁스맵으로 Z값을 산출한다.
	float		fViewZ = vDepthDesc.x * 300.f;

	// 0~1로 저장된 노말을 -1~1로 바꾼다.
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	// 노말과 빛의 방향을 내적해 shade를 구한다.
	Out.vShade = g_vLightDiffuse * saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient);
	Out.vShade.a = 1.f;

	// 스펙큘러를 구하기 위해 반사벡터를 구한다.
	vector		vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector		vWorldPos;

	// 깊이 맵으로 월드위치를 구한다.

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * w나누기. */
	//vWorldPos.x = (In.vTexUV.x * 2.f - 1.f);
	//vWorldPos.y = (In.vTexUV.y * -2.f + 1.f);
	//vWorldPos.z = vDepthDesc.y; /* 0 ~ 1 */
	//vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	//	월드위치를 구할 수 있다.

	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.y * fViewZ; /* 0 ~ f */
	vWorldPos.w = 1.f * fViewZ;

	// 여기서 역행렬 계산은 외부에서 가져오자.
	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬위치 * 월드행렬 */
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

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * w나누기. */
	//vWorldPos.x = (In.vTexUV.x * 2.f - 1.f);
	//vWorldPos.y = (In.vTexUV.y * -2.f + 1.f);
	//vWorldPos.z = vDepthDesc.y; /* 0 ~ 1 */
	//vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.y * fViewZ; /* 0 ~ f */
	vWorldPos.w = 1.f * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬위치 * 월드행렬 */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);
	
	if (fDistance > g_fLightRange)
		discard;

	// 감색
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

	// Diffuse 텍스처와 Shader / Specular를 연산공식에 맞게 그려준다.
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
	// 블랜딩 옵션 켜기
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	// add로 두개의 텍스처를 더한다.
	// 랜더타겟에 쓰어지는 최종 색상을 정한다.
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

	// 랜더타겟에 그려준다.
	pass Blend
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	// 포스트 프로세싱
	// 반전 4
	pass REVERSE
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POST_REVERSE();
	}

	// 외곽선
	pass EDGE
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POST_EDGE();
	}

	// 외곽선 블랜드
	pass EDGE_BLEND
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POST_EDGE_BLEND();
	}
	// 그림자
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