//------------------------------------------------------------------------
// プレイヤーの処理 Playchar.cpp
//------------------------------------------------------------------------

#include "Playchar.h"
#include "Weapon.h"
#include "Effect.h"
#include "ItemManager.h"
#include "Map.h"
#include "BackFore.h"

int m_nPcNum = 2;

// PCプロシージャのコンストラクタ
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pObjArray.push_back(new CPcObj(m_pGMain)); // m_pObjArrayにオブジェクトを登録する
}

// PCオブジェクトのコンストラクタ
CPcObj::CPcObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 672, 0, 48, 48); // スプライトの読み込み
	m_vPos = VECTOR2(48, 721 );
	ResetStatus();
	ResetAnim();
	m_bActive = TRUE;
	m_nDirIdx = DOWN;	// 初期状態（待機モーション）
	m_nHp = m_nMaxHp = 1000;
	m_nMaxMp = 1000;
	m_nMp = m_nMaxMp = 1000;	
	m_nMp  = m_nMaxMp;			
	m_nAtc = 0;			// 当たってもダメージを与えられない
	m_nAnimNum = 6;		// キャラのスプライト数
	m_nPcNum = 2;		// 残機数
	m_nFireballWait = 0;			// 火の玉のクールタイム
	m_nBombWait = 0;				// 爆発のクールタイム
}

// PCオブジェクトのデストラクタ
CPcObj::~CPcObj()
{
	SAFE_DELETE(m_pSprite);
}

