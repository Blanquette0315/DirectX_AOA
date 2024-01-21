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

	// 전달할 아이템 정보 구조체 만들기
	tItem_Info tItemInfo = {};

	tItem_Info tempItem = pPlayerScript->GetItemInfo(1);

	// bExist가 false일 경우 아이템이 없다는 것이다.
	// 따라서 바로 리턴한다.
	if (tempItem.bExist == false)
		return;

	// 아이템의 종류 확인 : 무기인지 아이템인지
	// 무기일 경우
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

	// 얻어온 Prefab을 토대로 아이템 만들기
	CGameObject* pItem = pItemPrefab->Instantiate();

	// 생성 후 begin 호출
	pItem->begin();

	// 랜더를 위한 z 값 지정
	pItem->Transform()->SetRelativePos_Z(-2);

	// UIMgr의 슬롯의 자식으로 설정한다.
	GetOwner()->AddChild(pItem);

	// 레이어 설정
	pItem->SetAllLayer(10);

	// 큰 카드인지 작은 카드인지 등록
	CItemCardScript* pCardScript = (CItemCardScript*)pItem->GetScript((UINT)ITEMCARDSCRIPT);
	pCardScript->SetCardSize(true);

	m_pTargetCardScript = pCardScript;

	// 아이템 구조체를 카드에게 전달
	pCardScript->SetItemInfo(tItemInfo);

	// 아이템이 Player 인밴토리 몇 번째인지 인덱스 전달
	m_iInvenIdx = 1;
}

void CBagSlot1Script::MoveItem()
{
	// 선택된 아이템 카드 스크립트 주소를 가져온다.
	CItemCardScript* pCardScript = m_pUIMgrScript->GetSelectedItemScript();

	// 대상 카드의 오너 슬롯 주소 가져오기
	CSlotScript* pTargetSlot = GetSlotScript(pCardScript->GetOwner()->GetParent());

	// 대상이 본인일 경우에는 선택 카드를 해제하고, 그냥 retun 한다.
	if (pTargetSlot == this)
	{
		m_pTargetCardScript->ReleaseSelectedCard();
		return;
	}

	// 선택된 아이템 카드가 지니고 있는 정보 가져오기
	tItem_Info tempInfo = pCardScript->GetItemInfo();

	// 변경된 아이템 정보를 Player가 지닌 아이템 정보에 반영해준다.
	ChangeInfo(tempInfo);

	// 타겟 카드가 없을 경우 예외처리를 해준다.
	if (m_pTargetCardScript != nullptr)
	{
		// 대상 카드가 원래 소속되어 있던 Slot에게 이 Slot이 지니고 있는 카드 정보로 변경하라고 전달
		pTargetSlot->ChangeInfo(m_pTargetCardScript->GetItemInfo());

		DeleteCard();
	}
	// nullptr일 경우 비워진 것으로 Info를 변경해 준다.
	else
	{
		tItem_Info tempInfo = {};
		pTargetSlot->ChangeInfo(tempInfo);
	}

	// 카드 객체 삭제
	pTargetSlot->DeleteCard();

	// 카드 다시 객체 생성
	CreateDefaultItemCard();
	pTargetSlot->CreateDefaultItemCard();

	// 효과음 출력
	CreateSetItemSound();
}

void CBagSlot1Script::ChangeInfo(tItem_Info _Info)
{
	m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript()->SetItemInfo(1, _Info);
}

void CBagSlot1Script::MouseCheck()
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
				// 선택된 아이템 카드 스크립트 주소를 가져온다.
				CItemCardScript* pCardScript = m_pUIMgrScript->GetSelectedItemScript();

				// 대상 카드의 오너 슬롯 주소 가져오기
				CSlotScript* pTargetSlot = GetSlotScript(pCardScript->GetOwner()->GetParent());

				// 지니고 있는 카드 스크립트가 없다면, true를 반환해주어야 한다.
				if (m_pTargetCardScript != nullptr)
				{
					// 선택된 아이템의 오너가 무기 슬롯이면, 본인이 무기를 지니고 있을 때만 선택되어야 한다.
					if (m_pTargetCardScript->GetItemInfo().eItemType != ITEM_TYPE::WAPON)
					{
						// 대상의 원래 부모 슬롯이 무기 슬롯일 경우 예외처리
						if (!pTargetSlot->IS_WaponSlot())
						{
							m_bLbtnPressed = true;
							// 해당 변수의 false로 해제는 각각 할일을 마치고나서 해제해주어야 한다.
						}
						else
						{
							// 에러 사운드 출력
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