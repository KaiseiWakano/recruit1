//------------------------------------------------------------------------
// アイテムの処理 ItemManager.cpp
//------------------------------------------------------------------------

#include "ItemManager.h"
#include "Playchar.h"
#include "Map.h"

//------------------------------------------------------------------------
// アイテムの処理メインプロシージャのコンストラクタ
//------------------------------------------------------------------------
CItemManagementProc::CItemManagementProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pItemProc = new CItemProc(m_pGMain);		// アイテムプロシージャ
	m_pProcArray.push_back(m_pItemProc);		// プロシージャをプロシージャ配列に登録する
}
//============================================================================
// プロシージャとオブジェクトを探索し全てのオブジェクトをノンアクティブにする
// プロシージャの開始フラグをリセットする
//============================================================================
void CItemManagementProc::SetNonActive()
{
	//オブジェクトポインタ配列の探索
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive(FALSE);
	}
	// 下位のプロシージャポインタ配列の探索
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag(0);							// 開始フラグのリセット
		((CItemManagementProc*)m_pProcArray[i])->SetNonActive();	// 下位のプロシージャの処理（再帰処理）
	}
}

// =================================================================================
//------------------------------------------------------------------------
// アイテムプロシージャのコンストラクタ
//------------------------------------------------------------------------
CItemProc::CItemProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CItemObj(m_pGMain));	// m_pObjArrayにアイテムを生成する
	}
	m_nStartFlag = 0;
	m_nCheckItem = 0;
}

//-----------------------------------------------------------------------------
// アイテムデータの読み込み K.Wakano
//-----------------------------------------------------------------------------
void CItemProc::ItemDataInput()
{
	int num, effect,effectNum, stack, weight;		// 入力用変数（整数型）
	char name[128] = {}, buf[128] = {};				// 入力用変数（キャラ型）
	FILE *m_pItemData = fopen("Data/Csv/ItemData.csv", "rt");		// csvファイルを開く
	if (!m_pItemData)
	{
		return;							// 読み込めなかった場合終了する
	}

	// 一行読み飛ばす
	fgets(buf, 128, m_pItemData);

	// 一行ずつ読み込んでアイテム構造体に代入
	for (int i = 0; fscanf(m_pItemData, "%d, %[^,], %d, %d, %d, %d", &num, name, &effect, &effectNum, &stack, &weight) != EOF; i++)
	{
		itemList[i].num = num;
		snprintf(itemList[i].name, sizeof name, name);
		itemList[i].effect = effect;
		itemList[i].effectNum = effectNum;
		itemList[i].stack = stack;
		itemList[i].weight = weight;
	}

	fclose(m_pItemData);				// ファイルを閉じる
}

//-----------------------------------------------------------------------------
// アイテムプロシージャの開始
// 戻り値 なし
//-----------------------------------------------------------------------------
void CItemProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	ItemDataInput();		// csvファイル読み込み

	// 乱数追加
	srand((unsigned int)time(NULL));
	int randomNum = 0;
	randomNum = rand() % 10;

	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、救急箱を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索 ( EvtID:2 アイテムの出現位置, EvtNo:0x10 救急箱	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x10, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMRESQUE);
						break;
					}
				}
			}
		}
		// イベントマップを探索し、ドア（出口）を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2 アイテムの出現位置,  EvtNo:0x20 ドア )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x20, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMDOOR);
						break;
					}
				}
			}
		}
		// イベントマップを探索し、魔法薬を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2 アイテムの出現位置, EvtNo:0x40 魔法薬 )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x40, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMAGIC);
						break;
					}
				}
			}
		}
		// イベントマップを探索し、宝箱を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2 アイテムの出現位置, EvtNo:0x80 宝箱 )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x80, vPos, nNext))
			{
				//if (randomNum < 5) {			// 宝箱ランダム生成
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMBOX);
						break;
					}
				}
				//}
				randomNum = rand() % 10;		// 乱数リセット
			}
		}
		// イベントマップを探索し、大きな宝箱を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2 アイテムの出現位置, EvtNo:0x100 宝箱 )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x100, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMGREATBOX);
						break;
					}
				}
			}
		}

		m_nStartFlag = 1;  // 一回発生させたら、もう発生しない
	}
}

// アイテム構造体のゲッター K.Wakano
// 引数 itemNum:アイテム番号 pick:取得する値番号
// 0:アイテム番号 1:初期値 2:増分 3:所持数 4:重み
int CItemProc::getItemList(int itemNum, int pick)
{
	if (0 > itemNum || itemNum > ITEM_TYPE_NUM || 0 > pick || pick > 4)
	{
		return -1;	// 引数に異常ありとして終了
	}
	switch (pick)
	{
	case 0:
		return itemList[itemNum].num;		// 番号を返す
		break;
	case 1:
		return itemList[itemNum].effect;	// 初期値を返す
		break;
	case 2:
		return itemList[itemNum].effectNum;	// 増分を返す
		break;
	case 3:
		return itemList[itemNum].stack;		// 所持数を返す
		break;
	case 4:
		return itemList[itemNum].weight;	// 重みを返す
		break;
	default:
		break;
	}
}

