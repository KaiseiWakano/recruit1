//------------------------------------------------------------------------
// �v���C���[�̏����w�b�_ Playchar.h
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

extern int m_nPcNum;

// �v���C�L�����N�^�[�@�I�u�W�F�N�g�N���X
class CPcObj : public CBaseObj
{
protected:
	int m_nFireballWait;
	int m_nBombWait;
	int checkDeath = 0;
public:
	void Update() override;
	// �R���X�g���N�^
	CPcObj(CGameMain* pGMain);
	~CPcObj();
};

// �v���C�L�����N�^�[�@�v���V�[�W���N���X
class CPcProc : public CBaseProc
{
public:
	// �A�N�Z�X�֐�
	CPcObj*			    GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }
	// �R���X�g���N�^
	CPcProc(CGameMain* pGMain);
	~CPcProc() { ; }
};
