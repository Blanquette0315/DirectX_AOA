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

    int     m_iInvenIdx;    // �ش� ī�尡 Player�� �ι��丮 �� ��°�� ����ϰ� �ִ����� �����Ѵ�.

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
    // ������ ���� ������ ī�� ��ü ���� �Լ�
    virtual void CreateDefaultItemCard() {};

    // ���콺 üũ ��� �Լ�
    virtual void MouseCheck() override;

    // ���԰��� ������ �̵� �Լ�
    virtual void MoveItem() {};

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CSlotScript);
public:
    CSlotScript(int _Script);
    ~CSlotScript();
};