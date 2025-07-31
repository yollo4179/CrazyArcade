#include"../../../EngineSDK/HLSL/HLSL_Defines.hlsli"
/*프로젝트 기준 아님*/
Texture2D g_InputTexture;
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
bool g_Empty;
struct VS_IN
{
    float3 vPosition : POSITION; //점 하나
};
struct VS_OUT
{
    float4 vPosition : POSITION;
};
VS_OUT VS_MAIN(VS_IN In)
{
    
    
    VS_OUT Out = (VS_OUT) 0;
  
    //(0,0)에 존재하는 점 하나( local) 
    Out.vPosition = float4(In.vPosition, 1.f);
    
	return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
};
struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};
[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    
    GS_OUT Out[4]  ;
 
    vector vExpandToX = vector((g_WorldMatrix._11_12_13) / 2.F, 0.f);
    vector vExpandToY = vector((g_WorldMatrix._21_22_23) / 2.F, 0.f);
    matrix mVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    vector vPos = (g_WorldMatrix._41_42_43_44);
    Out[0].vPosition = vPos - vExpandToX + vExpandToY;
    Out[0].vTexcoord = float2(0, 0);
    Out[1].vPosition = vPos + vExpandToX + vExpandToY;
    Out[1].vTexcoord = float2(1, 0);
    Out[2].vPosition = vPos + vExpandToX - vExpandToY;
    Out[2].vTexcoord = float2(1, 1);
    Out[3].vPosition = vPos - vExpandToX - vExpandToY;
    Out[3].vTexcoord = float2(0, 1);

    for (int i = 0; i < 4;++i)
    {
        Out[i].vPosition = mul(Out[i].vPosition, mVP);
    }
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();

    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();

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
    if (!g_Empty)
        Out.vColor = g_InputTexture.Sample(LinearSampler, In.vTexCoord);

    
    if (Out.vColor.a <0.1)
        discard;
	//Out.vColor.gb = 0.f;	

        return Out;
}

technique11 DefaultTechnique // 기본
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
    pass DefaultPass //0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}