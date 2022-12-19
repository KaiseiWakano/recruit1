//------------------------------------------------------------------------
// ����̏��� Weapon.cpp
//------------------------------------------------------------------------

#include "Weapon.h"
#include "Playchar.h"
#include "Enemy.h"
#include "BackFore.h"
#include "ItemManager.h"

// ���탁�C���v���V�[�W���̃R���X�g���N�^
CWeaponProc::CWeaponProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pWeaponShotProc = new CWeaponShotProc(m_pGMain);			// ����E�V���b�g�v���V�[�W��
	m_pProcArray.push_back(m_pWeaponShotProc);					// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponGrenadeProc = new CWeaponGrenadeProc(m_pGMain);	// ����E���^���e�v���V�[�W��
	m_pProcArray.push_back(m_pWeaponGrenadeProc);				// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponBossShotProc = new CWeaponBossShotProc(m_pGMain);	// �{�X����E�V���b�g�v���V�[�W��
	m_pProcArray.push_back(m_pWeaponBossShotProc);				// �v���V�[�W�����v���V�[�W���z��ɓo�^����
} 

// ����E�V���b�g�v���V�[�W���̃R���X�g���N�^
CWeaponShotProc::CWeaponShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// ����E�V���b�g
	for (DWORD i = 0; i < WEAPON_SHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponShotObj(m_pGMain));	// m_pObjArray�ɕ���E�V���b�g�I�u�W�F�N�g�𐶐�����
	}

	m_nWaitTime = m_nMaxwaitTime = 8;
}

// ����E�V���b�g�v���V�[�W���̊J�n
// VECTOR2 vPos			�����ʒu
// CBaseObj* pObj		���ˌ��̃I�u�W�F�N�g
// DWORD dwOwner		���ˌ��̃I�u�W�F�N�g�敪
// �߂�l ����ɔ���:TRUE �������Ȃ�����:FALSE
BOOL CWeaponShotProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, pObj, dwOwner);	// ����̔���
			m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}
	if (bRet) m_pGMain->m_pSeShot->Play();

	return bRet;
}

//------------------------------------------------------------------------
// ����E�V���b�g�I�u�W�F�N�g�̃R���X�g���N�^	
//
// ���� �Ȃ�
//------------------------------------------------------------------------
CWeaponShotObj::CWeaponShotObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 336, 96, 48, 48);
	m_nAnimNum = 6;

	m_nAtc = 100;	// ����U����
}
// ---------------------------------------------------------------------------
//
// ����E�V���b�g�I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CWeaponShotObj::~CWeaponShotObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// ����E�V���b�g�I�u�W�F�N�g�̊J�n
//
//   VECTOR2 vPos    �����ʒu
//   CBaseObj*   pObj    ���ˌ��̃I�u�W�F�N�g
//   DWORD       dwOwner ���ˌ��̃I�u�W�F�N�g�敪
//
//   �߂�l�@�@TRUE
//-----------------------------------------------------------------------------
BOOL CWeaponShotObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	float fSpeed = 8;	// �e�̑���
	// ���e�A�C�e���̏�������
	if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0)
	{
		// ���ɂ��U���͑��� + ���e�ɂ��{�[�i�X
		m_nAtc = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(1, 1) + 200;
	}
	else 
	{
		// ���ɂ��U���͑���
		m_nAtc = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(1, 1);
	}

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_dwOwner = dwOwner;

	m_vPos = vPos;	// �����ʒu
	m_vPosUp = VECTOR2(0, 0);

	// �v���C���[�̌����ɉ����čU��������ς���
	switch(pObj->GetDirIdx() )
	{
	case DOWN:
	case RIGHT:
		if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(5, 1) > 0) {
			m_pSprite->SetSrc(672, 96, 48, 48);
		}
		else {
			m_pSprite->SetSrc(336, 96, 48, 48);
		}
		m_vPosUp = VECTOR2(fSpeed,0);
		break;
	case LEFT:
	case LEFT + 1:
		if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(5, 1) > 0) {
			m_pSprite->SetSrc(672, 144, 48, 48);
		}
		else {
			m_pSprite->SetSrc(336, 144, 48, 48);
		}
		m_vPosUp = VECTOR2(-fSpeed, 0);
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����E�V���b�g�I�u�W�F�N�g�̍X�V
//
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CWeaponShotObj::Update()
{
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
			case  NORMAL:
				if (m_dwOwner == PC)	// PC�����˂����e
				{
					m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
				}
				else					// �G�����˂����e
				{ 
					m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
				}

				// ��ʊO�֏o�������
				if (m_vPos.x-m_pGMain->m_vScroll.x < 0 || m_vPos.x-m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
					m_vPos.y-m_pGMain->m_vScroll.y < 0 || m_vPos.y-m_pGMain->m_vScroll.y > WINDOW_HEIGHT) 
				{	
					m_bActive = FALSE;
				}
				break;

			case  DAMAGE:
				m_bActive = FALSE;
				break;
		}
		
		m_vPos += m_vPosUp;

		AnimCountup();
		Draw();
	}
}

