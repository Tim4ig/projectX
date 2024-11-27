struct VSIN
{
    float3 position : POSITION;
};

struct VSOUT
{
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
    output.position = mul(gViewProj, mul(float4(input.position, 1.0f), gTest));
    return output;
}
