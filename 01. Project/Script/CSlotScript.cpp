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
	// ���Կ� ��ġ�� ������ ī�� ��ü ����
	// �̶��� �÷��̾ ���� ������ ���� �����Ѵ�.
	if (!m_bCreateOnce)
	{
		CreateDefaultItemCard();
		m_bCreateOnce = true;
	}

	CUIScript::tick();

	// ������ ���õǾ��ٸ�, Move�� Swap�� �߻��ؾ� �Ѵ�.
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
	// ������ ���õ� �������� ���� ���� �ش� SlotUI ������ �����ϴ�.
	// ���콺 üũ
	if (m_pUIMgrScript->GetSelectedItem())
	{
		if (m_pMouseChScript->Is_MouseOverlap()
			&& !m_bLbtnPressed)
		{
			m_bMouseOn = true;
			// ���콺�� ������ ���� �ְ�, ��Ŭ������ ������ �Ǿ��� ��,
			if (KEY_TAP(KEY::LBTN))
			{
				m_bLbtnPressed = true;
				// �ش� ������ false�� ������ ���� ������ ��ġ���� �������־�� �Ѵ�.
			}
		}
		else
		{
			m_bMouseOn = false;
		}
	}
}