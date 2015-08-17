
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

	const float luminance = srcColor.r*0.299 + srcColor.g*0.587 + srcColor.b*0.114;

	return float4(luminance,luminance,luminance,srcColor.a) * input.color;
}
