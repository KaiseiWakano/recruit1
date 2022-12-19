//------------------------------------------------------------------------
// 敵の処理 Enemy.cpp
//------------------------------------------------------------------------

#include "Enemy.h"
#include "Weapon.h"
#include "Effect.h"
#include "ItemManager.h"
#include "Playchar.h"
#include "Map.h"

bool flagBossDeath = false;
bool flagBio = false;

//------------------------------------------------------------------------
// 敵メインプロシージャのコンストラクタ
//------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pEnmWaterProc = new CEnmWaterProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmWaterProc);

	m_pEnmTordProc = new CEnmTordProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmTordProc);

	m_pEnmWolfProc = new CEnmWolfProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmWolfProc);

	m_pEnmFoxProc = new CEnmFoxProc(m_pGMain);												
	m_pProcArray.push_back(m_pEnmFoxProc);    

	m_pEnmBossProc = new CEnmBossProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmBossProc);

	m_pEnmLastBossProc = new CEnmLastBossProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmLastBossProc);
}

//============================================================================
// 敵プロシージャとオブジェクトを探索し全てのオブジェクトをノンアクティブにする
// 敵プロシージャの開始フラグをリセットする
//============================================================================
void  CEnmProc::SetNonActive()
{
	// 敵のオブジェクトポインタ配列の探索
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}

	// 下位のプロシージャポインタ配列の探索
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag( 0 );				// 開始フラグのリセット
		((CEnmProc*)m_pProcArray[i])->SetNonActive();   // 下位のプロシージャの処理（再帰処理）
	}
}

// =====================================================================================================
// 
// 各敵プロシージャ毎の処理
// 
// =====================================================================================================

//------------------------------------------------------------------------
// 泉の水の敵プロシージャのコンストラクタ
//------------------------------------------------------------------------
CEnmWaterProc::CEnmWaterProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_WATER_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmWaterObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 泉の水の敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmWaterProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップ　 ( EvtID:3　敵の出現位置,     　EvtNo:0x01	泉の水の敵  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x01, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}

		m_nStartFlag = 1;  // 一回発させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
//	泉の水の敵オブジェクトのコンストラクタ
//  引数 なし
//------------------------------------------------------------------------
CEnmWaterObj::CEnmWaterObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 384, 192, 48, 48);
	m_fAlpha = 0.8f;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 10000;
	m_nAtc = 200;
	m_nAnimNum = 1;
}
// ---------------------------------------------------------------------------
// 泉の水の敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmWaterObj::~CEnmWaterObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// 泉の水の敵オブジェクトの開始
// 引数 なし
//-----------------------------------------------------------------------------
BOOL CEnmWaterObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = RIGHT;
	m_vPos = vPos;	// 発生位置
	m_vPosUp = VECTOR2(0, 0);
	m_nHp = m_nMaxHp;

	return TRUE;
}
//-----------------------------------------------------------------------------
// 泉の水の敵オブジェクトの更新
// 引数 なし
//-----------------------------------------------------------------------------
void CEnmWaterObj::Update()
{
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case NORMAL:
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			// 当たったらゲームオーバー
			m_pGMain->m_dwGameStatus = GAMEOVER;
			m_dwStatus = NORMAL;
			break;

		case DEAD:
			m_dwStatus = NORMAL;
			break;
		}
		AnimCountup();
		Draw();
	}
}

