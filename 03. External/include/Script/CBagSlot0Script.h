#pragma once
#include "CSlotScript.h"

class CBagSlot0Script :
    public CSlotScript
{
private:

public:
    // ������ ���� ������ ī�� ��ü ���� �Լ�
    virtual void CreateDefaultItemCard() override;

    // ���԰��� ������ �̵� �Լ�
    virtual void MoveItem() override;

    virtual void ChangeInfo(tItem_Info _Info);

    // ���콺 üũ ��� �Լ�
    virtual void MouseCheck() override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBagSlot0Script);
public:
    CBagSlot0Script();
    ~CBagSlot0Script();
};

