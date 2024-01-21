#include "pch.h"
#include "CCardMgrScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CCardScript.h"
#include "CTileMgrScript.h"

#include "CPlayerScript.h"

CCardMgrScript::CCardMgrScript()
	: CScript(CARDMGRSCRIPT)
	, m_pGameMgr(nullptr)
	, m_pGameMgrScript(nullptr)
	, m_pSelectedCardScript(nullptr)
{
}

CCardMgrScript::~CCardMgrScript()
{
}

void CCardMgrScript::begin()
{
	m_pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)(m_pGameMgr->GetScript(GAMEMGRSCRIPT));
	m_pGameMgrScript->RegisterCardMgrScript(this);

	// Player의 덱 만들어 주기
	// Player를 싱글톤으로 만들고 나면 begin으로도 해보는 것을 고려해볼 것.
	// Player 생성 시기 때문에 꼬이는게 많기 때문
	//MakeDeck();
}

void CCardMgrScript::tick()
{
}


void CCardMgrScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CCardMgrScript::Overlap(CCollider2D* _pOther)
{
}

void CCardMgrScript::EndOverlap(CCollider2D* _pOther)
{
}


void CCardMgrScript::MakeDeck()
{
	m_arrDeck = m_pGameMgrScript->GetPlayerScript()->GetCardList();

	Shuffle();
}

void CCardMgrScript::Shuffle()
{
	int iCardSize = (int)m_arrDeck.size();

	// 난수 뽑기를 위한 시드 주기
	srand(time(nullptr));

	// 50회 랜덤 수 2개를 뽑아서 셔플해준다.
	for (int i = 0; i < 50; ++i)
	{
		int x = rand() % iCardSize;
		int y = rand() % iCardSize;

		// 만약 두 수가 동일할 경우 스왑을 진행하지 않는다.
		if (x != y)
		{
			swap(m_arrDeck[x], m_arrDeck[y]);
		}
	}
}

void CCardMgrScript::Drow()
{
	int PlayerDrowCount = m_pGameMgrScript->GetPlayerScript()->GetDrowCount();

	// 만약 드로우를 할때, 덱에 남아있는 카드가 드로우할 카드 수 보다 적다면, 묘지에있던 카드를 덱에 넣은 뒤 섞어주고, 드로우를 진행한다.
	int iDecksize2 = (int)m_arrDeck.size();
	if (PlayerDrowCount > iDecksize2)
	{
		ReturnDeck();
		Shuffle();
	}

	Ptr<CPrefab> pTargetPrefab = nullptr;

	// 제일 먼저로는 이동 카드가 패에 들어가야한다.
	pTargetPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_Move_Prefab_0.pref");
	if (nullptr != pTargetPrefab.Get())
	{
		CGameObject* pObj = pTargetPrefab->Instantiate();

		// 부모 자식 관계 설정
		GetOwner()->AddChild(pObj);

		// CardScript 가져오기
		CCardScript* pCardScript = GetCardScript(pObj);
		pCardScript->begin();

		// HandIdx 지정해주기
		pCardScript->SetHandCardIdx(0);

		//tEvent tEVN = {};
		//tEVN.eType = EVENT_TYPE::ADD_CHILD;
		//tEVN.wParam = (DWORD_PTR)pObj;
		//tEVN.lParam = (DWORD_PTR)GetOwner();
		//
		//CEventMgr::GetInst()->AddEvent(tEVN);

		m_arrHand.push_back(pObj);
	}



	pTargetPrefab = nullptr;

	// 우선 테스트용으로 드로우는 6장을 뽑는다.
	for (size_t i = 0; i < PlayerDrowCount; ++i)
	{
		//CARD_TYPE tempType = CARD_TYPE::MOVE;

		// 덱의 가장 앞부터 드로우를 한다.
		// 0 ~ 5의 index를 확인해 알맞은 Prefab을 찾는다.
		switch (m_arrDeck[i].CardName)
		{
		case CARD_TYPE::ATTACK:

		{
			//tempType = CARD_TYPE::ATTACK;

			if (m_arrDeck[i].CardDir == CARD_DIR_TYPE::LEFT)
			{
				pTargetPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_L_Attack_Prefab_0.pref");
			}
			else
			{
				pTargetPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_R_Attack_Prefab_0.pref");
			}
			break;
		}
			
		case CARD_TYPE::DEFFENCE:
		{
			//tempType = CARD_TYPE::DEFFENCE;

			if (m_arrDeck[i].CardDir == CARD_DIR_TYPE::LEFT)
			{
				pTargetPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_L_Deffence_Prefab_0.pref");
			}
			else
			{
				pTargetPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_R_Deffence_Prefab_0.pref");
			}
			break;
		}
		}
	
		// switch case 문으로 얻은 카드 프리펩이 nullptr이 아니라면,
		if (pTargetPrefab.Get() != nullptr)
		{
			CGameObject* pObj = pTargetPrefab->Instantiate();

			// 부모 자식 관계 설정
			GetOwner()->AddChild(pObj);
	
			// CardScript 가져오기
			CCardScript* pCardScript = GetCardScript(pObj);
			pCardScript->begin();
			
			// 카드의 방향, 강화 여부 지정해주기
			if (pCardScript != nullptr)
			{
				pCardScript->SetCardDir(m_arrDeck[i].CardDir);
				pCardScript->SetCardReinforce(m_arrDeck[i].Reinforce);

				// 카드에게 본인이 CardMgr의 Hand의 몇번째인지 알려준다.
				pCardScript->SetHandCardIdx(i + 1);
			}
			else
			{
				assert(!pCardScript);
			}
			
			//tEvent tEVN = {};
			//tEVN.eType = EVENT_TYPE::ADD_CHILD;
			//tEVN.wParam = (DWORD_PTR)pObj;
			//tEVN.lParam = (DWORD_PTR)GetOwner();
			//
			//CEventMgr::GetInst()->AddEvent(tEVN);

			m_arrHand.push_back(pObj);
		}
	}

	for (int i = 0; i < PlayerDrowCount; ++i)
	{
		m_arrDeck.erase(m_arrDeck.begin());
	}

	GetOwner()->SetAllLayer(10);
}

