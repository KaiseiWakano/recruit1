//------------------------------------------------------------------------
// �G�̏��� Enemy.cpp
//------------------------------------------------------------------------

#include "Enemy.h"
#include "Weapon.h"
#include "Effect.h"
#include "ItemManager.h"
#include "Playchar.h"
#include "Map.h"

bool flagBossDeath = false;
bool flagBio = false;

//------------------------------------------------------------------------
// �G���C���v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pEnmWaterProc = new CEnmWaterProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmWaterProc);

	m_pEnmTordProc = new CEnmTordProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmTordProc);

	m_pEnmWolfProc = new CEnmWolfProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmWolfProc);

	m_pEnmFoxProc = new CEnmFoxProc(m_pGMain);												
	m_pProcArray.push_back(m_pEnmFoxProc);    

	m_pEnmBossProc = new CEnmBossProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmBossProc);

	m_pEnmLastBossProc = new CEnmLastBossProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmLastBossProc);
}

//============================================================================
// �G�v���V�[�W���ƃI�u�W�F�N�g��T�����S�ẴI�u�W�F�N�g���m���A�N�e�B�u�ɂ���
// �G�v���V�[�W���̊J�n�t���O�����Z�b�g����
//============================================================================
void  CEnmProc::SetNonActive()
{
	// �G�̃I�u�W�F�N�g�|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}

	// ���ʂ̃v���V�[�W���|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag( 0 );				// �J�n�t���O�̃��Z�b�g
		((CEnmProc*)m_pProcArray[i])->SetNonActive();   // ���ʂ̃v���V�[�W���̏����i�ċA�����j
	}
}

// =====================================================================================================
// 
// �e�G�v���V�[�W�����̏���
// 
// =====================================================================================================

//------------------------------------------------------------------------
// ��̐��̓G�v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEnmWaterProc::CEnmWaterProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_WATER_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmWaterObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ��̐��̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmWaterProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�@ ( EvtID:3�@�G�̏o���ʒu,     �@EvtNo:0x01	��̐��̓G  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x01, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}

		m_nStartFlag = 1;  // ��񔭂�������A�����������Ȃ�
	}
}

//------------------------------------------------------------------------
//	��̐��̓G�I�u�W�F�N�g�̃R���X�g���N�^
//  ���� �Ȃ�
//------------------------------------------------------------------------
CEnmWaterObj::CEnmWaterObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 384, 192, 48, 48);
	m_fAlpha = 0.8f;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 10000;
	m_nAtc = 200;
	m_nAnimNum = 1;
}
// ---------------------------------------------------------------------------
// ��̐��̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmWaterObj::~CEnmWaterObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ��̐��̓G�I�u�W�F�N�g�̊J�n
// ���� �Ȃ�
//-----------------------------------------------------------------------------
BOOL CEnmWaterObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = RIGHT;
	m_vPos = vPos;	// �����ʒu
	m_vPosUp = VECTOR2(0, 0);
	m_nHp = m_nMaxHp;

	return TRUE;
}
//-----------------------------------------------------------------------------
// ��̐��̓G�I�u�W�F�N�g�̍X�V
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CEnmWaterObj::Update()
{
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case NORMAL:
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			// ����������Q�[���I�[�o�[
			m_pGMain->m_dwGameStatus = GAMEOVER;
			m_dwStatus = NORMAL;
			break;

		case DEAD:
			m_dwStatus = NORMAL;
			break;
		}
		AnimCountup();
		Draw();
	}
}

//------------------------------------------------------------------------
//	���܂̓G�v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEnmTordProc::CEnmTordProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_TORD_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmTordObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ���܂̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmTordProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while ( nNext != -1 )
		{
			// �C�x���g�}�b�v (EvtID:3 �G�̏o���ʒu, EvtNo:0x02 ���܂̓G)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x02, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // ��񔭂�������A�����������Ȃ�
	}
}

