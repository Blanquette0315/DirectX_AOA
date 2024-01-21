#ifndef _WINLOGO
#define _WINLOGO

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

VTX_OUT VS_WinLogo(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_WinLogo(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    
    // 알파값이 0인 부분은 discard
    if (0.f >= vOutColor.a)
        discard;
    
    if(g_int_0 == 10)
    {
        // float_0에 들어온 값으로 알파를 적용해준다.
        vOutColor.a = g_float_0;
    }
    
    return vOutColor;
}

#endif