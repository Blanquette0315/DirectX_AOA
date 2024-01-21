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
	// UIMgr에 선택된 아이템이 없을 때만 마우스 채크를 해야한다.
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
			// 마우스가 카드 위에 올라가 있는지 체크한다.
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
		// 아이템 카드가 선택되면, 선택된 카드를 해제해준다.
		// 선택된 카드가 nullptr 이라는 것은 선택한 카드가 없다는 것이기 때문에 예외 처리
		if (m_pGameMgrScript->GetCardMgrScript()->GetSelectedCardScript() != nullptr)
		{
			m_pGameMgrScript->GetCardMgrScript()->ReleaseSelectedCard();
		}	
	}

	if (m_bSoundOnce)
	{
		if (m_bCardSelected && !m_pGameMgrScript->GetUIMgrScript()->IS_SelectedSlot())
		{
			// 카드 선택 효과음
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
	// 부모의 SlotScript를 가져온다.
	CSlotScript* pSlotScript = GetSlotScript(GetOwner()->GetParent());

	tItem_Info tempInfo = {};

	int Idx = pSlotScript->GetInvenIdx();

	// -1은 지정이 안되었다는 것으로 예외처리
	if (Idx < 0)
	{
		assert(nullptr);
	}

	// Player 인밴토리에 아이템이 없다고 지워주어야 한다.
	m_pGameMgrScript->GetPlayerScript()->SetItemInfo(Idx, tempInfo);

	// 원래 소유 Slot에 지닌 카드 스크립트를 nullptr로 채워주어야 한다.
	pSlotScript->SetItemCard(nullptr);

	// UIMgr의 선택카드 해제는 UIMgr에서 해준다.
	// 카드 객체를 지워준다.
	DeleteCard();
}

void CItemCardScript::CardMove()
{
	// 만약 카드위에 마우스가 올라가 있다면,
	if (m_bSelected)
	{
		// 카드 위에 마우스가 올라갔고, 카드가 움직인 적이 없다면 움직여 준다.
		if (!m_bCardMoveOnce)
		{
			m_bCardMoveOnce = true;
			Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
			vCardPos.y += 10;
			GetOwner()->Transform()->SetRelativePos(vCardPos);
		}
		// 마우스 좌클릭이 들어왔다면, 현재 카드를 선택된 카드로 생각한다.
		if (KEY_TAP(KEY::LBTN) && !m_pGameMgrScript->GetUIMgrScript()->GetSelectedItem())
		{
			m_bSoundOnce = true;
			m_bCardSelected = true;
			// 카드가 선택되었을 때 움직인 적이 없다면 움직여준다.
			if (!m_bCardSelectMoveOnce)
			{
				m_bCardSelectMoveOnce = true;
				Vec3 vCardPos = GetOwner()->Transform()->GetRelativePos();
				vCardPos.y -= 10;
				GetOwner()->Transform()->SetRelativePos(vCardPos);

				// 해당 카드가 선택되었음을 알려주어야 한다.
				// 대상은 UIMgr이다.
				m_pGameMgrScript->GetUIMgrScript()->SelectedItem(this);
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
				vCardPos.y -= 10;
				GetOwner()->Transform()->SetRelativePos(vCardPos);
			}
		}
	}
}