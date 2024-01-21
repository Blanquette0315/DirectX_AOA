#ifndef _COSTUI
#define _COSTUI

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

VTX_OUT VS_CostUI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_CostUI(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    if (0.f >= vOutColor.a)
    {
        vOutColor = g_tex_1.Sample(g_sam_1, _in.vUV);
    }
    
    // 알파값이 0인 부분은 discard
    if (0.f >= vOutColor.a)
        discard;
    
    return vOutColor;
}

#endif