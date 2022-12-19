//------------------------------------------------------------------------
// �G�̏����w�b�_ Enemy.h
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

#define  ENM_WATER_MAX  30
#define  ENM_TORD_MAX  20
#define  ENM_WOLF_MAX  20
#define  ENM_FOX_MAX  20
#define  ENM_BOSS_MAX  20
#define  ENM_LASTBOSS_MAX 20

extern bool flagBossDeath;
extern bool flagBio;

//======================================================================
// ��̐��̓G�@�I�u�W�F�N�g�N���X
class CEnmWaterObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// �R���X�g���N�^
	CEnmWaterObj(CGameMain* pGMain);
	~CEnmWaterObj();
};
//======================================================================
// ��̐��̓G�@�v���V�[�W���N���X
class CEnmWaterProc : public CBaseProc
{
public:
	void   Update() override;

	// �R���X�g���N�^
	CEnmWaterProc(CGameMain* pGMain);
	~CEnmWaterProc() { ; }
};
//======================================================================
// ���܂̓G�@�I�u�W�F�N�g�N���X
class CEnmTordObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// �R���X�g���N�^
	CEnmTordObj(CGameMain* pGMain);
	~CEnmTordObj();
};
//======================================================================
// ���܂̓G�@�v���V�[�W���N���X
class CEnmTordProc : public CBaseProc
{
public:
	void   Update() override;

	// �R���X�g���N�^
	CEnmTordProc(CGameMain* pGMain);
	~CEnmTordProc() { ; }
};
//======================================================================
// �I�I�J�~�̓G�@�I�u�W�F�N�g�N���X
class CEnmWolfObj : public CBaseObj
{
public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// �R���X�g���N�^
	CEnmWolfObj(CGameMain* pGMain);
	~CEnmWolfObj();
};
//======================================================================
// �I�I�J�~�̓G�@�v���V�[�W���N���X
class CEnmWolfProc : public CBaseProc
{
public:
	void   Update() override;

	// �R���X�g���N�^
	CEnmWolfProc(CGameMain* pGMain);
	~CEnmWolfProc() { ; }
};
//======================================================================
// ���˂̓G�@�I�u�W�F�N�g�N���X
class CEnmFoxObj : public CBaseObj
{
protected:
	int m_nFoxShotWait = 200;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// �R���X�g���N�^
	CEnmFoxObj(CGameMain* pGMain);
	~CEnmFoxObj();
};
//======================================================================
// ���˂̓G�@�v���V�[�W���N���X
class CEnmFoxProc : public CBaseProc
{
public:
	void   Update() override;

	// �R���X�g���N�^
	CEnmFoxProc(CGameMain* pGMain);
	~CEnmFoxProc() { ; }
};
//======================================================================
// �{�X�̓G�@�I�u�W�F�N�g�N���X
class CEnmBossObj : public CBaseObj
{
protected:
	int m_nBossShotWait = 200;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// �R���X�g���N�^
	CEnmBossObj(CGameMain* pGMain);
	~CEnmBossObj();
};
//======================================================================
// �{�X�̓G�@�v���V�[�W���N���X
class CEnmBossProc : public CBaseProc
{
public:
	void Update() override;

	// �R���X�g���N�^
	CEnmBossProc(CGameMain* pGMain);
	~CEnmBossProc() { ; }
};
//======================================================================
// ���X�{�X�̓G �I�u�W�F�N�g�N���X
class CEnmLastBossObj : public CBaseObj
{
protected:
	int m_nLastBossShotWait = 200;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	// �R���X�g���N�^
	CEnmLastBossObj(CGameMain* pGMain);
	~CEnmLastBossObj();
};
//======================================================================
// ���X�{�X�̓G �v���V�[�W���N���X
class CEnmLastBossProc : public CBaseProc
{
public:
	void Update() override;

	// �R���X�g���N�^
	CEnmLastBossProc(CGameMain* pGMain);
	~CEnmLastBossProc() { ; }
};

//======================================================================
// �G�L�����N�^�[�S�̂̃��C���v���V�[�W���N���X
//======================================================================
class CEnmProc : public CBaseProc
{
public:
	CEnmWaterProc*	m_pEnmWaterProc;
	CEnmTordProc*	m_pEnmTordProc;
	CEnmWolfProc*	m_pEnmWolfProc;
	CEnmFoxProc*	m_pEnmFoxProc;	
	CEnmBossProc*   m_pEnmBossProc;
	CEnmLastBossProc*   m_pEnmLastBossProc;

public:
	void  SetNonActive();
	CEnmProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CEnmProc() { ; }
};
