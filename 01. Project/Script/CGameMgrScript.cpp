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
	// 처음 진행하기에 앞써서 플레이어 카드 리스트를 이용해 덱을 만들어 준다.
	if (!m_bMakeDeckOnce)
	{
		// 덱을 만들고 셔플해 주기
		m_pCardMgrScript->MakeDeck();

		m_bMakeDeckOnce = true;
	}

	// TileMgr에 등록되어 있는 유효 타일을 지워 준다.
	m_pTileMgrScript->ValidTileClear();

	// 차례 진행 관리
	switch (m_eTurnType)
	{
		// 몇 번째 턴인지 알기 위해 카운팅을 한다.
	case TURN_TYPE::TURNCOUNTING:
	{
		m_iTurnCount++;

		// Player의 행동력 초기화
		GetPlayerScript()->ResetCurAct();

		// 해야할 일들을 마치면 알맞은 차례로 넘겨준다.
		m_eTurnType = TURN_TYPE::DROWCARD;
		break;
	}

	// Level의 Clear 조건을 체크한다.
	//case TURN_TYPE::LVCLEARCHECK:
	//{
	//	bool AllMonDead = true;
	//	// 필드에 존재하는 몬스터 스크립트를 반복문을 수행해, 모두 죽었는지 확인한다.
	//	for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
	//	{
	//		AllMonDead *= m_vecMonsterScript[i]->GetTempDead();
	//	}
	//
	//	// 클리어 조건을 달성 했다면, 다음 Level로 전환한다.
	//	if (AllMonDead)
	//	{
	//		if (nullptr != m_pLvChangerScript)
	//		{
	//			m_pLvChangerScript->LevelChange();
	//		}
	//	}
	//
	//	// Clear를 아직 못했다면,그래도 진행
	//	else
	//	{
	//		m_eTurnType = TURN_TYPE::DROWCARD;
	//	}
	//
	//	break;
	//}

	case TURN_TYPE::DROWCARD:
	{
		// 덱에서 카드 드로우
		m_pCardMgrScript->Drow();
		// 손 패 정렬
		m_pCardMgrScript->HandSort();

		// 해야할 일들을 마치면 Player 차례로 넘겨준다.
		m_eTurnType = TURN_TYPE::PLAYER;

		break;
	}

		// Player의 차례일 때 해야할 일
	case TURN_TYPE::PLAYER:
	{
		PlayerTurnPlay();


		// Level Clear조건 체크
		if (!m_bTestNow)
		{
			bool AllMonDead = true;
			// 필드에 존재하는 몬스터 스크립트를 반복문을 수행해, 모두 죽었는지 확인한다.
			for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
			{
				AllMonDead *= m_vecMonsterScript[i]->GetTempDead();
			}

			// 클리어 조건을 달성 했다면, 다음 Level로 전환한다.
			if (AllMonDead)
			{
				m_bWinOnce = true;

				// 턴 End 트럼펫이랑 승리 문구정도는 나오는게 좋을 것 같긴한데.. 애매하네 시간이..
				m_eTurnType = TURN_TYPE::WIN;
			}
		}

		break;
	}

	case TURN_TYPE::DISCARD:
	{
		m_fAccCardDiscTime += DT;

		// 한번 만 진행
		if (m_bCardDiscTurnOnce)
		{
			// 선택 중인 카드가 있었다면 해제 해주어야 한다.
			if (m_bCardSelected)
			{
				// 선택된 카드를 해제해 준다.
				m_pCardMgrScript->ReleaseSelectedCard();

				// 선택된 카드를 해제했다면, 플레이어에게 선택된 카드가 없다고 알린다.
				GetPlayerScript()->SetSelectedCard(false);

				// 이후 플레이어를 다시 원래 회전 상태로 변경해준다.
				GetPlayerScript()->PlayerRotLeft();
			}

			// 손패 비우기
			m_pCardMgrScript->HandClear();

			m_bCardDiscTurnOnce = false;
		}
		
		if (m_fAccCardDiscTime > 0.3f)
		{
			// 해야할 일들을 마치면 Monster 차례로 넘겨준다.
			m_eTurnType = TURN_TYPE::MONSTER;
		}

		break;
	}

	// Monster의 차례일 때 해야할 일
	case TURN_TYPE::MONSTER:
	{
		for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
		{
			// 몬스터가 죽지 않았을 때만 AI가 실행되어야 한다.
			// 몬스터가 스턴 상태면 차례를 넘겨야 한다.
			if (!m_vecMonsterScript[i]->GetTempDead()
				&& !m_vecMonsterScript[i]->IS_MonStun())
			{
				m_vecMonsterScript[i]->MosnsterAIPlay();
				m_pTileMgrScript->AstarInit();
			}
		}

		// MonsterAction Turn으로 변경해준다.
		m_eTurnType = TURN_TYPE::MONSTERACTION;
		break;
	}

	// Monster가 수행해야할 일들을 수행한다.
	case TURN_TYPE::MONSTERACTION:
	{
		for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
		{
			// 몬스터가 죽지 않았을 때만 패턴이 실행되어야 한다.
			if (!m_vecMonsterScript[i]->GetTempDead()
				&& !m_vecMonsterScript[i]->IS_MonStun())
			{

				m_vecMonsterScript[i]->MonsterAction();
				// 몬스터 공격 시 피격 모션이 끝나지 않았다면, 다음 몬스터의 패턴이 실행되면 안된다.
				if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::ATTACK
					&& GetPlayerScript()->Is_Hit())
				{
					break;
				}
				// 속박도 차례차례로 시행되어야 한다.
				else if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::RESTRAIN
					&& !m_vecMonsterScript[i]->MonsterTurnCheck())
				{
					break;
				}
				// 이제 이동도 차례차례 진행해볼 것이다.
				else if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::MOVE
					|| m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::RUN)
				{
					if (!m_vecMonsterScript[i]->MonsterTurnCheck())
					{
						break;
					}
				}
				// 화살 재장전도 차례차례 진행
				else if (m_vecMonsterScript[i]->GetMonsterPattern() == MONSTER_PATTERN::RELOAD
					&& !m_vecMonsterScript[i]->MonsterTurnCheck())
				{
					break;
				}
			}
		}

		// CheckMonster Turn으로 변경해준다.
		m_eTurnType = TURN_TYPE::CHECKMONSTER;
		break;
	}

	// Monster가 수행해야할 일들을 마쳤는지 확인한다.
	case TURN_TYPE::CHECKMONSTER:
	{
		bool endturn = true;
		for (size_t i = 0; i < m_vecMonsterScript.size(); ++i)
		{
			// 몬스터가 죽지 않았을 때만 AI가 실행되어야 한다.
			if (!m_vecMonsterScript[i]->GetTempDead()
				&& !m_vecMonsterScript[i]->IS_MonStun())
			{
				// 몬스터의 순서대로 차례를 진행한다.
				endturn *= m_vecMonsterScript[i]->MonsterTurnCheck();
			}
		}

		// 몬스터 AI에서 할 일들이 끝나면 다시 Player 턴으로 바꿔 준다.
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
			// 이긴 로고 표시
			CreateWinLogo();
			CreateWinSound();
			m_bWinOnce = false;
		}

		m_fWinAcctime += DT;

		// 0.55f 초가 지나고 마우스 좌클릭이 들어오면, 다음 레벨로 전환
		if (m_fWinAcctime > 0.55f)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				// 기존 BGM은 꺼주어야 한다.
				CArenaBGMScript* pArenaBGM = (CArenaBGMScript*)GetOwner()->GetScript((UINT)ARENABGMSCRIPT);
				pArenaBGM->SoundStop();

				// 레벨 전환
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
	// 지금 선택 중인 카드가 있다면,
	if (m_bCardSelected)
	{
		// 플레이어에게 선택한 카드가 있다고 알린다.
		GetPlayerScript()->SetSelectedCard(true);

		// Player가 지닌 발판에게 타일을 보여달라고 전달한다.
		m_pTileMgrScript->ShowRange(GetSelectedCardState().Range);


		// 플레이어의 예비 동작 실행하기
		// 선택된 카드 상황에 따라 플레이어의 예비 동작이 다르다.
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
			// 선택 가능한 타일들 중 마우스가 올라가 있는 타일이 있는지 확인한다.
			CTileScript* pTargetTileScript = m_pTileMgrScript->SelectInValidTile();
			CPlayerScript* pPlayerScript = (CPlayerScript*)(m_pPlayer->GetScript((UINT)PLAYERSCRIPT));

			// 선택 가능한 타일들 중 마우스가 올라가져 있는 타일이 있다면,
			if (nullptr != pTargetTileScript)
			{
				pPlayerScript->PosOnTempTile(pTargetTileScript);
			}
			// 선택 가능한 타일들 중 마우스가 올라가져 있는 타일이 없다면, 다시 원래 타일로 위치하게 해준다.
			else
			{
				pPlayerScript->PosOnTile();
			}

			break;
		}
		}

		// 좌클릭이 들어왔을 경우 유효한 타일 내에서 선택된 타일인지 확인한다.
		if (KEY_PRESSED(KEY::LBTN))
		{
			CTileScript* pTargetTileScript = m_pTileMgrScript->SelectInValidTile();

			if (nullptr != pTargetTileScript)
			{
				// 만약 유효 사거리 내의 타일이 선택되었다면, Player를 해당 타일로 이동시켜 준다.
				switch (GetSelectedCardType())
				{
				case CARD_TYPE::ATTACK:
				{
					// 허공에 공격해도 카드는 소모 된다.
					GetPlayerScript()->Attack(pTargetTileScript, m_pCardMgrScript->GetSelectedCardScript()->GetCardDir());

					// 공격하기로 선택했던 타일과 카드의 공격력을 저장해 둔다.
					m_pTargetTile = pTargetTileScript;
					m_iCardAttack = GetSelectedCardState().Attack;

					// 어떤 무기로 공격했는지 얻어둔다.
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
					// 카드의 방어력을 가져와서 플레이어의 현재 방어에 넣어준다.
					GetPlayerScript()->AddDeffence(GetSelectedCardState().Deffence);
					GetPlayerScript()->CreateShieldParticle();
					// 방어력 증가 효과음 재생
					GetPlayerScript()->CreateAddShieldSound();

					// 방어 카드 사용 효과음 재생
					GetCardMgrScript()->GetSelectedCardScript()->PlayShieldCardSound();
					UsedSelectedCard();
				}
				break;

				case CARD_TYPE::MOVE:
				{
					// 속박 중이라면 움직이면 안된다.
					if (!GetPlayerScript()->CheckDebuf(DEBUF_TYPE::RESTRAIN))
					{
						// 또한, 대상 타일에 몬스터가 존재하지 않을 때만 이동이 가능해야 한다.
						if (pTargetTileScript->GetTileTargetType() != OBJECT_TYPE::MONSTER)
						{
							GetPlayerScript()->PosOnTile();
							// Player의 이동 함수를 호출해 준다.
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

	// 만약 UIMgrScript에 선택된 아이템이 있다면,
	else if (m_pUIMgrScript->IS_ItemVaild())
	{
		// 선택된 아이템 카드가 지니고 있는 아이템 정보 가져오기
		CItemCardScript* tmepICardScript = m_pUIMgrScript->GetSelectedItemScript();
		tItem_Info tItemInfo = tmepICardScript->GetItemInfo();

		// 선택된 아이템이 소모 아이템일 경우
		if (tItemInfo.eItemType == ITEM_TYPE::ITEM)
		{
			// 아이템 이름에 따라 각각 아이템의 스크립트를 가져온다.
			switch (tItemInfo.tItem.eItemType)
			{
				// 체력 포션일 경우
			case ITEM_NAME::HPPOTION:
			{
				// 아이템의 스크립트를 가져온다.
				CHPPotionScript* pHPPotionScript = (CHPPotionScript*)GetOwner()->GetScript((UINT)HPPOTIONSCRIPT);
				break;
			}
			}

			// 가져온 스크립트의 범위를 확인해 Player 발판의 인접 발판을 표기해 준다.
			// 우선 임시로 Player 발판만 띄워본다.
			m_pTileMgrScript->ShowRange(0);

			// 좌클릭이 들어왔을 경우 유효한 타일 내에서 선택된 타일인지 확인한다.
			if (KEY_TAP(KEY::LBTN))
			{
				CTileScript* pTargetTileScript = m_pTileMgrScript->SelectInValidTile();

				// 인접타일이 유효할 경우, 아이템에 따른 사용을 진행해준다.
				if (nullptr != pTargetTileScript)
				{
					// 효과음 진행
					CHPPotionScript* pHPPotionScript = (CHPPotionScript*)GetOwner()->GetScript((UINT)HPPOTIONSCRIPT);
					pHPPotionScript->CreatePotionUseSound();

					// 우선 임시로 체력을 10만 회복해본다.
					GetPlayerScript()->RecoveryCurHP(10);

					// 사용을 마쳤다면 객체 지워주기
					m_pUIMgrScript->UseSelectedItemCard();
				}
			}
		}
	}

	// 선택된 카드가 없다면 플레이어에게 선택된 카드 해제를 알린다.
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
	// Player에 접근해서 카드의 코스트만큼 현재 Player의 행동력을 차감해 준다.
	GetPlayerScript()->DefCurAct(m_pCardMgrScript->GetSelectedCardCost());

	// 선택된 카드를 해제해 준다.
	//m_pCardMgrScript->ReleaseSelectedCard();
	SetCardSelected(false);

	// 이제 묘지를 만들었으니 카드 객체를 지운후 묘지로 보내주어야 한다.
	m_pCardMgrScript->UseSelectedCard();

	// 인접한 발판 표시를 꺼준다.
	m_pTileMgrScript->ValidTileSetAdj(false);
}

void CGameMgrScript::TargetMonHit()
{
	// 타일이 nullptr이 아닐 때만 동작 해야한다.
	if (m_pTargetTile != nullptr)
	{
		// 대상이 몬스터 일때 공격처리가 들어간다.
		if (m_pTargetTile->GetTileTargetType() == OBJECT_TYPE::MONSTER)
		{
			CMonsterScript* pMonsterScript = GetMonsterScript(m_pTargetTile->GetTargetObject());
			pMonsterScript->Hit(m_iCardAttack, m_eWaponType);
		}
	}

	// 처리 진행을 요청한 뒤에는 멤버를 초기화 한다.
	m_pTargetTile = nullptr;
	m_iCardAttack = 0;
}

void CGameMgrScript::CreateWinLogo()
{
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\WinLogo_Prefab_0.pref");

	CGameObject* pWinLogo = pPrefab->Instantiate();

	// 생성 지점 설정해주기
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