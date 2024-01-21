#pragma once
#include <Engine/CScript.h>

class CCardScript;
class CGameMgrScript;

class CCardMgrScript :
    public CScript
{
private:
    CGameObject*        m_pGameMgr;             // ���� �Ŵ��� ������Ʈ �ּ�
    CGameMgrScript*     m_pGameMgrScript;       // ���� �Ŵ��� ��ũ��Ʈ �ּ�

    CCardScript*        m_pSelectedCardScript;  // ���õ� ī�� ��ũ��Ʈ �ּ�

    vector<tPlayerCardInfo>    m_arrDeck;        // ī�� ��
    vector<CGameObject*>       m_arrHand;        // �� ��

    vector<tPlayerCardInfo>    m_arrDiscard;     // ����
    vector<tPlayerCardInfo>    m_arrExhust;      // �Ҹ�

public:
    // ������ ��ġ ��� �Լ�
    // ���� �����ϴ� ī�� ���� ����
    int GetDrawPileSize() { return (int)m_arrDeck.size(); }

    // ���� ���п� �ִ� ī�� ���� ����
    int GetHandSize() { return (int)m_arrHand.size(); }

    // �Ҹ� ���� �ִ� �� ����
    int GetExhust() { return (int)m_arrExhust.size(); }

    // ������ �ִ� ī�� ���� ����
    int GetDiscardSize() { return (int)m_arrDiscard.size(); }

public:
    // �� ���� �Լ�
    // Player�� ���� ����Ʈ�� ���� Deck �����
    void MakeDeck();

    // �� ����
    void Shuffle();

    // �� ��ο�
    // ���⼭ �̵�ī�带 ���� ���� �־��־�� �Ѵ�.
    void Drow();

    // �� �� ����
    void HandSort();

    // �� �� Clear : �̶� �����ִ� ī����� ��� ������ ������ �Ѵ�.
    void HandClear();

    // ������ ī�带 ������ �Լ� : �̶�, ī�带 ���ڷ� ������, �ش� ī���� ������ ������ ������ �����صд�.
    void DisCard(CGameObject* _Target);

    // �������� ������ ī�带 �ǵ����ִ� �Լ�
    void ReturnDeck();

    // ī�尡 ���ǰ� ������ ���� �� ����ϴ� �Լ�
    void UseSelectedCard();

public:
    CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }

public:
    // ���õ� ī���� �ڽ�Ʈ�� �������� �Լ�
    int GetSelectedCardCost();

    // ���õ� ī�尡 ���Ǿ ����� �� ���� ������ ī�� ���� �Լ�
    void ReleaseSelectedCard();

    // ���� ������ ���õ� ī�尡 �־��ٸ�, ���� ���õ� ī�带 �������ְ�, ���ο� ī�带 ���� ���� ī��� ������ش�.
    void SetSelectedCard(CCardScript* _SelectedCardScript);
    
    // ���õ� ī�� ��ũ��Ʈ�� ��ȯ���ִ� �Լ�
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

