
cbuffer cbPerObject: register(b0)
{
	float4x4 gWorldViewProj;
};


struct VertexIn
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct  VertexOut
{
	float3 Pos : SV_POSITION;
	float4 Color : COLOR;
};


VertexOut VS(VertexIn vin)
{
	VertexOut Out;

	Out.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	Out.Color = vin.Color;

	return Out;
}


float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}