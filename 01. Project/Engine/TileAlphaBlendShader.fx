#ifndef _TILEALPHABLEND
#define _TILEALPHABLEND

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

VTX_OUT VS_TileAlphaBlend(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_TileAlphaBlend(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
	

    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    // ���İ��� 0�� �κ��� discard
    //if (0.f >= vOutColor.a)
    //    discard;
    
    // Vec4�Ķ���ͷ� ���޵� �������� �����ϱ�
    // �켱 �׽�Ʈ������ Alpha���� ������ ������ RGB���� �����غ���
    vOutColor.x *= g_vec4_0.x;
    vOutColor.y *= g_vec4_0.y;
    vOutColor.z *= g_vec4_0.z;
    vOutColor.w *= g_vec4_0.w;
    
    return vOutColor;
}

#endif