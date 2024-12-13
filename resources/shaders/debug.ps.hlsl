struct PSIN
{
    float2 texCoord : TEXCOORD0;
};

struct PSOUT
{
    float4 color : SV_Target;
};

PSOUT main(PSIN input)
{
    PSOUT output;

    float4 texColor = float4(1, 0, 0, 1);

    output.color = texColor;

    return output;
}
