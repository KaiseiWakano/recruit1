// ========================================================================================
// スプライトの処理 Sprite.h
// ========================================================================================

#pragma once

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "Shader.h"

// マクロ定義
// 作成するフォントテクスチャの最大数
#define  TEXT_DATA_MAX  100

// スプライト用頂点の構造体
struct SpriteVertex
{
	VECTOR3 Pos;   //位置
	VECTOR2 UV;	   //テクスチャー座標
};

// CSpriteImageクラス
class CShader;

class CSpriteImage
{
public:
	// Direct3D11
	CDirect3D*     m_pD3D;
	CShader*       m_pShader;

	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）
	DWORD          m_dwImageWidth;
	DWORD          m_dwImageHeight;

	CSpriteImage(CShader* pShader);
	CSpriteImage(CShader* pShader, TCHAR* TName);
	~CSpriteImage();
	HRESULT Load(TCHAR* TName);
};

// CSpriteクラス
class CSprite
{
protected:
	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;
	DWORD			m_dwDestWidth;
	DWORD			m_dwDestHeight;

public:
	// Direct3D11
	CDirect3D*      m_pD3D;
	CShader*        m_pShader;

	ID3D11Buffer*   m_pVertexBufferSprite;
	ID3D11Buffer*   m_pVertexBufferLine;    // -- 2017.10.9
	ID3D11Buffer*   m_pVertexBufferRect;    // -- 2018.3.20

	CSpriteImage*   m_pImage;
	DWORD			m_ofX;
	DWORD			m_ofY;
	VECTOR4         m_vDiffuse;   // ディフューズ色(x:r y:g z:b w:a)。スプライトのみ使用。ラインや四角形には使用しない。  // -- 2020.1.24
	int             m_nBlend;     // ブレンドステート  0:通常 1:透過色（初期値） 2:加算合成     // -- 2019.8.18

public:
	CSprite(CShader* pShader);
	CSprite(CSpriteImage* pImage);    // -- 2017.10.9
	CSprite(CSpriteImage* pImage, DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight);
	CSprite(CSpriteImage* pImage, DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight, DWORD destwidth, DWORD destheight);
	~CSprite();
	void    SetImage(CSpriteImage* pImage);    // -- 2017.10.9
	HRESULT SetSrc( DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight);
	HRESULT SetSrc( DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight, DWORD destwidth, DWORD destheight);
	HRESULT SetSrc( CSpriteImage* pImage, DWORD srcX, DWORD srcY, DWORD width, DWORD height);
	HRESULT SetSrc( CSpriteImage* pImage, DWORD srcX, DWORD srcY, DWORD width, DWORD height, DWORD destwidth, DWORD destheight);
	DWORD   GetSrcX() { return m_dwSrcX; }    // -- 2017.10.9
	DWORD   GetSrcY() { return m_dwSrcY; }    // -- 2017.10.9
	DWORD   GetSrcWidth() { return m_dwSrcWidth; }
	DWORD   GetSrcHeight() { return m_dwSrcHeight; }
	DWORD   GetDestWidth() { return m_dwDestWidth; }
	DWORD   GetDestHeight() { return m_dwDestHeight; }

	void    Draw(float posX, float posY);
	void    Draw(MATRIX4X4 mWorld);
	void    Draw(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight, float fAlpha = 1.0f);
	void    Draw(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight, DWORD destwidth, DWORD destheight, float fAlpha = 1.0f);
	void    Draw(CSpriteImage* pImage, MATRIX4X4 mWorld, DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight, float fAlpha = 1.0f);
	void    Draw(CSpriteImage* pImage, MATRIX4X4 mWorld, DWORD srcX, DWORD srcY, DWORD srcwidth, DWORD srcheight, DWORD destwidth, DWORD destheight, float fAlpha = 1.0f);
	void    DrawLine(float StartX, float StartY, float EndX, float EndY, DWORD Width, DWORD colorABGR, float fAlpha = 1.0f);
	void    DrawRect(float posX, float posY, DWORD width, DWORD height, DWORD colorABGR, float fAlpha = 1.0f);    // -- 2018.3.20

	void    SetShader();
	void    ResetShader();
};

// ========================================================================================
// フォント テクスチャーの処理
// ========================================================================================
// 文字フォントの構造体
struct FontData
{
	TEXTMETRIC   TM;
	GLYPHMETRICS GM;
	BYTE*        ptr;
	FontData()
	{
		ZeroMemory(this, sizeof(FontData));
	}
};

// テキストデータの構造体
struct TextData
{
	TCHAR*                      m_szText;           // -- 2018.12.28
	int                         m_iFontsize;
	DWORD                       m_dwColor;
	float                       m_fAlpha;
	ID3D11ShaderResourceView*	m_pResourceView;	//テクスチャーリソースビュー
	ID3D11Buffer*				m_pVertexBufferFont;
	TextData()
	{
		ZeroMemory(this, sizeof(TextData));
	}
};

// CFontTextureクラス
class CFontTexture
{
public:
	// Direct3D11
	CDirect3D*     m_pD3D;
	CShader*       m_pShader;

	TextData       m_TextData[TEXT_DATA_MAX];
	DWORD          m_Idx;       // TextDataのインデックス

	void Refresh();
	void Draw(float posX, float posY, TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha=1.0f, TCHAR* szFontName=NULL);
	void Draw(MATRIX4X4 mWorld, TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha=1.0f, TCHAR* szFontName=NULL);
	void SetShader();
	void ResetShader();
	void CreateVB(DWORD dwWidth, DWORD dwHeight);
	void CreateTex(TCHAR* text, int fontsize, DWORD colorABGR, TCHAR* szFontName);

	CFontTexture(CShader* pShader);
	~CFontTexture();
};
