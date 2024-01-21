#include "pch.h"
#include "CLeftWaponSlotScript.h"

#include <Engine/CResMgr.h>

#include "CPlayerScript.h"
#include "CUIMgrScript.h"
#include "CGameMgrScript.h"

#include "CMouseCheckScript.h"
#include "CItemCardScript.h"

CLeftWaponSlotScript::CLeftWaponSlotScript()
	: CSlotScript(LEFTWAPONSLOTSCRIPT)
{
	// �ش� ������ ���� �������� �˸���.
	m_bWaponSlot = true;
}

CLeftWaponSlotScript::~CLeftWaponSlotScript()
{
}

void CLeftWaponSlotScript::begin()
{
	CSlotScript::begin();
}

void CLeftWaponSlotScript::tick()
{
	CSlotScript::tick();
}

void CLeftWaponSlotScript::CreateDefaultItemCard()
{
	Ptr<CPrefab> pWaponPrefab = nullptr;

	CPlayerScript* pPlayerScript = m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript();

	tPlayer_Wapon WaponInfo = pPlayerScript->GetLeftWaponInfo();

	switch (WaponInfo.eWaponType)
	{
	case WAPON_TYPE::KATANA:
	{
		pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_Katana_Prefab_0.pref");
		break;
	}
		
	case WAPON_TYPE::SWORD:
	{
		pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_SwordBasic_Prefab_0.pref");
		break;
	}
		
	case WAPON_TYPE::SHIELD:
	{
		pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_ShieldBasic_Prefab_0.pref");
		break;
	}

	}

	// ���� ���� ��ü ����
	CGameObject* pWaopon = pWaponPrefab->Instantiate();

	// ���� �� begin ȣ��
	pWaopon->begin();

	// ������ ���� z �� ����
	pWaopon->Transform()->SetRelativePos_Z(-2);

	// UIMgr�� ������ �ڽ����� �����Ѵ�.
	GetOwner()->AddChild(pWaopon);

	// ���̾� ����
	pWaopon->SetAllLayer(10);

	// ū ī������ ���� ī������ ���
	CItemCardScript* pCardScript = (CItemCardScript*)pWaopon->GetScript((UINT)ITEMCARDSCRIPT);
	pCardScript->SetCardSize(false);

	m_pTargetCardScript = pCardScript;

	// ������ ����ü�� ���� ī�忡�� ����
	tItem_Info tWaponInfo = {};
	tWaponInfo.eItemType = ITEM_TYPE::WAPON;
	tWaponInfo.tWapon = WaponInfo;
	tWaponInfo.bExist = true;

	pCardScript->SetItemInfo(tWaponInfo);
	
	// ���� ���� Ÿ�Կ� �ش� ������ Ÿ���� �����Ѵ�.
	pPlayerScript->SetLeftWaponType(WaponInfo.eWaponType);
}

void CLeftWaponSlotScript::MoveItem()
{
	// ���õ� ������ ī�� ��ũ��Ʈ �ּҸ� �����´�.
	CItemCardScript* pCardScript = m_pUIMgrScript->GetSelectedItemScript();

	// ��� ī���� ���� ���� �ּ� ��������
	CSlotScript* pTargetSlot = GetSlotScript(pCardScript->GetOwner()->GetParent());

	// ����� ������ ��쿡�� ���� ī�带 �����ϰ�, �׳� retun �Ѵ�.
	if (pTargetSlot == this)
	{
		//m_pUIMgrScript->ReleaseSelectedCard();
		m_pTargetCardScript->ReleaseSelectedCard();
		return;
	}

	// ���õ� ������ ī�尡 ���ϰ� �ִ� ���� ��������
	tItem_Info tempInfo = pCardScript->GetItemInfo();

	// ����� ������ ������ Player�� ���� ������ ������ �ݿ����ش�.
	ChangeInfo(tempInfo);

	// ��� ī�尡 ���� �ҼӵǾ� �ִ� Slot���� �� Slot�� ���ϰ� �ִ� ī�� ������ �����϶�� ����
	pTargetSlot->ChangeInfo(m_pTargetCardScript->GetItemInfo());

	// ī�� ��ü ����
	DeleteCard();
	pTargetSlot->DeleteCard();

	// ī�� �ٽ� ��ü ����
	CreateDefaultItemCard();
	pTargetSlot->CreateDefaultItemCard();

	// �÷��̾� �ִϸ��̼� �ٽ� �÷���
	m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript()->IDLEAnimPlay();

	// ȿ���� ���
	CreateSetItemSound();
}

void CLeftWaponSlotScript::ChangeInfo(tItem_Info _Info)
{
	if (_Info.eItemType == ITEM_TYPE::WAPON)
	{
		m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript()->SetLeftWaponInfo(_Info.tWapon);
	}
}

void CLeftWaponSlotScript::MouseCheck()
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
				// �ش� �������� ������ �����϶��� �����ϴ�.
				if (m_pUIMgrScript->GetSelectedItemScript()->GetItemInfo().eItemType == ITEM_TYPE::WAPON)
				{
					m_bLbtnPressed = true;
					// �ش� ������ false�� ������ ���� ������ ��ġ���� �������־�� �Ѵ�.
				}
			}
		}
		else
		{
			m_bMouseOn = false;
		}
	}
}