struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 positionCS :TEXCOORD1;
};

VertexOutput main(VertexInput IN)
{
    VertexOutput OUT;

    OUT.position = IN.position;
    OUT.position.z = 0.999;
    OUT.positionCS = IN.position;

    return OUT;
}