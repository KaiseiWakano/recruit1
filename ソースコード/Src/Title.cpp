//------------------------------------------------------------------------
// �I�[�v�j���O�^�C�g���E�Q�[���N�����[�E�Q�[���I�[�o�[�̏��� Title.cpp
//------------------------------------------------------------------------

#include "Title.h"
#include "Playchar.h"
#include "time.h"

// �^�C�g���v���V�[�W���̃R���X�g���N�^
CTitleProc::CTitleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;
	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/titleImage.png"));			// �C���[�W�̃��[�h
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/clearImage.png"));			// �C���[�W�̃��[�h
	m_pImageOver = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/overImage.png"));			// �C���[�W�̃��[�h
	m_pImageClearScore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ClearScore.png"));	// �C���[�W�̃��[�h
	m_pImageCursor = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/cursor.png"));			// �C���[�W�̃��[�h
	m_pImageGuide = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/guideImage.png"));			// �C���[�W�̃��[�h
	m_pSprite = new CSprite(m_pGMain->m_pShader ); // �X�v���C�g�̐���

	m_dwCntTime = 0;
	flagTitleOrOther = true;
}

// �^�C�g���^�C�g���v���V�[�W���̃f�X�g���N�^
CTitleProc::~CTitleProc()
{
	SAFE_DELETE(m_pImageTitle);
	SAFE_DELETE(m_pImageClear);
	SAFE_DELETE(m_pImageOver);
	SAFE_DELETE(m_pImageClearScore);
	SAFE_DELETE(m_pImageCursor);
	SAFE_DELETE(m_pImageGuide)
	SAFE_DELETE(m_pSprite);
}

// �^�C�g���̕\�� K.Wakano
void CTitleProc::Title()
{
	int showY;	// �J�[�\����Y���W���i�[
	// �w�i�̕\��
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	showY = WINDOW_HEIGHT - 300;	// �͂��߂���̈ʒu�ɕ\��
	
	// �L�[����̔���
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_DOWN))
	{
		flagTitleOrOther = false;	// �����т����̈ʒu
	}
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_UP))
	{
		flagTitleOrOther = true;	// �͂��߂���̈ʒu
	}
	m_dwCntTime++;
	// ���Ԃ��v�����\����\����؂�ւ��_�ł�����
	if (m_dwCntTime % 30 > 15)
	{
		if (flagTitleOrOther == true)
		{
			// �͂��߂���̈ʒu
			showY = WINDOW_HEIGHT - 300;
			m_pSprite->Draw(m_pImageCursor, WINDOW_WIDTH - 450, showY, 0, 0, m_pImageCursor->m_dwImageWidth, m_pImageCursor->m_dwImageHeight, 48, 48);
		}
		else
		{
			// �����т����̈ʒu
			showY = WINDOW_HEIGHT - 200;
			m_pSprite->Draw(m_pImageCursor, WINDOW_WIDTH - 450, showY, 0, 0, m_pImageCursor->m_dwImageWidth, m_pImageCursor->m_dwImageHeight, 48, 48);
		}
	}

	m_pGMain->m_pFont->Draw(WINDOW_WIDTH - 400, WINDOW_HEIGHT - 300, _T("�͂��߂�"), 48, RGB(255, 255, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH - 400, WINDOW_HEIGHT - 200, _T("�����т���"), 48, RGB(255, 255, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON))
	{
		if (flagTitleOrOther == true)
		{
			// �^�C�g���ɂ�����o�ߎ��Ԃ�}������
			m_pGMain->m_time = clock() / CLOCKS_PER_SEC;
			// �T�E���h�̍Đ�
			m_pGMain->m_pBgm1->Play(AUDIO_LOOP);
			m_pGMain->m_dwGameStatus = GAMEMAIN;
			m_dwCntTime = 0;
		}
		else
		{
			m_pGMain->m_dwGameStatus = GUIDE;
		}
	}
}

// �V�ѕ��̕\�� K.Wakano
void CTitleProc::Guide()
{
	m_pSprite->Draw(m_pImageGuide, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT - 75, _T("�X�y�[�X�Ŗ߂�"), 48, RGB(255, 255, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON))
	{
		m_pGMain->m_dwGameStatus = TITLE;
	}
}

// �Q�[���N���A�̕\��
void CTitleProc::GameClear()
{
	// �\��
	m_pSprite->Draw(m_pImageClear, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);	

	// �ŏI�X�R�A�\�� K.Wakano
	TCHAR str[256];
	m_pSprite->Draw(m_pImageClearScore, WINDOW_WIDTH / 2 - 128, WINDOW_HEIGHT / 2 - 108, 0, 0, 256, 256);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 80, WINDOW_HEIGHT / 2 - 108 + 17, _T("Score"), 36, RGB(255, 255, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	_stprintf(str, _T("%04d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 80, WINDOW_HEIGHT / 2 - 108 + 140, str, 36, RGB(255, 255, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)
	{
		m_pGMain->m_pBgm1->Stop();
		m_pGMain->m_dwGameStatus = GAMEEND;
		m_dwCntTime = 0;
	}
}

//	�Q�[���I�[�o�[�̕\��
void CTitleProc::GameOver()
{
	// �\��
	m_pSprite->Draw(m_pImageOver, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) ||
		m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)
	{
		m_pGMain->m_pBgm1->Stop();
		m_pGMain->m_dwGameStatus = GAMEEND;
		m_dwCntTime = 0;
	}
}