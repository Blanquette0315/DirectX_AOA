#pragma once
#include <Engine/CScript.h>

class CCardMgrScript;

class CCardScript :
    public CScript
{
private:
    CCardMgrScript* m_pCardMgrScript;       // ī��Mgr�� �ּ�
    CARD_TYPE       m_eCardType;            // ī���� ���� (����, ���, �̵�)
    CGameObject*    m_pPlayer;              // Target�� Player �ּ�
    bool            m_bSelected;            // ���� ī������ ���콺�� �ö� �ִ��� üũ�ϴ� ����
    bool            m_bCardSelected;        // ���� ī�尡 ������ �Ǿ����� Ȯ���ϴ� ����

    bool            m_bCardMoveOnce;
    bool            m_bCardSelectMoveOnce;

    int             m_iHandCardIdx;         // ������ ������ ���° ��ҷ� ��ϵǾ������� �����صд�.
                                            // ó������ -1�� �ش�. -1�� Hand�� ��ϵ��� �ʾ����� �ǹ��Ѵ�.

protected:
    tCardState      m_tCardState;            // ī�� ��
    CARD_DIR_TYPE   m_eCardDirType;          // ī�� �޼� ������ Ÿ��
    bool            m_bCardReinforce;        // ī�尡 ��ȭ �Ǿ����� Ȯ��

public:
    void PlayMoveSound();
    void PlaySelectedSound();

    // ��� ī�� ��� ȿ����
    void PlayShieldCardSound();

public:
    CCardMgrScript* GetCardMgrScript() { return m_pCardMgrScript; }

public:
    // ī�� �޼� ������ Ÿ���� �����ִ� �Լ�
    void SetCardDir(CARD_DIR_TYPE _Type) { m_eCardDirType = _Type; }

    // ī�� ��ȭ ���� �������ֱ�
    void SetCardReinforce(bool _Reinforce) { m_bCardReinforce = _Reinforce; }

    // ī���� ���� ��ȯ�ϴ� �Լ�
    tCardState GetCardState() { return m_tCardState; }

    // ī���� Ÿ���� ��ȯ�ϴ� �Լ�
    CARD_TYPE GetCardType() { return m_eCardType; }

    // ī���� ���� Ÿ�� ��ȯ �Լ�
    CARD_DIR_TYPE GetCardDir() { return m_eCardDirType; }

    // ī���� ��ȭ ���� Ȯ���ϴ� �Լ�
    bool GetCardReinforce() { return m_bCardReinforce; }

    // ī�尡 CardMgr�� Hand�� �� ��° �ε����� �����ִ��� �������ִ� �Լ�
    void SetHandCardIdx(int _Idx) { m_iHandCardIdx = _Idx; }

    // �ڽ��� Hand�� �� ��° ������� ��ȯ�Ѵ�.
    int GetHandCardIdx() { return m_iHandCardIdx; }

private:
    // ��Ȳ�� �°� ī�带 �������ִ� �Լ�
    // ���Ŀ� �ش� �Լ��� ������ �ɰ� �Լ�ȭ�� ���־�� �Ѵ�.
    void CardMove();

    // ������ ������ ī��� ������ Mgr���� �˷��ش�.
    void SelectCard();

protected:
    bool IS_Selected() { return m_bCardSelected; }

    CGameObject* GetPlayer() { return m_pPlayer; }

public:
    // ī�尡 ���õ� ��Ȳ�� �������ش�.
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

