//------------------------------------------------------------------------
// 基本プロシージャ・基本オブジェクトクラス処理 BaseProc.cpp
//------------------------------------------------------------------------

#include "BaseProc.h"

//------------------------------------------------------------------------
// 基本プロシージャクラスのデストラクタ	
//（全てのプロシージャやオブジェクトを読み下して削除する）
//
// 引数 なし
//------------------------------------------------------------------------
CBaseProc::~CBaseProc()
{
	// プロシージャポインタ配列の削除
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		SAFE_DELETE(m_pProcArray[i]);
	}
	// オブジェクトポインタ配列の削除
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		SAFE_DELETE(m_pObjArray[i]);
	}
}
//------------------------------------------------------------------------
// 基本プロシージャクラスの全体更新処理	
//（全てのプロシージャやオブジェクトを読み下して更新処理をする）
//
// 引数 なし
//------------------------------------------------------------------------
void CBaseProc::UpdateAll()
{
	Update();                              // プロシージャの更新処理
	if (m_nWaitTime > 0) m_nWaitTime--;    // ウェイトタイマーのカウントダウン

	// オブジェクトポインタ配列の更新
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->Update();          // オブジェクトの更新処理
	}

	// 下位のプロシージャポインタ配列の更新
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->UpdateAll();      // 下位のプロシージャの全体更新処理（再帰処理）
	}
}

//------------------------------------------------------------------------
// 基本オブジェクト
// コンストラクタ	
//
// 引数 CGameMain* pGMain  // ゲームメインクラス
//------------------------------------------------------------------------
CBaseObj::CBaseObj(CGameMain* pGMain)
{
	// 全体のゼロクリヤー
	// 注意点：クラス中にＳＴＬを宣言しているときは使用してはならない
	ZeroMemory(this, sizeof(CBaseObj));

	m_pGMain = pGMain;			// ゲームメインクラス
	m_dwStatus = NORMAL;
	m_dwStatusSub = NONE;
	m_vCenter.x = -1;			// 回転のX中心は、スプライト中心
	m_vCenter.y = -1;			// 回転のY中心は、スプライト中心
	m_fScale = 1.0f;			// 拡大縮小
	m_fAlpha = 1.0f;			// 透明度
	m_nAnimNum = 2;				// アニメーション要素数(初期値は２)
}
//------------------------------------------------------------------------
// 基本オブジェクト
// デストラクタ	
//------------------------------------------------------------------------
CBaseObj::~CBaseObj()
{
	SAFE_DELETE(m_pSprite);
}
//------------------------------------------------------------------------
// 基本オブジェクト
// ステータスのリセット処理	
//
// 引数 なし
// 戻り値 なし
//------------------------------------------------------------------------
void CBaseObj::ResetStatus()
{
	m_dwStatus = NORMAL;		// ステータス
	m_dwStatusSub = WALK;		// ステータスサブ
	m_dwStatusPhase = 0;		// ステータスフェーズ
}
//------------------------------------------------------------------------
// 基本オブジェクト
// アニメーションのリセット処理	
//
// 引数 なし
// 戻り値 なし
//------------------------------------------------------------------------
void CBaseObj::ResetAnim()
{
	m_nAnimIdx = 0;		// アニメーションインデックス数
	m_nAnimFrm = 0;		// アニメーションフレームカウント
	m_nFlashIdx = 0;		// フラッシュアニメーションインデックス数
	m_nFlashFrm = 0;		// フラッシュアニメーションフレームカウント
}
//------------------------------------------------------------------------
// 基本オブジェクト
// アニメーションのカウントアップ処理	
//
// 引数 なし
// 戻り値 int
// アニメーション継続中:0 アニメーション終了:1
//------------------------------------------------------------------------
int CBaseObj::AnimCountup()
{
	int ret = 0;
	int speed = 5;

	if (m_nAnimNum <= 1) return 1;

	m_nAnimFrm++;
	if (m_nAnimFrm >= speed) {
		m_nAnimFrm = 0;
		m_nAnimIdx++;
		if (m_nAnimIdx >= m_nAnimNum) {
			m_nAnimIdx = 0;
			ret = 1;
		}
	}
	return ret;
}
//------------------------------------------------------------------------
// 基本オブジェクト
// フラッシュアニメーションのカウントアップ処理	
//
// 引数 なし
// 戻り値 int
// アニメーション継続中:0 アニメーション終了:1
//------------------------------------------------------------------------
int CBaseObj::FlashCountup()
{
	int ret = 0;
	int speed = 2;
	int num = 2;

	m_nFlashFrm++;
	if (m_nFlashFrm >= speed) {
		m_nFlashFrm = 0;
		m_nFlashIdx++;
		if (m_nFlashIdx >= num) {
			m_nFlashIdx = 0;
			ret = 1;
		}
	}
	return ret;
}

