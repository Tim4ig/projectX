struct PSIN
{
};

struct PSOUT
{
    float4 color : SV_Target;
};

PSOUT main(PSIN input)
{
    PSOUT output;
    output.color = float4(1, 0, 0, 1);
    return output;
}
