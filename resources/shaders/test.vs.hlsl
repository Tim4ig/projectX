struct VSIN
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VSOUT
{
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
};

cbuffer cb0 : register(b0)
{
    float4x4 gViewProj;
};

cbuffer cb1 : register(b1)
{
    float4x4 gTest;
};

VSOUT main(VSIN input)
{
    VSOUT output;
    output.position = mul(gViewProj, mul(gTest, float4(input.position, 1.0f)));
    // output.position = mul(gViewProj, float4(input.position, 1.0f));
    output.texCoord = input.texCoord;
    return output;
}