//------------------------------------------------------------------------
// 基本オブジェクト
// 描画メソッド	
//（ スケールと回転を考慮して描画　）
//（ スクロールを考慮して描画　）
//
// 引数 なし
// 戻り値 なし
//------------------------------------------------------------------------
void CBaseObj::Draw()
{
	VECTOR2 vScr = m_pGMain->m_vScroll;  // スクロール
	int cx, cy;
	if (m_bActive) {

		// 透明度
		if (m_nFlashIdx == 0)
		{
			m_pSprite->m_vDiffuse.w = m_fAlpha;
		}
		else {
			m_pSprite->m_vDiffuse.w = m_fAlpha * 0.2f; // 透明に近くする
		}

		// アニメーション
		m_pSprite->m_ofX = m_pSprite->GetDestWidth() * m_nAnimIdx;
		m_pSprite->m_ofY = m_pSprite->GetDestHeight() * m_nDirIdx;

		if (m_fRotate == 0) {	// 回転しないとき
			MATRIX4X4 mWorld, mScale;
			mWorld = XMMatrixTranslation((m_vPos.x + m_vOf.x - vScr.x), (m_vPos.y + m_vOf.y - vScr.y), 0);
			mScale = XMMatrixScaling(m_fScale, m_fScale, 1.0f);
			mWorld = mScale * mWorld;
			// 画面外は描画しない
			if (mWorld._41 + m_pSprite->GetDestWidth()*m_fScale  < 0 || mWorld._41 > WINDOW_WIDTH ||
				mWorld._42 + m_pSprite->GetDestHeight()*m_fScale < 0 || mWorld._42 > WINDOW_HEIGHT)
			{
				;
			}
			else {
				m_pSprite->Draw(mWorld);
			}
		}
		else {			// 回転するとき
			if (m_vCenter.x == -1) {// 回転中心Ｘ座標の設定（-1のときはキャラクタ中心）
				cx = m_pSprite->GetDestWidth() / 2;
			}
			else {
				cx = (int)m_vCenter.x;
			}
			if (m_vCenter.y == -1) {// 回転中心Ｙ座標の設定（-1のときはキャラクタ中心）
				cy = m_pSprite->GetDestHeight() / 2;
			}
			else {
				cy = (int)m_vCenter.y;
			}

			// 回転マトリックスの作成
			MATRIX4X4 mWorld, mCenter, mRot, mScale;
			mWorld = XMMatrixTranslation((m_vPos.x + m_vOf.x - vScr.x), (m_vPos.y + m_vOf.y - vScr.y), 0);
			mScale = XMMatrixScaling(m_fScale, m_fScale, 1.0f);
			mCenter = XMMatrixTranslation(cx, cy, 0);
			mRot = XMMatrixRotationZ(XMConvertToRadians(m_fRotate));
			mWorld = mRot * mCenter * mScale * mWorld;
			mCenter = XMMatrixTranslation(-cx, -cy, 0);
			mWorld = mCenter * mWorld;

			// 画面外は描画しない
			if (mWorld._41 + m_pSprite->GetDestWidth()*m_fScale  < 0 || mWorld._41 > WINDOW_WIDTH ||
				mWorld._42 + m_pSprite->GetDestHeight()*m_fScale < 0 || mWorld._42 > WINDOW_HEIGHT)
			{
				;
			}
			else {
				m_pSprite->Draw(mWorld);
			}
		}
	}
}

