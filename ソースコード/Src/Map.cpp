//------------------------------------------------------------------------
// マップの処理 Map.cpp
//------------------------------------------------------------------------

#include "Map.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Effect.h"
#include "ItemManager.h"

// CMapクラス
// コンストラクタ
CMap::CMap(CGameMain* pGMain)
{
	m_pGMain = pGMain;
	m_MapFileName[0] = _T('\0');
	m_ImageName[0] = _T('\0');
	m_pMapImage = NULL;
	m_nMapX = 0;
	m_nMapY = 0;
	m_nMapchipWidth = 0;
	m_nMapchipHeight = 0;
	m_nMapLnLength = 0;
	m_nEvtMapLength = 0;
	m_MapBakTbl = NULL;
	m_MapForTbl = NULL;
	m_MapLn     = NULL;
	m_EvtMap    = NULL;
}

// デストラクタ
CMap::~CMap()
{
	SAFE_DELETE(m_pMapImage);
	SAFE_DELETE_ARRAY(m_MapBakTbl);
	SAFE_DELETE_ARRAY(m_MapForTbl);
	SAFE_DELETE_ARRAY(m_MapLn);
	SAFE_DELETE_ARRAY(m_EvtMap);
}

//------------------------------------------------------------------------
// CMapクラスのマップの初期化
//------------------------------------------------------------------------
void CMap::InitMap()
{
	int i;

	TCHAR szName[256];

	MakeERelativeFName(m_MapFileName, m_ImageName, szName);  // マップファイル名からマップイメージのパスを得る

	m_pMapImage = new CSpriteImage(m_pGMain->m_pShader, szName);
	m_MapBakTbl = new int[m_nMapX*m_nMapY];	// 要素数分の配列を確保
	for (i = 0; i < m_nMapX*m_nMapY; i++)
	{
		m_MapBakTbl[i] = -1;	// -1で初期化
	}
	m_MapForTbl = new int[m_nMapX*m_nMapY];	// 要素数分の配列を確保
	for (i = 0; i < m_nMapX*m_nMapY; i++)
	{
		m_MapForTbl[i] = -1;	// -1で初期化
	}
}

//-----------------------------------------------------------------------------
// 相対(relative)パスから基準フォルダからの相対(ERelative)パスを作成する
//
// 引数 TCHAR*  szBaseFullPath
//      TCHAR*  szPath
//      TCHAR*  szERelativeFName(Out)
//-----------------------------------------------------------------------------
BOOL CMap::MakeERelativeFName(TCHAR* szBaseFullPath, TCHAR* szPath, TCHAR* szERelativeFName)
{
	const DWORD NUM = 20;

	TCHAR BaseDrive[256], BasePath[512], BaseFName[512], BaseExt[256];
	TCHAR Drive[256], Path[512], FName[512], Ext[256];

	TCHAR *p;
	TCHAR BasePathArray[NUM][256] = { 0 };
	TCHAR PathArray[NUM][256] = { 0 };

	int i, j, n, m, d;

	// ドライブ番号、パス名、ファイル名、拡張子に分解
	_tsplitpath_s(szBaseFullPath, BaseDrive, sizeof(BaseDrive) / sizeof(TCHAR), BasePath, sizeof(BasePath) / sizeof(TCHAR),
		BaseFName, sizeof(BaseFName) / sizeof(TCHAR), BaseExt, sizeof(BaseExt) / sizeof(TCHAR));
	_tsplitpath_s(szPath, Drive, sizeof(Drive) / sizeof(TCHAR), Path, sizeof(Path) / sizeof(TCHAR),
		FName, sizeof(FName) / sizeof(TCHAR), Ext, sizeof(Ext) / sizeof(TCHAR));

	// ドライブ番号があるときは既に絶対パスになっている
	if (Drive[0] != _T('\0'))
	{
		_tcscpy(szERelativeFName, szPath);
		return TRUE;
	}

	// パス名を各フォルダ階層に分解
	i = 0;
	p = _tcstok(BasePath, _T("/\\"));
	while (p)
	{
		_tcscpy(BasePathArray[i], p);
		i++;
		p = _tcstok(NULL, _T("/\\"));
	}
	n = i;  // ベースの階層数

	i = 0;
	p = _tcstok(Path, _T("/\\"));
	while (p)
	{
		_tcscpy(PathArray[i], p);
		i++;
		p = _tcstok(NULL, _T("/\\"));
	}

	// 相対パスの'．'の数
	i = 0;
	d = 1;
	if (PathArray[i][0] == _T('.'))
	{
		for (j = 1; PathArray[i][j] != _T('\0'); j++) d++;
		i++;
	}
	else if (PathArray[i][0] == _T('\0'))
	{
		i++;
	}
	else {
		;
	}
	m = i;

	// 基準フォルダからの相対パスの作成
	_tcscpy(szERelativeFName, BaseDrive);
	if (BaseDrive[0] != _T('\0')) _tcscat(szERelativeFName, _T("/"));
	for (i = 0; i<n - d + 1; i++)
	{
		_tcscat(szERelativeFName, BasePathArray[i]);
		_tcscat(szERelativeFName, _T("/"));
	}
	for (; PathArray[m][0] != _T('\0'); m++)
	{
		_tcscat(szERelativeFName, PathArray[m]);
		_tcscat(szERelativeFName, _T("/"));
	}
	_tcscat(szERelativeFName, FName);
	_tcscat(szERelativeFName, Ext);

	return TRUE;
}

