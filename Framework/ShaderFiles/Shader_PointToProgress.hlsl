
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector	g_vCamPosition;

float2	g_ProgressBarSize;

float		g_Ratio;

texture2D	g_DiffuseTexture;



sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;


};

struct VS_OUT
{
	float4		vPosition : POSITION;

};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}

struct GS_IN
{
	float4		vPosition : POSITION;

};

struct GS_OUT
{
	vector		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

/* 지오메트리 셰이더. */

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> Stream)
{
	GS_OUT			Out[4];

	matrix		matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3	Width = float3(g_ProgressBarSize.x,0.f,0.f);

	float3	HalfHeight = float3(0.f, g_ProgressBarSize.y * 0.5f,0.f);

	Out[0].vPosition = vector(In[0].vPosition.xyz + HalfHeight, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexUV = float2(0.0f, 0.f);

	Out[1].vPosition = vector(In[0].vPosition.xyz + HalfHeight + Width, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexUV = float2(1.0f, 0.f);

	Out[2].vPosition = vector(In[0].vPosition.xyz - HalfHeight + Width, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexUV = float2(1.0f, 1.f);

	Out[3].vPosition = vector(In[0].vPosition.xyz - HalfHeight, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexUV = float2(0.0f, 1.f);

	Stream.Append(Out[0]);
	Stream.Append(Out[1]);
	Stream.Append(Out[2]);
	Stream.RestartStrip();

	Stream.Append(Out[0]);
	Stream.Append(Out[2]);
	Stream.Append(Out[3]);
	Stream.RestartStrip();
}





// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

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

	Out.vColor = 1.f;
	Out.vColor.gb = 0.f;

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_RATIO(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (In.vTexUV.x > g_Ratio )
		discard;

	return Out;
}




technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Ratio
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_RATIO();
	}
}