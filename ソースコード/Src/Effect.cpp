//------------------------------------------------------------------------
// �f�����ʏ��� Effect.cpp
//------------------------------------------------------------------------

#include "Effect.h"
#include "Playchar.h"
#include "Map.h"

//------------------------------------------------------------------------
// �f�����ʃ��C���v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEffectProc::CEffectProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pEffectBomProc = new CEffectBomProc(m_pGMain);	// �f�����ʁE�����v���V�[�W��
	m_pProcArray.push_back(m_pEffectBomProc);			// �v���V�[�W�����v���V�[�W���z��ɓo�^����
}
//============================================================================
// �v���V�[�W���ƃI�u�W�F�N�g��T�����S�ẴI�u�W�F�N�g���m���A�N�e�B�u�ɂ���
// �v���V�[�W���̊J�n�t���O�����Z�b�g����
//============================================================================
void CEffectProc::SetNonActive()
{
	//�I�u�W�F�N�g�|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}
	// ���ʂ̃v���V�[�W���|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag(0);					// �J�n�t���O�̃��Z�b�g
		((CEffectProc*)m_pProcArray[i])->SetNonActive();    // ���ʂ̃v���V�[�W���̏����i�ċA�����j
	}
}

// ================================================================================================================================
// �e�f�����ʃv���V�[�W�����̏���
// ================================================================================================================================

//------------------------------------------------------------------------
// �f�����ʁE�����v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CEffectBomProc::CEffectBomProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < EFFECT_BOM_MAX; i++)
	{
		m_pObjArray.push_back(new CEffectBomObj(m_pGMain));	// m_pObjArray�ɉf�����ʁE�����I�u�W�F�N�g�𐶐�����
	}
}

//-----------------------------------------------------------------------------
// �f�����ʁE�����v���V�[�W���̊J�n
//
// VECTOR2 vPos �����ʒu
//
// �߂�l ����ɔ��� : TRUE �������Ȃ����� : FALSE
//-----------------------------------------------------------------------------
BOOL CEffectBomProc::Start(VECTOR2 vPos)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive() )
		{
			m_pObjArray[i]->Start(vPos);	// �f�����ʁE�����̔���
			m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}

	return TRUE;
}

//------------------------------------------------------------------------
// �f�����ʁE�����I�u�W�F�N�g�̃R���X�g���N�^	
// �����@�Ȃ�
//------------------------------------------------------------------------
CEffectBomObj::CEffectBomObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 144, 480, 64, 64);
	m_nAnimNum = 7;
}
// ---------------------------------------------------------------------------
// �f�����ʁE�����I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEffectBomObj::~CEffectBomObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// �f�����ʁE�����I�u�W�F�N�g�̊J�n
//
// VECTOR2 vPos �����ʒu
//
// �߂�l TRUE
//-----------------------------------------------------------------------------
BOOL CEffectBomObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	m_dwStatus = NORMAL;
	m_dwStatusSub = STOP;
	m_dwStatusPhase = 0;

	m_vPos = vPos;	// �����ʒu

	m_nAnimIdx = 0;

	return TRUE;
}
//-----------------------------------------------------------------------------
// �f�����ʁE�����I�u�W�F�N�g�̍X�V
//
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CEffectBomObj::Update()
{
	if (m_bActive)
	{
		if (AnimCountup() == 1)
		{
			m_bActive = FALSE;
		}
		Draw();
	}
}