//------------------------------------------------------------------------
//	がまの敵プロシージャのコンストラクタ
//------------------------------------------------------------------------
CEnmTordProc::CEnmTordProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_TORD_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmTordObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// がまの敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmTordProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while ( nNext != -1 )
		{
			// イベントマップ (EvtID:3 敵の出現位置, EvtNo:0x02 がまの敵)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x02, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // 一回発させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
//	がまの敵オブジェクトのコンストラクタ
//  引数 なし
//------------------------------------------------------------------------
CEnmTordObj::CEnmTordObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 480+Random(0,1)*96, 192, 48, 48);
	m_vPos = VECTOR2(0,0);
	m_nHp = m_nMaxHp = 400;
	m_nAtc = 300;
	m_nAnimNum = 2;
}
// ---------------------------------------------------------------------------
// がまの敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmTordObj::~CEnmTordObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// がまの敵オブジェクトの開始
// 引数 なし
//-----------------------------------------------------------------------------
BOOL CEnmTordObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// 発生位置

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 300;

	return TRUE;
}
//-----------------------------------------------------------------------------
// がまの敵オブジェクトの更新
// 引数 なし
//-----------------------------------------------------------------------------
void CEnmTordObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// 毒による攻撃力低減
	if (m_nAtc >= 0)
	{
		m_nAtc = 300 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0,0);

		switch (m_dwStatus)
		{
			case  FLASH:
					m_nCnt1--;
					if (m_nCnt1 <= 0) 
					{
						ResetStatus();
						ResetAnim();
					}
					else 
					{
						FlashCountup();
					}
			case  NORMAL:
					switch (m_dwStatusSub)
					{
					case  WALK:
						// 自由移動の処理
						if (m_nDirIdx == RIGHT)
						{
							m_vPosUp.x = fSpeed;
						}
						else {
							m_vPosUp.x = -fSpeed;
						}
						
						// マップ線との接触判定と適切な位置への移動
						bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
						if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
						{
							// マップ線の端に来たので左右反転
							if (m_nDirIdx == RIGHT)
							{
								m_nDirIdx = LEFT;
								m_vPosUp.x = -fSpeed;
								m_vPosUp.y = 0;
							}
							else {
								m_nDirIdx = RIGHT;
								m_vPosUp.x = fSpeed;
								m_vPosUp.y = 0;
							}
						}
						break;
					}
					// 増分
					m_vPos += m_vPosUp;
					// ＰＣとのあたり判定
					m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
					break;

			case  DAMAGE:
					m_nHp -= m_pOtherObj->GetAtc();
					if (m_nHp <= 0) {
						m_dwStatus = DEAD;
						m_nCnt1 = 180;
					}
					else {
						m_dwStatus = FLASH;
						m_nCnt1 = 60;
					}
					// 爆発エフェクト表示
					if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0) {
						m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
					}
					m_pGMain->m_pSeHit->Play();
					break;

			case  DEAD:
					m_nCnt1--;
					if (m_nCnt1 <= 0)
					{
						m_pGMain->m_nScore += 10;
						m_bActive = FALSE;
					}
					else {
						FlashCountup();
					}
					break;
		}

		AnimCountup();
		Draw();

	}
}

// ===================================================================================================
//------------------------------------------------------------------------
//	狐の敵プロシージャのコンストラクタ
//------------------------------------------------------------------------
CEnmFoxProc::CEnmFoxProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_FOX_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmFoxObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
//  狐の敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmFoxProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索 ( EvtID:3 敵の出現位置, EvtNo:0x08 狐の敵)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x08, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // 一回発させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
//	狐の敵オブジェクトのコンストラクタ
//  引数 なし
//------------------------------------------------------------------------
CEnmFoxObj::CEnmFoxObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 192, 192, 48, 48);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 450;
	m_nAtc = 200;
	m_nAnimNum = 2;
}
// ---------------------------------------------------------------------------
//  狐の敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmFoxObj::~CEnmFoxObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// 狐の敵オブジェクトの開始
// 引数 なし
//-----------------------------------------------------------------------------
BOOL CEnmFoxObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = RIGHT;
	m_vPos = vPos;	// 発生位置

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 200;

	return TRUE;
}
//-----------------------------------------------------------------------------
// 狐の敵オブジェクトの更新
// 引数 なし
//-----------------------------------------------------------------------------
void CEnmFoxObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	if (m_nAtc >= 0)
	{
		m_nAtc = 200 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}
		case NORMAL:
			switch (m_dwStatusSub)
			{
			case  WALK:
				// 自由移動の処理
				if (m_nDirIdx == RIGHT)
				{
					m_vPosUp.x = fSpeed;
				}
				else 
				{
					m_vPosUp.x = -fSpeed;
				}

				// マップ線との接触判定と適切な位置への移動
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// マップ線の端に来たので左右反転
					if (m_nDirIdx == RIGHT)
					{
						m_nDirIdx = LEFT;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = RIGHT;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// 増分
			m_vPos += m_vPosUp;
			// ＰＣとのあたり判定
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;			
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;			
			}
			// 爆発エフェクト表示
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0)
			{
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				m_pGMain->m_nScore += 20;
				m_bActive = FALSE;
			}
			else {
				FlashCountup();
			}
			break;
		}

		if (m_nFoxShotWait <= 0)	// waitがない時のみ発射
		{
			// ショットの発射
			m_pGMain->m_pWeaponProc->m_pWeaponShotProc->Start(m_vPos + VECTOR2(16, 16), this, ENM);
			m_nFoxShotWait = 200;    // 武器発射のウェイトを掛ける
		}
		if (m_nFoxShotWait > 0) m_nFoxShotWait--;    // 武器発射のウェイトをカウントダウン

		AnimCountup();
		Draw();
	}
}

