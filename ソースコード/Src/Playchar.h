//------------------------------------------------------------------------
// プレイヤーの処理ヘッダ Playchar.h
//------------------------------------------------------------------------

#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

extern int m_nPcNum;

// プレイキャラクター　オブジェクトクラス
class CPcObj : public CBaseObj
{
protected:
	int m_nFireballWait;
	int m_nBombWait;
	int checkDeath = 0;
public:
	void Update() override;
	// コンストラクタ
	CPcObj(CGameMain* pGMain);
	~CPcObj();
};

// プレイキャラクター　プロシージャクラス
class CPcProc : public CBaseProc
{
public:
	// アクセス関数
	CPcObj*			    GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }
	// コンストラクタ
	CPcProc(CGameMain* pGMain);
	~CPcProc() { ; }
};
