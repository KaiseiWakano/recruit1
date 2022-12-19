//------------------------------------------------------------------------
// �A�C�e���̏��� ItemManager.cpp
//------------------------------------------------------------------------

#include "ItemManager.h"
#include "Playchar.h"
#include "Map.h"

//------------------------------------------------------------------------
// �A�C�e���̏������C���v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CItemManagementProc::CItemManagementProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pItemProc = new CItemProc(m_pGMain);		// �A�C�e���v���V�[�W��
	m_pProcArray.push_back(m_pItemProc);		// �v���V�[�W�����v���V�[�W���z��ɓo�^����
}
//============================================================================
// �v���V�[�W���ƃI�u�W�F�N�g��T�����S�ẴI�u�W�F�N�g���m���A�N�e�B�u�ɂ���
// �v���V�[�W���̊J�n�t���O�����Z�b�g����
//============================================================================
void CItemManagementProc::SetNonActive()
{
	//�I�u�W�F�N�g�|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive(FALSE);
	}
	// ���ʂ̃v���V�[�W���|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag(0);							// �J�n�t���O�̃��Z�b�g
		((CItemManagementProc*)m_pProcArray[i])->SetNonActive();	// ���ʂ̃v���V�[�W���̏����i�ċA�����j
	}
}

// =================================================================================
//------------------------------------------------------------------------
// �A�C�e���v���V�[�W���̃R���X�g���N�^
//------------------------------------------------------------------------
CItemProc::CItemProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CItemObj(m_pGMain));	// m_pObjArray�ɃA�C�e���𐶐�����
	}
	m_nStartFlag = 0;
	m_nCheckItem = 0;
}

//-----------------------------------------------------------------------------
// �A�C�e���f�[�^�̓ǂݍ��� K.Wakano
//-----------------------------------------------------------------------------
void CItemProc::ItemDataInput()
{
	int num, effect,effectNum, stack, weight;		// ���͗p�ϐ��i�����^�j
	char name[128] = {}, buf[128] = {};				// ���͗p�ϐ��i�L�����^�j
	FILE *m_pItemData = fopen("Data/Csv/ItemData.csv", "rt");		// csv�t�@�C�����J��
	if (!m_pItemData)
	{
		return;							// �ǂݍ��߂Ȃ������ꍇ�I������
	}

	// ��s�ǂݔ�΂�
	fgets(buf, 128, m_pItemData);

	// ��s���ǂݍ���ŃA�C�e���\���̂ɑ��
	for (int i = 0; fscanf(m_pItemData, "%d, %[^,], %d, %d, %d, %d", &num, name, &effect, &effectNum, &stack, &weight) != EOF; i++)
	{
		itemList[i].num = num;
		snprintf(itemList[i].name, sizeof name, name);
		itemList[i].effect = effect;
		itemList[i].effectNum = effectNum;
		itemList[i].stack = stack;
		itemList[i].weight = weight;
	}

	fclose(m_pItemData);				// �t�@�C�������
}

//-----------------------------------------------------------------------------
// �A�C�e���v���V�[�W���̊J�n
// �߂�l �Ȃ�
//-----------------------------------------------------------------------------
void CItemProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	ItemDataInput();		// csv�t�@�C���ǂݍ���

	// �����ǉ�
	srand((unsigned int)time(NULL));
	int randomNum = 0;
	randomNum = rand() % 10;

	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�~�}����ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T�� ( EvtID:2 �A�C�e���̏o���ʒu, EvtNo:0x10 �~�}��	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x10, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMRESQUE);
						break;
					}
				}
			}
		}
		// �C�x���g�}�b�v��T�����A�h�A�i�o���j��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2 �A�C�e���̏o���ʒu,  EvtNo:0x20 �h�A )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x20, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMDOOR);
						break;
					}
				}
			}
		}
		// �C�x���g�}�b�v��T�����A���@���ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2 �A�C�e���̏o���ʒu, EvtNo:0x40 ���@�� )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x40, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMAGIC);
						break;
					}
				}
			}
		}
		// �C�x���g�}�b�v��T�����A�󔠂�ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2 �A�C�e���̏o���ʒu, EvtNo:0x80 �� )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x80, vPos, nNext))
			{
				//if (randomNum < 5) {			// �󔠃����_������
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMBOX);
						break;
					}
				}
				//}
				randomNum = rand() % 10;		// �������Z�b�g
			}
		}
		// �C�x���g�}�b�v��T�����A�傫�ȕ󔠂�ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2 �A�C�e���̏o���ʒu, EvtNo:0x100 �� )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x100, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMGREATBOX);
						break;
					}
				}
			}
		}

		m_nStartFlag = 1;  // ��񔭐���������A�����������Ȃ�
	}
}

// �A�C�e���\���̂̃Q�b�^�[ K.Wakano
// ���� itemNum:�A�C�e���ԍ� pick:�擾����l�ԍ�
// 0:�A�C�e���ԍ� 1:�����l 2:���� 3:������ 4:�d��
int CItemProc::getItemList(int itemNum, int pick)
{
	if (0 > itemNum || itemNum > ITEM_TYPE_NUM || 0 > pick || pick > 4)
	{
		return -1;	// �����Ɉُ킠��Ƃ��ďI��
	}
	switch (pick)
	{
	case 0:
		return itemList[itemNum].num;		// �ԍ���Ԃ�
		break;
	case 1:
		return itemList[itemNum].effect;	// �����l��Ԃ�
		break;
	case 2:
		return itemList[itemNum].effectNum;	// ������Ԃ�
		break;
	case 3:
		return itemList[itemNum].stack;		// ��������Ԃ�
		break;
	case 4:
		return itemList[itemNum].weight;	// �d�݂�Ԃ�
		break;
	default:
		break;
	}
}

