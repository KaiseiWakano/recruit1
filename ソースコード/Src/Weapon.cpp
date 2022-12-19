//------------------------------------------------------------------------
// 武器の処理 Weapon.cpp
//------------------------------------------------------------------------

#include "Weapon.h"
#include "Playchar.h"
#include "Enemy.h"
#include "BackFore.h"
#include "ItemManager.h"

// 武器メインプロシージャのコンストラクタ
CWeaponProc::CWeaponProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pWeaponShotProc = new CWeaponShotProc(m_pGMain);			// 武器・ショットプロシージャ
	m_pProcArray.push_back(m_pWeaponShotProc);					// プロシージャをプロシージャ配列に登録する

	m_pWeaponGrenadeProc = new CWeaponGrenadeProc(m_pGMain);	// 武器・小型爆弾プロシージャ
	m_pProcArray.push_back(m_pWeaponGrenadeProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponBossShotProc = new CWeaponBossShotProc(m_pGMain);	// ボス武器・ショットプロシージャ
	m_pProcArray.push_back(m_pWeaponBossShotProc);				// プロシージャをプロシージャ配列に登録する
} 

// 武器・ショットプロシージャのコンストラクタ
CWeaponShotProc::CWeaponShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 武器・ショット
	for (DWORD i = 0; i < WEAPON_SHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponShotObj(m_pGMain));	// m_pObjArrayに武器・ショットオブジェクトを生成する
	}

	m_nWaitTime = m_nMaxwaitTime = 8;
}

// 武器・ショットプロシージャの開始
// VECTOR2 vPos			発生位置
// CBaseObj* pObj		発射元のオブジェクト
// DWORD dwOwner		発射元のオブジェクト区分
// 戻り値 正常に発生:TRUE 発生しなかった:FALSE
BOOL CWeaponShotProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, pObj, dwOwner);	// 武器の発生
			m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}
	if (bRet) m_pGMain->m_pSeShot->Play();

	return bRet;
}

//------------------------------------------------------------------------
// 武器・ショットオブジェクトのコンストラクタ	
//
// 引数 なし
//------------------------------------------------------------------------
CWeaponShotObj::CWeaponShotObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 336, 96, 48, 48);
	m_nAnimNum = 6;

	m_nAtc = 100;	// 武器攻撃力
}
// ---------------------------------------------------------------------------
//
// 武器・ショットオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponShotObj::~CWeaponShotObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// 武器・ショットオブジェクトの開始
//
//   VECTOR2 vPos    発生位置
//   CBaseObj*   pObj    発射元のオブジェクト
//   DWORD       dwOwner 発射元のオブジェクト区分
//
//   戻り値　　TRUE
//-----------------------------------------------------------------------------
BOOL CWeaponShotObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	float fSpeed = 8;	// 弾の速さ
	// 爆弾アイテムの所持判定
	if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0)
	{
		// 剣による攻撃力増加 + 爆弾によるボーナス
		m_nAtc = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(1, 1) + 200;
	}
	else 
	{
		// 剣による攻撃力増加
		m_nAtc = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(1, 1);
	}

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_dwOwner = dwOwner;

	m_vPos = vPos;	// 発生位置
	m_vPosUp = VECTOR2(0, 0);

	// プレイヤーの向きに応じて攻撃方向を変える
	switch(pObj->GetDirIdx() )
	{
	case DOWN:
	case RIGHT:
		if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(5, 1) > 0) {
			m_pSprite->SetSrc(672, 96, 48, 48);
		}
		else {
			m_pSprite->SetSrc(336, 96, 48, 48);
		}
		m_vPosUp = VECTOR2(fSpeed,0);
		break;
	case LEFT:
	case LEFT + 1:
		if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(5, 1) > 0) {
			m_pSprite->SetSrc(672, 144, 48, 48);
		}
		else {
			m_pSprite->SetSrc(336, 144, 48, 48);
		}
		m_vPosUp = VECTOR2(-fSpeed, 0);
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 武器・ショットオブジェクトの更新
//
// 引数 なし
//-----------------------------------------------------------------------------
void CWeaponShotObj::Update()
{
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
			case  NORMAL:
				if (m_dwOwner == PC)	// PCが発射した弾
				{
					m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
				}
				else					// 敵が発射した弾
				{ 
					m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
				}

				// 画面外へ出たら消去
				if (m_vPos.x-m_pGMain->m_vScroll.x < 0 || m_vPos.x-m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
					m_vPos.y-m_pGMain->m_vScroll.y < 0 || m_vPos.y-m_pGMain->m_vScroll.y > WINDOW_HEIGHT) 
				{	
					m_bActive = FALSE;
				}
				break;

			case  DAMAGE:
				m_bActive = FALSE;
				break;
		}
		
		m_vPos += m_vPosUp;

		AnimCountup();
		Draw();
	}
}

