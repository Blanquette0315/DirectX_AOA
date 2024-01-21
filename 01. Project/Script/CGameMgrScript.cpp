#include "pch.h"
#include "CGameMgrScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CLevel.h>
#include <Engine/CSound.h>

#include "CPlayerScript.h"
#include "CTileMgrScript.h"
#include "CCardMgrScript.h"
#include "CUIMgrScript.h"

#include "CLevelChangerScript.h"

#include "CTileScript.h"
#include "CCardScript.h"

#include "CMonsterScript.h"

#include "CItemCardScript.h"
#include "CHPPotionScript.h"
#include "CArenaBGMScript.h"

CGameMgrScript::CGameMgrScript()
	: CScript(GAMEMGRSCRIPT)
	, m_pPlayer(nullptr)
	, m_pTileMgrScript(nullptr)
	, m_pCardMgrScript(nullptr)
	, m_eTurnType(TURN_TYPE::DROWCARD)
	, m_bCardSelected(false)
	, m_RefreshStateOnce(false)
	, m_iTurnCount(0)
	, m_bMakeDeckOnce(false)
	, m_bTestNow(false)
	, m_fWinAcctime(0.f)
	, m_bWinOnce(false)
{
}

CGameMgrScript::~CGameMgrScript()
{
}

void CGameMgrScript::begin()
{
	m_iTurnCount = 0;
}