// ===================================================================================================
//------------------------------------------------------------------------
//	オオカミの敵プロシージャのコンストラクタ
//------------------------------------------------------------------------
CEnmWolfProc::CEnmWolfProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_TORD_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmWolfObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// オオカミの敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmWolfProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 (EvtID:3 敵の出現位置, EvtNo:0x04 オオカミの敵)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x04, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // 一回発生させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
//	オオカミの敵オブジェクトのコンストラクタ	
//  引数 なし
//------------------------------------------------------------------------
CEnmWolfObj::CEnmWolfObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 96, 192, 48, 48);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 650;
	m_nAtc = 400;
	m_nAnimNum = 2;
}
// ---------------------------------------------------------------------------
// オオカミの敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmWolfObj::~CEnmWolfObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// オオカミの敵オブジェクトの開始
// 引数 なし
//-----------------------------------------------------------------------------
BOOL CEnmWolfObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// 発生位置

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 400;

	return TRUE;
}
//-----------------------------------------------------------------------------
// オオカミの敵オブジェクトの更新
// 引数 なし
//-----------------------------------------------------------------------------
void CEnmWolfObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// 毒による攻撃力低減
	if (m_nAtc >= 0)
	{
		m_nAtc = 400 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}		
		case NORMAL:
			switch (m_dwStatusSub)
			{
			case WALK:
				// 自由移動の処理
				if (m_nDirIdx == RIGHT)
				{
					m_vPosUp.x = fSpeed;
				}
				else {
					m_vPosUp.x = -fSpeed;
				}

				// マップ線との接触判定と適切な位置への移動
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// マップ線の端に来たので左右反転
					if (m_nDirIdx == RIGHT)
					{
						m_nDirIdx = LEFT;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = RIGHT;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// 増分
			m_vPos += m_vPosUp;
			// ＰＣとのあたり判定
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;			
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;		
			}
			// 爆発エフェクト表示
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0) {
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				m_pGMain->m_nScore += 20;
				m_bActive = FALSE;
			}
			else {
				FlashCountup();
			}
			break;
		}
		AnimCountup();
		Draw();
	}
}

// ===================================================================================================
//------------------------------------------------------------------------
//	ボスの敵プロシージャのコンストラクタ
//------------------------------------------------------------------------
CEnmBossProc::CEnmBossProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_BOSS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmBossObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// ボスの敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmBossProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:3　敵の出現位置, EvtNo:0x200 ボスの敵 )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x200, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // 一回発生させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
//	ボスの敵オブジェクトのコンストラクタ	
//  引数 なし
//------------------------------------------------------------------------
CEnmBossObj::CEnmBossObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 1152, 0, 144, 96);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 3000;
	m_nAtc = 500;
	m_nAnimNum = 4;
}
// ---------------------------------------------------------------------------
// ボスの敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmBossObj::~CEnmBossObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ボスの敵オブジェクトの開始
// 引数 なし
//-----------------------------------------------------------------------------
BOOL CEnmBossObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = RIGHT;
	m_vPos = vPos;	// 発生位置

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 500;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ボスの敵オブジェクトの更新
// 引数 なし
//-----------------------------------------------------------------------------
void CEnmBossObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// 毒による攻撃力低減
	if (m_nAtc >= 0)
	{
		m_nAtc = 500 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}		
		case NORMAL:
			switch (m_dwStatusSub)
			{
			case WALK:
				// 自由移動の処理
				if (m_nDirIdx == RIGHT)
				{
					m_vPosUp.x = fSpeed;
				}
				else {
					m_vPosUp.x = -fSpeed;
				}

				// マップ線との接触判定と適切な位置への移動
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// マップ線の端に来たので左右反転
					if (m_nDirIdx == RIGHT)
					{
						m_nDirIdx = LEFT;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = RIGHT;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// 増分
			m_vPos += m_vPosUp;
			// PCとのあたり判定
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;
			}
			// 爆発エフェクト表示
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0)
			{
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				flagBossDeath = true;	// 死亡判定
				m_pGMain->m_nScore += 100;
				m_bActive = FALSE;
			}
			else 
			{
				FlashCountup();
			}
			break;
		}

		if (m_nBossShotWait <= 0)	// waitがないのみ発射
		{
			// ショットの発射
			m_pGMain->m_pWeaponProc->m_pWeaponBossShotProc->Start(m_vPos + VECTOR2(16, 16), this, ENM);
			m_nBossShotWait = 30;    // 武器発射のウェイトを掛ける
		}
		if (m_nBossShotWait > 0) m_nBossShotWait--;    // 武器発射のウェイトをカウントダウン

		AnimCountup();
		Draw();
	}
}