// CMapProcクラス
// コンストラクタ
CMapProc::CMapProc(CGameMain* pGMain)
{
	m_pGMain       = pGMain;
	m_nMapNo       = 0;
	m_bDrawMapLine = FALSE;
	for (DWORD i = 0; i < MAP_MAX; i++)
	{
		m_pMapArray[i] = NULL;
	}
	m_pBakImage   = new CSpriteImage(m_pGMain->m_pShader, _T("Data/image/aozora.png"));
	m_pSpriteMap  = new CSprite(m_pGMain->m_pShader); // スプライトオブジェクトの生成
	m_pSpriteBak  = new CSprite(m_pBakImage);
	m_pSpriteLine = new CSprite(m_pGMain->m_pShader);

	InitMapAll();
}

// デストラクタ
CMapProc::~CMapProc()
{
	for (DWORD i = 0; i < MAP_MAX; i++)
	{
		SAFE_DELETE(m_pMapArray[i]);
	}
	SAFE_DELETE(m_pSpriteMap);
	SAFE_DELETE(m_pSpriteBak);
	SAFE_DELETE(m_pSpriteLine);
	SAFE_DELETE(m_pBakImage);
}

// ----------------------------------------------------------------------
// 各マップスクリプトファイルを読み込んでマップオブジェクトの作成
// -----------------------------------------------------------------------
void CMapProc::InitMapAll()
{
	// ステージ0　宝箱回収ステージ
	MapRead( _T("Data/Script/Map10.txt"), m_pMapArray[0]);

	// ステージ1　敵メインステージ
	MapRead( _T("Data/Script/Map100x.txt"), m_pMapArray[1]);

	// ステージ2　ボスのみステージ
	MapRead( _T("Data/Script/Map101.txt"), m_pMapArray[2]);	

	// 開始マップの設定
	SetMap(0);
}

