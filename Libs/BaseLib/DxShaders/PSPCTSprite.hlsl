//--------------------------------------------------------------------------------------
// File: PSPCTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"


Texture2D g_texture : register(t0);
Texture2D g_textures[2] : register(t1);
SamplerState g_sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
    int count : packoffset(c6);
};

float4 main(PSPCTInput input) : SV_TARGET
{
	float4 Light = (saturate(input.color) * Diffuse) + Emissive;
	Light.a = Diffuse.a;
	Light = g_texture.Sample(g_sampler, input.tex) * Light;
    for (int i = 0; i < count; i++)
    {
        float4 color = g_textures[i].Sample(g_sampler, input.tex);
        if (color.a > 0.5f)
        {
            Light.rgb = color.rgb;
        }
    }
    return Light;
}

