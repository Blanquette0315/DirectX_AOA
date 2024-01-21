#include "pch.h"
#include "CCardScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CSound.h>

#include "CGameMgrScript.h"
#include "CCardMgrScript.h"
#include "CUIMgrScript.h"

#include "CItemCardScript.h"

#include "CMouseCheckScript.h"
#include "CPlayerScript.h"


CCardScript::CCardScript()
	: CScript(CARDSCRIPT)
	, m_pCardMgrScript(nullptr)
	, m_pPlayer(nullptr)
	, m_bSelected(false)
	, m_bCardSelected(false)
	, m_bCardMoveOnce(false)
	, m_bCardSelectMoveOnce(false)
	, m_iHandCardIdx(-1)
{
}

CCardScript::CCardScript(int _iScriptType, CARD_TYPE _iCardType)
	: CScript(_iScriptType)
	, m_pCardMgrScript(nullptr)
	, m_eCardType(_iCardType)
	, m_pPlayer(nullptr)
	, m_bSelected(false)
	, m_bCardSelected(false)
	, m_bCardMoveOnce(false)
	, m_bCardSelectMoveOnce(false)
	, m_tCardState{}
	, m_iHandCardIdx(-1)
{
}

CCardScript::~CCardScript()
{
}

void CCardScript::begin()
{
	// 해당 부분은 나중에 Prefab으로 생성하면서 지정해줄 것이기 떄문에 나중에 생성자로 옮겨야 한다.
	m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	
	m_pCardMgrScript = (CCardMgrScript*)(GetOwner()->GetParent()->GetScript(CARDMGRSCRIPT));
	
	GetOwner()->GetChildObject(L"CardBorder")->SetRender(false);
}

void CCardScript::tick()
{
	// 마우스가 카드 위에 올라가 있는지 체크한다.
	CMouseCheckScript* pMouseChScript = (CMouseCheckScript*)(GetOwner()->GetScript((UINT)MOUSECHECKSCRIPT));
	m_bSelected = pMouseChScript->Is_MouseOverlap();

	if (m_bSelected || m_bCardSelected)
	{
		GetOwner()->GetChildObject(L"CardBorder")->SetRender(true);
	}
	else
	{
		GetOwner()->GetChildObject(L"CardBorder")->SetRender(false);
	}

	CardMove();

	// 카드가 선택이 되었다면, ItemUIMgr에 선택된 아이템 카드가 있는지 확인해서 해제해 준다.
	if (m_bCardSelected)
	{
		// 선택된 아이템이 있다면,
		if (m_pCardMgrScript->GetGameMgrScript()->GetUIMgrScript()->GetSelectedItem())
		{
			m_pCardMgrScript->GetGameMgrScript()->GetUIMgrScript()->GetSelectedItemScript()->ReleaseSelectedCard();
		}
	}
}

void CCardScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CCardScript::Overlap(CCollider2D* _pOther)
{
}

void CCardScript::EndOverlap(CCollider2D* _pOther)
{
}


void CCardScript::SaveToFile(FILE* _File)
{
}

void CCardScript::LoadFromFile(FILE* _File)
{
}


void CCardScript::PlayMoveSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Card\\CardMove_0(Crumbling Printer Paper 9).wav");
	pSound->Play(1, 0.8f, true);
}

void CCardScript::PlaySelectedSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Card\\CardSelect_0(PM_BA_BOOK_2_6 Book, Paper, Page Turn).wav");
	pSound->Play(1, 1.f, true);
}

void CCardScript::PlayShieldCardSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\AddArmor_1(Robotic Lifeforms 2 - Impacts Source - Metal Tool Drawer Punch 03).wav");
	pSound->Play(1, 1.f, true);
}

void CCardScript::CardMove()
{
	// 만약 카드위에 마우스가 올라가 있다면,
	if (m_bSelected)
	{
		// 카드 위에 마우스가 올라갔고, 카드가 움직인 적이 없다면 움직여 준다.
		if (!m_bCardMoveOnce)
		{
			m_bCardMoveOnce = true;
			Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
			vCardPos.y += 24;
			vCardPos.z -= 2;
			GetOwner()->Transform()->SetRelativePos(vCardPos);

			//효과음 실행
			PlayMoveSound();
		}
		// 마우스 좌클릭이 들어왔다면, 현재 카드를 선택된 카드로 생각한다.
		if (KEY_TAP(KEY::LBTN))
		{
			// Player의 현재 행동력 보다 카드의 코스트가 더 크면 진행되면 안된다.
			if (m_pCardMgrScript->GetGameMgrScript()->GetPlayerScript()->GetCurAct()
				>= m_tCardState.Cost)
			{
				m_bCardSelected = true;
				// 카드가 선택되었을 때 움직인 적이 없다면 움직여준다.
				if (!m_bCardSelectMoveOnce)
				{
					m_bCardSelectMoveOnce = true;
					Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
					vCardPos.y -= 14;
					GetOwner()->Transform()->SetRelativePos(vCardPos);

					// CardMgr에 선택된 카드로 등록해준다.
					SelectCard();

					// 효과음 실행
					PlaySelectedSound();
				}
			}
		}
	}

	// 만약 카드위에 마우스가 올라가 있지 않는다면,
	else
	{
		// 선택된 카드가 없을 때만 동작해야함.
		if (!m_bCardSelectMoveOnce)
		{
			// 아직 원래 위치로 가지 않았으면 보내준다.
			if (m_bCardMoveOnce)
			{
				m_bCardMoveOnce = false;
				Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
				vCardPos.y -= 24;
				vCardPos.z += 2;
				GetOwner()->Transform()->SetRelativePos(vCardPos);
			}
		}
	}
}

void CCardScript::SelectCard()
{
	m_pCardMgrScript->SetSelectedCard(this);
}

void CCardScript::CardSelectedRelease()
{
	m_bCardSelectMoveOnce = false;
	m_bCardMoveOnce = false;
	m_bCardSelected = false;
	Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
	vCardPos.y -= 10;
	vCardPos.z += 2;
	GetOwner()->Transform()->SetRelativePos(vCardPos);
}