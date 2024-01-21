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
	// �ش� �κ��� ���߿� Prefab���� �����ϸ鼭 �������� ���̱� ������ ���߿� �����ڷ� �Űܾ� �Ѵ�.
	m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	
	m_pCardMgrScript = (CCardMgrScript*)(GetOwner()->GetParent()->GetScript(CARDMGRSCRIPT));
	
	GetOwner()->GetChildObject(L"CardBorder")->SetRender(false);
}

void CCardScript::tick()
{
	// ���콺�� ī�� ���� �ö� �ִ��� üũ�Ѵ�.
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

	// ī�尡 ������ �Ǿ��ٸ�, ItemUIMgr�� ���õ� ������ ī�尡 �ִ��� Ȯ���ؼ� ������ �ش�.
	if (m_bCardSelected)
	{
		// ���õ� �������� �ִٸ�,
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
	// ���� ī������ ���콺�� �ö� �ִٸ�,
	if (m_bSelected)
	{
		// ī�� ���� ���콺�� �ö󰬰�, ī�尡 ������ ���� ���ٸ� ������ �ش�.
		if (!m_bCardMoveOnce)
		{
			m_bCardMoveOnce = true;
			Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
			vCardPos.y += 24;
			vCardPos.z -= 2;
			GetOwner()->Transform()->SetRelativePos(vCardPos);

			//ȿ���� ����
			PlayMoveSound();
		}
		// ���콺 ��Ŭ���� ���Դٸ�, ���� ī�带 ���õ� ī��� �����Ѵ�.
		if (KEY_TAP(KEY::LBTN))
		{
			// Player�� ���� �ൿ�� ���� ī���� �ڽ�Ʈ�� �� ũ�� ����Ǹ� �ȵȴ�.
			if (m_pCardMgrScript->GetGameMgrScript()->GetPlayerScript()->GetCurAct()
				>= m_tCardState.Cost)
			{
				m_bCardSelected = true;
				// ī�尡 ���õǾ��� �� ������ ���� ���ٸ� �������ش�.
				if (!m_bCardSelectMoveOnce)
				{
					m_bCardSelectMoveOnce = true;
					Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
					vCardPos.y -= 14;
					GetOwner()->Transform()->SetRelativePos(vCardPos);

					// CardMgr�� ���õ� ī��� ������ش�.
					SelectCard();

					// ȿ���� ����
					PlaySelectedSound();
				}
			}
		}
	}

	// ���� ī������ ���콺�� �ö� ���� �ʴ´ٸ�,
	else
	{
		// ���õ� ī�尡 ���� ���� �����ؾ���.
		if (!m_bCardSelectMoveOnce)
		{
			// ���� ���� ��ġ�� ���� �ʾ����� �����ش�.
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