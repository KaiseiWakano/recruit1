//------------------------------------------------------------------------
// アイテムの処理ヘッダ ItemManager.h
//------------------------------------------------------------------------

#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"
#include "BackFore.h"
#include "Enemy.h"

// 定数定義
#define	ITEMRESQUE		1	// 救急箱
#define	ITEMDOOR		2	// ドア
#define	ITEMMAGIC		3	// 魔法薬
#define	ITEMBOX			4	// 宝箱
#define ITEMGREATBOX	5	// 大きな宝箱

#define ITEM_MAX 50

// アイテム関連追加 K.Wakano

// アイテムの種類数
const int ITEM_TYPE_NUM = 10;

// レア度の重みの定数
typedef enum
{
	COMMON = 100,
	UNCOMMON = 50,
	RARE = 10
}RARITY;

// アイテム構造体
typedef struct
{
	int num;			// アイテム番号
	char name[256];		// アイテム名
	int effect;			// 効果の初期値
	int effectNum;		// 効果の増分
	int stack;			// 所持数
	int weight;			// レア度の重み
}ITEM;

// ここまで

static int cntArray;	// ItemObj配列探索用

//======================================================================
// アイテム オブジェクトクラス
class CItemObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2, DWORD) override;
	void Update() override;

	// アイテムのフラグをセットする関数
	void SetItemGetFlag(int setResult);

	// コンストラクタ
	CItemObj(CGameMain* pGMain);
	// デストラクタ
	~CItemObj();
};
//======================================================================
// アイテム プロシージャクラス
class CItemProc : public CBaseProc
{
protected:
	int m_nCheckItem;	// アイテムを所持しているか否か判断
public:
	// アクセス関数
	CItemObj*			GetItemObjPtr() { return (CItemObj*)GetObjArrayPtr()[cntArray]; }
	// アイテムの所持判定のアクセスの関数
	int GetItemFlag() { return m_nCheckItem; }
	// アイテム構造体のアクセス関数
	int getItemList(int itemNum, int pick);
	// 効果及び所持量のセッター
	void setItemList(int itemNum, int pick, int setValue);

	void Update() override;
	// コンストラクタ
	CItemProc(CGameMain* pGMain);
	// デストラクタ
	~CItemProc() { ; }

	// ファイル読み込み
	void ItemDataInput();
private:
	ITEM itemList[ITEM_TYPE_NUM];
};
//======================================================================
// アイテム管理 プロシージャクラス
class CItemManagementProc : public CBaseProc
{
public:
	CItemProc* m_pItemProc;

public:
	void  SetNonActive();

	// コンストラクタ
	CItemManagementProc(CGameMain* pGMain);
	// デストラクタ
	~CItemManagementProc() { ; }
};