// ===================================================================================================
//------------------------------------------------------------------------
// ラスボスの敵プロシージャのコンストラクタ
//------------------------------------------------------------------------
CEnmLastBossProc::CEnmLastBossProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_LASTBOSS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmLastBossObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// ラスボスの敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmLastBossProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 (EvtID:3 敵の出現位置, EvtNo:0x400 ラスボスの敵)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x400, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // 一回発させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
//	ラスボスの敵オブジェクトのコンストラクタ
//  引数 なし
//------------------------------------------------------------------------
CEnmLastBossObj::CEnmLastBossObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 384, 480, 240, 192);
	m_vPos = VECTOR2(0, 0);
	// ラスボスのHP
	m_nHp = m_nMaxHp = 6000;
	m_nAtc = 500;
	m_nAnimNum = 6;
}
// ---------------------------------------------------------------------------
// ラスボスの敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmLastBossObj::~CEnmLastBossObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ラスボスの敵オブジェクトの開始
// 引数 なし
//-----------------------------------------------------------------------------
BOOL CEnmLastBossObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = DOWN;
	m_vPos = vPos;	// 発生位置
	flagBio = true;
	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 500;

	return TRUE;
}
//-----------------------------------------------------------------------------
// ラスボスの敵オブジェクトの更新
// 引数 なし
//-----------------------------------------------------------------------------
void CEnmLastBossObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// 毒による攻撃力低減
	if (m_nAtc >= 0)
	{
		m_nAtc = 500 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case  FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}
		case  NORMAL:
			switch (m_dwStatusSub)
			{
			case  WALK:
				// 自由移動の処理...移動しない K.Wakano
				/*
				if (m_nDirIdx == UP)
				{
					//m_vPosUp.x = fSpeed;
				}
				else {
					//m_vPosUp.x = -fSpeed;
				}
				*/

				// マップ線との接触判定と適切な位置への移動
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// マップ線の端に来たので左右反転
					if (m_nDirIdx == UP)
					{
						m_nDirIdx = DOWN;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = UP;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// 増分
			m_vPos += m_vPosUp;
			// ＰＣとのあたり判定
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case  DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;
			}
			// 爆発エフェクト表示
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0) {
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case  DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				flagBossDeath = true;	// 死亡判定
				m_pGMain->m_nScore += 200;
				m_bActive = FALSE;
			}
			else {
				FlashCountup();
			}
			break;
		}

		// 攻撃処理 K.Wakano
		if (m_nLastBossShotWait <= 0)
		{
			m_nAnimNum = 4;
			if (m_nDirIdx == UP)
			{
				m_nDirIdx = RIGHT;
			}
			else 
			{
				m_nDirIdx = LEFT;
			}
			// 弾の発射
			for (int i = 0; i < 10 - Random(0, 3); i++)			// 弾の列が確率で欠けるように
			{
				m_pGMain->m_pWeaponProc->m_pWeaponBossShotProc->Start(m_vPos + VECTOR2(16, 0 + i * 15), this, ENM);
			}
			m_nLastBossShotWait = 50 * Random(1,2);				// ボスの攻撃感覚をランダムに
		}
		if (m_nLastBossShotWait > 0) m_nLastBossShotWait--;		// 武器発射のウェイトをカウントダウン

		AnimCountup();
		Draw();

	}
}
