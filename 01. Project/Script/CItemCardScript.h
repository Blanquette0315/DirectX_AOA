#pragma once
#include <Engine/CScript.h>

class CGameMgrScript;

class CItemCardScript :
    public CScript
{
public:
    CGameMgrScript* m_pGameMgrScript;
    tItem_Info      m_tItemInfo;

    bool            m_bSelected;        // 현재 카드위에 마우스가 올라가 있는지 체크하는 변수
    bool            m_bCardSelected;    // 현재 카드가 선택이 되었는지 확인하는 변수

    bool            m_bCardMoveOnce;
    bool            m_bCardSelectMoveOnce;

    bool            m_bClear;

    bool            m_bSmall;   // Bag에 있어서 스몰 카드로 생성된 상태인지 체크

    bool            m_bSoundOnce;

public:
    void CreateSelectCardSound();

public:
    void SetCardSize(bool _Size) { m_bSmall == _Size; };
    void SetItemInfo(tItem_Info _Info) { m_tItemInfo = _Info; }

    tItem_Info GetItemInfo() { return m_tItemInfo; }

    // 아이템 카드 삭제 함수
    void DeleteCard();

    // 아이템 카드 선택 해제 함수
    void ReleaseSelectedCard();

    // 아이템이 사용되면 호출될 함수
    void UseSelectedItemCard();

private:
    // 상황에 맞게 카드를 움직여주는 함수
    void CardMove();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CItemCardScript);
public:
    CItemCardScript();
    ~CItemCardScript();
};

