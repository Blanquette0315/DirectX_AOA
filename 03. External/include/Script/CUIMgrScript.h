#pragma once
#include <Engine/CScript.h>

class CGameMgrScript;
class CItemCardScript;

class CUIMgrScript :
    public CScript
{
private:
    CGameMgrScript* m_pGameMgrScript;

    CGameObject* m_pFocusedUI;
    CGameObject* m_pTargetUI;

    CItemCardScript* m_pSelectItemScript;  // 선택된 아이템 카드의 주소

    bool              m_bSelectedItem;      // 아이템이 선택되었음을 알려준다.
    bool              m_bNext;

public:
    bool IS_SelectedSlot() { return m_bNext; }

public:
    // 아이템 카드관련 함수들
    void SelectedItem(CItemCardScript* _TargetItem) { m_pSelectItemScript = _TargetItem; m_bSelectedItem = true; }

    // 아이템이 선택된게 있는지 가져오는 함수
    bool GetSelectedItem() { return m_bSelectedItem; }

    // GameMgr에서 선택된 아이템이 있는지 확인할 때사용하기 위한 유효성 검사
    bool IS_ItemVaild();

    // 대상 아이템 스크립트 가져오기
    CItemCardScript* GetSelectedItemScript() { return m_pSelectItemScript; }

    // 대상 아이템 카드 해제 함수
    void ReleaseSelectedCard();

    // 아이템 카드 사용시 호출할 함수
    void UseSelectedItemCard();

public:
    CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CUIMgrScript);
public:
    CUIMgrScript();
    ~CUIMgrScript();
};