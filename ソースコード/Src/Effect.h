//------------------------------------------------------------------------
// �f�����ʏ����w�b�_ Effect.h
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

#define  EFFECT_BOM_MAX  50

//======================================================================
// �f�����ʁE���� �I�u�W�F�N�g�N���X
class CEffectBomObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2) override;
	void Update() override;

	// �R���X�g���N�^
	CEffectBomObj(CGameMain* pGMain);
	// �f�X�g���N�^
	~CEffectBomObj();
};
//======================================================================
// �f�����ʁE���� �v���V�[�W���N���X
class CEffectBomProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2);

	// �R���X�g���N�^
	CEffectBomProc(CGameMain* pGMain);	
	// �f�X�g���N�^
	~CEffectBomProc() { ; }	
};
//======================================================================
// �f������ �v���V�[�W���N���X
class CEffectProc : public CBaseProc
{
public:
	CEffectBomProc* m_pEffectBomProc;

public:
	void  SetNonActive();

	// �R���X�g���N�^
	CEffectProc(CGameMain* pGMain);	
	// �f�X�g���N�^
	~CEffectProc() { ; }	
};
