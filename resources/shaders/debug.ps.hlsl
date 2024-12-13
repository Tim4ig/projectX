struct PSIN
{
    float4 color : COLOR;
};

struct PSOUT
{
    float4 color : SV_Target;
};

PSOUT main(PSIN input)
{
    PSOUT output;

    float4 texColor = input.color;

    output.color = texColor;

    return output;
}