void CCardMgrScript::HandSort()
{
	int iHandSize = (int)m_arrHand.size();

	// 짝수인지 홀수 인지 판단 하기위한 판별 식
	int iLeft = iHandSize % 2;
	bool Is_Even = false; // 짝수라면 true
	
	// 홀수인지 짝수인지
	if (iLeft == 0)
		Is_Even = true;
	else
		Is_Even = false;

	int iQuotient = iHandSize / 2;
	int iCount = iQuotient;
	int iCount2 = 0;
	bool bHalf = false; // 절반을 넘어섯는지 확인

	bool MOnce = false; // 짝수일 경우 55만큼 한번씩 이동을 해주어야 한다.
	bool POnce = false;

	size_t idx = 0;

	while (idx < m_arrHand.size())
	{
		Vec3 vPos = { 0.f,0.f,200.f };

		// 홀수인지 짝수인지에 따라 달라진다.
		if (Is_Even)
		{
			if (!bHalf)
			{
				vPos.x = -55.f;
				vPos.x -= 110.f * (iCount - 1);
			}
			else
			{
				vPos.x = 55.f;
				vPos.x += 110.f * (iCount - 1);
			}
			vPos.z -= (1.f * iCount2);
		}
		else
		{
			// 아직 중앙을 넘지 못했다면,
			if (!bHalf)
				vPos.x = -110.f * iCount;
			else
				vPos.x = 110.f * iCount;

			vPos.z -= (1.f * iCount2);
		}

		// Transform 입력
		CCardScript* pCardScript = GetCardScript(m_arrHand[idx]);
		if (nullptr != pCardScript)
		{
			pCardScript->Transform()->SetRelativePos(vPos);
		}

		idx++;

		if (Is_Even)
		{
			if (!bHalf)
				iCount--;
			else
				iCount++;

			if (iCount == 0)
			{
				bHalf = true;
				iCount = 1;
			}
		}
		else
		{
			if (iCount == 0)
				bHalf = true;

			if (!bHalf)
				iCount--;
			else
				iCount++;
		}
		
		iCount2++;
	}
}

