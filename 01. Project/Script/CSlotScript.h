#pragma once
#include "CUIScript.h"

class CItemCardScript;

class CSlotScript :
    public CUIScript
{
private:
    bool    m_bCreateOnce;

protected:
    CItemCardScript* m_pTargetCardScript;
    bool    m_bWaponSlot;

    int     m_iInvenIdx;    // 해당 카드가 Player의 인밴토리 몇 번째를 담당하고 있는지를 저장한다.

public:
    void CreateSetItemSound();
    void CreateErrSound();

public:
    void SetItemCard(CItemCardScript* _Target) { m_pTargetCardScript = _Target ;}
    CItemCardScript* GetItemCard() { return m_pTargetCardScript; }

    virtual void ChangeInfo(tItem_Info _Info) {};

    void DeleteCard();

    bool IS_WaponSlot() { return m_bWaponSlot; }

    int GetInvenIdx() { return m_iInvenIdx; }

public:
    // 슬롯이 지닐 아이템 카드 객체 생성 함수
    virtual void CreateDefaultItemCard() {};

    // 마우스 체크 기능 함수
    virtual void MouseCheck() override;

    // 슬롯간의 아이템 이동 함수
    virtual void MoveItem() {};

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CSlotScript);
public:
    CSlotScript(int _Script);
    ~CSlotScript();
};