//------------------------------------------------------------------------
// �{�X����E�V���b�g�v���V�[�W���̃R���X�g���N�^	
//------------------------------------------------------------------------
CWeaponBossShotProc::CWeaponBossShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// ����E�V���b�g
	for (DWORD i = 0; i < WEAPON_BOSSATTACK_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponBossShotObj(m_pGMain));	// m_pObjArray�ɕ���E�V���b�g�I�u�W�F�N�g�𐶐�����
	}

	m_nWaitTime = m_nMaxwaitTime = 8;
}

//-----------------------------------------------------------------------------
// �{�X����E�V���b�g�v���V�[�W���̊J�n
//
//   VECTOR2 vPos    �����ʒu
//   CBaseObj*   pObj    ���ˌ��̃I�u�W�F�N�g
//   DWORD       dwOwner ���ˌ��̃I�u�W�F�N�g�敪
//
//   �߂�l�@�@����ɔ��� : TRUE    �������Ȃ����� : FALSE
//-----------------------------------------------------------------------------
BOOL CWeaponBossShotProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, pObj, dwOwner);	// ����̔���
			//m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}
	//if (bRet) m_pGMain->m_pSeShot->Play();

	return bRet;
}

//------------------------------------------------------------------------
// �{�X����E�V���b�g�I�u�W�F�N�g�̃R���X�g���N�^	
//
// ���� �Ȃ�
//------------------------------------------------------------------------
CWeaponBossShotObj::CWeaponBossShotObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 672, 288, 48, 48);
	m_nAnimNum = 1;

	m_nAtc = 800;		// �U����
}
// ---------------------------------------------------------------------------
// �{�X����E�V���b�g�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponBossShotObj::~CWeaponBossShotObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// �{�X����E�V���b�g�I�u�W�F�N�g�̊J�n
//
//   VECTOR2 vPos    �����ʒu
//   CBaseObj*   pObj    ���ˌ��̃I�u�W�F�N�g
//   DWORD       dwOwner ���ˌ��̃I�u�W�F�N�g�敪
//
//   �߂�l�@�@TRUE
//-----------------------------------------------------------------------------
BOOL CWeaponBossShotObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	float fSpeed = 8;	// �e�̑���

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_dwOwner = dwOwner;

	// �łɂ��U���͒ጸ
	if (m_nAtc >= 0) 
	{
		m_nAtc = 800 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	m_vPos = vPos;	// �����ʒu
	m_vPosUp = VECTOR2(0, 0);

	switch (pObj->GetDirIdx())
	{
	case DOWN:
	case RIGHT:
		if (flagBio == false) {
			m_pSprite->SetSrc(672, 336, 48, 48);
		}
		else {
			m_pSprite->SetSrc(672, 384, 48, 48);
		}
		m_vPosUp = VECTOR2(fSpeed, 0);
		break;
	case LEFT:
	case LEFT + 1:
		if (flagBio == false) {
			m_pSprite->SetSrc(672, 288, 48, 48);
		}
		else {
			m_pSprite->SetSrc(672, 384, 48, 48);
		}
		m_vPosUp = VECTOR2(-fSpeed, 0);
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// �{�X����E�V���b�g�I�u�W�F�N�g�̍X�V
//
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CWeaponBossShotObj::Update()
{
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case  NORMAL:
			if (m_dwOwner == PC) // PC�����˂����e
			{
				m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
			}
			else				 //  �G�����˂����e
			{ 
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			}

			if (m_vPos.x - m_pGMain->m_vScroll.x < 0 || m_vPos.x - m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
				m_vPos.y - m_pGMain->m_vScroll.y < 0 || m_vPos.y - m_pGMain->m_vScroll.y > WINDOW_HEIGHT) {	// ��ʊO�֏o���̂ŏ���
				m_bActive = FALSE;
			}
			break;

		case  DAMAGE:
			m_bActive = FALSE;
			break;
		}

		m_vPos += m_vPosUp;

		AnimCountup();
		Draw();

	}
}

// =================================================================================

//------------------------------------------------------------------------
// ����E���^���e�v���V�[�W���̃R���X�g���N�^	
//
// �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//------------------------------------------------------------------------
CWeaponGrenadeProc::CWeaponGrenadeProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// ����E���^���e
	for (DWORD i = 0; i < WEAPON_GRENADE_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponGrenadeObj(m_pGMain));	// m_pObjArray�ɕ���E���^���e�I�u�W�F�N�g�𐶐�����
	}

	m_nWaitTime = m_nMaxwaitTime = 0;
}

