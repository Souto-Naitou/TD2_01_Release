#include "FullScreen.hlsli"

struct VignetteParam
{
    float intensity;
    float power;
};

ConstantBuffer<VignetteParam> gVignetteParam : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

float4 main(VertexShaderOutput input) : SV_TARGET
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texCoord);
    
    float32_t2 correct = input.texCoord * (gVignetteParam.intensity - input.texCoord.xy);
    
    float32_t vignette = correct.x * correct.y * 15.0f;
    
    vignette = saturate(pow(vignette, gVignetteParam.power));
    
     // �Â��Ȃ镔���ɐԐF��ǉ�
    float3 redTint = float3(1.0f, 0.0f, 0.0f); // �ԐF
    output.color.rgb = dot(output.color.rgb, float32_t3(0.2125f, 0.7154f, 0.0721f));
    output.color.rgb = lerp(output.color.rgb, redTint, 1.0f - vignette); // vignette�ŐF����
    
    return output.color;

}