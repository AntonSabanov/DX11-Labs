struct ConstantData 
{
	float4x4 WorldViewProj;
};

cbuffer ConstBuf:register(b0) 
{
	ConstantData ConstData;
}

struct VS_IN 
{
	float4 pos 	: POSITION;	
	float4 tex 	: TEXTCOORD;
};

struct PS_IN
{
	float4 pos 	: SV_POSITION;
	float2 tex 	: TEXCOORD;
};

Texture2D 	DiffuseMap 	:register(t0);
SamplerState 	Sampler 	: register(s0);

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;

	output.pos = mul(float4(input.pos.xyz, 1.0f), ConstData.WorldViewProj);
	output.tex = input.tex.xy;

	return output;
}

float4 PSMain(PS_IN input, in bool isFrontFace : SV_IsFrontFace) : SV_Target
{
	float4 col = {1.0f, 1.0f, 0.0f, 1.0f};

	col = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
	
	return col;
}