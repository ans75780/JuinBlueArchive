
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


vector		g_vLightDir;

vector		g_vLightPos;
float		g_fRange; 

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;


texture2D	g_NormalTexture;
texture2D	g_Texture;

texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;


sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
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
	VS_OUT		Out = (VS_OUT)0;

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

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);	

	return Out;
}

struct PS_OUT_LIGHT
{
	vector		vShade : SV_TARGET0;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
	vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	/* 0 -> -1, 1 -> 1*/
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	
	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal));
	Out.vShade.a = 1.f;
	return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	vector			vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade;

	if (Out.vColor.a == 0.f)
		discard;
	return Out;
}


BlendState BS_Default
{
	BlendEnable[0] = false;
};

RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_ZEnable_ZWriteEnable_false
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

technique11 DefaultTechnique
{
	pass Debug
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Blend
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}
}