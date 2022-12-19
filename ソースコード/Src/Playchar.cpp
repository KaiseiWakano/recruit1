//------------------------------------------------------------------------
// �v���C���[�̏��� Playchar.cpp
//------------------------------------------------------------------------

#include "Playchar.h"
#include "Weapon.h"
#include "Effect.h"
#include "ItemManager.h"
#include "Map.h"
#include "BackFore.h"

int m_nPcNum = 2;

// PC�v���V�[�W���̃R���X�g���N�^
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pObjArray.push_back(new CPcObj(m_pGMain)); // m_pObjArray�ɃI�u�W�F�N�g��o�^����
}

// PC�I�u�W�F�N�g�̃R���X�g���N�^
CPcObj::CPcObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 672, 0, 48, 48); // �X�v���C�g�̓ǂݍ���
	m_vPos = VECTOR2(48, 721 );
	ResetStatus();
	ResetAnim();
	m_bActive = TRUE;
	m_nDirIdx = DOWN;	// ������ԁi�ҋ@���[�V�����j
	m_nHp = m_nMaxHp = 1000;
	m_nMaxMp = 1000;
	m_nMp = m_nMaxMp = 1000;	
	m_nMp  = m_nMaxMp;			
	m_nAtc = 0;			// �������Ă��_���[�W��^�����Ȃ�
	m_nAnimNum = 6;		// �L�����̃X�v���C�g��
	m_nPcNum = 2;		// �c�@��
	m_nFireballWait = 0;			// �΂̋ʂ̃N�[���^�C��
	m_nBombWait = 0;				// �����̃N�[���^�C��
}

// PC�I�u�W�F�N�g�̃f�X�g���N�^
CPcObj::~CPcObj()
{
	SAFE_DELETE(m_pSprite);
}

