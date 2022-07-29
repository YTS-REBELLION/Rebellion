#ifndef _TRIAL
#define _TRIAL

#include "value.fx"
#include "func.fx"

vector	g_vLightDir = (vector)0.f;

vector	g_vLightDiffuse;
vector	g_vLightSpecular;
vector	g_vLightAmbient;

vector	g_vMtrlDiffuse;
vector	g_vMtrlSpecular;
vector	g_vMtrlAmbient;

vector	g_vCamPos;
float	g_fPower;

struct	VS_IN
{
	vector			vPosition : POSITION;
	float2			vTexUV : TEXCOORD;
};

struct	VS_OUT
{
	vector			vPosition : POSITION;
	float2			vTexUV : TEXCOORD;
};


VS_OUT	VS_TRAIL(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD;
};

struct PS_OUT
{
	float4			vColor : COLOR;
};


PS_OUT	PS_TRAIL(PS_IN In) : SV_Target
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_tex_1.Sample(g_sam_1, In.vTexUV);
	//Out.vColor = In.vColor;
	float fValue = ((In.vTexUV.x + (In.vTexUV.y)) * 0.9f);
	fValue = min(fValue, 1.f);
	Out.vColor.argb *= fValue;
	Out.vColor.argb = min(Out.vColor.a, 1.f);
	Out.vColor.argb = saturate(pow(Out.vColor.a, 2));

	return Out;
}
PS_OUT	PS_TEST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_tex_1.Sample(g_sam_1, In.vTexUV);
	//Out.vColor = In.vColor;
	float fValue = ((In.vTexUV.x + (In.vTexUV.y)) * 0.5f);
	fValue = min(fValue, 1.f);
	Out.vColor.rg *= fValue;
	Out.vColor.rg = min(Out.vColor.a, 1.f);
	Out.vColor.rg = saturate(pow(Out.vColor.a, 2));

	return Out;
}
PS_OUT	PS_BOSS(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_tex_1.Sample(g_sam_1, In.vTexUV);
	//Out.vColor = In.vColor;
	float fValue = ((In.vTexUV.x + (In.vTexUV.y)) * 0.8f);
	fValue = min(fValue, 1.f);
	Out.vColor.a *= fValue;
	Out.vColor.a = min(Out.vColor.a, 1.f);
	Out.vColor.a = saturate(pow(Out.vColor.a, 2));

	return Out;
}

//technique Default_Device
//{
//	// ±â´ÉÀÇ Ä¸½¶È­
//	pass
//	{
//		alphablendenable = true;
//		SRCBLEND = SRCALPHA;
//		destBlend = invsrcalpha;
//		vertexshader = compile vs_5_0 VS_TRAIL();
//		pixelshader = compile ps_5_0 PS_TRAIL();
//	}
//	pass TEST
//	{
//		alphablendenable = true;
//		SRCBLEND = SRCALPHA;
//		destBlend = invsrcalpha;
//		vertexshader = compile ps_5_0 VS_TRAIL();
//		pixelshader = compile ps_5_0 PS_TEST();
//	}
//	pass BOSS
//	{
//		alphablendenable = true;
//		SRCBLEND = SRCALPHA;
//		destBlend = invsrcalpha;
//		vertexshader = compile ps_5_0 VS_TRAIL();
//		pixelshader = compile ps_5_0 PS_BOSS();
//	}
//
//};








#endif