// PCオブジェクトの更新
void CPcObj::Update()
{
	CDirectInput* pDI = m_pGMain->m_pDI;
	CMapLine*  pHitmapline = NULL;
	float fSpeed     = 5;
	float fJumpPlus  = 10;
	int nBlockDamage = 0;

	// 乱数追加
	srand((unsigned int)time(NULL));
	int randomNum = 0;
	randomNum = rand() % 10;

	m_nPcNum = (2 + m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(8, 1)) - checkDeath;	// 残機数

	m_nMaxHp = 1000 + (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(3, 1));		// HP増加

	if (!m_bActive) return;

	m_vPosUp = VECTOR2(0, 0);

	switch (m_dwStatus)
	{
		case  FLASH:
				m_nCnt1--;
				if (m_nCnt1 <= 0) {
					ResetStatus();
					ResetAnim();
				}
				else {
					FlashCountup();
				}
				// breakをせずそのまま処理を続行する
		case  NORMAL:
				m_nMp += 1; // Mp自然増加
				if (m_nMp >= m_nMaxMp)m_nMp = m_nMaxMp;
				break;
		case DAMAGE:
				if (m_pOtherObj->GetAtc() > 0)
				{
					// 一定確率で攻撃を回避
					if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(9, 1) < randomNum)
					{
						nBlockDamage = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(4, 1);
						// 攻撃を受けたダメージ 盾のスタックに応じて50ずつ軽減
						if (m_pOtherObj->GetAtc() >= nBlockDamage)
						{
							// ダメージがブロックを上回るときのみHpを減算
							m_nHp -= m_pOtherObj->GetAtc() - nBlockDamage;
						}
					}
					if (m_nHp <= 0)
					{
						m_nHp = 0;
						m_dwStatus = DEAD;		// HPが0なので死亡へ
						m_nCnt1 = 180;			// 死亡フラッシュ中の時間設定
					}
					else 
					{
						m_dwStatus = FLASH;
						m_nCnt1 = 60;
					}
				}
				else {
					m_dwStatus = NORMAL;  // 攻撃力なしのときはすぐにNORMALに戻る
				}
				break;
		case DEAD:
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_nPcNum -= 1;
					checkDeath++;
					if (m_nPcNum <= 0)	// PCがなくなったのでゲームオーバー
					{
						m_pGMain->m_dwGameStatus = GAMEOVER;
					}
					else {
						m_nHp = m_nMaxHp;
						ResetStatus();
						ResetAnim();
					}
				}
				else {
					FlashCountup();
				}
				break;
	}

	switch (m_dwStatusSub)
	{
		// 歩行中の処理
		case WALK:		
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))	// →キー
			{
				m_vPosUp.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))		// ←キー
			{
				m_vPosUp.x = -fSpeed;
				m_nDirIdx = LEFT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_UP) || pDI->CheckJoy(KD_DAT, DIJ_UP))			// ↑キー
			{	
				// ジャンプ開始
				m_dwStatusSub  = JUMP;
				m_vJumpSpeed.x = m_vPosUp.x;
				m_vJumpSpeed.y = -(fSpeed*2.5 + fJumpPlus);
				m_fJumpTime    = 0;
				m_vPosUp.y     = m_vJumpSpeed.y;
				m_nCnt2 = 0;	// 二段ジャンプ用変数
			}
			else 
			{
				// 自然落下
				if (pDI->CheckKey(KD_DAT, DIK_DOWN) || pDI->CheckJoy(KD_DAT, DIJ_DOWN))	// ↓キー
				{
					m_vPosUp.y = fSpeed;
					m_nDirIdx = DOWN;
				}
				else {
					m_dwStatusSub = JUMP;
					m_vJumpSpeed.x = m_vPosUp.x;
					m_vJumpSpeed.y = fSpeed / 2;
					m_fJumpTime = 0;
					m_vPosUp.y = m_vJumpSpeed.y;
				}
			}

			// 待機モーション処理
			if (!pDI->CheckKey(KD_DAT, DIK_RIGHT) && !pDI->CheckKey(KD_DAT, DIK_LEFT)) {
				if (m_nDirIdx == LEFT) {
					m_nDirIdx = LEFT + 1;
				}
				else if (m_nDirIdx == RIGHT) {
					m_nDirIdx = DOWN;
				}
			}
			break;

			// ジャンプ中の処理
		case JUMP:		
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))	// →キー
			{
				m_vJumpSpeed.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))		// ←キー
			{
				m_vJumpSpeed.x = -fSpeed;
				m_nDirIdx = LEFT;
			}

			// 多段ジャンプ処理
			if ((pDI->CheckKey(KD_TRG, DIK_UP) || pDI->CheckJoy(KD_TRG, DIJ_UP)) &&
				m_nCnt2 <= m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(0, 1)) // ↑キー
			{
				m_nCnt2++;
				m_vJumpSpeed.y = -(fSpeed*2.5 + fJumpPlus);
				m_vPosUp.y = m_vJumpSpeed.y;
				m_fJumpTime = 0;
				m_dwStatusSub = JUMP;
			}
			m_fJumpTime++;
			m_vPosUp.x = m_vJumpSpeed.x;
			m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY*m_fJumpTime);
			break;
	}

	// 火の玉の処理
	if (pDI->CheckKey(KD_DAT, DIK_Z) || pDI->CheckJoy(KD_DAT, DIJ_A))
	{
		if (m_nFireballWait <= 0 && m_nMp >= 50)	// waitがなく、Mpが100以上のときのみ発射 
		{
			// ショットの発射
			m_pGMain->m_pWeaponProc->m_pWeaponShotProc->Start(m_vPos+VECTOR2(16,4), this, PC);
			m_nFireballWait = 10;			// 武器発射のウェイトを掛ける
			m_nMp -= 50;				
			if (m_nMp < 0)m_nMp = 0;	
		}
	}
	if (m_nFireballWait > 0) m_nFireballWait--;    // 武器発射のウェイトをカウントダウン

	// ボムの発射処理
	if (pDI->CheckKey(KD_DAT, DIK_X) || pDI->CheckJoy(KD_DAT, DIJ_X))
	{
		if (m_nBombWait <= 0 && m_nMp >= 200)
		{
			// ショットの発射
			m_pGMain->m_pWeaponProc->m_pWeaponGrenadeProc->Start(m_vPos + VECTOR2(16, 16), this, PC);
			m_nBombWait = 50;    // 武器発射のウェイトを掛ける
			m_nMp -= 200;
			if (m_nMp < 0)m_nMp = 0;
		}
	}
	if (m_nBombWait > 0) m_nBombWait--;    // 武器発射のウェイトをカウントダウン

	// マップ線との接触判定と適切な位置への移動
	if (m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline))
	{
		if (pHitmapline && pHitmapline->m_vNormal.y < 0)	// 接地していないとジャンプできない
		{
			m_dwStatusSub = WALK;	// マップ線に接触しているときは、ジャンプを歩行に戻す
			m_fJumpTime = 0;
			m_vJumpSpeed.x = m_vJumpSpeed.y = 0;
		}
	}

	// 増分計算
	m_vPos += m_vPosUp;

	AnimCountup();
	Draw();
}