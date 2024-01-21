#include "pch.h"
#include "CSlotScript.h"

#include "CUIMgrScript.h"

#include "CMouseCheckScript.h"

#include "CItemCardScript.h"

CSlotScript::CSlotScript(int _Script)
	: CUIScript(_Script)
	, m_bCreateOnce(false)
	, m_bWaponSlot(false)
	, m_iInvenIdx(-1)
{
}

CSlotScript::~CSlotScript()
{
}

void CSlotScript::begin()
{
	CUIScript::begin();
}

void CSlotScript::tick()
{
	// 슬롯에 위치할 아이템 카드 객체 생성
	// 이때는 플레이어가 지닌 정보를 토대로 생성한다.
	if (!m_bCreateOnce)
	{
		CreateDefaultItemCard();
		m_bCreateOnce = true;
	}

	CUIScript::tick();

	// 슬롯이 선택되었다면, Move나 Swap이 발생해야 한다.
	if (m_bLbtnPressed)
	{
		MoveItem();

		//CreateSetItemSound();
		
		m_bLbtnPressed = false;
	}
}

void CSlotScript::CreateSetItemSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Turn\\metal_object_small_move_impact_03.wav");
	pSound->Play(1, 0.8f, true);
}

void CSlotScript::CreateErrSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Btn\\Error(Button 2).wav");
	pSound->Play(1, 0.8f, true);
}

void CSlotScript::DeleteCard()
{
	m_pTargetCardScript->DeleteCard();

	m_pTargetCardScript = nullptr;
	m_pUIMgrScript->ReleaseSelectedCard();
}

void CSlotScript::MouseCheck()
{
	// 이전에 선택된 아이템이 있을 때만 해당 SlotUI 선택이 가능하다.
	// 마우스 체크
	if (m_pUIMgrScript->GetSelectedItem())
	{
		if (m_pMouseChScript->Is_MouseOverlap()
			&& !m_bLbtnPressed)
		{
			m_bMouseOn = true;
			// 마우스가 본인의 위에 있고, 좌클릭으로 선택이 되었을 때,
			if (KEY_TAP(KEY::LBTN))
			{
				m_bLbtnPressed = true;
				// 해당 변수의 false로 해제는 각각 할일을 마치고나서 해제해주어야 한다.
			}
		}
		else
		{
			m_bMouseOn = false;
		}
	}
}