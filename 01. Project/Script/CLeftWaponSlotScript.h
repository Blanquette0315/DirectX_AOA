#pragma once
#include "CSlotScript.h"

class CLeftWaponSlotScript :
    public CSlotScript
{
private:

public:
    virtual void CreateDefaultItemCard() override;

    // ���԰��� ������ �̵� �Լ�
    virtual void MoveItem() override;

    // ���콺 üũ ��� �Լ�
    virtual void MouseCheck() override;

public:
    // ���� ���� ������ ������ ���԰� ����� �÷��̾� ������ ��������ش�.
    virtual void ChangeInfo(tItem_Info _Info) override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CLeftWaponSlotScript);
public:
    CLeftWaponSlotScript();
    ~CLeftWaponSlotScript();
};