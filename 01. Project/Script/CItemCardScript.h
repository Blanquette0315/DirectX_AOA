#pragma once
#include <Engine/CScript.h>

class CGameMgrScript;

class CItemCardScript :
    public CScript
{
public:
    CGameMgrScript* m_pGameMgrScript;
    tItem_Info      m_tItemInfo;

    bool            m_bSelected;        // ���� ī������ ���콺�� �ö� �ִ��� üũ�ϴ� ����
    bool            m_bCardSelected;    // ���� ī�尡 ������ �Ǿ����� Ȯ���ϴ� ����

    bool            m_bCardMoveOnce;
    bool            m_bCardSelectMoveOnce;

    bool            m_bClear;

    bool            m_bSmall;   // Bag�� �־ ���� ī��� ������ �������� üũ

    bool            m_bSoundOnce;

public:
    void CreateSelectCardSound();

public:
    void SetCardSize(bool _Size) { m_bSmall == _Size; };
    void SetItemInfo(tItem_Info _Info) { m_tItemInfo = _Info; }

    tItem_Info GetItemInfo() { return m_tItemInfo; }

    // ������ ī�� ���� �Լ�
    void DeleteCard();

    // ������ ī�� ���� ���� �Լ�
    void ReleaseSelectedCard();

    // �������� ���Ǹ� ȣ��� �Լ�
    void UseSelectedItemCard();

private:
    // ��Ȳ�� �°� ī�带 �������ִ� �Լ�
    void CardMove();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CItemCardScript);
public:
    CItemCardScript();
    ~CItemCardScript();
};

