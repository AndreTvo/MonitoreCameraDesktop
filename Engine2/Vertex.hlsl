cbuffer ConstantBuffer
{
	float4x4 WorldViewProj;
}

struct VertexIn
{
	min16float3 Pos   : POSITION;
	min16float4 Color : COLOR;
	min16float2 Tex   : TEXCOORD;
};

struct VertexOut
{
	min16float4 Pos   : SV_POSITION;
	min16float4 Color : COLOR;
	min16float2 Tex   : TEXCOORD;
};

VertexOut main( VertexIn vIn )
{
	VertexOut vOut;

	// transforma vértices para coordenadas da tela
	vOut.Pos = min16float4(mul(float4(vIn.Pos, 1.0f), WorldViewProj));
	
	// mantém as cores inalteradas
	vOut.Color = vIn.Color;

	// mantém as coordenadas da textura inalteradas
	vOut.Tex = vIn.Tex;

	return vOut;
}