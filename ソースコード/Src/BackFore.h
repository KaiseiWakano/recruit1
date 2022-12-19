//------------------------------------------------------------------------
// 前景・後景処理ヘッダ BackFore.h
//------------------------------------------------------------------------

#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

// ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

#define BOX_ITEM_MAX 10
#define TIME_LIMIT 180

//======================================================================
// バッククラウンド　プロシージャクラス
//======================================================================
class CBackProc
{
protected:
	CGameMain*		m_pGMain;
	CSpriteImage*	m_pImageBk;
	CSprite*		m_pBackgrd;
	VECTOR2			m_vScroll;
};
//======================================================================
// フォアクラウンド　プロシージャクラス
//======================================================================
class CForeProc
{
private:
	int canOpen;

protected:
	CGameMain*		m_pGMain;
	CSprite*		m_pForegrd;

public:
	void Update();
	// アイテム表示、管理関数
	void ShowAndSetItem(int num, int isGetItem[], int stack, int cntItem[]);
	int getCanOpen() { return canOpen; }

	// コンストラクタ
	CForeProc(CGameMain* pGMain);
	// デストラクタ
	~CForeProc();
};
