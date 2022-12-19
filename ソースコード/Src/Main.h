//------------------------------------------------------------------------
// メイン処理ヘッダ Main.h
//------------------------------------------------------------------------

#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "resource.h"
#include "GameMain.h"

//定数定義
#define WINDOW_WIDTH   1366   //ウィンドウ幅
#define WINDOW_HEIGHT  768    //ウィンドウ高さ

#define APP_NAME _T("KEEP YOUR LIFE")

class CGameMain;

// Windowsメインクラス
class CMain
{
public:
	// メンバ変数
	CGameMain*	   m_pGMain;		  // ゲームメインクラス
	HINSTANCE      m_hInstance;
	HWND           m_hWnd;
	BOOL           m_bLoopFlag;		  // ループフラグ　TRUE:ループ　FALSE:ループ終了
	double         m_MainLoopTime;    // ループスピード　1/60Sec

	// メソッド
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCTSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	HRESULT Init();
	void    MessageLoop();
	BOOL    MessageProcess(MSG* msg);
	void    Loop();
	void    DispFps();
	void    Quit();
	CMain();
	~CMain();
};
//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