//-----------------------------------------------------------------------------
// ����E���^���e�v���V�[�W���̊J�n
//
//   VECTOR2 vPos      �����ʒu
//   DWORD       dwOwner   ���ˌ��̃I�u�W�F�N�g�敪
//
//   �߂�l�@�@����ɔ��� : TRUE    �������Ȃ����� : FALSE
//-----------------------------------------------------------------------------
BOOL CWeaponGrenadeProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			((CWeaponGrenadeObj*)(m_pObjArray[i]))->Start(vPos, pObj, dwOwner);	// ����̔���
			bRet = TRUE;
			break;
		}
	}

	if (bRet) m_pGMain->m_pSeShot->Play();

	return bRet;
}


//------------------------------------------------------------------------
// ����E���^���e�I�u�W�F�N�g�̃R���X�g���N�^	
//
// ���� �Ȃ�
//------------------------------------------------------------------------
CWeaponGrenadeObj::CWeaponGrenadeObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 320, 0, 64, 64);
	//m_nAnimNum = 2;

	m_nAtc = 200;
}
// ---------------------------------------------------------------------------
// ����E���^���e�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponGrenadeObj::~CWeaponGrenadeObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// ����E���^���e�I�u�W�F�N�g�̊J�n
//
//   VECTOR2 vPos      �����ʒu
//   DWORD       dwOwner   ���ˌ��̃I�u�W�F�N�g�敪
//
//   �߂�l�@�@TRUE
//-----------------------------------------------------------------------------
BOOL CWeaponGrenadeObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	m_bActive = TRUE;
	//ResetStatus();
	//ResetAnim();
	//m_nAnimNum = 2;
	m_dwOwner = dwOwner;
	m_dwStatusPhase = 1;

	m_vPos = vPos;	// �����ʒu
	m_vOf = VECTOR2(0, 0);
	m_vTarget = VECTOR2(0, 0);
	m_vPosUp = VECTOR2(0, 0);

	if (m_dwOwner == PC)
	{
		// PC�����˂����e
		m_vTarget.x = m_vPos.x;
		m_vTarget.y = m_vPos.y / 2;	// ��ʓ��Ɏ��߂Ă��� ��������������}�b�v�̒[���Q�̋����ŕK����������
	}
	else {
		// �G���甭�˂����e
		m_vTarget.x = m_vPos.x;
		//m_vTarget.y = m_vPos.y + (WINDOW_HEIGHT - m_vPos.y) / 2;
	}

	m_pSprite->SetSrc(320, 0, 64, 64);
	m_fRotate = 0.0f;
	m_fScale = 1.0f;
	m_fAlpha = 0.5f;

	m_nHp = m_nMaxHp;

	m_nCnt1 = 0;
	m_nCnt2 = 0;

	return TRUE;
}
//-----------------------------------------------------------------------------
// ����E���^���e�I�u�W�F�N�g�̍X�V
//
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CWeaponGrenadeObj::Update()
{
	float fSpeed = 10;	// �e�̑���

	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case  NORMAL:
			switch (m_dwStatusPhase)
			{
			case 0:
				if (TargetMove(fSpeed))
				{   // �ړI�n�ivTarget�j�ւ̈ړ��������s���A�ړI�n�ɒB�����Ƃ�
					m_vPos.x -= ((64 - 32) / 2);   // �����X�v���C�g�ɕύX
					m_vPos.y -= ((64 - 14) / 2);   // �����X�v���C�g�ɕύX
					ResetAnim();
					m_pSprite->SetSrc(320, 0, 64, 64);
					m_fAlpha = 0.7f;
					m_fScale = 1.0f;
					//m_nAnimNum = 2;
					m_nHp = 0;
					m_pOtherObj = NULL;
					m_dwStatusPhase = 1;   // ����������
					m_pGMain->m_pSeShot->Play();
				}
				break;

			case 1:
				m_fScale += 0.15f;     // �������������Âg�債�Ă���
				if (m_fScale > 5) {
					m_bActive = false;
				}
				m_vOf.x = -(m_fScale * m_pSprite->GetSrcWidth() / 2);     // �g��ɍ��킹�ĕ\���ʒu���Â炵�Ă���
				m_vOf.y = -(m_fScale * m_pSprite->GetSrcHeight() / 2);
				break;
			}

			// �����̌v�Z
			m_vPos += m_vPosUp;

			if (m_dwOwner == PC) // PC�����˂����e
			{
				m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
			}
			else { //  �G�����˂����e
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			}

			// ��ʊO�֏o�������
			if (m_vPos.x - m_pGMain->m_vScroll.x < 0 || m_vPos.x - m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
				m_vPos.y - m_pGMain->m_vScroll.y < 0 || m_vPos.y - m_pGMain->m_vScroll.y > WINDOW_HEIGHT) 
			{	
				m_bActive = FALSE;
			}
			break;

		case  DAMAGE:
			m_dwStatus = NORMAL;
			break;
		}
		AnimCountup();
		Draw();
	}
}
