#pragma once
#include "CSlotScript.h"

class CLeftWaponSlotScript :
    public CSlotScript
{
private:

public:
    virtual void CreateDefaultItemCard() override;

    // 슬롯간의 아이템 이동 함수
    virtual void MoveItem() override;

    // 마우스 체크 기능 함수
    virtual void MouseCheck() override;

public:
    // 전달 받은 아이템 정보를 슬롯과 연결된 플레이어 정보에 적용시켜준다.
    virtual void ChangeInfo(tItem_Info _Info) override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CLeftWaponSlotScript);
public:
    CLeftWaponSlotScript();
    ~CLeftWaponSlotScript();
};