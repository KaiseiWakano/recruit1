//------------------------------------------------------------------------
// ����̏��� Weapon.h
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

#define  WEAPON_SHOT_MAX  200
#define  WEAPON_GRENADE_MAX 50
#define  WEAPON_BOSSATTACK_MAX 200

//======================================================================
// ����E�V���b�g �I�u�W�F�N�g�N���X
class CWeaponShotObj : public CBaseObj
{
protected:
	DWORD m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	// �R���X�g���N�^
	CWeaponShotObj(CGameMain* pGMain);
	// �f�X�g���N�^
	~CWeaponShotObj();
};

//======================================================================
// ����E�V���b�g �v���V�[�W���N���X
class CWeaponShotProc : public CBaseProc
{
public:
	BOOL Start( VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	// �R���X�g���N�^
	CWeaponShotProc(CGameMain* pGMain);	
	// �f�X�g���N�^
	~CWeaponShotProc() { ; }
};

//======================================================================
// ����E���^���e �I�u�W�F�N�g�N���X
class CWeaponGrenadeObj : public CBaseObj
{
protected:
	DWORD  m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);
	void Update() override;

	// �R���X�g���N�^
	CWeaponGrenadeObj(CGameMain* pGMain);
	// �f�X�g���N�^
	~CWeaponGrenadeObj();
};

//======================================================================
// ����E���^���e �v���V�[�W���N���X
class CWeaponGrenadeProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	// �R���X�g���N�^
	CWeaponGrenadeProc(CGameMain* pGMain);	
	// �f�X�g���N�^
	~CWeaponGrenadeProc() { ; }
};

//======================================================================
// ���X�{�X�̍U�� �I�u�W�F�N�g�N���X
class CWeaponBossShotObj : public CBaseObj
{
protected:
	DWORD m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	// �R���X�g���N�^
	CWeaponBossShotObj(CGameMain* pGMain);
	// �f�X�g���N�^
	~CWeaponBossShotObj();
};

//======================================================================
// ���X�{�X�̍U�� �v���V�[�W���N���X
class CWeaponBossShotProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	// �R���X�g���N�^
	CWeaponBossShotProc(CGameMain* pGMain);	
	// �f�X�g���N�^
	~CWeaponBossShotProc() { ; }

};

//======================================================================
// ����S�̂̃��C���v���V�[�W���N���X
//======================================================================
class CWeaponProc : public CBaseProc
{
public:
	CWeaponShotProc*	m_pWeaponShotProc;
	CWeaponGrenadeProc* m_pWeaponGrenadeProc;
	CWeaponBossShotProc* m_pWeaponBossShotProc;

public:

	CWeaponProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CWeaponProc() { ; }			// �f�X�g���N�^
};