// ----------------------------------------------------------------------
// マップ移動の処理 扉に接触したとき呼ばれる
// ----------------------------------------------------------------------
void CMapProc::MoveMap()
{
	if (m_nMapNo == 0) 
	{
		SetMap(1);	// 次のマップは1
		flagBossDeath = false;
		m_pGMain->m_pEnmProc->SetNonActive();				// 敵を全て消す。敵の発生フラグをリセットする
		m_pGMain->m_pEffectProc->SetNonActive();			// アイテムを全て消す。アイテムの発生フラグをリセットする
		m_pGMain->m_pItemManagementProc->SetNonActive();
	}
	else if (m_nMapNo == 1)
	{
		SetMap(2);	// 次のマップは2
		flagBossDeath = false;
		m_pGMain->m_pEnmProc->SetNonActive();		// 敵を全て消す。敵の発生フラグをリセットする
		m_pGMain->m_pEffectProc->SetNonActive();	// アイテムを全て消す。アイテムの発生フラグをリセットする
		m_pGMain->m_pItemManagementProc->SetNonActive();
	}
	else
	{
		m_pGMain->m_dwGameStatus = GAMECLEAR;  // ゲームクリヤー
	}
}
// ----------------------------------------------------------------------
// 開始マップの設定
//
// 引数:int no マップ番号
// ----------------------------------------------------------------------
void CMapProc::SetMap(int no)
{
	// 開始マップナンバー
	m_nMapNo = no;

	// マップ用スプライトの設定
	m_pSpriteMap->SetSrc( m_pMapArray[m_nMapNo]->m_pMapImage, 0, 0, m_pMapArray[m_nMapNo]->m_nMapchipWidth, m_pMapArray[m_nMapNo]->m_nMapchipHeight);

	// PC開始位置の設定
	VECTOR2 vPos = VECTOR2(0, 0);
	for (DWORD i = 0; i < m_pMapArray[m_nMapNo]->m_nEvtMapLength; i++)
	{
		if (m_pMapArray[m_nMapNo]->m_EvtMap[i].m_nEvtID == 1 )  // ＰＣのスタート位置
		{
			vPos = m_pMapArray[m_nMapNo]->m_EvtMap[i].m_vStart;  // ＰＣのスタート座標
			break;
		}
	}
	// PC開始位置を設定し、HPを回復する
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetPos(vPos);
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
}

// ----------------------------------------------------------------------
// マップの更新1
// ----------------------------------------------------------------------
void CMapProc::Update()
{
	VECTOR2 vScr;

	// PCの位置からスクロール座標を設定する
	vScr.x = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x + m_pGMain->m_pPcProc->GetPcObjPtr()->GetOf().x +  m_pGMain->m_pPcProc->GetPcObjPtr()->GetSprite()->GetSrcWidth() / 2 - WINDOW_WIDTH / 2;
	if (vScr.x > m_pMapArray[m_nMapNo]->m_nMapX * m_pMapArray[m_nMapNo]->m_nMapchipWidth - WINDOW_WIDTH)
		vScr.x = m_pMapArray[m_nMapNo]->m_nMapX * m_pMapArray[m_nMapNo]->m_nMapchipWidth - WINDOW_WIDTH;
	if (vScr.x < 0) vScr.x = 0;

	vScr.y = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y + m_pGMain->m_pPcProc->GetPcObjPtr()->GetOf().y + m_pGMain->m_pPcProc->GetPcObjPtr()->GetSprite()->GetSrcHeight() / 2 - WINDOW_HEIGHT / 2;
	if (vScr.y > m_pMapArray[m_nMapNo]->m_nMapY * m_pMapArray[m_nMapNo]->m_nMapchipHeight - WINDOW_HEIGHT)
		vScr.y = m_pMapArray[m_nMapNo]->m_nMapY * m_pMapArray[m_nMapNo]->m_nMapchipHeight - WINDOW_HEIGHT;
	if (vScr.y < 0) vScr.y = 0;

	m_pGMain->m_vScroll = vScr;	// スクロール座標を設定

	// マップ線の描画をするかどうか
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_P))
	{
		if (m_bDrawMapLine)
		{
			m_bDrawMapLine = FALSE;
		}
		else {
			m_bDrawMapLine = TRUE;
		}
	}
	DrawBack(); // 背景マップの描画
}

// ----------------------------------------------------------------------
// マップの更新2
// ----------------------------------------------------------------------
void  CMapProc::Update2()
{
	DrawFore();
	if (m_bDrawMapLine) DrawMapLine();
}

