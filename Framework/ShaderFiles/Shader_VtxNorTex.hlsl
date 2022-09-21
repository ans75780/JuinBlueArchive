
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


vector	g_vBrushPos = vector(15.f, 0.f, 15.f, 1.f);
float	g_fBrushRange = 10.f;

// vector	g_vMtrlDiffuse;
texture2D	g_SourDiffTexture;
texture2D	g_DestDiffTexture;
texture2D	g_FilterTexture;
texture2D	g_BrushTexture;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};



struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_PHONG
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};


VS_OUT_PHONG VS_MAIN_TERRAIN_PHONG(VS_IN In)
{
	VS_OUT_PHONG		Out = (VS_OUT_PHONG)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}

struct PS_IN_PHONG
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
};

PS_OUT PS_MAIN_TERRAIN_PHONG(PS_IN_PHONG In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vSourDiffuse = g_SourDiffTexture.Sample(DefaultSampler, In.vTexUV * 30.f);
	vector		vDestDiffuse = g_DestDiffTexture.Sample(DefaultSampler, In.vTexUV * 30.f);
	vector		vFilter = g_FilterTexture.Sample(PointSampler, In.vTexUV);
	vector		vBrush = (vector)0;

	if (g_vBrushPos.x - g_fBrushRange * 0.5f < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fBrushRange * 0.5f && 
		g_vBrushPos.z - g_fBrushRange * 0.5f < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fBrushRange * 0.5f)
	{
		float2	vTexUV;

		vTexUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange * 0.5f)) / g_fBrushRange;
		vTexUV.y = ((g_vBrushPos.z + g_fBrushRange * 0.5f) - In.vWorldPos.z) / g_fBrushRange;
		
		vBrush = g_BrushTexture.Sample(DefaultSampler, vTexUV);
	}

	Out.vDiffuse = vSourDiffuse * vFilter + vDestDiffuse * (1.f - vFilter) + vBrush;
	Out.vDiffuse.a = 1.f;
	

	/* -1 -> 0, 1 -> 1*/
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);

	return Out;
}

technique11 DefaultTechnique
{
	pass Terrain_Phong
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN_PHONG();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_PHONG();
	}

}