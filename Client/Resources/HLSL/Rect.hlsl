#include"../../../EngineSDK/HLSL/HLSL_Defines.hlsli"
/*프로젝트 기준 아님*/
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
VS_OUT VS_MAIN(VS_IN In)
{
    
    
    VS_OUT Out = (VS_OUT) 0;
    matrix mWV = mul(g_worldMatrix, g_ViewMatrix);
    matrix mWVP = mul(mWV, g_ProjMatrix);
    vector vPos = mul(vector(In.vPosition, 1.f), mWVP);
    
    Out.vPosition = vPos; 
    Out.vTexCoord = In.vTexCoord; 
	return Out;
}
struct PS_OUT
{
    float4 vColor : SV_TARGET0; //일단 포워드 
};
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_InputTexture.Sample(LinearSampler, In.vTexCoord);

	//Out.vColor.gb = 0.f;	

    return Out;
}
technique11 DefaultTechnique
{


    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}