// ----------------------------------------------------------------------
// イベントマップの探索処理
//
// 引数
// 　　int         nStart;		// 探索開始位置
// 　　int         nEvtID;		// イベントID
// 　　DWORD       dwEvtNo;		// イベントNO
// 　　VECTOR2 vPos;			// 座標(OUT)
//     int         nNext;       // 次開始位置(OUT) 最終に達したら -1
// 戻り値
//　　TRUE:見つかった FALSE:見つからない
// ----------------------------------------------------------------------
BOOL CMapProc::SearchEvt( int nStart, int nEvtID, DWORD dwEvtNo, VECTOR2& vPos, int& nNext)
{
	BOOL bRet = FALSE;

	if (nStart < 0 || nStart >= m_pMapArray[m_nMapNo]->m_nEvtMapLength)
	{
		nNext = -1;
		return bRet;
	}

	for (DWORD i = nStart; i < m_pMapArray[m_nMapNo]->m_nEvtMapLength; i++)
	{
		if (m_pMapArray[m_nMapNo]->m_EvtMap[i].m_nEvtID == nEvtID &&	    // イベントマップ　ID　敵の出現位置
			m_pMapArray[m_nMapNo]->m_EvtMap[i].m_dwEvtNo & dwEvtNo)			// イベントマップ　NO
		{
			vPos = m_pMapArray[m_nMapNo]->m_EvtMap[i].m_vStart;	// 発生位置
			nNext = i + 1;  // 次の開始位置を設定
			bRet = TRUE;
			break;
		}
	}
	if (!bRet)
	{
		nNext = -1;
	}

	return bRet;
}

// ----------------------------------------------------------------------
// マップの背景の描画
// ----------------------------------------------------------------------
void CMapProc::DrawBack()
{
	// ステージ背景の描画（４方向スクロール）
	VECTOR2 scr;

	scr.x = WINDOW_WIDTH  - ((int)(m_pGMain->m_vScroll.x / 2)) % WINDOW_WIDTH;
	scr.y = WINDOW_HEIGHT - ((int)(m_pGMain->m_vScroll.y / 2)) % WINDOW_HEIGHT;

	m_pSpriteBak->Draw(m_pBakImage, 0, 0, WINDOW_WIDTH - scr.x, WINDOW_HEIGHT - scr.y, scr.x, scr.y);
	m_pSpriteBak->Draw(m_pBakImage, scr.x, 0, 0, WINDOW_HEIGHT - scr.y, WINDOW_WIDTH - scr.x, scr.y);
	m_pSpriteBak->Draw(m_pBakImage, 0, scr.y, WINDOW_WIDTH - scr.x, 0, scr.x, WINDOW_HEIGHT - scr.y);
	m_pSpriteBak->Draw(m_pBakImage, scr.x, scr.y, 0, 0, WINDOW_WIDTH - scr.x, WINDOW_HEIGHT - scr.y);

	// マップ背景の描画　
	int x, y, no;

	for (y = 0; y < m_pMapArray[m_nMapNo]->m_nMapY; y++)
	{
		for (x = 0; x < m_pMapArray[m_nMapNo]->m_nMapX; x++)
		{
			no = m_pMapArray[m_nMapNo]->m_MapBakTbl[y*m_pMapArray[m_nMapNo]->m_nMapX + x];
			if ( no == -1)
			{
				;  		// マップの無い所は描画しない
			}else{
				m_pSpriteMap->m_ofX = no % 1000 * m_pMapArray[m_nMapNo]->m_nMapchipWidth;
				m_pSpriteMap->m_ofY = no / 1000 * m_pMapArray[m_nMapNo]->m_nMapchipHeight;
				m_pSpriteMap->Draw(x*m_pMapArray[m_nMapNo]->m_nMapchipWidth - m_pGMain->m_vScroll.x, y*m_pMapArray[m_nMapNo]->m_nMapchipHeight - m_pGMain->m_vScroll.y);
			}
		}
	}
}
// ----------------------------------------------------------------------
// マップの前景の描画
// ----------------------------------------------------------------------
void CMapProc::DrawFore()
{
	int x, y, no;

	for (y = 0; y < m_pMapArray[m_nMapNo]->m_nMapY; y++)
	{
		for (x = 0; x < m_pMapArray[m_nMapNo]->m_nMapX; x++)
		{
			no = m_pMapArray[m_nMapNo]->m_MapForTbl[y*m_pMapArray[m_nMapNo]->m_nMapX + x];
			if (no == -1)
			{
				;		// マップの無い所は描画しない
			}else {
				m_pSpriteMap->m_ofX = no % 1000 * m_pMapArray[m_nMapNo]->m_nMapchipWidth;
				m_pSpriteMap->m_ofY = no / 1000 * m_pMapArray[m_nMapNo]->m_nMapchipHeight;
				m_pSpriteMap->Draw(x*m_pMapArray[m_nMapNo]->m_nMapchipWidth - m_pGMain->m_vScroll.x, y*m_pMapArray[m_nMapNo]->m_nMapchipHeight - m_pGMain->m_vScroll.y);
			}
		}
	}
}

