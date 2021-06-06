struct ConstantData
{
    float4x4 WorldViewProj;
    float4x4 World;   
    float4 ViewerPos;
};

struct LightData
{
    float4 Direction;
    float4 Color;
    float4 KaSpecPowKsX;
};

cbuffer ConstBuf : register(b0)
{
    ConstantData ConstData;
}

cbuffer LightBuf : register(b1)
{
    LightData Lights;
}

struct VS_IN 
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 tex : TEXTCOORD;    
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float4 worldPos : TEXCOORD1;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;

    output.pos = mul(float4(input.pos.xyz, 1.0f), ConstData.WorldViewProj);
    output.normal = mul(float4(input.normal.xyz, 1.0f), ConstData.World);
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), ConstData.World);

    output.tex = input.tex.xy;

    return output;
}

float4 PSMain1(PS_IN input, in bool isFrontFace : SV_IsFrontFace) : SV_Target
{
    float4 col = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
	
    clip(col.a - 0.01f);
    
    return col;
}

float4 PSMain(PS_IN input, in bool isFrontFace : SV_IsFrontFace) : SV_Target
{
    float4 col = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
	
    clip(col.a - 0.01f);
    
    float3 kd = col.xyz;
    float3 normal = normalize(input.normal.xyz);
    float3 viewDir = normalize(ConstData.ViewerPos.xyz - input.worldPos.xyz);
    float3 lightDir = -Lights.Direction.xyz; //float3(10, 10, 0)
    float3 refVec = normalize(reflect(lightDir, normal));
    
    float3 diffuse = max(0, dot(lightDir, normal)) * kd;
    float3 ambient = kd * Lights.KaSpecPowKsX.x; //
    float3 spec = pow(max(0, dot(-viewDir, refVec)), Lights.KaSpecPowKsX.y) * Lights.KaSpecPowKsX.z; // 
    
    
    return float4((diffuse + ambient + spec), 1.0f); //ConstData.Color.xyzfloat3(255, 255, 255) * 

}