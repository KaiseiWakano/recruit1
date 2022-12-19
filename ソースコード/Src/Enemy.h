//------------------------------------------------------------------------
// 敵の処理ヘッダ Enemy.h
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

#define  ENM_WATER_MAX  30
#define  ENM_TORD_MAX  20
#define  ENM_WOLF_MAX  20
#define  ENM_FOX_MAX  20
#define  ENM_BOSS_MAX  20
#define  ENM_LASTBOSS_MAX 20

extern bool flagBossDeath;
extern bool flagBio;

//======================================================================
// 泉の水の敵　オブジェクトクラス
class CEnmWaterObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// コンストラクタ
	CEnmWaterObj(CGameMain* pGMain);
	~CEnmWaterObj();
};
//======================================================================
// 泉の水の敵　プロシージャクラス
class CEnmWaterProc : public CBaseProc
{
public:
	void   Update() override;

	// コンストラクタ
	CEnmWaterProc(CGameMain* pGMain);
	~CEnmWaterProc() { ; }
};
//======================================================================
// がまの敵　オブジェクトクラス
class CEnmTordObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// コンストラクタ
	CEnmTordObj(CGameMain* pGMain);
	~CEnmTordObj();
};
//======================================================================
// がまの敵　プロシージャクラス
class CEnmTordProc : public CBaseProc
{
public:
	void   Update() override;

	// コンストラクタ
	CEnmTordProc(CGameMain* pGMain);
	~CEnmTordProc() { ; }
};
//======================================================================
// オオカミの敵　オブジェクトクラス
class CEnmWolfObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// コンストラクタ
	CEnmWolfObj(CGameMain* pGMain);
	~CEnmWolfObj();
};
//======================================================================
// オオカミの敵　プロシージャクラス
class CEnmWolfProc : public CBaseProc
{
public:
	void   Update() override;

	// コンストラクタ
	CEnmWolfProc(CGameMain* pGMain);
	~CEnmWolfProc() { ; }
};
//======================================================================
// きつねの敵　オブジェクトクラス
class CEnmFoxObj : public CBaseObj
{
protected:
	int m_nFoxShotWait = 200;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// コンストラクタ
	CEnmFoxObj(CGameMain* pGMain);
	~CEnmFoxObj();
};
//======================================================================
// きつねの敵　プロシージャクラス
class CEnmFoxProc : public CBaseProc
{
public:
	void   Update() override;

	// コンストラクタ
	CEnmFoxProc(CGameMain* pGMain);
	~CEnmFoxProc() { ; }
};
//======================================================================
// ボスの敵　オブジェクトクラス
class CEnmBossObj : public CBaseObj
{
protected:
	int m_nBossShotWait = 200;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// コンストラクタ
	CEnmBossObj(CGameMain* pGMain);
	~CEnmBossObj();
};
//======================================================================
// ボスの敵　プロシージャクラス
class CEnmBossProc : public CBaseProc
{
public:
	void Update() override;

	// コンストラクタ
	CEnmBossProc(CGameMain* pGMain);
	~CEnmBossProc() { ; }
};
//======================================================================
// ラスボスの敵 オブジェクトクラス
class CEnmLastBossObj : public CBaseObj
{
protected:
	int m_nLastBossShotWait = 200;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// コンストラクタ
	CEnmLastBossObj(CGameMain* pGMain);
	~CEnmLastBossObj();
};
//======================================================================
// ラスボスの敵 プロシージャクラス
class CEnmLastBossProc : public CBaseProc
{
public:
	void Update() override;

	// コンストラクタ
	CEnmLastBossProc(CGameMain* pGMain);
	~CEnmLastBossProc() { ; }
};

//======================================================================
// 敵キャラクター全体のメインプロシージャクラス
//======================================================================
class CEnmProc : public CBaseProc
{
public:
	CEnmWaterProc*	m_pEnmWaterProc;
	CEnmTordProc*	m_pEnmTordProc;
	CEnmWolfProc*	m_pEnmWolfProc;
	CEnmFoxProc*	m_pEnmFoxProc;	
	CEnmBossProc*   m_pEnmBossProc;
	CEnmLastBossProc*   m_pEnmLastBossProc;

public:
	void  SetNonActive();
	CEnmProc(CGameMain* pGMain);	// コンストラクタ
	~CEnmProc() { ; }
};