//------------------------------------------------------------------------
// 基本オブジェクト
// オブジェクト同士の衝突判定メソッド（スケールを考慮）
//
// 引数
// CBaseObj* pObj : 対象のオブジェクト
//
// 戻り値 BOOL
// 接触している:TRUE 接触していない:FALSE
//------------------------------------------------------------------------
BOOL CBaseObj::isCollision(CBaseObj* pObj)
{
	float ratio = 0.8f; // 接触判定比率。例えば、0.8にすると外側の20%は接触判定から除外される。

	// オブジェクトが表示されていること
	if (!m_bActive || !pObj->m_bActive) return FALSE;

	// 衝突判定
	BOOL bRet = FALSE;
	DWORD  xd, yd, w1, w2, h1, h2;

	w1 = m_pSprite->GetDestWidth() * m_fScale;
	w2 = pObj->m_pSprite->GetDestWidth() * pObj->m_fScale;
	h1 = m_pSprite->GetDestHeight() * m_fScale;
	h2 = pObj->m_pSprite->GetDestHeight() * pObj->m_fScale;
	xd = abs((long)(((m_vPos.x + m_vOf.x) + w1 / 2) - ((pObj->m_vPos.x + pObj->m_vOf.x) + w2 / 2)));     // 中心点間の距離Ｘ方向
	yd = abs((long)(((m_vPos.y + m_vOf.y) + h1 / 2) - ((pObj->m_vPos.y + pObj->m_vOf.y) + h2 / 2)));     // 中心点間の距離Ｙ方向

	if ((xd < (w1 / 2 + w2 / 2)*ratio) &&     // 中心点間の距離がオブジェクトの大きさの1/2*ratioより小さければ接触している
		(yd < (h1 / 2 + h2 / 2)*ratio)) bRet = TRUE;

	return bRet;
}

