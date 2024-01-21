#ifndef _HPBARUI
#define _HPBARUI

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

VTX_OUT VS_HPBarUI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_HPBarUI(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);

    if (_in.vUV.x >= g_vec4_0.x && _in.vUV.x <= (g_vec4_0.z * g_float_0)
        && _in.vUV.y >= g_vec4_0.y && _in.vUV.y <= g_vec4_0.w)
    {
        // 체력 색은 빨강색이다.
        float4 vHPColor = float4(1.f, 0.1f, 0.1f, 1.f);
        if (0.f >= vOutColor.a)
        {
            vOutColor = vHPColor;
        }
        else
        {
            vOutColor *= vHPColor;
        }
    }
    else
    {
         // 알파값이 0인 부분은 discard
        if (0.f >= vOutColor.a)
            discard;
    }
    
    return vOutColor;
}

#endif