//------------------------------------------------------------------------
//	���܂̓G�I�u�W�F�N�g�̃R���X�g���N�^
//  ���� �Ȃ�
//------------------------------------------------------------------------
CEnmTordObj::CEnmTordObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 480+Random(0,1)*96, 192, 48, 48);
	m_vPos = VECTOR2(0,0);
	m_nHp = m_nMaxHp = 400;
	m_nAtc = 300;
	m_nAnimNum = 2;
}
// ---------------------------------------------------------------------------
// ���܂̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmTordObj::~CEnmTordObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ���܂̓G�I�u�W�F�N�g�̊J�n
// ���� �Ȃ�
//-----------------------------------------------------------------------------
BOOL CEnmTordObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// �����ʒu

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 300;

	return TRUE;
}
//-----------------------------------------------------------------------------
// ���܂̓G�I�u�W�F�N�g�̍X�V
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CEnmTordObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// �łɂ��U���͒ጸ
	if (m_nAtc >= 0)
	{
		m_nAtc = 300 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0,0);

		switch (m_dwStatus)
		{
			case  FLASH:
					m_nCnt1--;
					if (m_nCnt1 <= 0) 
					{
						ResetStatus();
						ResetAnim();
					}
					else 
					{
						FlashCountup();
					}
			case  NORMAL:
					switch (m_dwStatusSub)
					{
					case  WALK:
						// ���R�ړ��̏���
						if (m_nDirIdx == RIGHT)
						{
							m_vPosUp.x = fSpeed;
						}
						else {
							m_vPosUp.x = -fSpeed;
						}
						
						// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
						bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
						if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
						{
							// �}�b�v���̒[�ɗ����̂ō��E���]
							if (m_nDirIdx == RIGHT)
							{
								m_nDirIdx = LEFT;
								m_vPosUp.x = -fSpeed;
								m_vPosUp.y = 0;
							}
							else {
								m_nDirIdx = RIGHT;
								m_vPosUp.x = fSpeed;
								m_vPosUp.y = 0;
							}
						}
						break;
					}
					// ����
					m_vPos += m_vPosUp;
					// �o�b�Ƃ̂����蔻��
					m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
					break;

			case  DAMAGE:
					m_nHp -= m_pOtherObj->GetAtc();
					if (m_nHp <= 0) {
						m_dwStatus = DEAD;
						m_nCnt1 = 180;
					}
					else {
						m_dwStatus = FLASH;
						m_nCnt1 = 60;
					}
					// �����G�t�F�N�g�\��
					if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0) {
						m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
					}
					m_pGMain->m_pSeHit->Play();
					break;

			case  DEAD:
					m_nCnt1--;
					if (m_nCnt1 <= 0)
					{
						m_pGMain->m_nScore += 10;
						m_bActive = FALSE;
					}
					else {
						FlashCountup();
					}
					break;
		}

		AnimCountup();
		Draw();

	}
}

// ===================================================================================================
//------------------------------------------------------------------------
//	�ς̓G�v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEnmFoxProc::CEnmFoxProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_FOX_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmFoxObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
//  �ς̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmFoxProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T�� ( EvtID:3 �G�̏o���ʒu, EvtNo:0x08 �ς̓G)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x08, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // ��񔭂�������A�����������Ȃ�
	}
}

//------------------------------------------------------------------------
//	�ς̓G�I�u�W�F�N�g�̃R���X�g���N�^
//  ���� �Ȃ�
//------------------------------------------------------------------------
CEnmFoxObj::CEnmFoxObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 192, 192, 48, 48);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 450;
	m_nAtc = 200;
	m_nAnimNum = 2;
}
// ---------------------------------------------------------------------------
//  �ς̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmFoxObj::~CEnmFoxObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// �ς̓G�I�u�W�F�N�g�̊J�n
// ���� �Ȃ�
//-----------------------------------------------------------------------------
BOOL CEnmFoxObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = RIGHT;
	m_vPos = vPos;	// �����ʒu

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 200;

	return TRUE;
}
//-----------------------------------------------------------------------------
// �ς̓G�I�u�W�F�N�g�̍X�V
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CEnmFoxObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	if (m_nAtc >= 0)
	{
		m_nAtc = 200 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}
		case NORMAL:
			switch (m_dwStatusSub)
			{
			case  WALK:
				// ���R�ړ��̏���
				if (m_nDirIdx == RIGHT)
				{
					m_vPosUp.x = fSpeed;
				}
				else 
				{
					m_vPosUp.x = -fSpeed;
				}

				// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// �}�b�v���̒[�ɗ����̂ō��E���]
					if (m_nDirIdx == RIGHT)
					{
						m_nDirIdx = LEFT;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = RIGHT;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// ����
			m_vPos += m_vPosUp;
			// �o�b�Ƃ̂����蔻��
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;			
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;			
			}
			// �����G�t�F�N�g�\��
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0)
			{
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				m_pGMain->m_nScore += 20;
				m_bActive = FALSE;
			}
			else {
				FlashCountup();
			}
			break;
		}

		if (m_nFoxShotWait <= 0)	// wait���Ȃ����̂ݔ���
		{
			// �V���b�g�̔���
			m_pGMain->m_pWeaponProc->m_pWeaponShotProc->Start(m_vPos + VECTOR2(16, 16), this, ENM);
			m_nFoxShotWait = 200;    // ���픭�˂̃E�F�C�g���|����
		}
		if (m_nFoxShotWait > 0) m_nFoxShotWait--;    // ���픭�˂̃E�F�C�g���J�E���g�_�E��

		AnimCountup();
		Draw();
	}
}

