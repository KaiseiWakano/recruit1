//------------------------------------------------------------------------
// 映像効果処理ヘッダ Effect.h
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
#include "BackFore.h"
#include "Enemy.h"

#define  EFFECT_BOM_MAX  50

//======================================================================
// 映像効果・爆発 オブジェクトクラス
class CEffectBomObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2) override;
	void Update() override;

	// コンストラクタ
	CEffectBomObj(CGameMain* pGMain);
	// デストラクタ
	~CEffectBomObj();
};
//======================================================================
// 映像効果・爆発 プロシージャクラス
class CEffectBomProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2);

	// コンストラクタ
	CEffectBomProc(CGameMain* pGMain);	
	// デストラクタ
	~CEffectBomProc() { ; }	
};
//======================================================================
// 映像効果 プロシージャクラス
class CEffectProc : public CBaseProc
{
public:
	CEffectBomProc* m_pEffectBomProc;

public:
	void  SetNonActive();

	// コンストラクタ
	CEffectProc(CGameMain* pGMain);	
	// デストラクタ
	~CEffectProc() { ; }	
};
