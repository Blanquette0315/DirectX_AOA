#pragma once
#include <Engine/CScript.h>

class CCardScript;
class CGameMgrScript;

class CCardMgrScript :
    public CScript
{
private:
    CGameObject*        m_pGameMgr;             // 게임 매니져 오브젝트 주소
    CGameMgrScript*     m_pGameMgrScript;       // 게임 매니져 스크립트 주소

    CCardScript*        m_pSelectedCardScript;  // 선택된 카드 스크립트 주소

    vector<tPlayerCardInfo>    m_arrDeck;        // 카드 덱
    vector<CGameObject*>       m_arrHand;        // 손 패

    vector<tPlayerCardInfo>    m_arrDiscard;     // 묘지
    vector<tPlayerCardInfo>    m_arrExhust;      // 소멸

public:
    // 덱관련 수치 출력 함수
    // 덱에 존재하는 카드 갯수 전달
    int GetDrawPileSize() { return (int)m_arrDeck.size(); }

    // 현재 손패에 있는 카드 갯수 전달
    int GetHandSize() { return (int)m_arrHand.size(); }

    // 소멸 덱에 있는 수 전달
    int GetExhust() { return (int)m_arrExhust.size(); }

    // 묘지에 있는 카드 갯수 전달
    int GetDiscardSize() { return (int)m_arrDiscard.size(); }

public:
    // 덱 관련 함수
    // Player가 지닌 리스트를 토대로 Deck 만들기
    void MakeDeck();

    // 덱 셔플
    void Shuffle();

    // 덱 드로우
    // 여기서 이동카드를 제일 먼저 넣어주어야 한다.
    void Drow();

    // 손 패 정렬
    void HandSort();

    // 손 패 Clear : 이때 남아있는 카드들은 모두 묘지로 보내야 한다.
    void HandClear();

    // 묘지로 카드를 보내는 함수 : 이때, 카드를 인자로 받으면, 해당 카드의 정보를 구조로 변경해 저장해둔다.
    void DisCard(CGameObject* _Target);

    // 묘지에서 덱으로 카드를 되돌려주는 함수
    void ReturnDeck();

    // 카드가 사용되고 묘지로 보낼 때 사용하는 함수
    void UseSelectedCard();

public:
    CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }

public:
    // 선택된 카드의 코스트를 가져오는 함수
    int GetSelectedCardCost();

    // 선택된 카드가 사용되어서 사라질 때 현제 지정된 카드 해제 함수
    void ReleaseSelectedCard();

    // 만약 이전에 선택된 카드가 있었다면, 이전 선택된 카드를 해제해주고, 새로운 카드를 현재 선택 카드로 등록해준다.
    void SetSelectedCard(CCardScript* _SelectedCardScript);
    
    // 선택된 카드 스크립트를 반환해주는 함수
    CCardScript* GetSelectedCardScript() { return m_pSelectedCardScript; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CCardMgrScript);
public:
    CCardMgrScript();
    ~CCardMgrScript();
};

