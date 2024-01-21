#ifndef _ACTORHPBARUI
#define _ACTORHPBARUI

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

VTX_OUT VS_ActorHPBarUI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_ActorHPBarUI(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // 체력 비율에 따라 체력 표시해주기
    // 범위 내만 출력
    vOutColor = g_tex_1.Sample(g_sam_1, _in.vUV);
    
    // 체력바의 비율에 맞게 출력해준다.
    if (_in.vUV.x < g_float_0)
    {
        vOutColor.rgb = float3(1.f, 0.f, 0.f);
    }
    // 체력 범위 밖이라면, a값을 0을 넣어주어 보더를 출력한다.
    else
    {
        vOutColor.a = 0.f;
    }
    
    // 만약 a가 0이면 보더 출력
    if (vOutColor.a == 0.f)
    {
        vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    
    return vOutColor;
}

#endif