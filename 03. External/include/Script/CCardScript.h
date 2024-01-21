#pragma once
#include <Engine/CScript.h>

class CCardMgrScript;

class CCardScript :
    public CScript
{
private:
    CCardMgrScript* m_pCardMgrScript;       // 카드Mgr의 주소
    CARD_TYPE       m_eCardType;            // 카드의 종류 (공격, 방어, 이동)
    CGameObject*    m_pPlayer;              // Target인 Player 주소
    bool            m_bSelected;            // 현재 카드위에 마우스가 올라가 있는지 체크하는 변수
    bool            m_bCardSelected;        // 현재 카드가 선택이 되었는지 확인하는 변수

    bool            m_bCardMoveOnce;
    bool            m_bCardSelectMoveOnce;

    int             m_iHandCardIdx;         // 본인이 손패의 몇번째 요소로 등록되었는지를 저장해둔다.
                                            // 처음에는 -1을 준다. -1을 Hand에 등록되지 않았음을 의미한다.

protected:
    tCardState      m_tCardState;            // 카드 값
    CARD_DIR_TYPE   m_eCardDirType;          // 카드 왼손 오른손 타입
    bool            m_bCardReinforce;        // 카드가 강화 되었는지 확인

public:
    void PlayMoveSound();
    void PlaySelectedSound();

    // 방어 카드 사용 효과음
    void PlayShieldCardSound();

public:
    CCardMgrScript* GetCardMgrScript() { return m_pCardMgrScript; }

public:
    // 카드 왼손 오른손 타입을 정해주는 함수
    void SetCardDir(CARD_DIR_TYPE _Type) { m_eCardDirType = _Type; }

    // 카드 강화 여부 지정해주기
    void SetCardReinforce(bool _Reinforce) { m_bCardReinforce = _Reinforce; }

    // 카드의 값을 반환하는 함수
    tCardState GetCardState() { return m_tCardState; }

    // 카드의 타입을 반환하는 함수
    CARD_TYPE GetCardType() { return m_eCardType; }

    // 카드의 방향 타입 반환 함수
    CARD_DIR_TYPE GetCardDir() { return m_eCardDirType; }

    // 카드의 강화 여부 확인하는 함수
    bool GetCardReinforce() { return m_bCardReinforce; }

    // 카드가 CardMgr의 Hand의 몇 번째 인덱스에 속해있는지 전달해주는 함수
    void SetHandCardIdx(int _Idx) { m_iHandCardIdx = _Idx; }

    // 자신이 Hand의 몇 번째 요소인지 반환한다.
    int GetHandCardIdx() { return m_iHandCardIdx; }

private:
    // 상황에 맞게 카드를 움직여주는 함수
    // 추후에 해당 함수를 단위로 쪼개 함수화를 해주어야 한다.
    void CardMove();

    // 본인을 선택한 카드로 지정해 Mgr에게 알려준다.
    void SelectCard();

protected:
    bool IS_Selected() { return m_bCardSelected; }

    CGameObject* GetPlayer() { return m_pPlayer; }

public:
    // 카드가 선택된 상황을 해제해준다.
    void CardSelectedRelease();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CCardScript);

public:
    CCardScript();
    CCardScript(int _iScriptType, CARD_TYPE _iCardType);
    ~CCardScript();
};

