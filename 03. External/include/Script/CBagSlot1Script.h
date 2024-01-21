#pragma once
#include "CSlotScript.h"

class CBagSlot1Script :
    public CSlotScript
{
private:

public:
    // 슬롯이 지닐 아이템 카드 객체 생성 함수
    virtual void CreateDefaultItemCard() override;

    // 슬롯간의 아이템 이동 함수
    virtual void MoveItem() override;

    virtual void ChangeInfo(tItem_Info _Info);

    // 마우스 체크 기능 함수
    virtual void MouseCheck() override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBagSlot1Script);
public:
    CBagSlot1Script();
    ~CBagSlot1Script();
};