// ----------------------------------------------------------------------
// マップ線の描画
// ----------------------------------------------------------------------
void CMapProc::DrawMapLine()
{
	int i;

	for (i = 0; i < m_pMapArray[m_nMapNo]->m_nMapLnLength; i++)
	{
		m_pSpriteLine->DrawLine(m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x - m_pGMain->m_vScroll.x, m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y - m_pGMain->m_vScroll.y,
										m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x - m_pGMain->m_vScroll.x, m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y - m_pGMain->m_vScroll.y, 3, RGB(255, 0, 0));
		float cx = (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x) / 2;
		float cy = (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y) / 2;
		float nx = cx + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x * 10.0f;
		float ny = cy + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y * 10.0f;
		m_pSpriteLine->DrawLine( cx - m_pGMain->m_vScroll.x, cy - m_pGMain->m_vScroll.y, nx - m_pGMain->m_vScroll.x, ny - m_pGMain->m_vScroll.y, 3, RGB(255, 0, 0));
	}
}

// ----------------------------------------------------------------------------------------
// マップの接触判定と適切な位置への移動
//
// ①マップ線を突き抜けているかチェックする
// ②マップ線に近接しているか（キャラが食い込んでいるか）チェックする
// 突き抜けているか近接しているとき、増分値m_vPosUpを法線方向に食い込み分だけ戻す
//
// 戻り値：突き抜けているか近接しているとき真。pHitmapline:接触したマップ線のアドレスが返る
// ----------------------------------------------------------------------------------------
BOOL CMapProc::isCollisionMoveMap( CBaseObj* obj, CMapLine* &pHitmapline)
{
	int i, n, rw;
	BOOL bRet = FALSE;
	VECTOR2 vHitpos = VECTOR2(0,0);
	VECTOR2 vHpw = VECTOR2(0,0);
	float dist, dw;

	pHitmapline = NULL;
	dist = 999999;
	for (n = 0, i = 0; i< m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
		rw = CheckMapcross(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);		// ①　マップ線との突き抜け判定
		if (rw) {	// 突き抜けているとき
			bRet = rw;
			dist = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
			n = i;
			vHitpos = vHpw;
			break;
		}
		else {
			rw = CheckMapnear(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);	// ②　マップ線との近接判定（キャラがマップ線に食い込んでいるか）
			if (rw) {
				bRet = rw;
				dw = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
				if (dist > dw) {						// 一番近いマップ線を探す
					n = i;
					dist = dw;
					vHitpos = vHpw;
				}
			}
		}
	}

	i = n;
	if (bRet) {	// 突き抜けているか近接しているとき、法線方向に食い込み分だけ、戻してやる

		pHitmapline = &(m_pMapArray[m_nMapNo]->m_MapLn[i]);  // 接触したマップ線のアドレス
		obj->SetPosUp( obj->GetPosUp() + VECTOR2(
			round(-(dist - obj->GetSprite()->GetSrcWidth() / 2)   * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x),
			round(-(dist - obj->GetSprite()->GetSrcHeight() / 2)  * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y)));
	}

	// 接触しているときのみ２回目のチェックを行う
	if (bRet) {
		bRet = FALSE;	// 一旦クリヤーする
		dist = 999999;
		for (n = 0, i = 0; i<m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
			rw = CheckMapcross(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);		// マップ線との突き抜け判定
			if (rw) {	// 突き抜けているとき
				bRet = rw;
				dist = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
				n = i;
				vHitpos = vHpw;
				break;
			}
			else {
				rw = CheckMapnear(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);	// マップ線との近接判定（キャラがマップ線に食い込んでいるか）
				if (rw) {
					bRet = rw;
					dw = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
					if (dist > dw) {						// 一番近いマップ線を探す
						n = i;
						dist = dw;
						vHitpos = vHpw;
					}
				}
			}
		}

		i = n;
		if (bRet) {	// 突き抜けているか近接しているとき、m_vPosUpを法線方向に食い込み分だけ、戻してやる

			obj->SetPosUp(obj->GetPosUp() + VECTOR2(
				round(-(dist - obj->GetSprite()->GetSrcWidth() / 2)   * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x),
				round(-(dist - obj->GetSprite()->GetSrcHeight() / 2)  * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y)));
		}
		bRet = TRUE;	// 再度セットする
	}
	return bRet;
}