void CGameMgrScript::tick()
{
	// ó�� �����ϱ⿡ �սἭ �÷��̾� ī�� ����Ʈ�� �̿��� ���� ����� �ش�.
	if (!m_bMakeDeckOnce)
	{
		// ���� ����� ������ �ֱ�
		m_pCardMgrScript->MakeDeck();

		m_bMakeDeckOnce = true;
	}

	// TileMgr�� ��ϵǾ� �ִ� ��ȿ Ÿ���� ���� �ش�.
	m_pTileMgrScript->ValidTileClear();

	// ���� ���� ����
	switch (m_eTurnType)
	{
		// �� ��° ������ �˱� ���� ī������ �Ѵ�.
	case TURN_TYPE::TURNCOUNTING:
	{
		m_iTurnCount++;

		// Player�� �ൿ�� �ʱ�ȭ
		GetPlayerScript()->ResetCurAct();

		// �ؾ��� �ϵ��� ��ġ�� �˸��� ���ʷ� �Ѱ��ش�.
		m_eTurnType = TURN_TYPE::DROWCARD;
		break;
	}

	// Level�� Clear ������ üũ�Ѵ�.
	//case TURN_TYPE::LVCLEARCHECK:
	//{
	//	bool AllMonDead = true;
	//	// �ʵ忡 �����ϴ� ���� ��ũ��Ʈ�� �ݺ����� ������, ��� �׾����� Ȯ���Ѵ�.
	//	for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
	//	{
	//		AllMonDead *= m_vecMonsterScript[i]->GetTempDead();
	//	}
	//
	//	// Ŭ���� ������ �޼� �ߴٸ�, ���� Level�� ��ȯ�Ѵ�.
	//	if (AllMonDead)
	//	{
	//		if (nullptr != m_pLvChangerScript)
	//		{
	//			m_pLvChangerScript->LevelChange();
	//		}
	//	}
	//
	//	// Clear�� ���� ���ߴٸ�,�׷��� ����
	//	else
	//	{
	//		m_eTurnType = TURN_TYPE::DROWCARD;
	//	}
	//
	//	break;
	//}

	case TURN_TYPE::DROWCARD:
	{
		// ������ ī�� ��ο�
		m_pCardMgrScript->Drow();
		// �� �� ����
		m_pCardMgrScript->HandSort();

		// �ؾ��� �ϵ��� ��ġ�� Player ���ʷ� �Ѱ��ش�.
		m_eTurnType = TURN_TYPE::PLAYER;

		break;
	}

		// Player�� ������ �� �ؾ��� ��
	case TURN_TYPE::PLAYER:
	{
		PlayerTurnPlay();


		// Level Clear���� üũ
		if (!m_bTestNow)
		{
			bool AllMonDead = true;
			// �ʵ忡 �����ϴ� ���� ��ũ��Ʈ�� �ݺ����� ������, ��� �׾����� Ȯ���Ѵ�.
			for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
			{
				AllMonDead *= m_vecMonsterScript[i]->GetTempDead();
			}

			// Ŭ���� ������ �޼� �ߴٸ�, ���� Level�� ��ȯ�Ѵ�.
			if (AllMonDead)
			{
				m_bWinOnce = true;

				// �� End Ʈ�����̶� �¸� ���������� �����°� ���� �� �����ѵ�.. �ָ��ϳ� �ð���..
				m_eTurnType = TURN_TYPE::WIN;
			}
		}

		break;
	}

	case TURN_TYPE::DISCARD:
	{
		m_fAccCardDiscTime += DT;

		// �ѹ� �� ����
		if (m_bCardDiscTurnOnce)
		{
			// ���� ���� ī�尡 �־��ٸ� ���� ���־�� �Ѵ�.
			if (m_bCardSelected)
			{
				// ���õ� ī�带 ������ �ش�.
				m_pCardMgrScript->ReleaseSelectedCard();

				// ���õ� ī�带 �����ߴٸ�, �÷��̾�� ���õ� ī�尡 ���ٰ� �˸���.
				GetPlayerScript()->SetSelectedCard(false);

				// ���� �÷��̾ �ٽ� ���� ȸ�� ���·� �������ش�.
				GetPlayerScript()->PlayerRotLeft();
			}

			// ���� ����
			m_pCardMgrScript->HandClear();

			m_bCardDiscTurnOnce = false;
		}
		
		if (m_fAccCardDiscTime > 0.3f)
		{
			// �ؾ��� �ϵ��� ��ġ�� Monster ���ʷ� �Ѱ��ش�.
			m_eTurnType = TURN_TYPE::MONSTER;
		}

		break;
	}

	// Monster�� ������ �� �ؾ��� ��
	case TURN_TYPE::MONSTER:
	{
		for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
		{
			// ���Ͱ� ���� �ʾ��� ���� AI�� ����Ǿ�� �Ѵ�.
			// ���Ͱ� ���� ���¸� ���ʸ� �Ѱܾ� �Ѵ�.
			if (!m_vecMonsterScript[i]->GetTempDead()
				&& !m_vecMonsterScript[i]->IS_MonStun())
			{
				m_vecMonsterScript[i]->MosnsterAIPlay();
				m_pTileMgrScript->AstarInit();
			}
		}

		// MonsterAction Turn���� �������ش�.
		m_eTurnType = TURN_TYPE::MONSTERACTION;
		break;
	}

	// Monster�� �����ؾ��� �ϵ��� �����Ѵ�.
	case TURN_TYPE::MONSTERACTION:
	{
		for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
		{
			// ���Ͱ� ���� �ʾ��� ���� ������ ����Ǿ�� �Ѵ�.
			if (!m_vecMonsterScript[i]->GetTempDead()
				&& !m_vecMonsterScript[i]->IS_MonStun())
			{

				m_vecMonsterScript[i]->MonsterAction();
				// ���� ���� �� �ǰ� ����� ������ �ʾҴٸ�, ���� ������ ������ ����Ǹ� �ȵȴ�.
				if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::ATTACK
					&& GetPlayerScript()->Is_Hit())
				{
					break;
				}
				// �ӹڵ� �������ʷ� ����Ǿ�� �Ѵ�.
				else if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::RESTRAIN
					&& !m_vecMonsterScript[i]->MonsterTurnCheck())
				{
					break;
				}
				// ���� �̵��� �������� �����غ� ���̴�.
				else if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::MOVE
					|| m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::RUN)
				{
					if (!m_vecMonsterScript[i]->MonsterTurnCheck())
					{
						break;
					}
				}
				// ȭ�� �������� �������� ����
				else if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::RELOAD
					&& !m_vecMonsterScript[i]->MonsterTurnCheck())
				{
					break;
				}
			}
		}

		// CheckMonster Turn���� �������ش�.
		m_eTurnType = TURN_TYPE::CHECKMONSTER;
		break;
	}

	// Monster�� �����ؾ��� �ϵ��� ���ƴ��� Ȯ���Ѵ�.
	case TURN_TYPE::CHECKMONSTER:
	{
		bool endturn = true;
		for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
		{
			// ���Ͱ� ���� �ʾ��� ���� AI�� ����Ǿ�� �Ѵ�.
			if (!m_vecMonsterScript[i]->GetTempDead()
				&& !m_vecMonsterScript[i]->IS_MonStun())
			{
				// ������ ������� ���ʸ� �����Ѵ�.
				endturn *= m_vecMonsterScript[i]->MonsterTurnCheck();
			}
		}

		// ���� AI���� �� �ϵ��� ������ �ٽ� Player ������ �ٲ� �ش�.
		if (endturn)
		{
			m_eTurnType = TURN_TYPE::TURNCOUNTING;
		}
		else
		{
			m_eTurnType = TURN_TYPE::MONSTERACTION;
		}

		break;
	}

	case TURN_TYPE::WIN:
	{
		if (m_bWinOnce)
		{
			// �̱� �ΰ� ǥ��
			CreateWinLogo();
			CreateWinSound();
			m_bWinOnce = false;
		}

		m_fWinAcctime += DT;

		// 0.55f �ʰ� ������ ���콺 ��Ŭ���� ������, ���� ������ ��ȯ
		if (m_fWinAcctime > 0.55f)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				// ���� BGM�� ���־�� �Ѵ�.
				CArenaBGMScript* pArenaBGM = (CArenaBGMScript*)GetOwner()->GetScript((UINT)ARENABGMSCRIPT);
				pArenaBGM->SoundStop();

				// ���� ��ȯ
				if (m_pLvChangerScript != nullptr)
				{
					m_pLvChangerScript->LevelChange();
				}
				
			}
		}
			
		break;
	}

	}

}

void CGameMgrScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CGameMgrScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}




void CGameMgrScript::PlayerTurnPlay()
{
	// ���� ���� ���� ī�尡 �ִٸ�,
	if (m_bCardSelected)
	{
		// �÷��̾�� ������ ī�尡 �ִٰ� �˸���.
		GetPlayerScript()->SetSelectedCard(true);

		// Player�� ���� ���ǿ��� Ÿ���� �����޶�� �����Ѵ�.
		m_pTileMgrScript->ShowRange(GetSelectedCardState().Range);


		// �÷��̾��� ���� ���� �����ϱ�
		// ���õ� ī�� ��Ȳ�� ���� �÷��̾��� ���� ������ �ٸ���.
		switch (GetSelectedCardType())
		{
		case CARD_TYPE::ATTACK:
		{
			break;
		}
			
		case CARD_TYPE::DEFFENCE:
		{
			break;
		}
			
		case CARD_TYPE::MOVE:
		{
			// ���� ������ Ÿ�ϵ� �� ���콺�� �ö� �ִ� Ÿ���� �ִ��� Ȯ���Ѵ�.
			CTileScript* pTargetTileScript = m_pTileMgrScript->SelectInValidTile();
			CPlayerScript* pPlayerScript = (CPlayerScript*)(m_pPlayer->GetScript((UINT)PLAYERSCRIPT));

			// ���� ������ Ÿ�ϵ� �� ���콺�� �ö��� �ִ� Ÿ���� �ִٸ�,
			if (nullptr != pTargetTileScript)
			{
				pPlayerScript->PosOnTempTile(pTargetTileScript);
			}
			// ���� ������ Ÿ�ϵ� �� ���콺�� �ö��� �ִ� Ÿ���� ���ٸ�, �ٽ� ���� Ÿ�Ϸ� ��ġ�ϰ� ���ش�.
			else
			{
				pPlayerScript->PosOnTile();
			}

			break;
		}
		}

		// ��Ŭ���� ������ ��� ��ȿ�� Ÿ�� ������ ���õ� Ÿ������ Ȯ���Ѵ�.
		if (KEY_PRESSED(KEY::LBTN))
		{
			CTileScript* pTargetTileScript = m_pTileMgrScript->SelectInValidTile();

			if (nullptr != pTargetTileScript)
			{
				// ���� ��ȿ ��Ÿ� ���� Ÿ���� ���õǾ��ٸ�, Player�� �ش� Ÿ�Ϸ� �̵����� �ش�.
				switch (GetSelectedCardType())
				{
				case CARD_TYPE::ATTACK:
				{
					// ����� �����ص� ī��� �Ҹ� �ȴ�.
					GetPlayerScript()->Attack(pTargetTileScript, m_pCardMgrScript->GetSelectedCardScript()->GetCardDir());

					// �����ϱ�� �����ߴ� Ÿ�ϰ� ī���� ���ݷ��� ������ �д�.
					m_pTargetTile = pTargetTileScript;
					m_iCardAttack = GetSelectedCardState().Attack;

					// � ����� �����ߴ��� ���д�.
					CARD_DIR_TYPE eDir = m_pCardMgrScript->GetSelectedCardScript()->GetCardDir();
					if (eDir == CARD_DIR_TYPE::RIGHT)
					{
						m_eWaponType = GetPlayerScript()->GetRightWaponType();
					}
					else
					{
						m_eWaponType = GetPlayerScript()->GetLeftWaponType();
					}

					UsedSelectedCard();
				}
				break;

				case CARD_TYPE::DEFFENCE:
				{
					// ī���� ������ �����ͼ� �÷��̾��� ���� �� �־��ش�.
					GetPlayerScript()->AddDeffence(GetSelectedCardState().Deffence);
					GetPlayerScript()->CreateShieldParticle();
					// ���� ���� ȿ���� ���
					GetPlayerScript()->CreateAddShieldSound();

					// ��� ī�� ��� ȿ���� ���
					GetCardMgrScript()->GetSelectedCardScript()->PlayShieldCardSound();
					UsedSelectedCard();
				}
				break;

				case CARD_TYPE::MOVE:
				{
					// �ӹ� ���̶�� �����̸� �ȵȴ�.
					if (!GetPlayerScript()->CheckDebuf(DEBUF_TYPE::RESTRAIN))
					{
						// ����, ��� Ÿ�Ͽ� ���Ͱ� �������� ���� ���� �̵��� �����ؾ� �Ѵ�.
						if (pTargetTileScript->GetTileTargetType() != OBJECT_TYPE::MONSTER)
						{
							GetPlayerScript()->PosOnTile();
							// Player�� �̵� �Լ��� ȣ���� �ش�.
							GetPlayerScript()->Move(pTargetTileScript);
							UsedSelectedCard();
						}
						
					}
				}
				break;
				}

			}
		}
	}

	// ���� UIMgrScript�� ���õ� �������� �ִٸ�,
	else if (m_pUIMgrScript->IS_ItemVaild())
	{
		// ���õ� ������ ī�尡 ���ϰ� �ִ� ������ ���� ��������
		CItemCardScript* tmepICardScript = m_pUIMgrScript->GetSelectedItemScript();
		tItem_Info tItemInfo = tmepICardScript->GetItemInfo();

		// ���õ� �������� �Ҹ� �������� ���
		if (tItemInfo.eItemType == ITEM_TYPE::ITEM)
		{
			// ������ �̸��� ���� ���� �������� ��ũ��Ʈ�� �����´�.
			switch (tItemInfo.tItem.eItemType)
			{
				// ü�� ������ ���
			case ITEM_NAME::HPPOTION:
			{
				// �������� ��ũ��Ʈ�� �����´�.
				CHPPotionScript* pHPPotionScript = (CHPPotionScript*)GetOwner()->GetScript((UINT)HPPOTIONSCRIPT);
				break;
			}
			}

			// ������ ��ũ��Ʈ�� ������ Ȯ���� Player ������ ���� ������ ǥ���� �ش�.
			// �켱 �ӽ÷� Player ���Ǹ� �������.
			m_pTileMgrScript->ShowRange(0);

			// ��Ŭ���� ������ ��� ��ȿ�� Ÿ�� ������ ���õ� Ÿ������ Ȯ���Ѵ�.
			if (KEY_TAP(KEY::LBTN))
			{
				CTileScript* pTargetTileScript = m_pTileMgrScript->SelectInValidTile();

				// ����Ÿ���� ��ȿ�� ���, �����ۿ� ���� ����� �������ش�.
				if (nullptr != pTargetTileScript)
				{
					// ȿ���� ����
					CHPPotionScript* pHPPotionScript = (CHPPotionScript*)GetOwner()->GetScript((UINT)HPPOTIONSCRIPT);
					pHPPotionScript->CreatePotionUseSound();

					// �켱 �ӽ÷� ü���� 10�� ȸ���غ���.
					GetPlayerScript()->RecoveryCurHP(10);

					// ����� ���ƴٸ� ��ü �����ֱ�
					m_pUIMgrScript->UseSelectedItemCard();
				}
			}
		}
	}

	// ���õ� ī�尡 ���ٸ� �÷��̾�� ���õ� ī�� ������ �˸���.
	if (!m_bCardSelected)
	{
		GetPlayerScript()->SetSelectedCard(false);
	}
}

