//------------------------------------------------------------------------
// �O�i�E��i���� BackFore.cpp
//------------------------------------------------------------------------

#include "GameMain.h"
#include "Playchar.h"
#include "BackFore.h"
#include "Effect.h"
#include "Map.h"
#include "ItemManager.h"

// ========================================================================================
// �O�i�̏���
// ========================================================================================
//------------------------------------------------------------------------
//	�O�i�v���V�[�W���̃R���X�g���N�^	
//
//  �����@CGameMain* pGMain
//------------------------------------------------------------------------
CForeProc::CForeProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;
	m_pForegrd = new CSprite(m_pGMain->m_pShader);
	canOpen = -1;
}
// ---------------------------------------------------------------------------
// �O�i�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CForeProc::~CForeProc()
{
	SAFE_DELETE(m_pForegrd);
}
//-----------------------------------------------------------------------------
// �A�C�e����\������ѕϐ����Z�b�g����֐� K.Wakano
//-----------------------------------------------------------------------------
void CForeProc::ShowAndSetItem(int num, int isGetItem[], int initEffect, int cntItem[])
{
	// �A�C�e�����������Ă��Ȃ�
	if (isGetItem[num] == 0)
	{
		isGetItem[num] = 1;
		cntItem[num] = 1;
		m_pGMain->m_pItemManagementProc->m_pItemProc->setItemList(num, 0, initEffect);	// �A�C�e���������̏����l�ݒ�
		return;
	}
	else // �A�C�e�����������Ă���
	{
		cntItem[num] += 1;
		m_pGMain->m_pItemManagementProc->m_pItemProc->setItemList(num, 1, cntItem[num]);
		m_pGMain->m_pItemManagementProc->m_pItemProc->setItemList(num, 2, m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(num, 2));
		return;
	}
}
//-----------------------------------------------------------------------------
// �O�i�v���V�[�W���̍X�V
//
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CForeProc::Update()
{
	CSpriteImage* pImageFore = m_pGMain->m_pImageSprite; // ���łɓǂݍ���ł���X�v���C�g�C���[�W���g�p����
	float h = 0, m = 0, t = 0, s = 0;
	int time = clock() / CLOCKS_PER_SEC;		// �o�ߎ��Ԍv��
	int scoreMax = 1000;						// �X�R�A�̍ő�l
	TCHAR str[256];
	int DestX, DestY;
	int checkArray[BOX_ITEM_MAX] = {};			// �A�C�e����������z�� ������:0 ����:1
	int checkArrayCnt[BOX_ITEM_MAX] = {};		// �A�C�e�����v���萔�i�[�z��
	time -= m_pGMain->m_time;					// �^�C�g���̎��Ԃƍ��킹�ă��Z�b�g

	// �X�e�[�^�X�o�[�̕\��
	h = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp();
	if (h < 0) h = 0;
	m = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetMp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxMp();
	if (m < 0) m = 0;
	t = (float)time / TIME_LIMIT;
	// �X�R�A�v�Z
	s = (float)m_pGMain->m_nScore / scoreMax;
	if (s > 1) s = 1;

	// ���ԃI�[�o�[�ŃQ�[���I�[�o�[��
	if (time == TIME_LIMIT) {
		m_pGMain->m_dwGameStatus = GAMEOVER;
	}

	// �Q�[���N���A���Ɏc�莞�Ԃɉ������X�R�A�̉��Z���� ���~�b�g�𒴉߂����ꍇ���Z���Ȃ� K.Wakano
	if (m_pGMain->m_dwGameStatus == GAMECLEAR && time <= TIME_LIMIT)
	{
		m_pGMain->m_nEndGameSecond = time;
		// �������� * 20����A�o�ߎ��Ԃɂ�菙�X�Ɍ��炵�Ă���
		m_pGMain->m_nScore += (TIME_LIMIT * 20 - (m_pGMain->m_nEndGameSecond * 20));
	}

	// HP��PC�c���̕\��
	DestX = 10;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 0, 213, 31);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 32, (int)(144 * h), 6);
	_stprintf_s(str, _T("%d"), m_nPcNum);
	m_pGMain->m_pFont->Draw(DestX + 6, DestY + 15, str, 16, RGB(255, 0, 0));
	_stprintf(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
	m_pGMain->m_pFont->Draw(DestX + 26, DestY + 16, str, 12, RGB(0, 0, 0));

	// MP�̕\��
	DestY = 50;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 156, 213, 28);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 186, (int)(144 * m), 6);
	_stprintf(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetMp());
	m_pGMain->m_pFont->Draw(DestX + 8, DestY + 16, str, 12, RGB(0, 0, 0));

	// �o�ߎ��Ԃ̕\��
	DestX = WINDOW_WIDTH - 213 - 10;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 200, 213, 28);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 230, (int)(144 * t), 6);
	_stprintf(str, _T("%d/180"), time);
	m_pGMain->m_pFont->Draw(DestX + 10, DestY + 16, str, 12, RGB(0, 0, 0));

	// �X�R�A�̕\�� K.Wakano
	DestY = 50;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 112, 213, 28);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 142, (int)(144 * s), 6);
	_stprintf(str, _T("%04d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(DestX + 10, DestY + 16, str, 12, RGB(0, 0, 0));

	// �g�̕\��
	DestX = 300;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 80, 213, 31);

	// �󗓂̕\��
	for (int i = 0; i < 10; i++) {
		m_pForegrd->Draw(pImageFore, DestX + 19 + 48 * i, DestY + 16, 0, 336, 48, 48);
	}

	// �A�C�e���̕\��
	for (cntArray = 0; cntArray < 50; cntArray++) {

		switch (m_pGMain->m_pItemManagementProc->m_pItemProc->GetItemObjPtr()->GetItemFlag()) {
			case 0:		// �H�̏ꍇ �W�����v�񐔂������Ƃ�1�オ��
				ShowAndSetItem(0, checkArray, 0, checkArrayCnt);
				break;
			case 1:		// ���̏ꍇ �ʏ�U��(Z)�̍U���͂�100���オ��
				ShowAndSetItem(1, checkArray, 200, checkArrayCnt);
				break;
			case 2:		// ���̏ꍇ ���؂ȕ󔠂��J������悤�ɂȂ�
				ShowAndSetItem(2, checkArray, 1, checkArrayCnt);
				canOpen = 1;
				break;
			case 3:		// ���̏ꍇ �ő�̗͂�100���オ��
				ShowAndSetItem(3, checkArray, 100, checkArrayCnt);
				break;
			case 4:		// ���̏ꍇ ��_���[�W��50���y������
				ShowAndSetItem(4, checkArray, 50, checkArrayCnt);
				break;
			case 5:		// ��̏ꍇ �U�����h��ɁA���U���̓A�b�v
				ShowAndSetItem(5, checkArray, 1, checkArrayCnt);
				break;
			case 6:		// �L�m�R�̏ꍇ �G�̍U���͂�������
				ShowAndSetItem(6, checkArray, 50, checkArrayCnt);
				break;
			case 7:		// ���e�̏ꍇ �U������������
				ShowAndSetItem(7, checkArray, 1, checkArrayCnt);
				break;
			case 8:		// �u���[�`�̏ꍇ �c�@���𑝉�������
				ShowAndSetItem(8, checkArray, 1, checkArrayCnt);
				break;
			case 9:		// ���̏ꍇ ��m���Ń_���[�W�����
				ShowAndSetItem(9, checkArray, 0, checkArrayCnt);
				break;
		}
	}

	// �A�C�e�����ɕ`�悷�鏈��
	for (int j = 0; j < 10; j++) {
		switch (checkArray[j]) {
			case 1:
				m_pForegrd->Draw(pImageFore, DestX + 19 + 48 * j, DestY + 16, 48 * j, 288, 48, 48);
				_stprintf(str, _T("%d"), checkArrayCnt[j]);
				m_pGMain->m_pFont->Draw(DestX + 59 + 48 * j, DestY + 49, str, 16, RGB(255, 255, 255));
				break;
			default:
				break;
		}
	}
}
