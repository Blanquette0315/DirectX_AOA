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

	// Player�� �� ����� �ֱ�
	// Player�� �̱������� ����� ���� begin���ε� �غ��� ���� ����غ� ��.
	// Player ���� �ñ� ������ ���̴°� ���� ����
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

	// ���� �̱⸦ ���� �õ� �ֱ�
	srand(time(nullptr));

	// 50ȸ ���� �� 2���� �̾Ƽ� �������ش�.
	for (int i = 0; i < 50; ++i)
	{
		int x = rand() % iCardSize;
		int y = rand() % iCardSize;

		// ���� �� ���� ������ ��� ������ �������� �ʴ´�.
		if (x != y)
		{
			swap(m_arrDeck[x], m_arrDeck[y]);
		}
	}
}

void CCardMgrScript::Drow()
{
	int PlayerDrowCount = m_pGameMgrScript->GetPlayerScript()->GetDrowCount();

	// ���� ��ο츦 �Ҷ�, ���� �����ִ� ī�尡 ��ο��� ī�� �� ���� ���ٸ�, �������ִ� ī�带 ���� ���� �� �����ְ�, ��ο츦 �����Ѵ�.
	int iDecksize2 = (int)m_arrDeck.size();
	if (PlayerDrowCount > iDecksize2)
	{
		ReturnDeck();
		Shuffle();
	}

	Ptr<CPrefab> pTargetPrefab = nullptr;

	// ���� �����δ� �̵� ī�尡 �п� �����Ѵ�.
	pTargetPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_Move_Prefab_0.pref");
	if (nullptr != pTargetPrefab.Get())
	{
		CGameObject* pObj = pTargetPrefab->Instantiate();

		// �θ� �ڽ� ���� ����
		GetOwner()->AddChild(pObj);

		// CardScript ��������
		CCardScript* pCardScript = GetCardScript(pObj);
		pCardScript->begin();

		// HandIdx �������ֱ�
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

	// �켱 �׽�Ʈ������ ��ο�� 6���� �̴´�.
	for (size_t i = 0; i < PlayerDrowCount; ++i)
	{
		//CARD_TYPE tempType = CARD_TYPE::MOVE;

		// ���� ���� �պ��� ��ο츦 �Ѵ�.
		// 0 ~ 5�� index�� Ȯ���� �˸��� Prefab�� ã�´�.
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
	
		// switch case ������ ���� ī�� �������� nullptr�� �ƴ϶��,
		if (pTargetPrefab.Get() != nullptr)
		{
			CGameObject* pObj = pTargetPrefab->Instantiate();

			// �θ� �ڽ� ���� ����
			GetOwner()->AddChild(pObj);
	
			// CardScript ��������
			CCardScript* pCardScript = GetCardScript(pObj);
			pCardScript->begin();
			
			// ī���� ����, ��ȭ ���� �������ֱ�
			if (pCardScript != nullptr)
			{
				pCardScript->SetCardDir(m_arrDeck[i].CardDir);
				pCardScript->SetCardReinforce(m_arrDeck[i].Reinforce);

				// ī�忡�� ������ CardMgr�� Hand�� ���°���� �˷��ش�.
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

	// ¦������ Ȧ�� ���� �Ǵ� �ϱ����� �Ǻ� ��
	int iLeft = iHandSize % 2;
	bool Is_Even = false; // ¦����� true
	
	// Ȧ������ ¦������
	if (iLeft == 0)
		Is_Even = true;
	else
		Is_Even = false;

	int iQuotient = iHandSize / 2;
	int iCount = iQuotient;
	int iCount2 = 0;
	bool bHalf = false; // ������ �Ѿ���� Ȯ��

	bool MOnce = false; // ¦���� ��� 55��ŭ �ѹ��� �̵��� ���־�� �Ѵ�.
	bool POnce = false;

	size_t idx = 0;

	while (idx < m_arrHand.size())
	{
		Vec3 vPos = { 0.f,0.f,200.f };

		// Ȧ������ ¦�������� ���� �޶�����.
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
			// ���� �߾��� ���� ���ߴٸ�,
			if (!bHalf)
				vPos.x = -110.f * iCount;
			else
				vPos.x = 110.f * iCount;

			vPos.z -= (1.f * iCount2);
		}

		// Transform �Է�
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

	// ������ ������
	for (size_t i = 0; i < m_arrHand.size(); ++i)
	{
		DisCard(m_arrHand[i]);
	}

	// ��ü �����
	Safe_Del_Vec(m_arrHand);

	m_arrHand.clear();
}

void CCardMgrScript::DisCard(CGameObject* _Target)
{
	tPlayerCardInfo tempCardInfo = {};

	// Ÿ���� ī�� ��ũ��Ʈ�� �����´�.
	CCardScript* pTargetScript = GetCardScript(_Target);

	// �̵� ī��� �⺻ ���� ī���̱� ������ ������ ������ �ȵȴ�. ���� ���� ó��
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

	// �־��ִ°� �����ٸ� ������ ����ش�.
	m_arrDiscard.clear();
}

void CCardMgrScript::UseSelectedCard()
{
	CGameObject* pTargetCard = m_pSelectedCardScript->GetOwner();

	// ������ ī�带 �����ش�.
	DisCard(pTargetCard);

	int TargetIdx = m_pSelectedCardScript->GetHandCardIdx();

	// Hand������ ���־�� �Ѵ�.
	m_arrHand.erase(m_arrHand.begin() + TargetIdx);

	// ����� �� �� �ڿ� �ִ� ��� ī���� �ε����� ������ ���� �ش�.
	for (int i = TargetIdx; i < m_arrHand.size(); ++i)
	{
		CCardScript* pTargetScript =  GetCardScript(m_arrHand[i]);
		int tempIdx = pTargetScript->GetHandCardIdx();
		pTargetScript->SetHandCardIdx(tempIdx - 1);
	}

	// ���õ� ī���� ��ü�� �����ش�.
	pTargetCard->Destroy();

	// ���õ� ī��� ������ �־�� �Ѵ�.
	m_pSelectedCardScript = nullptr;

	// ���Ĵٽ� ���־�� �Ѵ�.
	HandSort();
}

int CCardMgrScript::GetSelectedCardCost()
{
	return m_pSelectedCardScript->GetCardState().Cost;
}

void CCardMgrScript::ReleaseSelectedCard()
{
	// ���õ� ī�带 ������ �ش�.
	m_pSelectedCardScript->CardSelectedRelease();
	m_pSelectedCardScript = nullptr;

	m_pGameMgrScript->SetCardSelected(false);
}

void CCardMgrScript::SetSelectedCard(CCardScript* _SelectedCardScript)
{
	// ���� ī�尡 ó�� �����Ǵ� �ſ��ٸ�, ������ش�.
	if (nullptr == m_pSelectedCardScript)
	{
		m_pSelectedCardScript = _SelectedCardScript;
	}
	// ���� ������ ���õ� ī�尡 �־��ٸ�, ���� ���õ� ī�带 �������ְ�, ���ο� ī�带 ���� ���� ī��� ������ش�.
	else
	{
		m_pSelectedCardScript->CardSelectedRelease();
		m_pGameMgrScript->GetTileMgrScript()->ValidTileSetAdj(false);
		m_pSelectedCardScript = _SelectedCardScript;
	}

	// ���� Mgr�� ���õ� ī�尡 �ִٰ� �˸���.
	m_pGameMgrScript->SetCardSelected(true);
}


void CCardMgrScript::SaveToFile(FILE* _File)
{
}

void CCardMgrScript::LoadFromFile(FILE* _File)
{
}
