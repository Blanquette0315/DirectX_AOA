#ifndef _NUMUI
#define _NUMUI

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

VTX_OUT VS_NumUI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_NumUI(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    // 만약 인자로 받은게 -1일 경우 아무것도 출력하지 않는다.
    if (g_vec2_0.x == -1.f || g_vec2_0.y == -1.f)
    {
        discard;
    }
    // 아닐 경우 숫자를 출력한다.
    else
    {
        // UV를 계산한다.
        float2 vDiff = (g_vec2_2 - g_vec2_1) / 2.f;
        float2 vUV = (g_vec2_0 - vDiff) + (g_vec2_2 * _in.vUV);
	    
        if (vUV.x < g_vec2_0.x || g_vec2_0.x + g_vec2_1.x < vUV.x
			|| vUV.y < g_vec2_0.y || g_vec2_0.y + g_vec2_1.y < vUV.y)
        {
            discard;
        }
        
        vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    }
    
    // 알파값이 0인 부분은 discard
    if (0.f >= vOutColor.a)
        discard;
    
    return vOutColor;
}

#endif