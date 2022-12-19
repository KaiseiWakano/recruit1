//------------------------------------------------------------------------
// マップの処理ヘッダ Map.h
//------------------------------------------------------------------------

#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

#define   MAP_IMAGE_PATH  _T("Data/")
#define   MAP_MAX 4

//----------------------------------------------------------------------------
// マップ線クラスの定義
// 始点座標 {Ｘ座標,Ｙ座標}, 終点座標 {Ｘ座標, Ｙ座標}......
// 法線座標 (開始位置から終了位置に向かって、左手方向に法線が出来る)
//----------------------------------------------------------------------------
class CMapLine
{
public:
	VECTOR2 m_vStart;		// 始点座標
	VECTOR2 m_vEnd;			// 終点座標
	VECTOR2 m_vNormal;		// 法線座標 (開始位置から終了位置に向かって、左手方向に法線が出来る)

public:
	// コンストラクタ
	CMapLine()
	{
		m_vStart  = VECTOR2(0, 0);
		m_vEnd    = VECTOR2(0, 0);
		m_vNormal = VECTOR2(0, 0);
	}
};
//----------------------------------------------------------------------------
// イベントマップクラスの定義
// 座標｛Ｘ座標、Ｙ座標｝、イベントＩＤ、イベントＮｏ(種類を論理和で)、汎用カウンタ
// イベントID 1:PCのスタート位置 2:アイテム 3:敵
// イベントNo Effect.cpp参照
//----------------------------------------------------------------------------
class CEvtMap
{
public:
	VECTOR2     m_vStart;		// 座標
	int         m_nEvtID;		// イベントＩＤ
	DWORD       m_dwEvtNo;		// イベントＮｏ
	int         m_nCnt1;		// 汎用カウンタ
public:
	// コンストラクタ
	CEvtMap()
	{
		m_vStart     = VECTOR2(0, 0);
		m_nEvtID     = 0;
		m_dwEvtNo    = 0;
		m_nCnt1      = 0;
	}
	CEvtMap(VECTOR2 vStart, int nEvtID, DWORD dwEvtNo, int nCnt1)
	{
		m_vStart      = vStart;
		m_nEvtID      = nEvtID;
		m_dwEvtNo     = dwEvtNo;
		m_nCnt1       = nCnt1;
	}
};

class CGameMain;

//----------------------------------------------------------------------------
// マップ配列クラスの定義
//----------------------------------------------------------------------------
class CMap
{ 
public:
	CGameMain*				 m_pGMain;
	TCHAR                    m_MapFileName[512];	// マップファイル名
	TCHAR                    m_ImageName[512];		// マップチップイメージファイル名
	CSpriteImage*            m_pMapImage;			// マップチップのスプライトイメージ
	int                      m_nMapX;				// 画面の幅　（マップチップが何個分か）
	int                      m_nMapY;				// 画面の高さ（マップチップが何個分か）
	int                      m_nMapchipWidth;		// 一つのマップチップの幅
	int                      m_nMapchipHeight;		// 一つのマップチップの高さ
	int                      m_nMapLnLength;		// マップライン配列の要素数
	int                      m_nEvtMapLength;		// イベントマップ配列の要素数
	int*                     m_MapBakTbl;			// マップ配列　背景
	int*                     m_MapForTbl;			// マップ配列　前景
	CMapLine*                m_MapLn;				// マップライン配列
	CEvtMap*                 m_EvtMap;				// イベントマップ配列

public:
	// コンストラクタ
	CMap(CGameMain*	pGMain);
	void InitMap();
	BOOL  MakeERelativeFName(TCHAR* szBaseFullPath, TCHAR* szPath, TCHAR* szERelativeFName);

	~CMap();
};

//----------------------------------------------------------------------------
// マッププロシージャクラスの定義
//----------------------------------------------------------------------------
class CMapProc
{
protected:
	CGameMain*	m_pGMain;

	// マップ名
	int   m_nMapNo;
	// マップ配列
	CMap* m_pMapArray[MAP_MAX];
	// マップ線の描画フラグ
	BOOL  m_bDrawMapLine;
	// マップイメージ
	CSpriteImage* m_pBakImage;
	// マップスプライト
	CSprite*      m_pSpriteMap;
	CSprite*      m_pSpriteBak;
	CSprite*      m_pSpriteLine;

public:
	// コンストラクタ
	CMapProc(CGameMain*	pGMain);
	~CMapProc();
	void InitMapAll();
	BOOL MapRead(TCHAR* FileName, CMap* &pMap);
	void MoveMap();
	void SetMap(int);
	void Update();
	void Update2();
	void DrawBack();
	void DrawFore();
	void DrawMapLine();
	BOOL isCollisionMoveMap(CBaseObj* obj, CMapLine* &pHitmapline);
	BOOL CheckMapcross(CMapLine map, CBaseObj* obj, VECTOR2 &hitpos);
	BOOL CheckLinecross(VECTOR2 a1, VECTOR2 a2, VECTOR2 b1, VECTOR2 b2, VECTOR2 &hitpos);
	BOOL CheckRange(float l, float r, float pt); 
	BOOL CheckMapnear(CMapLine MapLn, CBaseObj* obj, VECTOR2 &vHitpos);
	float GetDistance(CMapLine MapLn, CBaseObj* obj);
	float GetLength(VECTOR2 p1, VECTOR2 p2);
	float GetCross(VECTOR2 a, VECTOR2 b);
	float GetDot(VECTOR2 a, VECTOR2 b);

	BOOL SearchEvt(int nStart, int nEvtID, DWORD dwEvtNo, VECTOR2& vPos, int& nNext);
};
