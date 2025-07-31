#include"..\..\EngineSDK\HLSL\HLSL_Defines.h"
Texture2D g_InputTexture;
float4x4 g_worldMatrix, g_ViewMatrix, g_ProjMatrix;
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD0;
};
struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
 
};
float4 VS_P1_RECT( VS_IN In) : SV_POSITION
{
    
    
    VS_OUT Out = { };
    matrix mWV = mul(g_worldMatrix, g_ViewMatrix);
    matrix mWVP = mul(mWV, g_ProjMatrix);
    vector vPos = mul(vector(In.vPosition, 1.f), mWVP);
    
    Out.vPosition = vPos; 
    Out.vTexCoord = In.vTexCoord; 
	return Out;
}
struct PS_OUT
{
    float vColor : SV_TARGET0; //일단 포워드 
};
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_InputTexture.Sample(LinearSampler, In.vTexcoord);

	//Out.vColor.gb = 0.f;	

    return Out;
}
