//------------------------------------------------------------------------
// 武器の処理 Weapon.h
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

#define  WEAPON_SHOT_MAX  200
#define  WEAPON_GRENADE_MAX 50
#define  WEAPON_BOSSATTACK_MAX 200

//======================================================================
// 武器・ショット オブジェクトクラス
class CWeaponShotObj : public CBaseObj
{
protected:
	DWORD m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	// コンストラクタ
	CWeaponShotObj(CGameMain* pGMain);
	// デストラクタ
	~CWeaponShotObj();
};

//======================================================================
// 武器・ショット プロシージャクラス
class CWeaponShotProc : public CBaseProc
{
public:
	BOOL Start( VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	// コンストラクタ
	CWeaponShotProc(CGameMain* pGMain);	
	// デストラクタ
	~CWeaponShotProc() { ; }
};

//======================================================================
// 武器・小型爆弾 オブジェクトクラス
class CWeaponGrenadeObj : public CBaseObj
{
protected:
	DWORD  m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);
	void Update() override;

	// コンストラクタ
	CWeaponGrenadeObj(CGameMain* pGMain);
	// デストラクタ
	~CWeaponGrenadeObj();
};

//======================================================================
// 武器・小型爆弾 プロシージャクラス
class CWeaponGrenadeProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	// コンストラクタ
	CWeaponGrenadeProc(CGameMain* pGMain);	
	// デストラクタ
	~CWeaponGrenadeProc() { ; }
};

//======================================================================
// ラスボスの攻撃 オブジェクトクラス
class CWeaponBossShotObj : public CBaseObj
{
protected:
	DWORD m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	// コンストラクタ
	CWeaponBossShotObj(CGameMain* pGMain);
	// デストラクタ
	~CWeaponBossShotObj();
};

//======================================================================
// ラスボスの攻撃 プロシージャクラス
class CWeaponBossShotProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	// コンストラクタ
	CWeaponBossShotProc(CGameMain* pGMain);	
	// デストラクタ
	~CWeaponBossShotProc() { ; }

};

//======================================================================
// 武器全体のメインプロシージャクラス
//======================================================================
class CWeaponProc : public CBaseProc
{
public:
	CWeaponShotProc*	m_pWeaponShotProc;
	CWeaponGrenadeProc* m_pWeaponGrenadeProc;
	CWeaponBossShotProc* m_pWeaponBossShotProc;

public:

	CWeaponProc(CGameMain* pGMain);	// コンストラクタ
	~CWeaponProc() { ; }			// デストラクタ
};