// アイテム構造体のセッター K.Wakano
// 引数 itemNum:アイテム番号 setValue:セットする数字
// 0:初期値 1:所持量 2:初期値に増分を加算
void CItemProc::setItemList(int itemNum, int pick, int value)
{
	switch (pick)
	{
	case 0:
		itemList[itemNum].effect = value;	// 初期値
		break;
	case 1:
		itemList[itemNum].stack = value;	// 所持量
		break;
	case 2:
		itemList[itemNum].effect += value;	// 初期値に増分を加算
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------
// アイテムオブジェクトのコンストラクタ	
// 引数 なし
//------------------------------------------------------------------------
CItemObj::CItemObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar);
	m_nAnimNum = 1;
	m_nCheckItem = -1;
}
// ---------------------------------------------------------------------------
// アイテムオブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CItemObj::~CItemObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// アイテムオブジェクトの開始
//
// VECTOR2 vPos 発生位置
// DWORD dwNo アイテム種別
//
// 戻り値 TRUE
//-----------------------------------------------------------------------------
BOOL CItemObj::Start(VECTOR2 vPos, DWORD dwNo)
{
	ResetStatus();
	ResetAnim();

	m_bActive = TRUE;
	m_dwStatusSub = dwNo;

	srand((unsigned int)time(NULL));	// 乱数の種のリセット

	m_vPos = vPos;	// 発生位置
	m_vPosUp = VECTOR2(0, 0);

	switch (m_dwStatusSub)
	{
	case  ITEMRESQUE:		// 救急箱
		m_pSprite->SetSrc(384, 336, 48, 48);
		break;

	case  ITEMDOOR:			// ドア、透明色
		m_pSprite->SetSrc(192, 0, 48, 48);
		break;

	case  ITEMMAGIC:		// 魔法薬
		m_pSprite->SetSrc(288, 336, 48, 48);
		break;

	case  ITEMBOX:			// 宝箱
		m_pSprite->SetSrc(0, 384, 48, 48);
		break;

	case ITEMGREATBOX:		// 大きな宝箱
		m_pSprite->SetSrc(144, 384, 48, 48);
		break;
	}
	return TRUE;
}

// アイテムをセットする関数 K.Wakano
void CItemObj::SetItemGetFlag(int setResult)
{
	int itemNum = setResult;
	m_pSprite->SetSrc(48 * setResult, 432, 48, 48);
	m_nCheckItem = setResult;
}

//-----------------------------------------------------------------------------
// アイテムオブジェクトの更新
// 引数 なし
//-----------------------------------------------------------------------------
void CItemObj::Update()
{
	if (!m_bActive) return;
	CDirectInput* pDI = m_pGMain->m_pDI;

	//
	// 宝箱の中身の処理 K.Wakano ここから
	//

	int weightList[ITEM_TYPE_NUM];	// 重み格納配列

	// 抽選結果
	int result = -1;

	// 重みの合計計算
	int weightSum = 0;
	for (int i = 0; i < ITEM_TYPE_NUM; i++)
	{
		weightList[i] = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(i, 4);
		weightSum += weightList[i];
	}

	// 乱数追加
	int randomNum;
	randomNum = rand() % weightSum;

	// 抽選決定
	for (int i = 0; i < ITEM_TYPE_NUM; i++)
	{
		if (randomNum < weightList[i])
		{
			result = i;
			break;
		}
		randomNum -= weightList[i];
	}

	//
	// 宝箱の中身の処理 K.Wakano ここまで
	//

	if (isCollision(m_pGMain->m_pPcProc->GetPcObjPtr())) // PCとの接触判定
	{
		switch (m_dwStatusSub)
		{
		case  ITEMRESQUE:     // 救急箱
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
			m_pGMain->m_pSePower->Play();
			m_bActive = FALSE;
			break;

		case  ITEMDOOR:      // ドア
			if (pDI->CheckKey(KD_DAT, DIK_E) && flagBossDeath == true) {
				m_pGMain->m_pSePower->Play();
				m_pGMain->m_pMapProc->MoveMap();
				m_bActive = FALSE;
			}
			break;

		case  ITEMMAGIC:	// 魔法薬
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetMp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxMp());
			m_pGMain->m_pSePower->Play();
			m_bActive = FALSE;
			break;

			// 宝箱の処理
		case  ITEMBOX:
			if (pDI->CheckKey(KD_DAT, DIK_E)) {		// Eキーが押されると開く
				m_pGMain->m_pSeUnlock->Play();		// SEの再生
				SetItemGetFlag(result);				// 結果を渡してアイテムを表示＆所持フラグをセット
				m_bActive = FALSE;					// 消す
			}
			break;

			// 大きな宝箱
		case ITEMGREATBOX:
			if (pDI->CheckKey(KD_DAT, DIK_E) && 
				m_pGMain->m_pForeProc->getCanOpen() > 0) {	// Eが押され、かつ鍵を所持していると開く
				m_pGMain->m_pSeUnlock->Play();				// SEの再生
				SetItemGetFlag(9);							// 兜
				m_bActive = FALSE;							// 消す
				break;
			}
			break;
		}
	}
	AnimCountup();
	Draw();
}
