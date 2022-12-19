//------------------------------------------------------------------------
// �A�C�e���̏����w�b�_ ItemManager.h
//------------------------------------------------------------------------

#pragma once

//�x����\��
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"
#include "BackFore.h"
#include "Enemy.h"

// �萔��`
#define	ITEMRESQUE		1	// �~�}��
#define	ITEMDOOR		2	// �h�A
#define	ITEMMAGIC		3	// ���@��
#define	ITEMBOX			4	// ��
#define ITEMGREATBOX	5	// �傫�ȕ�

#define ITEM_MAX 50

// �A�C�e���֘A�ǉ� K.Wakano

// �A�C�e���̎�ސ�
const int ITEM_TYPE_NUM = 10;

// ���A�x�̏d�݂̒萔
typedef enum
{
	COMMON = 100,
	UNCOMMON = 50,
	RARE = 10
}RARITY;

// �A�C�e���\����
typedef struct
{
	int num;			// �A�C�e���ԍ�
	char name[256];		// �A�C�e����
	int effect;			// ���ʂ̏����l
	int effectNum;		// ���ʂ̑���
	int stack;			// ������
	int weight;			// ���A�x�̏d��
}ITEM;

// �����܂�

static int cntArray;	// ItemObj�z��T���p

//======================================================================
// �A�C�e�� �I�u�W�F�N�g�N���X
class CItemObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2, DWORD) override;
	void Update() override;

	// �A�C�e���̃t���O���Z�b�g����֐�
	void SetItemGetFlag(int setResult);

	// �R���X�g���N�^
	CItemObj(CGameMain* pGMain);
	// �f�X�g���N�^
	~CItemObj();
};
//======================================================================
// �A�C�e�� �v���V�[�W���N���X
class CItemProc : public CBaseProc
{
protected:
	int m_nCheckItem;	// �A�C�e�����������Ă��邩�ۂ����f
public:
	// �A�N�Z�X�֐�
	CItemObj*			GetItemObjPtr() { return (CItemObj*)GetObjArrayPtr()[cntArray]; }
	// �A�C�e���̏�������̃A�N�Z�X�̊֐�
	int GetItemFlag() { return m_nCheckItem; }
	// �A�C�e���\���̂̃A�N�Z�X�֐�
	int getItemList(int itemNum, int pick);
	// ���ʋy�я����ʂ̃Z�b�^�[
	void setItemList(int itemNum, int pick, int setValue);

	void Update() override;
	// �R���X�g���N�^
	CItemProc(CGameMain* pGMain);
	// �f�X�g���N�^
	~CItemProc() { ; }

	// �t�@�C���ǂݍ���
	void ItemDataInput();
private:
	ITEM itemList[ITEM_TYPE_NUM];
};
//======================================================================
// �A�C�e���Ǘ� �v���V�[�W���N���X
class CItemManagementProc : public CBaseProc
{
public:
	CItemProc* m_pItemProc;

public:
	void  SetNonActive();

	// �R���X�g���N�^
	CItemManagementProc(CGameMain* pGMain);
	// �f�X�g���N�^
	~CItemManagementProc() { ; }
};
