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
	// 해당 슬롯이 무기 슬롯임을 알린다.
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

	// 얻어온 무기 객체 생성
	CGameObject* pWaopon = pWaponPrefab->Instantiate();

	// 생성 후 begin 호출
	pWaopon->begin();

	// 랜더를 위한 z 값 지정
	pWaopon->Transform()->SetRelativePos_Z(-2);

	// UIMgr의 슬롯의 자식으로 설정한다.
	GetOwner()->AddChild(pWaopon);

	// 레이어 설정
	pWaopon->SetAllLayer(10);

	// 큰 카드인지 작은 카드인지 등록
	CItemCardScript* pCardScript = (CItemCardScript*)pWaopon->GetScript((UINT)ITEMCARDSCRIPT);
	pCardScript->SetCardSize(false);

	m_pTargetCardScript = pCardScript;

	// 아이템 구조체를 만들어서 카드에게 전달
	tItem_Info tWaponInfo = {};
	tWaponInfo.eItemType = ITEM_TYPE::WAPON;
	tWaponInfo.tWapon = WaponInfo;
	tWaponInfo.bExist = true;

	pCardScript->SetItemInfo(tWaponInfo);
	
	// 장착 무기 타입에 해당 무기의 타입을 저장한다.
	pPlayerScript->SetLeftWaponType(WaponInfo.eWaponType);
}

void CLeftWaponSlotScript::MoveItem()
{
	// 선택된 아이템 카드 스크립트 주소를 가져온다.
	CItemCardScript* pCardScript = m_pUIMgrScript->GetSelectedItemScript();

	// 대상 카드의 오너 슬롯 주소 가져오기
	CSlotScript* pTargetSlot = GetSlotScript(pCardScript->GetOwner()->GetParent());

	// 대상이 본인일 경우에는 선택 카드를 해제하고, 그냥 retun 한다.
	if (pTargetSlot == this)
	{
		//m_pUIMgrScript->ReleaseSelectedCard();
		m_pTargetCardScript->ReleaseSelectedCard();
		return;
	}

	// 선택된 아이템 카드가 지니고 있는 정보 가져오기
	tItem_Info tempInfo = pCardScript->GetItemInfo();

	// 변경된 아이템 정보를 Player가 지닌 아이템 정보에 반영해준다.
	ChangeInfo(tempInfo);

	// 대상 카드가 원래 소속되어 있던 Slot에게 이 Slot이 지니고 있는 카드 정보로 변경하라고 전달
	pTargetSlot->ChangeInfo(m_pTargetCardScript->GetItemInfo());

	// 카드 객체 삭제
	DeleteCard();
	pTargetSlot->DeleteCard();

	// 카드 다시 객체 생성
	CreateDefaultItemCard();
	pTargetSlot->CreateDefaultItemCard();

	// 플레이어 애니메이션 다시 플레이
	m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript()->IDLEAnimPlay();

	// 효과음 출력
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
				// 해당 아이템의 종류가 무기일때만 가능하다.
				if (m_pUIMgrScript->GetSelectedItemScript()->GetItemInfo().eItemType == ITEM_TYPE::WAPON)
				{
					m_bLbtnPressed = true;
					// 해당 변수의 false로 해제는 각각 할일을 마치고나서 해제해주어야 한다.
				}
			}
		}
		else
		{
			m_bMouseOn = false;
		}
	}
}