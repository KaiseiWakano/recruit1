//------------------------------------------------------------------------
// オープニングタイトル・ゲームクリヤー・ゲームオーバーの処理 Title.cpp
//------------------------------------------------------------------------

#include "Title.h"
#include "Playchar.h"
#include "time.h"

// タイトルプロシージャのコンストラクタ
CTitleProc::CTitleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;
	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/titleImage.png"));			// イメージのロード
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/clearImage.png"));			// イメージのロード
	m_pImageOver = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/overImage.png"));			// イメージのロード
	m_pImageClearScore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ClearScore.png"));	// イメージのロード
	m_pImageCursor = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/cursor.png"));			// イメージのロード
	m_pImageGuide = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/guideImage.png"));			// イメージのロード
	m_pSprite = new CSprite(m_pGMain->m_pShader ); // スプライトの生成

	m_dwCntTime = 0;
	flagTitleOrOther = true;
}

// タイトルタイトルプロシージャのデストラクタ
CTitleProc::~CTitleProc()
{
	SAFE_DELETE(m_pImageTitle);
	SAFE_DELETE(m_pImageClear);
	SAFE_DELETE(m_pImageOver);
	SAFE_DELETE(m_pImageClearScore);
	SAFE_DELETE(m_pImageCursor);
	SAFE_DELETE(m_pImageGuide)
	SAFE_DELETE(m_pSprite);
}

// タイトルの表示 K.Wakano
void CTitleProc::Title()
{
	int showY;	// カーソルのY座標を格納
	// 背景の表示
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	showY = WINDOW_HEIGHT - 300;	// はじめからの位置に表示
	
	// キー操作の判定
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_DOWN))
	{
		flagTitleOrOther = false;	// あそびかたの位置
	}
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_UP))
	{
		flagTitleOrOther = true;	// はじめからの位置
	}
	m_dwCntTime++;
	// 時間を計測し表示非表示を切り替え点滅させる
	if (m_dwCntTime % 30 > 15)
	{
		if (flagTitleOrOther == true)
		{
			// はじめからの位置
			showY = WINDOW_HEIGHT - 300;
			m_pSprite->Draw(m_pImageCursor, WINDOW_WIDTH - 450, showY, 0, 0, m_pImageCursor->m_dwImageWidth, m_pImageCursor->m_dwImageHeight, 48, 48);
		}
		else
		{
			// あそびかたの位置
			showY = WINDOW_HEIGHT - 200;
			m_pSprite->Draw(m_pImageCursor, WINDOW_WIDTH - 450, showY, 0, 0, m_pImageCursor->m_dwImageWidth, m_pImageCursor->m_dwImageHeight, 48, 48);
		}
	}

	m_pGMain->m_pFont->Draw(WINDOW_WIDTH - 400, WINDOW_HEIGHT - 300, _T("はじめる"), 48, RGB(255, 255, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH - 400, WINDOW_HEIGHT - 200, _T("あそびかた"), 48, RGB(255, 255, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON))
	{
		if (flagTitleOrOther == true)
		{
			// タイトルにおける経過時間を挿入する
			m_pGMain->m_time = clock() / CLOCKS_PER_SEC;
			// サウンドの再生
			m_pGMain->m_pBgm1->Play(AUDIO_LOOP);
			m_pGMain->m_dwGameStatus = GAMEMAIN;
			m_dwCntTime = 0;
		}
		else
		{
			m_pGMain->m_dwGameStatus = GUIDE;
		}
	}
}

// 遊び方の表示 K.Wakano
void CTitleProc::Guide()
{
	m_pSprite->Draw(m_pImageGuide, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT - 75, _T("スペースで戻る"), 48, RGB(255, 255, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON))
	{
		m_pGMain->m_dwGameStatus = TITLE;
	}
}

// ゲームクリアの表示
void CTitleProc::GameClear()
{
	// 表示
	m_pSprite->Draw(m_pImageClear, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);	

	// 最終スコア表示 K.Wakano
	TCHAR str[256];
	m_pSprite->Draw(m_pImageClearScore, WINDOW_WIDTH / 2 - 128, WINDOW_HEIGHT / 2 - 108, 0, 0, 256, 256);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 80, WINDOW_HEIGHT / 2 - 108 + 17, _T("Score"), 36, RGB(255, 255, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	_stprintf(str, _T("%04d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 80, WINDOW_HEIGHT / 2 - 108 + 140, str, 36, RGB(255, 255, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)
	{
		m_pGMain->m_pBgm1->Stop();
		m_pGMain->m_dwGameStatus = GAMEEND;
		m_dwCntTime = 0;
	}
}

//	ゲームオーバーの表示
void CTitleProc::GameOver()
{
	// 表示
	m_pSprite->Draw(m_pImageOver, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) ||
		m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)
	{
		m_pGMain->m_pBgm1->Stop();
		m_pGMain->m_dwGameStatus = GAMEEND;
		m_dwCntTime = 0;
	}
}