// ----------------------------------------------------------------------------------------
// マップ線との突き抜け判定
//
// 戻り値:交差しているとき真。交点座標がhitposに返る
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckMapcross(CMapLine map, CBaseObj* obj, VECTOR2 &hitpos)
{
	BOOL bRet = FALSE;

	// 直線1 m1, m2...マップ線
	VECTOR2  m1 = map.m_vStart, m2 = map.m_vEnd;
	// 直線2 oj1, oj2... オブジェクト移動
	VECTOR2  oj1 = obj->GetCenterPos();
	VECTOR2  oj2 = obj->GetCenterPos() + obj->GetPosUp();

	// ２直線の交差チェック
	bRet = CheckLinecross(m1, m2, oj1, oj2, hitpos);

	return bRet;
}

// ----------------------------------------------------------------------------------------
// 2直線の交差チェック
// 直線aと直線bの交差チェック。
//
// 戻り値:交差しているとき真。交点座標がhitposに返る
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckLinecross(VECTOR2 a1, VECTOR2 a2, VECTOR2 b1, VECTOR2 b2, VECTOR2 &hitpos)
{
	BOOL bRet = FALSE;

	// 交点ap
	VECTOR2  ap = VECTOR2(0, 0);
	float d1, d2;

	// 直線の長さが0のとき
	if ((a1.x == a2.x && a1.y == a2.y) ||
		(b1.x == b2.x && b1.x == b2.y)) {
		return bRet;
	}

	// 交点があるか
	float dev = (a2.y - a1.y) * (b2.x - b1.x) - (a2.x - a1.x) * (b2.y - b1.y);
	if (dev == 0) {// 平行線のとき
		return bRet;
	}

	// 交点を求める
	d1 = (b1.y*b2.x - b1.x*b2.y);
	d2 = (a1.y*a2.x - a1.x*a2.y);

	ap.x = d1*(a2.x - a1.x) - d2*(b2.x - b1.x);
	ap.x /= dev;
	ap.y = d1*(a2.y - a1.y) - d2*(b2.y - b1.y);
	ap.y /= dev;

	// 交点が直線の範囲の中にあるか
	if ((CheckRange(a1.x, a2.x, ap.x) && CheckRange(a1.y, a2.y, ap.y)) &&
		(CheckRange(b1.x, b2.x, ap.x) && CheckRange(b1.y, b2.y, ap.y))) {
		bRet = TRUE;
	}

	if (bRet) {
		hitpos = ap;
	}

	return bRet;
}