//------------------------------------------------------------------------
// ボス武器・ショットプロシージャのコンストラクタ	
//------------------------------------------------------------------------
CWeaponBossShotProc::CWeaponBossShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 武器・ショット
	for (DWORD i = 0; i < WEAPON_BOSSATTACK_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponBossShotObj(m_pGMain));	// m_pObjArrayに武器・ショットオブジェクトを生成する
	}

	m_nWaitTime = m_nMaxwaitTime = 8;
}

//-----------------------------------------------------------------------------
// ボス武器・ショットプロシージャの開始
//
//   VECTOR2 vPos    発生位置
//   CBaseObj*   pObj    発射元のオブジェクト
//   DWORD       dwOwner 発射元のオブジェクト区分
//
//   戻り値　　正常に発生 : TRUE    発生しなかった : FALSE
//-----------------------------------------------------------------------------
BOOL CWeaponBossShotProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, pObj, dwOwner);	// 武器の発生
			//m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}
	//if (bRet) m_pGMain->m_pSeShot->Play();

	return bRet;
}

//------------------------------------------------------------------------
// ボス武器・ショットオブジェクトのコンストラクタ	
//
// 引数 なし
//------------------------------------------------------------------------
CWeaponBossShotObj::CWeaponBossShotObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 672, 288, 48, 48);
	m_nAnimNum = 1;

	m_nAtc = 800;		// 攻撃力
}
// ---------------------------------------------------------------------------
// ボス武器・ショットオブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponBossShotObj::~CWeaponBossShotObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// ボス武器・ショットオブジェクトの開始
//
//   VECTOR2 vPos    発生位置
//   CBaseObj*   pObj    発射元のオブジェクト
//   DWORD       dwOwner 発射元のオブジェクト区分
//
//   戻り値　　TRUE
//-----------------------------------------------------------------------------
BOOL CWeaponBossShotObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	float fSpeed = 8;	// 弾の速さ

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_dwOwner = dwOwner;

	// 毒による攻撃力低減
	if (m_nAtc >= 0) 
	{
		m_nAtc = 800 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	m_vPos = vPos;	// 発生位置
	m_vPosUp = VECTOR2(0, 0);

	switch (pObj->GetDirIdx())
	{
	case DOWN:
	case RIGHT:
		if (flagBio == false) {
			m_pSprite->SetSrc(672, 336, 48, 48);
		}
		else {
			m_pSprite->SetSrc(672, 384, 48, 48);
		}
		m_vPosUp = VECTOR2(fSpeed, 0);
		break;
	case LEFT:
	case LEFT + 1:
		if (flagBio == false) {
			m_pSprite->SetSrc(672, 288, 48, 48);
		}
		else {
			m_pSprite->SetSrc(672, 384, 48, 48);
		}
		m_vPosUp = VECTOR2(-fSpeed, 0);
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ボス武器・ショットオブジェクトの更新
//
// 引数 なし
//-----------------------------------------------------------------------------
void CWeaponBossShotObj::Update()
{
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case  NORMAL:
			if (m_dwOwner == PC) // PCが発射した弾
			{
				m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
			}
			else				 //  敵が発射した弾
			{ 
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			}

			if (m_vPos.x - m_pGMain->m_vScroll.x < 0 || m_vPos.x - m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
				m_vPos.y - m_pGMain->m_vScroll.y < 0 || m_vPos.y - m_pGMain->m_vScroll.y > WINDOW_HEIGHT) {	// 画面外へ出たので消去
				m_bActive = FALSE;
			}
			break;

		case  DAMAGE:
			m_bActive = FALSE;
			break;
		}

		m_vPos += m_vPosUp;

		AnimCountup();
		Draw();

	}
}

// =================================================================================

//------------------------------------------------------------------------
// 武器・小型爆弾プロシージャのコンストラクタ	
//
// なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//------------------------------------------------------------------------
CWeaponGrenadeProc::CWeaponGrenadeProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 武器・小型爆弾
	for (DWORD i = 0; i < WEAPON_GRENADE_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponGrenadeObj(m_pGMain));	// m_pObjArrayに武器・小型爆弾オブジェクトを生成する
	}

	m_nWaitTime = m_nMaxwaitTime = 0;
}

