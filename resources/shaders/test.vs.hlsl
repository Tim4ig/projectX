struct VSIN
{
    float3 position : POSITION;
};

struct VSOUT
{
    float4 position : SV_POSITION;
};

VSOUT main(VSIN input)
{
    VSOUT output;
    output.position = float4(input.position, 1.0f);
    return output;
}
