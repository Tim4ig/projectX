struct GS_INPUT
{
    float3 normal : NORMAL;
    float4 position : SV_POSITION;
};

struct GS_OUTPUT
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

[maxvertexcount(12)]
void main(triangle GS_INPUT input[3], inout LineStream<GS_OUTPUT> lineStream)
{
    GS_OUTPUT v0, v1, v2;
    GS_OUTPUT n0, n1, n2;
    float multiplier = 0.01;

    v0.position = input[0].position;
    v0.color = float4(1, 1, 1, 1);

    v1.position = input[1].position;
    v1.color = float4(1, 1, 1, 1);

    v2.position = input[2].position;
    v2.color = float4(1, 1, 1, 1);

    n0.position = input[0].position + float4(input[0].normal, 0) * multiplier;
    n0.color = float4(1, 0, 0, 1);

    n1.position = input[1].position + float4(input[1].normal, 0) * multiplier;
    n1.color = float4(1, 0, 0, 1);

    n2.position = input[2].position + float4(input[2].normal, 0) * multiplier;
    n2.color = float4(1, 0, 0, 1);

    lineStream.Append(v0);
    lineStream.Append(v1);
    lineStream.RestartStrip();

    lineStream.Append(v1);
    lineStream.Append(v2);
    lineStream.RestartStrip();

    lineStream.Append(v2);
    lineStream.Append(v0);
    lineStream.RestartStrip();

    lineStream.Append(v0);
    lineStream.Append(n0);
    lineStream.RestartStrip();

    lineStream.Append(v1);
    lineStream.Append(n1);
    lineStream.RestartStrip();

    lineStream.Append(v2);
    lineStream.Append(n2);
    lineStream.RestartStrip();
}