void CGameMgrScript::PlayerChangeTile(CTileScript* _TargetTile)
{
	m_pTileMgrScript->ObjectChangeTile(m_pPlayer, _TargetTile, OBJECT_TYPE::PLAYER);
}



void CGameMgrScript::SetCardSelected(bool _Select)
{
	m_bCardSelected = _Select;
	m_pTileMgrScript->SetCardSelected(_Select);
}

tCardState CGameMgrScript::GetSelectedCardState()
{
	return m_pCardMgrScript->GetSelectedCardScript()->GetCardState();
}

CARD_TYPE CGameMgrScript::GetSelectedCardType()
{
	return m_pCardMgrScript->GetSelectedCardScript()->GetCardType();
}

void CGameMgrScript::UsedSelectedCard()
{
	// Player�� �����ؼ� ī���� �ڽ�Ʈ��ŭ ���� Player�� �ൿ���� ������ �ش�.
	GetPlayerScript()->DefCurAct(m_pCardMgrScript->GetSelectedCardCost());

	// ���õ� ī�带 ������ �ش�.
	//m_pCardMgrScript->ReleaseSelectedCard();
	SetCardSelected(false);

	// ���� ������ ��������� ī�� ��ü�� ������ ������ �����־�� �Ѵ�.
	m_pCardMgrScript->UseSelectedCard();

	// ������ ���� ǥ�ø� ���ش�.
	m_pTileMgrScript->ValidTileSetAdj(false);
}

void CGameMgrScript::TargetMonHit()
{
	// Ÿ���� nullptr�� �ƴ� ���� ���� �ؾ��Ѵ�.
	if (m_pTargetTile != nullptr)
	{
		// ����� ���� �϶� ����ó���� ����.
		if (m_pTargetTile->GetTileTargetType() == OBJECT_TYPE::MONSTER)
		{
			CMonsterScript* pMonsterScript = GetMonsterScript(m_pTargetTile->GetTargetObject());
			pMonsterScript->Hit(m_iCardAttack, m_eWaponType);
		}
	}

	// ó�� ������ ��û�� �ڿ��� ����� �ʱ�ȭ �Ѵ�.
	m_pTargetTile = nullptr;
	m_iCardAttack = 0;
}

void CGameMgrScript::CreateWinLogo()
{
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\WinLogo_Prefab_0.pref");

	CGameObject* pWinLogo = pPrefab->Instantiate();

	// ���� ���� �������ֱ�
	pWinLogo->Transform()->SetRelativePos(Vec3(0.f, 200.f, 0.f));

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pWinLogo;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);
}

void CGameMgrScript::CreateWinSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Turn\\TurnEnd(338_Tambores).wav");
	pSound->Play(1, 0.9f, true);
}