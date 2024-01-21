#include "pch.h"
#include "CBagSlot1Script.h"

#include <Engine/CResMgr.h>

#include "CPlayerScript.h"
#include "CUIMgrScript.h"
#include "CGameMgrScript.h"

#include "CMouseCheckScript.h"
#include "CItemCardScript.h"

CBagSlot1Script::CBagSlot1Script()
	: CSlotScript(BAGSLOT1SCRIPT)
{
}

CBagSlot1Script::~CBagSlot1Script()
{
}

void CBagSlot1Script::begin()
{
	CSlotScript::begin();
}

void CBagSlot1Script::tick()
{
	CSlotScript::tick();
}

void CBagSlot1Script::CreateDefaultItemCard()
{
	Ptr<CPrefab> pItemPrefab = nullptr;

	CPlayerScript* pPlayerScript = m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript();

	// ������ ������ ���� ����ü �����
	tItem_Info tItemInfo = {};

	tItem_Info tempItem = pPlayerScript->GetItemInfo(1);

	// bExist�� false�� ��� �������� ���ٴ� ���̴�.
	// ���� �ٷ� �����Ѵ�.
	if (tempItem.bExist == false)
		return;

	// �������� ���� Ȯ�� : �������� ����������
	// ������ ���
	if (tempItem.eItemType == ITEM_TYPE::WAPON)
	{
		tItemInfo.eItemType = ITEM_TYPE::WAPON;
		tItemInfo.tWapon = tempItem.tWapon;
		tItemInfo.bExist = true;

		switch (tempItem.tWapon.eWaponType)
		{
		case WAPON_TYPE::KATANA:
		{
			pItemPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SCard_Katana_Prefab_0.pref");
			break;
		}

		case WAPON_TYPE::SWORD:
		{
			pItemPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SCard_SwordBasic_Prefab_0.pref");
			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			pItemPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SCard_ShieldBasic_Prefab_0.pref");
			break;
		}

		}
	}
	else if (tempItem.eItemType == ITEM_TYPE::ITEM)
	{
		tItemInfo.eItemType = ITEM_TYPE::ITEM;
		tItemInfo.tItem = tempItem.tItem;
		tItemInfo.bExist = true;

		switch (tempItem.tItem.eItemType)
		{
		case ITEM_NAME::HPPOTION:
		{
			pItemPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SCard_HPPotion_Prefab_0.pref");
			break;
		}

		}
	}

	// ���� Prefab�� ���� ������ �����
	CGameObject* pItem = pItemPrefab->Instantiate();

	// ���� �� begin ȣ��
	pItem->begin();

	// ������ ���� z �� ����
	pItem->Transform()->SetRelativePos_Z(-2);

	// UIMgr�� ������ �ڽ����� �����Ѵ�.
	GetOwner()->AddChild(pItem);

	// ���̾� ����
	pItem->SetAllLayer(10);

	// ū ī������ ���� ī������ ���
	CItemCardScript* pCardScript = (CItemCardScript*)pItem->GetScript((UINT)ITEMCARDSCRIPT);
	pCardScript->SetCardSize(true);

	m_pTargetCardScript = pCardScript;

	// ������ ����ü�� ī�忡�� ����
	pCardScript->SetItemInfo(tItemInfo);

	// �������� Player �ι��丮 �� ��°���� �ε��� ����
	m_iInvenIdx = 1;
}

void CBagSlot1Script::MoveItem()
{
	// ���õ� ������ ī�� ��ũ��Ʈ �ּҸ� �����´�.
	CItemCardScript* pCardScript = m_pUIMgrScript->GetSelectedItemScript();

	// ��� ī���� ���� ���� �ּ� ��������
	CSlotScript* pTargetSlot = GetSlotScript(pCardScript->GetOwner()->GetParent());

	// ����� ������ ��쿡�� ���� ī�带 �����ϰ�, �׳� retun �Ѵ�.
	if (pTargetSlot == this)
	{
		m_pTargetCardScript->ReleaseSelectedCard();
		return;
	}

	// ���õ� ������ ī�尡 ���ϰ� �ִ� ���� ��������
	tItem_Info tempInfo = pCardScript->GetItemInfo();

	// ����� ������ ������ Player�� ���� ������ ������ �ݿ����ش�.
	ChangeInfo(tempInfo);

	// Ÿ�� ī�尡 ���� ��� ����ó���� ���ش�.
	if (m_pTargetCardScript != nullptr)
	{
		// ��� ī�尡 ���� �ҼӵǾ� �ִ� Slot���� �� Slot�� ���ϰ� �ִ� ī�� ������ �����϶�� ����
		pTargetSlot->ChangeInfo(m_pTargetCardScript->GetItemInfo());

		DeleteCard();
	}
	// nullptr�� ��� ����� ������ Info�� ������ �ش�.
	else
	{
		tItem_Info tempInfo = {};
		pTargetSlot->ChangeInfo(tempInfo);
	}

	// ī�� ��ü ����
	pTargetSlot->DeleteCard();

	// ī�� �ٽ� ��ü ����
	CreateDefaultItemCard();
	pTargetSlot->CreateDefaultItemCard();

	// ȿ���� ���
	CreateSetItemSound();
}

void CBagSlot1Script::ChangeInfo(tItem_Info _Info)
{
	m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript()->SetItemInfo(1, _Info);
}

void CBagSlot1Script::MouseCheck()
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
				// ���õ� ������ ī�� ��ũ��Ʈ �ּҸ� �����´�.
				CItemCardScript* pCardScript = m_pUIMgrScript->GetSelectedItemScript();

				// ��� ī���� ���� ���� �ּ� ��������
				CSlotScript* pTargetSlot = GetSlotScript(pCardScript->GetOwner()->GetParent());

				// ���ϰ� �ִ� ī�� ��ũ��Ʈ�� ���ٸ�, true�� ��ȯ���־�� �Ѵ�.
				if (m_pTargetCardScript != nullptr)
				{
					// ���õ� �������� ���ʰ� ���� �����̸�, ������ ���⸦ ���ϰ� ���� ���� ���õǾ�� �Ѵ�.
					if (m_pTargetCardScript->GetItemInfo().eItemType != ITEM_TYPE::WAPON)
					{
						// ����� ���� �θ� ������ ���� ������ ��� ����ó��
						if (!pTargetSlot->IS_WaponSlot())
						{
							m_bLbtnPressed = true;
							// �ش� ������ false�� ������ ���� ������ ��ġ���� �������־�� �Ѵ�.
						}
						else
						{
							// ���� ���� ���
							CreateErrSound();
							m_bLbtnPressed = false;
						}
					}
					else
					{
						m_bLbtnPressed = true;
					}
				}
				else
				{
					m_bLbtnPressed = true;
				}
			}
		}
		else
		{
			m_bMouseOn = false;
		}
	}
}