// ----------------------------------------------------------------------------------------
// 範囲チェック
//
// 戻り値:ptがlとrの間に入っているとき真
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckRange(float l, float r, float pt)
{
	float low, hi;
	float mgn = 0.05f;	// 誤差

	if (l <= r) {
		low = l;
		hi = r;
	}
	else {
		low = r;
		hi = l;
	}
	low -= mgn;
	hi += mgn;

	if (low <= pt && pt <= hi) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// ----------------------------------------------------------------------------------------
// マップ線との近接チェック
// キャラがマップ線に食い込んでいるかチェック
//
// 戻り値:食い込んでいるとき真 接触位置がvHitposに返る
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckMapnear(CMapLine MapLn, CBaseObj* obj, VECTOR2 &vHitpos)
{
	BOOL bRet = FALSE;

	// ① キャラとマップ線の法線方向との食い込みチェックを行う
	// キャラの中心から、マップ線の法線方向にキャラの大きさ分の直線を引く
	VECTOR2  p0, p1, p2;
	p0 = obj->GetCenterPos() + obj->GetPosUp();
	p1.x = p0.x - MapLn.m_vNormal.x * obj->GetSprite()->GetSrcWidth() / 2;
	p1.y = p0.y - MapLn.m_vNormal.y * obj->GetSprite()->GetSrcHeight() / 2;
	p2.x = p0.x + MapLn.m_vNormal.x * obj->GetSprite()->GetSrcWidth() / 2;
	p2.y = p0.y + MapLn.m_vNormal.y * obj->GetSprite()->GetSrcHeight() / 2;

	// 直線とマップ線との交差チェック
	bRet = CheckLinecross( MapLn.m_vStart,  MapLn.m_vEnd, p1, p2, vHitpos);

	// ②　交差していない場合、キャラと垂直下方向との食い込みチェックを行う
	if (!bRet) {
		// キャラの中心から、垂直下方向にキャラの大きさ分の直線を引く
		p0 = obj->GetCenterPos() + obj->GetPosUp();

		p1.x = p0.x - 0 * obj->GetSprite()->GetSrcWidth() / 2;
		p1.y = p0.y - 1 * obj->GetSprite()->GetSrcHeight() / 2;
		p2.x = p0.x + 0 * obj->GetSprite()->GetSrcWidth() / 2;
		p2.y = p0.y + 1 * obj->GetSprite()->GetSrcHeight() / 2;

		// 直線とマップ線との交差チェック
		bRet = CheckLinecross(MapLn.m_vStart, MapLn.m_vEnd, p1, p2, vHitpos);
	}
	return bRet;
}

// ----------------------------------------------------------------------------------------
// マップ線との距離を求める
//
// 戻り値:マップ線との距離。ただし、法線方向がプラスとなる
// ----------------------------------------------------------------------------------------
float CMapProc::GetDistance(CMapLine MapLn, CBaseObj* obj)
{
	VECTOR2 b = obj->GetCenterPos() + obj->GetPosUp() - MapLn.m_vStart;
	float len;

	// 内積は、ベクトルの法線方向の距離になる
	// (法線の長さが１のため)
	len = GetDot( MapLn.m_vNormal, b );

	return len;
}

// ----------------------------------------------------------------------------------------
// 2点間の距離を求める
// ----------------------------------------------------------------------------------------
float CMapProc::GetLength(VECTOR2 p1, VECTOR2 p2)
{
	return sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

// ----------------------------------------------------------------------------------------
// ベクトルの外積を求める
// ----------------------------------------------------------------------------------------
float CMapProc::GetCross(VECTOR2 a, VECTOR2 b)
{
	return  a.x * b.y - a.y * b.x;
}

// ----------------------------------------------------------------------------------------
// ベクトルの内積を求める
// ----------------------------------------------------------------------------------------
float CMapProc::GetDot( VECTOR2 a, VECTOR2 b)
{
	return  a.x * b.x + a.y * b.y;
}
