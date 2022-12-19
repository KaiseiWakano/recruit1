// ========================================================================================
//
// シェーダーの処理                                               ver 3.0        2021.1.11
//
//   シェーダープログラムの読み込みとシェーダーの作成
//   インプットレイアウトの作成
//   コンスタントバッファの作成
//
//
//    登録されているシェーダー
//
//		・スプライト用のシェーダー　InitShaderSprite()
//
//                                                                              Shader.h
// ========================================================================================
#pragma once

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"

//シェーダーのバッファ構造体定義

//　スプライトシェーダー用のコンスタントバッファーのアプリ側構造体 
struct CONSTANT_BUFFER_SPRITE
{
	MATRIX4X4   mW;
	float       ViewPortWidth;
	float       ViewPortHeight;
	VECTOR2     vUVOffset;
	VECTOR4     vColor;      // カラー情報。半透明の割合を指定する
	VECTOR4     vMatInfo;    // マテリアル関連情報　x:テクスチャ有り無し。DrawRect()、DrawLine()で使用。
	CONSTANT_BUFFER_SPRITE()
	{
		ZeroMemory(this, sizeof(CONSTANT_BUFFER_SPRITE));
	}
};


//
// CShaderクラス
//
class CShader
{
public:
	// Direct3D11
	CDirect3D*              m_pD3D;

	// シェーダー
	ID3D11InputLayout*      m_pSprite2D_VertexLayout;
	ID3D11VertexShader*     m_pSprite2D_VS;
	ID3D11PixelShader*      m_pSprite2D_PS;

	// コンスタントバッファ
	ID3D11Buffer*			m_pConstantBufferSprite2D;


public:
	HRESULT InitShader();
	HRESULT InitShaderSprite();
	HRESULT InitShaderConstant();

	HRESULT MakeShader(TCHAR ProfileName[], TCHAR FileName[], void** ppShader, D3D11_INPUT_ELEMENT_DESC Fluid_layout[] = NULL, UINT numElements = 0, ID3D11InputLayout** ppInputLayout = NULL);
	HRESULT MakeConstantBuffer(UINT size, ID3D11Buffer**  ppConstantBuffer);

	CShader(CDirect3D* pD3D);
	~CShader();

};
