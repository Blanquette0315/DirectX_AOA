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
    
    // ü�� ������ ���� ü�� ǥ�����ֱ�
    // ���� ���� ���
    vOutColor = g_tex_1.Sample(g_sam_1, _in.vUV);
    
    // ü�¹��� ������ �°� ������ش�.
    if (_in.vUV.x < g_float_0)
    {
        vOutColor.rgb = float3(1.f, 0.f, 0.f);
    }
    // ü�� ���� ���̶��, a���� 0�� �־��־� ������ ����Ѵ�.
    else
    {
        vOutColor.a = 0.f;
    }
    
    // ���� a�� 0�̸� ���� ���
    if (vOutColor.a == 0.f)
    {
        vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    
    return vOutColor;
}

#endif