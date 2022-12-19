//------------------------------------------------------------------------
// 基本プロシージャ・基本オブジェクトクラス処理 BaseProc.h
//------------------------------------------------------------------------

#pragma once
#include "GameMain.h"

class CSprite;
class CGameMain;

// --------------------------------------------------------------------------------------------------
// 基本オブジェクトクラス
// --------------------------------------------------------------------------------------------------
class CBaseObj
{
protected:
	CGameMain*		m_pGMain;			// ゲームメインクラス　　
	DWORD			m_dwStatus;			// ステータス
	DWORD			m_dwStatusSub;		// ステータスサブ
	DWORD			m_dwStatusPhase;	// ステータスフェーズ
	BOOL			m_bActive;			// TRUE:表示  FALSE:非表示
	CSprite*		m_pSprite;			// スプライトオブジェクト
	VECTOR2			m_vPos;				// 表示位置
	VECTOR2			m_vPosUp;			// 表示位置　移動増分
	VECTOR2			m_vOf;				// 表示位置　移動変位
	float			m_fRotate;			// 回転角度
	VECTOR2			m_vCenter;			// 回転中心
	VECTOR2			m_vTarget;			// 移動目的地
	float			m_fJumpTime;		// ジャンプ時間
	VECTOR2			m_vJumpSpeed;		// ジャンプスピード
	float			m_fScale;			// 拡大縮小
	float			m_fAlpha;			// 透明度
	int				m_nAnimNum;			// アニメーション要素数(初期値は２)
	int				m_nAnimIdx;			// アニメーションインデックス数(インデックス位置はＸ方向)
	int				m_nAnimFrm;			// アニメーションフレームカウント
	int				m_nDirIdx;			// 方向アニメーションインデックス数(インデックス位置はＹ方向)
	int				m_nFlashIdx;		// フラッシュアニメーションインデックス数
	int				m_nFlashFrm;		// フラッシュアニメーションフレームカウント
	CBaseObj*		m_pOtherObj;		// ぶつかった相手のオブジェクト
	int				m_nCnt1;			// 汎用カウンタ
	int				m_nCnt2;			// 汎用カウンタ2
	int				m_nMaxHp;			// 最大体力
	int				m_nHp;				// 体力
	int				m_nMaxMp;			// 最大魔力
	int				m_nMp;				// 魔力
	int				m_nAtc;				// 攻撃力
	int				m_nCheckItem;		// アイテム取得フラグ

public:
	virtual			void Update() { ; }
	virtual			void Draw();

	virtual			BOOL Start() { return TRUE; }
	virtual			BOOL Start(VECTOR2 vPos) { return TRUE; }
	virtual			BOOL Start(VECTOR2 vPos, DWORD dwOwner) { return TRUE; }
	virtual			BOOL Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner) { return TRUE; }

	virtual			void  ResetStatus() final;
	virtual			void  ResetAnim() final;
	virtual			int  AnimCountup() final;
	virtual			int  FlashCountup() final;
	virtual			BOOL isCollision(CBaseObj*) final;
	virtual			BOOL isCollisionClick(int nMx, int nMy) final;
	virtual			BOOL Hitcheck(CBaseObj*);

	virtual			VECTOR2 GetCenterPos();

	// 目的地へ向かう関数
	virtual			BOOL TargetMove( float speed);

	// アクセス関数
	BOOL	GetActive() { return m_bActive; }
	void	SetActive(BOOL bActive) { m_bActive = bActive; }
	DWORD	GetStatus() { return m_dwStatus; }
	CSprite* GetSprite() { return m_pSprite; }
	VECTOR2	GetPos() { return m_vPos; }				// 表示位置
	void    SetPos(VECTOR2 vPos) { m_vPos = vPos; }
	VECTOR2 GetPosUp() { return m_vPosUp; }
	void    SetPosUp(VECTOR2 vPosUp) { m_vPosUp = vPosUp; }
	VECTOR2 GetOf() { return m_vOf; }
	int		GetDirIdx() { return m_nDirIdx;	}
	int		GetMaxHp() { return m_nMaxHp; }
	int		GetHp() { return m_nHp; }
	void	SetHp(int nHp) { m_nHp = nHp; }
	int		GetMaxMp() { return m_nMaxMp; }
	int		GetMp() { return m_nMp; }
	void	SetMp(int nMp) { m_nMp = nMp; }
	int		GetAtc() { return m_nAtc; }
	int		GetItemFlag() { return m_nCheckItem; }	// アイテム取得アクセス関数

	// コンストラクタ（共通オブジェクトの初期化）
	CBaseObj(CGameMain* pGMain);
	virtual ~CBaseObj();
};

// --------------------------------------------------------------------------------------------------
// 基本プロシージャクラス
// --------------------------------------------------------------------------------------------------
class CBaseProc
{
protected:
	CGameMain*		m_pGMain;				// ゲームメインクラス

	// 
	// 下位のプロシージャポインタ配列は、プロシージャを階層構造にして管理するときに使用する
	// オブジェクトポインタ配列は、プロシージャの中にオブジェクトを配置して管理するときに使用する
	// 一般的には、どちらか片方のみを使用すること
	// 
	std::vector<CBaseProc*>  m_pProcArray;  // 下位のプロシージャポインタ配列
	std::vector<CBaseObj*>   m_pObjArray;	// オブジェクトポインタ配列

	int	m_nMaxwaitTime;		// 最大待ち時間
	int m_nWaitTime;		// 現在の待ち時間
	int	m_nStartFlag;		// 開始フラグ

public:
	// 全体更新処理（全てのプロシージャやオブジェクトを読み下して更新処理をする）
	virtual void	UpdateAll() final;

	// 更新処理（UpdateAll()の中で起動。プロシージャの更新処理）
	virtual void    Update() { ; }

	// あたり判定（全てのプロシージャやオブジェクトを読み下して処理をする）
	virtual BOOL	Hitcheck(CBaseObj*) final;

	// アクセス関数
	std::vector<CBaseProc*> GetProcArrayPtr() { return m_pProcArray; } // 下位のプロシージャポインタ配列のアドレスを返す
	std::vector<CBaseObj*>  GetObjArrayPtr() { return m_pObjArray; }  // オブジェクトポインタ配列のアドレスを返す

	int GetStartFlag(){ return m_nStartFlag;}
	void SetStartFlag(int nFlg) { m_nStartFlag = nFlg; }

	// コンストラクタ（基本プロシージャの初期化）
	CBaseProc(CGameMain* pGMain) {
		m_pGMain = pGMain;		// ゲームメインクラス
		m_nMaxwaitTime = 0;		// 最大待ち時間
		m_nWaitTime = 0;		// 現在の待ち時間
		m_nStartFlag = 0;		// 開始フラグ
	}
	// デストラクタ（プロシージャとオブジェクトの解放）
	virtual         ~CBaseProc();

};