// PC�I�u�W�F�N�g�̍X�V
void CPcObj::Update()
{
	CDirectInput* pDI = m_pGMain->m_pDI;
	CMapLine*  pHitmapline = NULL;
	float fSpeed     = 5;
	float fJumpPlus  = 10;
	int nBlockDamage = 0;

	// �����ǉ�
	srand((unsigned int)time(NULL));
	int randomNum = 0;
	randomNum = rand() % 10;

	m_nPcNum = (2 + m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(8, 1)) - checkDeath;	// �c�@��

	m_nMaxHp = 1000 + (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(3, 1));		// HP����

	if (!m_bActive) return;

	m_vPosUp = VECTOR2(0, 0);

	switch (m_dwStatus)
	{
		case  FLASH:
				m_nCnt1--;
				if (m_nCnt1 <= 0) {
					ResetStatus();
					ResetAnim();
				}
				else {
					FlashCountup();
				}
				// break���������̂܂܏����𑱍s����
		case  NORMAL:
				m_nMp += 1; // Mp���R����
				if (m_nMp >= m_nMaxMp)m_nMp = m_nMaxMp;
				break;
		case DAMAGE:
				if (m_pOtherObj->GetAtc() > 0)
				{
					// ���m���ōU�������
					if (m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(9, 1) < randomNum)
					{
						nBlockDamage = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(4, 1);
						// �U�����󂯂��_���[�W ���̃X�^�b�N�ɉ�����50���y��
						if (m_pOtherObj->GetAtc() >= nBlockDamage)
						{
							// �_���[�W���u���b�N������Ƃ��̂�Hp�����Z
							m_nHp -= m_pOtherObj->GetAtc() - nBlockDamage;
						}
					}
					if (m_nHp <= 0)
					{
						m_nHp = 0;
						m_dwStatus = DEAD;		// HP��0�Ȃ̂Ŏ��S��
						m_nCnt1 = 180;			// ���S�t���b�V�����̎��Ԑݒ�
					}
					else 
					{
						m_dwStatus = FLASH;
						m_nCnt1 = 60;
					}
				}
				else {
					m_dwStatus = NORMAL;  // �U���͂Ȃ��̂Ƃ��͂�����NORMAL�ɖ߂�
				}
				break;
		case DEAD:
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_nPcNum -= 1;
					checkDeath++;
					if (m_nPcNum <= 0)	// PC���Ȃ��Ȃ����̂ŃQ�[���I�[�o�[
					{
						m_pGMain->m_dwGameStatus = GAMEOVER;
					}
					else {
						m_nHp = m_nMaxHp;
						ResetStatus();
						ResetAnim();
					}
				}
				else {
					FlashCountup();
				}
				break;
	}

	switch (m_dwStatusSub)
	{
		// ���s���̏���
		case WALK:		
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))	// ���L�[
			{
				m_vPosUp.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))		// ���L�[
			{
				m_vPosUp.x = -fSpeed;
				m_nDirIdx = LEFT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_UP) || pDI->CheckJoy(KD_DAT, DIJ_UP))			// ���L�[
			{	
				// �W�����v�J�n
				m_dwStatusSub  = JUMP;
				m_vJumpSpeed.x = m_vPosUp.x;
				m_vJumpSpeed.y = -(fSpeed*2.5 + fJumpPlus);
				m_fJumpTime    = 0;
				m_vPosUp.y     = m_vJumpSpeed.y;
				m_nCnt2 = 0;	// ��i�W�����v�p�ϐ�
			}
			else 
			{
				// ���R����
				if (pDI->CheckKey(KD_DAT, DIK_DOWN) || pDI->CheckJoy(KD_DAT, DIJ_DOWN))	// ���L�[
				{
					m_vPosUp.y = fSpeed;
					m_nDirIdx = DOWN;
				}
				else {
					m_dwStatusSub = JUMP;
					m_vJumpSpeed.x = m_vPosUp.x;
					m_vJumpSpeed.y = fSpeed / 2;
					m_fJumpTime = 0;
					m_vPosUp.y = m_vJumpSpeed.y;
				}
			}

			// �ҋ@���[�V��������
			if (!pDI->CheckKey(KD_DAT, DIK_RIGHT) && !pDI->CheckKey(KD_DAT, DIK_LEFT)) {
				if (m_nDirIdx == LEFT) {
					m_nDirIdx = LEFT + 1;
				}
				else if (m_nDirIdx == RIGHT) {
					m_nDirIdx = DOWN;
				}
			}
			break;

			// �W�����v���̏���
		case JUMP:		
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))	// ���L�[
			{
				m_vJumpSpeed.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))		// ���L�[
			{
				m_vJumpSpeed.x = -fSpeed;
				m_nDirIdx = LEFT;
			}

			// ���i�W�����v����
			if ((pDI->CheckKey(KD_TRG, DIK_UP) || pDI->CheckJoy(KD_TRG, DIJ_UP)) &&
				m_nCnt2 <= m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(0, 1)) // ���L�[
			{
				m_nCnt2++;
				m_vJumpSpeed.y = -(fSpeed*2.5 + fJumpPlus);
				m_vPosUp.y = m_vJumpSpeed.y;
				m_fJumpTime = 0;
				m_dwStatusSub = JUMP;
			}
			m_fJumpTime++;
			m_vPosUp.x = m_vJumpSpeed.x;
			m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY*m_fJumpTime);
			break;
	}

	// �΂̋ʂ̏���
	if (pDI->CheckKey(KD_DAT, DIK_Z) || pDI->CheckJoy(KD_DAT, DIJ_A))
	{
		if (m_nFireballWait <= 0 && m_nMp >= 50)	// wait���Ȃ��AMp��100�ȏ�̂Ƃ��̂ݔ��� 
		{
			// �V���b�g�̔���
			m_pGMain->m_pWeaponProc->m_pWeaponShotProc->Start(m_vPos+VECTOR2(16,4), this, PC);
			m_nFireballWait = 10;			// ���픭�˂̃E�F�C�g���|����
			m_nMp -= 50;				
			if (m_nMp < 0)m_nMp = 0;	
		}
	}
	if (m_nFireballWait > 0) m_nFireballWait--;    // ���픭�˂̃E�F�C�g���J�E���g�_�E��

	// �{���̔��ˏ���
	if (pDI->CheckKey(KD_DAT, DIK_X) || pDI->CheckJoy(KD_DAT, DIJ_X))
	{
		if (m_nBombWait <= 0 && m_nMp >= 200)
		{
			// �V���b�g�̔���
			m_pGMain->m_pWeaponProc->m_pWeaponGrenadeProc->Start(m_vPos + VECTOR2(16, 16), this, PC);
			m_nBombWait = 50;    // ���픭�˂̃E�F�C�g���|����
			m_nMp -= 200;
			if (m_nMp < 0)m_nMp = 0;
		}
	}
	if (m_nBombWait > 0) m_nBombWait--;    // ���픭�˂̃E�F�C�g���J�E���g�_�E��

	// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
	if (m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline))
	{
		if (pHitmapline && pHitmapline->m_vNormal.y < 0)	// �ڒn���Ă��Ȃ��ƃW�����v�ł��Ȃ�
		{
			m_dwStatusSub = WALK;	// �}�b�v���ɐڐG���Ă���Ƃ��́A�W�����v����s�ɖ߂�
			m_fJumpTime = 0;
			m_vJumpSpeed.x = m_vJumpSpeed.y = 0;
		}
	}

	// �����v�Z
	m_vPos += m_vPosUp;

	AnimCountup();
	Draw();
}