// ===================================================================================================
//------------------------------------------------------------------------
//	�I�I�J�~�̓G�v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEnmWolfProc::CEnmWolfProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_TORD_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmWolfObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// �I�I�J�~�̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmWolfProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ (EvtID:3 �G�̏o���ʒu, EvtNo:0x04 �I�I�J�~�̓G)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x04, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // ��񔭐���������A�����������Ȃ�
	}
}

//------------------------------------------------------------------------
//	�I�I�J�~�̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//  ���� �Ȃ�
//------------------------------------------------------------------------
CEnmWolfObj::CEnmWolfObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 96, 192, 48, 48);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 650;
	m_nAtc = 400;
	m_nAnimNum = 2;
}
// ---------------------------------------------------------------------------
// �I�I�J�~�̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmWolfObj::~CEnmWolfObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// �I�I�J�~�̓G�I�u�W�F�N�g�̊J�n
// ���� �Ȃ�
//-----------------------------------------------------------------------------
BOOL CEnmWolfObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// �����ʒu

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 400;

	return TRUE;
}
//-----------------------------------------------------------------------------
// �I�I�J�~�̓G�I�u�W�F�N�g�̍X�V
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CEnmWolfObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// �łɂ��U���͒ጸ
	if (m_nAtc >= 0)
	{
		m_nAtc = 400 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}		
		case NORMAL:
			switch (m_dwStatusSub)
			{
			case WALK:
				// ���R�ړ��̏���
				if (m_nDirIdx == RIGHT)
				{
					m_vPosUp.x = fSpeed;
				}
				else {
					m_vPosUp.x = -fSpeed;
				}

				// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// �}�b�v���̒[�ɗ����̂ō��E���]
					if (m_nDirIdx == RIGHT)
					{
						m_nDirIdx = LEFT;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = RIGHT;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// ����
			m_vPos += m_vPosUp;
			// �o�b�Ƃ̂����蔻��
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;			
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;		
			}
			// �����G�t�F�N�g�\��
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0) {
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				m_pGMain->m_nScore += 20;
				m_bActive = FALSE;
			}
			else {
				FlashCountup();
			}
			break;
		}
		AnimCountup();
		Draw();
	}
}

// ===================================================================================================
//------------------------------------------------------------------------
//	�{�X�̓G�v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEnmBossProc::CEnmBossProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_BOSS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmBossObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// �{�X�̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmBossProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:3�@�G�̏o���ʒu, EvtNo:0x200 �{�X�̓G )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x200, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // ��񔭐���������A�����������Ȃ�
	}
}

//------------------------------------------------------------------------
//	�{�X�̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//  ���� �Ȃ�
//------------------------------------------------------------------------
CEnmBossObj::CEnmBossObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 1152, 0, 144, 96);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = 3000;
	m_nAtc = 500;
	m_nAnimNum = 4;
}
// ---------------------------------------------------------------------------
// �{�X�̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmBossObj::~CEnmBossObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// �{�X�̓G�I�u�W�F�N�g�̊J�n
// ���� �Ȃ�
//-----------------------------------------------------------------------------
BOOL CEnmBossObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = RIGHT;
	m_vPos = vPos;	// �����ʒu

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 500;

	return TRUE;
}

//-----------------------------------------------------------------------------
// �{�X�̓G�I�u�W�F�N�g�̍X�V
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CEnmBossObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// �łɂ��U���͒ጸ
	if (m_nAtc >= 0)
	{
		m_nAtc = 500 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}		
		case NORMAL:
			switch (m_dwStatusSub)
			{
			case WALK:
				// ���R�ړ��̏���
				if (m_nDirIdx == RIGHT)
				{
					m_vPosUp.x = fSpeed;
				}
				else {
					m_vPosUp.x = -fSpeed;
				}

				// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// �}�b�v���̒[�ɗ����̂ō��E���]
					if (m_nDirIdx == RIGHT)
					{
						m_nDirIdx = LEFT;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = RIGHT;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// ����
			m_vPos += m_vPosUp;
			// PC�Ƃ̂����蔻��
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;
			}
			// �����G�t�F�N�g�\��
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0)
			{
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				flagBossDeath = true;	// ���S����
				m_pGMain->m_nScore += 100;
				m_bActive = FALSE;
			}
			else 
			{
				FlashCountup();
			}
			break;
		}

		if (m_nBossShotWait <= 0)	// wait���Ȃ��̂ݔ���
		{
			// �V���b�g�̔���
			m_pGMain->m_pWeaponProc->m_pWeaponBossShotProc->Start(m_vPos + VECTOR2(16, 16), this, ENM);
			m_nBossShotWait = 30;    // ���픭�˂̃E�F�C�g���|����
		}
		if (m_nBossShotWait > 0) m_nBossShotWait--;    // ���픭�˂̃E�F�C�g���J�E���g�_�E��

		AnimCountup();
		Draw();
	}
}

