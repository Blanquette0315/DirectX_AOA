#pragma once
#include "CSlotScript.h"

class CRightWaponSlotScript :
    public CSlotScript
{
private:

public:
    virtual void CreateDefaultItemCard() override;

    // ���԰��� ������ �̵� �Լ�
    virtual void MoveItem() override;

public:
    // ���� ���� ������ ������ ���԰� ����� �÷��̾� ������ ��������ش�.
    virtual void ChangeInfo(tItem_Info _Info) override;

    // ���콺 üũ ��� �Լ�
    virtual void MouseCheck() override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CRightWaponSlotScript);
public:
    CRightWaponSlotScript();
    ~CRightWaponSlotScript();
};