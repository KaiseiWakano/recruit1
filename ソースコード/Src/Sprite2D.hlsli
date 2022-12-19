//-----------------------------------------------------------------------
// ２Ｄスプライトのシェーダー Sprite2D.hlsli
//-----------------------------------------------------------------------

//グローバル
Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

//グローバル
cbuffer global_0:register(b0)
{
	matrix g_W;              //ワールド変換行列のみ
	float  g_ViewPortWidth;  //ビューポート（スクリーン）横サイズ
	float  g_ViewPortHeight; //ビューポート（スクリーン）縦サイズ
	float2 g_UVOffset;       //テクスチャ座標　オフセット
	float4 g_Diffuse;        // ディフューズカラー                        // 2020.1.24
	float4 g_MatInfo;        // マテリアル関連情報　x:テクスチャ有り無し  // 2017.10.8
};

//構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

//
// バーテックスシェーダー
//
PS_INPUT VS(float4 Pos : POSITION, float2 UV : TEXCOORD)
{
	PS_INPUT Out;

	Out.Pos = mul(Pos, g_W);

	Out.Pos.x = (Out.Pos.x / g_ViewPortWidth) * 2 - 1;
	Out.Pos.y = 1 - (Out.Pos.y / g_ViewPortHeight) * 2;

	Out.UV = UV + g_UVOffset;

	return Out;
}
//
// ピクセルシェーダー
//
float4 PS(PS_INPUT In) : SV_Target
{
	float4 Color;
	if (g_MatInfo.x == 1)
	{   // テクスチャ有りのとき
		float4 texColor = g_Texture.Sample(g_Sampler, In.UV);
		Color.rgb = texColor.rgb * g_Diffuse.rgb;
		Color.a   = texColor.a - (1 - g_Diffuse.a); 
	}
	else { // テクスチャなしのとき
		Color = g_Diffuse;
	}
	return saturate( Color );
}
