//------------------------------------------------------------------------
// 前景・後景処理 BackFore.cpp
//------------------------------------------------------------------------

#include "GameMain.h"
#include "Playchar.h"
#include "BackFore.h"
#include "Effect.h"
#include "Map.h"
#include "ItemManager.h"

// ========================================================================================
// 前景の処理
// ========================================================================================
//------------------------------------------------------------------------
//	前景プロシージャのコンストラクタ	
//
//  引数　CGameMain* pGMain
//------------------------------------------------------------------------
CForeProc::CForeProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;
	m_pForegrd = new CSprite(m_pGMain->m_pShader);
	canOpen = -1;
}
// ---------------------------------------------------------------------------
// 前景プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CForeProc::~CForeProc()
{
	SAFE_DELETE(m_pForegrd);
}
//-----------------------------------------------------------------------------
// アイテムを表示および変数をセットする関数 K.Wakano
//-----------------------------------------------------------------------------
void CForeProc::ShowAndSetItem(int num, int isGetItem[], int initEffect, int cntItem[])
{
	// アイテムを所持していない
	if (isGetItem[num] == 0)
	{
		isGetItem[num] = 1;
		cntItem[num] = 1;
		m_pGMain->m_pItemManagementProc->m_pItemProc->setItemList(num, 0, initEffect);	// アイテム所持時の初期値設定
		return;
	}
	else // アイテムを所持している
	{
		cntItem[num] += 1;
		m_pGMain->m_pItemManagementProc->m_pItemProc->setItemList(num, 1, cntItem[num]);
		m_pGMain->m_pItemManagementProc->m_pItemProc->setItemList(num, 2, m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(num, 2));
		return;
	}
}
//-----------------------------------------------------------------------------
// 前景プロシージャの更新
//
// 引数 なし
//-----------------------------------------------------------------------------
void CForeProc::Update()
{
	CSpriteImage* pImageFore = m_pGMain->m_pImageSprite; // すでに読み込んであるスプライトイメージを使用する
	float h = 0, m = 0, t = 0, s = 0;
	int time = clock() / CLOCKS_PER_SEC;		// 経過時間計測
	int scoreMax = 1000;						// スコアの最大値
	TCHAR str[256];
	int DestX, DestY;
	int checkArray[BOX_ITEM_MAX] = {};			// アイテム所持判定配列 未所持:0 所持:1
	int checkArrayCnt[BOX_ITEM_MAX] = {};		// アイテム合計入手数格納配列
	time -= m_pGMain->m_time;					// タイトルの時間と合わせてリセット

	// ステータスバーの表示
	h = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp();
	if (h < 0) h = 0;
	m = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetMp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxMp();
	if (m < 0) m = 0;
	t = (float)time / TIME_LIMIT;
	// スコア計算
	s = (float)m_pGMain->m_nScore / scoreMax;
	if (s > 1) s = 1;

	// 時間オーバーでゲームオーバーに
	if (time == TIME_LIMIT) {
		m_pGMain->m_dwGameStatus = GAMEOVER;
	}

	// ゲームクリア時に残り時間に応じたスコアの加算処理 リミットを超過した場合加算しない K.Wakano
	if (m_pGMain->m_dwGameStatus == GAMECLEAR && time <= TIME_LIMIT)
	{
		m_pGMain->m_nEndGameSecond = time;
		// 制限時間 * 20から、経過時間により徐々に減らしていく
		m_pGMain->m_nScore += (TIME_LIMIT * 20 - (m_pGMain->m_nEndGameSecond * 20));
	}

	// HPとPC残数の表示
	DestX = 10;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 0, 213, 31);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 32, (int)(144 * h), 6);
	_stprintf_s(str, _T("%d"), m_nPcNum);
	m_pGMain->m_pFont->Draw(DestX + 6, DestY + 15, str, 16, RGB(255, 0, 0));
	_stprintf(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
	m_pGMain->m_pFont->Draw(DestX + 26, DestY + 16, str, 12, RGB(0, 0, 0));

	// MPの表示
	DestY = 50;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 156, 213, 28);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 186, (int)(144 * m), 6);
	_stprintf(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetMp());
	m_pGMain->m_pFont->Draw(DestX + 8, DestY + 16, str, 12, RGB(0, 0, 0));

	// 経過時間の表示
	DestX = WINDOW_WIDTH - 213 - 10;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 200, 213, 28);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 230, (int)(144 * t), 6);
	_stprintf(str, _T("%d/180"), time);
	m_pGMain->m_pFont->Draw(DestX + 10, DestY + 16, str, 12, RGB(0, 0, 0));

	// スコアの表示 K.Wakano
	DestY = 50;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 112, 213, 28);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 142, (int)(144 * s), 6);
	_stprintf(str, _T("%04d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(DestX + 10, DestY + 16, str, 12, RGB(0, 0, 0));

	// 枠の表示
	DestX = 300;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 80, 213, 31);

	// 空欄の表示
	for (int i = 0; i < 10; i++) {
		m_pForegrd->Draw(pImageFore, DestX + 19 + 48 * i, DestY + 16, 0, 336, 48, 48);
	}

	// アイテムの表示
	for (cntArray = 0; cntArray < 50; cntArray++) {

		switch (m_pGMain->m_pItemManagementProc->m_pItemProc->GetItemObjPtr()->GetItemFlag()) {
			case 0:		// 羽の場合 ジャンプ回数が個数ごとに1上がる
				ShowAndSetItem(0, checkArray, 0, checkArrayCnt);
				break;
			case 1:		// 剣の場合 通常攻撃(Z)の攻撃力が100ずつ上がる
				ShowAndSetItem(1, checkArray, 200, checkArrayCnt);
				break;
			case 2:		// 鍵の場合 豪華な宝箱が開けられるようになる
				ShowAndSetItem(2, checkArray, 1, checkArrayCnt);
				canOpen = 1;
				break;
			case 3:		// 肉の場合 最大体力が100ずつ上がる
				ShowAndSetItem(3, checkArray, 100, checkArrayCnt);
				break;
			case 4:		// 盾の場合 被ダメージを50ずつ軽減する
				ShowAndSetItem(4, checkArray, 50, checkArrayCnt);
				break;
			case 5:		// 薬の場合 攻撃が派手に、かつ攻撃力アップ
				ShowAndSetItem(5, checkArray, 1, checkArrayCnt);
				break;
			case 6:		// キノコの場合 敵の攻撃力を下げる
				ShowAndSetItem(6, checkArray, 50, checkArrayCnt);
				break;
			case 7:		// 爆弾の場合 攻撃が爆発する
				ShowAndSetItem(7, checkArray, 1, checkArrayCnt);
				break;
			case 8:		// ブローチの場合 残機数を増加させる
				ShowAndSetItem(8, checkArray, 1, checkArrayCnt);
				break;
			case 9:		// 兜の場合 低確率でダメージを回避
				ShowAndSetItem(9, checkArray, 0, checkArrayCnt);
				break;
		}
	}

	// アイテム欄に描画する処理
	for (int j = 0; j < 10; j++) {
		switch (checkArray[j]) {
			case 1:
				m_pForegrd->Draw(pImageFore, DestX + 19 + 48 * j, DestY + 16, 48 * j, 288, 48, 48);
				_stprintf(str, _T("%d"), checkArrayCnt[j]);
				m_pGMain->m_pFont->Draw(DestX + 59 + 48 * j, DestY + 49, str, 16, RGB(255, 255, 255));
				break;
			default:
				break;
		}
	}
}
