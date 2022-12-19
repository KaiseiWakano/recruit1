//------------------------------------------------------------------------------
// オープニングタイトル・ゲームクリヤー・ゲームオーバーの処理ヘッダ Title.h
//------------------------------------------------------------------------------

#pragma once

#include "GameMain.h"

// タイトル プロシージャクラス
class CTitleProc : public CBaseProc
{
protected:
	CGameMain*		m_pGMain;

	CSpriteImage*   m_pImageTitle;
	CSpriteImage*   m_pImageClear;
	CSpriteImage*   m_pImageOver;
	CSpriteImage*   m_pImageClearScore;
	CSpriteImage*	m_pImageCursor;
	CSpriteImage*   m_pImageGuide;
	CSprite*        m_pSprite;

	DWORD           m_dwCntTime;
public:
	void Title();
	void GameClear();
	void GameOver();
	void Guide();
	CTitleProc(CGameMain* pGMain);	// コンストラクタ
	~CTitleProc();
private:
	bool flagTitleOrOther;
};