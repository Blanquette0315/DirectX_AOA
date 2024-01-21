#ifndef _STDEFFECT
#define _STDEFFECT

#include "register.fx"
#include "func.fx"

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_stdEffect(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_stdEffect(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    if (g_iAnim2DUse)
    {
        float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
        float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);
	
        if (vUV.x < g_vLeftTop.x || g_vLeftTop.x + g_vSlice.x < vUV.x
			|| vUV.y < g_vLeftTop.y || g_vLeftTop.y + g_vSlice.y < vUV.y)
        {
            discard;
        }
	
        vOutColor = g_Atlas.Sample(g_sam_1, vUV);
    }
    else
    {
        if (g_btex_0)        
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vOutColor = float4(0.f, 0.f, 0.f, 0.f);
    }
    
    // 알파값이 0인 부분은 discard
    if (0.f >= vOutColor.a)
        discard;
    
    return vOutColor;
}

#endif