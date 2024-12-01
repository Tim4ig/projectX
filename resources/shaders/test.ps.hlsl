struct PSIN
{
    float2 texCoord : TEXCOORD0;
};

struct PSOUT
{
    float4 color : SV_Target;
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

PSOUT main(PSIN input)
{
    PSOUT output;

    float4 texColor = myTexture.Sample(mySampler, input.texCoord);

    output.color = float4(1, 0, 0, 1);

    return output;
}