void CCardMgrScript::HandClear()
{
	GetOwner()->ClearChild();

	// 묘지로 보내기
	for (size_t i = 0; i < m_arrHand.size(); ++i)
	{
		DisCard(m_arrHand[i]);
	}

	// 객체 지우기
	Safe_Del_Vec(m_arrHand);

	m_arrHand.clear();
}

void CCardMgrScript::DisCard(CGameObject* _Target)
{
	tPlayerCardInfo tempCardInfo = {};

	// 타겟의 카드 스크립트를 가져온다.
	CCardScript* pTargetScript = GetCardScript(_Target);

	// 이동 카드는 기본 제공 카드이기 때문에 묘지로 보내면 안된다. 따라서 예외 처리
	if (pTargetScript->GetCardType() != CARD_TYPE::MOVE)
	{
		tempCardInfo.CardName = pTargetScript->GetCardType();
		tempCardInfo.CardDir = pTargetScript->GetCardDir();
		tempCardInfo.Reinforce = pTargetScript->GetCardReinforce();

		m_arrDiscard.push_back(tempCardInfo);
	}
}

void CCardMgrScript::ReturnDeck()
{
	for (size_t i = 0; i < m_arrDiscard.size(); ++i)
	{
		m_arrDeck.push_back(m_arrDiscard[i]);
	}

	// 넣어주는게 끝났다면 묘지는 비워준다.
	m_arrDiscard.clear();
}

void CCardMgrScript::UseSelectedCard()
{
	CGameObject* pTargetCard = m_pSelectedCardScript->GetOwner();

	// 묘지로 카드를 보내준다.
	DisCard(pTargetCard);

	int TargetIdx = m_pSelectedCardScript->GetHandCardIdx();

	// Hand에서도 빼주어야 한다.
	m_arrHand.erase(m_arrHand.begin() + TargetIdx);

	// 지우고 난 뒤 뒤에 있는 모든 카드의 인덱스를 앞으로 땡겨 준다.
	for (int i = TargetIdx; i < m_arrHand.size(); ++i)
	{
		CCardScript* pTargetScript =  GetCardScript(m_arrHand[i]);
		int tempIdx = pTargetScript->GetHandCardIdx();
		pTargetScript->SetHandCardIdx(tempIdx - 1);
	}

	// 선택된 카드의 객체를 지워준다.
	pTargetCard->Destroy();

	// 선택된 카드는 해제해 주어야 한다.
	m_pSelectedCardScript = nullptr;

	// 정렬다시 해주어야 한다.
	HandSort();
}

int CCardMgrScript::GetSelectedCardCost()
{
	return m_pSelectedCardScript->GetCardState().Cost;
}

void CCardMgrScript::ReleaseSelectedCard()
{
	// 선택된 카드를 해제해 준다.
	m_pSelectedCardScript->CardSelectedRelease();
	m_pSelectedCardScript = nullptr;

	m_pGameMgrScript->SetCardSelected(false);
}

void CCardMgrScript::SetSelectedCard(CCardScript* _SelectedCardScript)
{
	// 만약 카드가 처음 지정되는 거였다면, 등록해준다.
	if (nullptr == m_pSelectedCardScript)
	{
		m_pSelectedCardScript = _SelectedCardScript;
	}
	// 만약 이전에 선택된 카드가 있었다면, 이전 선택된 카드를 해제해주고, 새로운 카드를 현재 선택 카드로 등록해준다.
	else
	{
		m_pSelectedCardScript->CardSelectedRelease();
		m_pGameMgrScript->GetTileMgrScript()->ValidTileSetAdj(false);
		m_pSelectedCardScript = _SelectedCardScript;
	}

	// 게임 Mgr에 선택된 카드가 있다고 알린다.
	m_pGameMgrScript->SetCardSelected(true);
}


void CCardMgrScript::SaveToFile(FILE* _File)
{
}

void CCardMgrScript::LoadFromFile(FILE* _File)
{
}
