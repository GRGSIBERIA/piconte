
Texture2D texture0 : register( t0 );
SamplerState sampler0 : register( s0 );

struct VS_OUTPUT
{
	float4 postion : SV_POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
};

float4 PS(in VS_OUTPUT input) : SV_Target
{
	const float2 uv = input.tex;

	const float4 srcColor = texture0.Sample(sampler0, uv);

	return srcColor * input.color;
}
