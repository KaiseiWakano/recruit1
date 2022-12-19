//------------------------------------------------------------------------
// ゲームメイン処理ヘッダ GameMain.h
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

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Shader.h"
#include "Dinput.h"
#include "Sprite.h"
#include "BaseProc.h"

// ステータスタイプ
#define  NONE				0x00000000
#define  TITLE				0x00000001
#define  GUIDE		  		0x00000002
#define  GAMEMAIN			0x00000004
#define  GAMEOVER			0x00000008
#define  GAMECLEAR			0x00000010
#define  GAMEEND			0x00000020

#define  NORMAL				0x00000001
#define  STANBY				0x00000002
#define  CATCH				0x00000004
#define  ATTACK				0x00000008
#define  DAMAGE				0x00000010
#define  DEAD				0x00000020
#define  FLASH				0x00000040
#define  WALK				0x00000080
#define  JUMP				0x00000100
#define  RAND				0x00000200
#define  STOP				0x00000400

// ステータスサブ
#define  ATTACKNONE			0x00000001
#define  ATTACKMOVE			0x00000002
#define  ATTACKLASER		0x00000004
#define  ATTACKCANNON		0x00000008
#define  ATTACKBOM			0x00000010
#define  ATTACKEARTH		0x00000020

// オブジェクト区分
#define  PC					0x00000001
#define  NPC				0x00000002
#define  ENM				0x00000004

// 方向（DirIdx）
#define  UP					0
#define  RIGHT				1
#define  DOWN				2
#define  LEFT				3

#define  GRAVITY  1.0

class CMain;
class CShader;
class CSpriteImage;
class CSprite;
class CFontTexture;
class CBaseObj;
class CBaseProc;
class CPcProc;
class CEnmProc;
class CWeaponProc;
class CEffectProc;
class CItemManagementProc;
class CForeProc;
class CMapProc;
class CTitleProc;
class CBaseProc;

// --------------------------------------------------------------------------------------------------
//  ゲームメインクラス
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// メンバ変数
	CMain*			m_pMain;		// メインクラス
	CDirect3D*		m_pD3D;			// Direct3Dオブジェクト
	CXAudio*		m_pXAudio;		// XAudioオブジェクト
	CDirectInput*	m_pDI;			// DirectInputDオブジェクト
	CShader*		m_pShader;		// シェーダー
	CFontTexture*	m_pFont;		// フォントテクスチャー

	DWORD			m_dwGameStatus;
	VECTOR2			m_vScroll;

	// プロシージャ
	CPcProc*				m_pPcProc;
	CEnmProc*				m_pEnmProc;
	CWeaponProc*			m_pWeaponProc;
	CEffectProc*			m_pEffectProc;
	CItemManagementProc*	m_pItemManagementProc;
	CForeProc*				m_pForeProc;
	CMapProc*				m_pMapProc;
	CTitleProc*				m_pTitleProc;

	// サウンド
	CXAudioSource*  m_pSeShot;
	CXAudioSource*  m_pSeHit;
	CXAudioSource*  m_pSePower;
	CXAudioSource*  m_pBgm1;
	CXAudioSource*  m_pSeUnlock;

	// スプライト
	CSpriteImage*   m_pImageChar;
	CSpriteImage*   m_pImageSprite;
	
	// タイトルの時間計測用
	int m_time;

	// スコア
	int m_nScore;
	int m_nEndGameSecond;

public:
	// メソッド
	CGameMain(CMain* pMain);
	~CGameMain();

	HRESULT Init();
	void    Loop();
	void    GameMain();
	void    Quit();

	HRESULT ChangeScreenMode(int nMode=-1); 

};
