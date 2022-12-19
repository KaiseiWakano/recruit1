//------------------------------------------------------------------------------
// �I�[�v�j���O�^�C�g���E�Q�[���N�����[�E�Q�[���I�[�o�[�̏����w�b�_ Title.h
//------------------------------------------------------------------------------

#pragma once

#include "GameMain.h"

// �^�C�g�� �v���V�[�W���N���X
class CTitleProc : public CBaseProc
{
protected:
	CGameMain*		m_pGMain;

	CSpriteImage*   m_pImageTitle;
	CSpriteImage*   m_pImageClear;
	CSpriteImage*   m_pImageOver;
	CSpriteImage*   m_pImageClearScore;
	CSpriteImage*	m_pImageCursor;
	CSpriteImage*   m_pImageGuide;
	CSprite*        m_pSprite;

	DWORD           m_dwCntTime;
public:
	void Title();
	void GameClear();
	void GameOver();
	void Guide();
	CTitleProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CTitleProc();
private:
	bool flagTitleOrOther;
};