// �A�C�e���\���̂̃Z�b�^�[ K.Wakano
// ���� itemNum:�A�C�e���ԍ� setValue:�Z�b�g���鐔��
// 0:�����l 1:������ 2:�����l�ɑ��������Z
void CItemProc::setItemList(int itemNum, int pick, int value)
{
	switch (pick)
	{
	case 0:
		itemList[itemNum].effect = value;	// �����l
		break;
	case 1:
		itemList[itemNum].stack = value;	// ������
		break;
	case 2:
		itemList[itemNum].effect += value;	// �����l�ɑ��������Z
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------
// �A�C�e���I�u�W�F�N�g�̃R���X�g���N�^	
// ���� �Ȃ�
//------------------------------------------------------------------------
CItemObj::CItemObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar);
	m_nAnimNum = 1;
	m_nCheckItem = -1;
}
// ---------------------------------------------------------------------------
// �A�C�e���I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CItemObj::~CItemObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// �A�C�e���I�u�W�F�N�g�̊J�n
//
// VECTOR2 vPos �����ʒu
// DWORD dwNo �A�C�e�����
//
// �߂�l TRUE
//-----------------------------------------------------------------------------
BOOL CItemObj::Start(VECTOR2 vPos, DWORD dwNo)
{
	ResetStatus();
	ResetAnim();

	m_bActive = TRUE;
	m_dwStatusSub = dwNo;

	srand((unsigned int)time(NULL));	// �����̎�̃��Z�b�g

	m_vPos = vPos;	// �����ʒu
	m_vPosUp = VECTOR2(0, 0);

	switch (m_dwStatusSub)
	{
	case  ITEMRESQUE:		// �~�}��
		m_pSprite->SetSrc(384, 336, 48, 48);
		break;

	case  ITEMDOOR:			// �h�A�A�����F
		m_pSprite->SetSrc(192, 0, 48, 48);
		break;

	case  ITEMMAGIC:		// ���@��
		m_pSprite->SetSrc(288, 336, 48, 48);
		break;

	case  ITEMBOX:			// ��
		m_pSprite->SetSrc(0, 384, 48, 48);
		break;

	case ITEMGREATBOX:		// �傫�ȕ�
		m_pSprite->SetSrc(144, 384, 48, 48);
		break;
	}
	return TRUE;
}

// �A�C�e�����Z�b�g����֐� K.Wakano
void CItemObj::SetItemGetFlag(int setResult)
{
	int itemNum = setResult;
	m_pSprite->SetSrc(48 * setResult, 432, 48, 48);
	m_nCheckItem = setResult;
}

//-----------------------------------------------------------------------------
// �A�C�e���I�u�W�F�N�g�̍X�V
// ���� �Ȃ�
//-----------------------------------------------------------------------------
void CItemObj::Update()
{
	if (!m_bActive) return;
	CDirectInput* pDI = m_pGMain->m_pDI;

	//
	// �󔠂̒��g�̏��� K.Wakano ��������
	//

	int weightList[ITEM_TYPE_NUM];	// �d�݊i�[�z��

	// ���I����
	int result = -1;

	// �d�݂̍��v�v�Z
	int weightSum = 0;
	for (int i = 0; i < ITEM_TYPE_NUM; i++)
	{
		weightList[i] = m_pGMain->m_pItemManagementProc->m_pItemProc->getItemList(i, 4);
		weightSum += weightList[i];
	}

	// �����ǉ�
	int randomNum;
	randomNum = rand() % weightSum;

	// ���I����
	for (int i = 0; i < ITEM_TYPE_NUM; i++)
	{
		if (randomNum < weightList[i])
		{
			result = i;
			break;
		}
		randomNum -= weightList[i];
	}

	//
	// �󔠂̒��g�̏��� K.Wakano �����܂�
	//

	if (isCollision(m_pGMain->m_pPcProc->GetPcObjPtr())) // PC�Ƃ̐ڐG����
	{
		switch (m_dwStatusSub)
		{
		case  ITEMRESQUE:     // �~�}��
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
			m_pGMain->m_pSePower->Play();
			m_bActive = FALSE;
			break;

		case  ITEMDOOR:      // �h�A
			if (pDI->CheckKey(KD_DAT, DIK_E) && flagBossDeath == true) {
				m_pGMain->m_pSePower->Play();
				m_pGMain->m_pMapProc->MoveMap();
				m_bActive = FALSE;
			}
			break;

		case  ITEMMAGIC:	// ���@��
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetMp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxMp());
			m_pGMain->m_pSePower->Play();
			m_bActive = FALSE;
			break;

			// �󔠂̏���
		case  ITEMBOX:
			if (pDI->CheckKey(KD_DAT, DIK_E)) {		// E�L�[���������ƊJ��
				m_pGMain->m_pSeUnlock->Play();		// SE�̍Đ�
				SetItemGetFlag(result);				// ���ʂ�n���ăA�C�e����\���������t���O���Z�b�g
				m_bActive = FALSE;					// ����
			}
			break;

			// �傫�ȕ�
		case ITEMGREATBOX:
			if (pDI->CheckKey(KD_DAT, DIK_E) && 
				m_pGMain->m_pForeProc->getCanOpen() > 0) {	// E��������A�������������Ă���ƊJ��
				m_pGMain->m_pSeUnlock->Play();				// SE�̍Đ�
				SetItemGetFlag(9);							// ��
				m_bActive = FALSE;							// ����
				break;
			}
			break;
		}
	}
	AnimCountup();
	Draw();
}
