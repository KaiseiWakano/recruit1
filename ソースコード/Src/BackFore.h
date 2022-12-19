//------------------------------------------------------------------------
// �O�i�E��i�����w�b�_ BackFore.h
//------------------------------------------------------------------------

#pragma once

//�x����\��
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

// �w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

#define BOX_ITEM_MAX 10
#define TIME_LIMIT 180

//======================================================================
// �o�b�N�N���E���h�@�v���V�[�W���N���X
//======================================================================
class CBackProc
{
protected:
	CGameMain*		m_pGMain;
	CSpriteImage*	m_pImageBk;
	CSprite*		m_pBackgrd;
	VECTOR2			m_vScroll;
};
//======================================================================
// �t�H�A�N���E���h�@�v���V�[�W���N���X
//======================================================================
class CForeProc
{
private:
	int canOpen;

protected:
	CGameMain*		m_pGMain;
	CSprite*		m_pForegrd;

public:
	void Update();
	// �A�C�e���\���A�Ǘ��֐�
	void ShowAndSetItem(int num, int isGetItem[], int stack, int cntItem[]);
	int getCanOpen() { return canOpen; }

	// �R���X�g���N�^
	CForeProc(CGameMain* pGMain);
	// �f�X�g���N�^
	~CForeProc();
};
