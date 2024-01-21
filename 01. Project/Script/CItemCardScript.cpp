#include "pch.h"
#include "CItemCardScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CCardMgrScript.h"
#include "CUIMgrScript.h"

#include "CMouseCheckScript.h"

#include "CPlayerScript.h"

CItemCardScript::CItemCardScript()
	: CScript(ITEMCARDSCRIPT)
	, m_pGameMgrScript(nullptr)
	, m_bSelected(false)
	, m_bCardSelected(false)
	, m_bCardMoveOnce(false)
	, m_bCardSelectMoveOnce(false)
	, m_bSmall(false)
	, m_bClear(false)
	, m_bSoundOnce(false)
{
}

CItemCardScript::~CItemCardScript()
{
}

void CItemCardScript::begin()
{
	CGameObject* pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)pGameMgr->GetScript((UINT)GAMEMGRSCRIPT);
}

void CItemCardScript::tick()
{
	// UIMgr�� ���õ� �������� ���� ���� ���콺 äũ�� �ؾ��Ѵ�.
	if (!m_pGameMgrScript->GetUIMgrScript()->GetSelectedItem())
	{
		if (m_bClear)
		{
			m_bCardSelected = false;
			m_bCardMoveOnce = false;
			m_bCardSelectMoveOnce = false;
			m_bClear = false;
		}
		else
		{
			// ���콺�� ī�� ���� �ö� �ִ��� üũ�Ѵ�.
			CMouseCheckScript* pMouseChScript = (CMouseCheckScript*)(GetOwner()->GetScript((UINT)MOUSECHECKSCRIPT));
			m_bSelected = pMouseChScript->Is_MouseOverlap();
		}
	}

	if (m_bSelected || m_bCardSelected)
	{
		GetOwner()->GetChildObject(L"CardBorder")->SetRender(true);
	}
	else
	{
		GetOwner()->GetChildObject(L"CardBorder")->SetRender(false);
	}

	CardMove();

	if (m_bCardSelected)
	{
		// ������ ī�尡 ���õǸ�, ���õ� ī�带 �������ش�.
		// ���õ� ī�尡 nullptr �̶�� ���� ������ ī�尡 ���ٴ� ���̱� ������ ���� ó��
		if (m_pGameMgrScript->GetCardMgrScript()->GetSelectedCardScript() != nullptr)
		{
			m_pGameMgrScript->GetCardMgrScript()->ReleaseSelectedCard();
		}	
	}

	if (m_bSoundOnce)
	{
		if (m_bCardSelected && !m_pGameMgrScript->GetUIMgrScript()->IS_SelectedSlot())
		{
			// ī�� ���� ȿ����
			CreateSelectCardSound();
		}
		
		m_bSoundOnce = false;
	}
}

void CItemCardScript::CreateSelectCardSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Turn\\metal_lid_movement_impact_03.wav");
	pSound->Play(1, 0.8f, true);
}

void CItemCardScript::DeleteCard()
{
	GetOwner()->Destroy();
}

void CItemCardScript::ReleaseSelectedCard()
{
	m_pGameMgrScript->GetUIMgrScript()->ReleaseSelectedCard();
	m_bClear = true;
}

void CItemCardScript::UseSelectedItemCard()
{
	// �θ��� SlotScript�� �����´�.
	CSlotScript* pSlotScript = GetSlotScript(GetOwner()->GetParent());

	tItem_Info tempInfo = {};

	int Idx = pSlotScript->GetInvenIdx();

	// -1�� ������ �ȵǾ��ٴ� ������ ����ó��
	if (Idx < 0)
	{
		assert(nullptr);
	}

	// Player �ι��丮�� �������� ���ٰ� �����־�� �Ѵ�.
	m_pGameMgrScript->GetPlayerScript()->SetItemInfo(Idx, tempInfo);

	// ���� ���� Slot�� ���� ī�� ��ũ��Ʈ�� nullptr�� ä���־�� �Ѵ�.
	pSlotScript->SetItemCard(nullptr);

	// UIMgr�� ����ī�� ������ UIMgr���� ���ش�.
	// ī�� ��ü�� �����ش�.
	DeleteCard();
}

void CItemCardScript::CardMove()
{
	// ���� ī������ ���콺�� �ö� �ִٸ�,
	if (m_bSelected)
	{
		// ī�� ���� ���콺�� �ö󰬰�, ī�尡 ������ ���� ���ٸ� ������ �ش�.
		if (!m_bCardMoveOnce)
		{
			m_bCardMoveOnce = true;
			Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
			vCardPos.y += 10;
			GetOwner()->Transform()->SetRelativePos(vCardPos);
		}
		// ���콺 ��Ŭ���� ���Դٸ�, ���� ī�带 ���õ� ī��� �����Ѵ�.
		if (KEY_TAP(KEY::LBTN) && !m_pGameMgrScript->GetUIMgrScript()->GetSelectedItem())
		{
			m_bSoundOnce = true;
			m_bCardSelected = true;
			// ī�尡 ���õǾ��� �� ������ ���� ���ٸ� �������ش�.
			if (!m_bCardSelectMoveOnce)
			{
				m_bCardSelectMoveOnce = true;
				Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
				vCardPos.y -= 10;
				GetOwner()->Transform()->SetRelativePos(vCardPos);

				// �ش� ī�尡 ���õǾ����� �˷��־�� �Ѵ�.
				// ����� UIMgr�̴�.
				m_pGameMgrScript->GetUIMgrScript()->SelectedItem(this);
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
				vCardPos.y -= 10;
				GetOwner()->Transform()->SetRelativePos(vCardPos);
			}
		}
	}
}