//-----------------------------------------------------------------------------
// 武器・小型爆弾プロシージャの開始
//
//   VECTOR2 vPos      発生位置
//   DWORD       dwOwner   発射元のオブジェクト区分
//
//   戻り値　　正常に発生 : TRUE    発生しなかった : FALSE
//-----------------------------------------------------------------------------
BOOL CWeaponGrenadeProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			((CWeaponGrenadeObj*)(m_pObjArray[i]))->Start(vPos, pObj, dwOwner);	// 武器の発生
			bRet = TRUE;
			break;
		}
	}

	if (bRet) m_pGMain->m_pSeShot->Play();

	return bRet;
}


//------------------------------------------------------------------------
// 武器・小型爆弾オブジェクトのコンストラクタ	
//
// 引数 なし
//------------------------------------------------------------------------
CWeaponGrenadeObj::CWeaponGrenadeObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 320, 0, 64, 64);
	//m_nAnimNum = 2;

	m_nAtc = 200;
}
// ---------------------------------------------------------------------------
// 武器・小型爆弾オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponGrenadeObj::~CWeaponGrenadeObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// 武器・小型爆弾オブジェクトの開始
//
//   VECTOR2 vPos      発生位置
//   DWORD       dwOwner   発射元のオブジェクト区分
//
//   戻り値　　TRUE
//-----------------------------------------------------------------------------
BOOL CWeaponGrenadeObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	m_bActive = TRUE;
	//ResetStatus();
	//ResetAnim();
	//m_nAnimNum = 2;
	m_dwOwner = dwOwner;
	m_dwStatusPhase = 1;

	m_vPos = vPos;	// 発生位置
	m_vOf = VECTOR2(0, 0);
	m_vTarget = VECTOR2(0, 0);
	m_vPosUp = VECTOR2(0, 0);

	if (m_dwOwner == PC)
	{
		// PCが発射した弾
		m_vTarget.x = m_vPos.x;
		m_vTarget.y = m_vPos.y / 2;	// 画面内に収めている 撃った距離からマップの端÷２の距離で必ず爆発する
	}
	else {
		// 敵から発射した弾
		m_vTarget.x = m_vPos.x;
		//m_vTarget.y = m_vPos.y + (WINDOW_HEIGHT - m_vPos.y) / 2;
	}

	m_pSprite->SetSrc(320, 0, 64, 64);
	m_fRotate = 0.0f;
	m_fScale = 1.0f;
	m_fAlpha = 0.5f;

	m_nHp = m_nMaxHp;

	m_nCnt1 = 0;
	m_nCnt2 = 0;

	return TRUE;
}
//-----------------------------------------------------------------------------
// 武器・小型爆弾オブジェクトの更新
//
// 引数 なし
//-----------------------------------------------------------------------------
void CWeaponGrenadeObj::Update()
{
	float fSpeed = 10;	// 弾の速さ

	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case  NORMAL:
			switch (m_dwStatusPhase)
			{
			case 0:
				if (TargetMove(fSpeed))
				{   // 目的地（vTarget）への移動処理を行い、目的地に達したとき
					m_vPos.x -= ((64 - 32) / 2);   // 爆発スプライトに変更
					m_vPos.y -= ((64 - 14) / 2);   // 爆発スプライトに変更
					ResetAnim();
					m_pSprite->SetSrc(320, 0, 64, 64);
					m_fAlpha = 0.7f;
					m_fScale = 1.0f;
					//m_nAnimNum = 2;
					m_nHp = 0;
					m_pOtherObj = NULL;
					m_dwStatusPhase = 1;   // 爆発処理へ
					m_pGMain->m_pSeShot->Play();
				}
				break;

			case 1:
				m_fScale += 0.15f;     // 爆発をすこしづつ拡大していく
				if (m_fScale > 5) {
					m_bActive = false;
				}
				m_vOf.x = -(m_fScale * m_pSprite->GetSrcWidth() / 2);     // 拡大に合わせて表示位置をづらしていく
				m_vOf.y = -(m_fScale * m_pSprite->GetSrcHeight() / 2);
				break;
			}

			// 増分の計算
			m_vPos += m_vPosUp;

			if (m_dwOwner == PC) // PCが発射した弾
			{
				m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
			}
			else { //  敵が発射した弾
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			}

			// 画面外へ出たら消去
			if (m_vPos.x - m_pGMain->m_vScroll.x < 0 || m_vPos.x - m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
				m_vPos.y - m_pGMain->m_vScroll.y < 0 || m_vPos.y - m_pGMain->m_vScroll.y > WINDOW_HEIGHT) 
			{	
				m_bActive = FALSE;
			}
			break;

		case  DAMAGE:
			m_dwStatus = NORMAL;
			break;
		}
		AnimCountup();
		Draw();
	}
}