// ===================================================================================================
//------------------------------------------------------------------------
// ���X�{�X�̓G�v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEnmLastBossProc::CEnmLastBossProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_LASTBOSS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmLastBossObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ���X�{�X�̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmLastBossProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ (EvtID:3 �G�̏o���ʒu, EvtNo:0x400 ���X�{�X�̓G)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x400, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // ��񔭂�������A�����������Ȃ�
	}
}

//------------------------------------------------------------------------
//	���X�{�X�̓G�I�u�W�F�N�g�̃R���X�g���N�^
//  ���� �Ȃ�
//------------------------------------------------------------------------
CEnmLastBossObj::CEnmLastBossObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 384, 480, 240, 192);
	m_vPos = VECTOR2(0, 0);
	// ���X�{�X��HP
	m_nHp = m_nMaxHp = 6000;
	m_nAtc = 500;
	m_nAnimNum = 6;
}
// ---------------------------------------------------------------------------
// ���X�{�X�̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmLastBossObj::~CEnmLastBossObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ���X�{�X�̓G�I�u�W�F�N�g�̊J�n
// ���� �Ȃ�
//-----------------------------------------------------------------------------
BOOL CEnmLastBossObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = DOWN;
	m_vPos = vPos;	// �����ʒu
	flagBio = true;
	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	m_nAtc = 500;

	return TRUE;
}
//-----------------------------------------------------------------------------
// ���X�{�X�̓G�I�u�W�F�N�g�̍X�V
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CEnmLastBossObj::Update()
{
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	// �łɂ��U���͒ጸ
	if (m_nAtc >= 0)
	{
		m_nAtc = 500 - m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(6, 1);
	}

	if (m_bActive)
	{
		m_vPosUp = VECTOR2(0, 0);

		switch (m_dwStatus)
		{
		case  FLASH:
			m_nCnt1--;
			if (m_nCnt1 <= 0) 
			{
				ResetStatus();
				ResetAnim();
			}
			else 
			{
				FlashCountup();
			}
		case  NORMAL:
			switch (m_dwStatusSub)
			{
			case  WALK:
				// ���R�ړ��̏���...�ړ����Ȃ� K.Wakano
				/*
				if (m_nDirIdx == UP)
				{
					//m_vPosUp.x = fSpeed;
				}
				else {
					//m_vPosUp.x = -fSpeed;
				}
				*/

				// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					// �}�b�v���̒[�ɗ����̂ō��E���]
					if (m_nDirIdx == UP)
					{
						m_nDirIdx = DOWN;
						m_vPosUp.x = -fSpeed;
						m_vPosUp.y = 0;
					}
					else {
						m_nDirIdx = UP;
						m_vPosUp.x = fSpeed;
						m_vPosUp.y = 0;
					}
				}
				break;
			}
			// ����
			m_vPos += m_vPosUp;
			// �o�b�Ƃ̂����蔻��
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case  DAMAGE:
			m_nHp -= m_pOtherObj->GetAtc();
			if (m_nHp <= 0) {
				m_dwStatus = DEAD;
				m_nCnt1 = 90;
			}
			else {
				m_dwStatus = FLASH;
				m_nCnt1 = 15;
			}
			// �����G�t�F�N�g�\��
			if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(7, 1) > 0) {
				m_pGMain->m_pEffectProc->m_pEffectBomProc->Start(m_vPos);
			}
			m_pGMain->m_pSeHit->Play();
			break;

		case  DEAD:
			m_nCnt1--;
			if (m_nCnt1 <= 0)
			{
				flagBossDeath = true;	// ���S����
				m_pGMain->m_nScore += 200;
				m_bActive = FALSE;
			}
			else {
				FlashCountup();
			}
			break;
		}

		// �U������ K.Wakano
		if (m_nLastBossShotWait <= 0)
		{
			m_nAnimNum = 4;
			if (m_nDirIdx == UP)
			{
				m_nDirIdx = RIGHT;
			}
			else 
			{
				m_nDirIdx = LEFT;
			}
			// �e�̔���
			for (int i = 0; i < 10 - Random(0, 3); i++)			// �e�̗񂪊m���Ō�����悤��
			{
				m_pGMain->m_pWeaponProc->m_pWeaponBossShotProc->Start(m_vPos + VECTOR2(16, 0 + i * 15), this, ENM);
			}
			m_nLastBossShotWait = 50 * Random(1,2);				// �{�X�̍U�����o�������_����
		}
		if (m_nLastBossShotWait > 0) m_nLastBossShotWait--;		// ���픭�˂̃E�F�C�g���J�E���g�_�E��

		AnimCountup();
		Draw();

	}
}