//------------------------------------------------------------------------
// 基本オブジェクト
// オブジェクトとクリック位置の衝突判定メソッド（スケールを考慮）
//
// 引数
// int nMx :  マウス座標　Ｘ座標
// int nMy :  マウス座標　Ｙ座標
//
// 戻り値 BOOL
// ヒットしている:TRUE ヒットしていない:FALSE
//------------------------------------------------------------------------
BOOL CBaseObj::isCollisionClick(int nMx, int nMy)
{
	BOOL bRet = FALSE;

	// 矩形の判定
	if (m_bActive && (m_vPos.x + m_vOf.x) <= nMx && (m_vPos.x + m_vOf.x) + (m_pSprite->GetDestWidth() * m_fScale) >= nMx &&
		(m_vPos.y + m_vOf.y) <= nMy && (m_vPos.y + m_vOf.y) + (m_pSprite->GetDestHeight() * m_fScale) >= nMy) bRet = TRUE;

	return bRet;
}
//-----------------------------------------------------------------------------
// 基本プロシージャクラスのあたり判定
//
// 全ての相手プロシージャと相手オブジェクトを読み下しあたり判定を行う
//
// CBaseObj* pOtherObj 相手のオブジェクト
//
// 戻り値 BOOL
// TRUE:ヒットしたとき FALSE：ヒットしていないとき
//
// 判定結果　　
// ヒットしたときは　　　 相手と自分のオブジェクトのm_pOtherObjに値をセット
//                        相手と自分のオブジェクトのm_dwStatusにDAMAGEを設定する
// ヒットしていないときは 相手と自分のオブジェクトのm_pOtherObjにNULLをセット
//-----------------------------------------------------------------------------
BOOL CBaseProc::Hitcheck(CBaseObj* pOtherObj)
{
	BOOL bRet = FALSE;

	// 全ての相手とのあたり判定
	for (DWORD i = 0; i < m_pObjArray.size(); i++)  // 全ての相手オブジェクト
	{
		bRet = m_pObjArray[i]->Hitcheck(pOtherObj);  // 相手オブジェクトとのあたり判定
		if (bRet) return bRet;
	}

	for (DWORD i = 0; i < m_pProcArray.size(); i++)  // 下位の相手プロシージャ
	{
		bRet = m_pProcArray[i]->Hitcheck(pOtherObj);  // 相手プロシージャとのあたり判定
		if (bRet) return bRet;
	}

	return bRet;
}
//------------------------------------------------------------------------
// 基本オブジェクト
// オブジェクト同士のあたり判定メソッド
//
// isCollisionメソッドを使用してオブジェクト同士のあたり判定を行う
// 接触していれば、両方のオブジェクトのm_dwStatusを DAMAGE に、
// m_pOtherObjに相手のオブジェクトを設定する。
//
// 引数
// CBaseObj* pObj:相手のオブジェクト
//
// 戻り値 BOOL
// ヒットしている:TRUE ヒットしていない:FALSE
//------------------------------------------------------------------------
BOOL CBaseObj::Hitcheck(CBaseObj* pObj)
{
	if (m_dwStatus == NORMAL && pObj->m_dwStatus == NORMAL && isCollision(pObj))
	{
		m_dwStatus = DAMAGE;
		pObj->m_dwStatus = DAMAGE;
		m_pOtherObj = pObj;
		pObj->m_pOtherObj = this;
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------------
// 基本オブジェクト
// スプライトの中心点の座標を求める
//
// 引数 なし
// 戻り値 VECTOR2
// スプライト中心点の座標
//------------------------------------------------------------------------
VECTOR2 CBaseObj::GetCenterPos()
{
	VECTOR2 vPos = m_vPos;
	if (m_pSprite)
	{
		vPos.x += m_pSprite->GetSrcWidth() / 2;
		vPos.y += m_pSprite->GetSrcHeight() / 2;
	}
	return vPos;
}

//======================================================================
// 目的地（targetX,Y）への移動処理
//   引数　 ： speed:一回の移動量
//   戻り値 ： true:目的地に達した false:まだ目的地に達していない
//   オブジェクトに設定する値:m_vPosUpに移動量を設定する
//======================================================================
BOOL CBaseObj::TargetMove( float speed)
{
	float mvX, mvY, n;
	BOOL ret = FALSE;

	if (m_vPos.x == m_vTarget.x && m_vPos.y == m_vTarget.y) {  // 目的地に達したとき
		m_vPosUp.x = 0;
		m_vPosUp.y = 0;
		ret = TRUE; // 目的地に達した
	}
	else {    // 目的地への移動処理
		mvX = m_vTarget.x - m_vPos.x;   // 目的地までのＸ方向の変位
		mvY = m_vTarget.y - m_vPos.y;   // 目的地までのＹ方向の変位
		if (abs(mvX) >= abs(mvY)) {   // Ｘ方向の距離が長いとき
			if (abs(mvX) < speed) {        // １回の移動量より近接しているとき
				m_vPosUp.x = mvX;
				m_vPosUp.y = mvY;
			}
			else {
				if (mvX >= 0) {
					m_vPosUp.x = speed; // Ｘ方向の移動量をspeedにする
				}
				else {
					m_vPosUp.x = -speed; // Ｘ方向の移動量を-speedにする
				}
				n = abs(mvX / speed);
				m_vPosUp.y = floor(mvY / n); // Ｘ方向の移動量に合わせて、Ｙ方向の移動量を設定する
			}
		}
		else {                                // Ｙ方向の距離が長いとき
			if (abs(mvY) < speed) {        // １回の移動量より近接していると
				m_vPosUp.x = mvX;
				m_vPosUp.y = mvY;
			}
			else {
				if (mvY >= 0) {
					m_vPosUp.y = speed; // Ｙ方向の移動量をspeedにする
				}
				else {
					m_vPosUp.y = -speed; // Ｙ方向の移動量を-speedにする
				}
				n = abs(mvY / speed);
				m_vPosUp.x = floor(mvX / n); // Ｙ方向の移動量に合わせて、Ｘ方向の移動量を設定する
			}
		}
		ret = FALSE;    // まだ目的地に達していない
	}

	return ret;
}
