#pragma once
#include <Engine/CScript.h>

class CTileMgrScript;
class CCardMgrScript;
class CMonsterScript;
class CPlayerScript;
class CUIMgrScript;
class CLevelChangerScript;

class CGameMgrScript :
    public CScript
{
private:
    CGameObject*            m_pPlayer;          //Player�� �ּ�
    CTileMgrScript*         m_pTileMgrScript;   // TileMgrScript�� �ּ�
    CCardMgrScript*         m_pCardMgrScript;   // CardMgrScript�� �ּ�
    CUIMgrScript*           m_pUIMgrScript;     // UIMgrScript�� �ּ�

    CLevelChangerScript*    m_pLvChangerScript; // LevelChangerScript �ּ�

    vector<CMonsterScript*> m_vecMonsterScript; // �ʵ忡 �����ϴ� Monster�� Script�� �ּҸ� ��� vector
    TURN_TYPE               m_eTurnType;        // ���� ���� ���������� �˱� ���� ����

    bool            m_bTestNow; // �׽�Ʈ ���� ���� ���� Ŭ���� �˻縦 ���� �ʴ´�.

    bool            m_bCardSelected;    // ���õ� ī�尡 �ִ��� Ȯ���ϴ� ����

    bool            m_RefreshStateOnce; // �÷��̾ ���� ����, ���ݷµ��� �ջ��ؼ� ī�忡 �־��ش�. (�� ���� Drow�Ҷ� �ѹ� ����Ǿ�� �ϹǷ� Once ������ �ʿ��ϴ�.
                                        // ���� ��ĥ��, �ٽ� false�� �ٲ�� ���̴�.

    float           m_fWinAcctime;
    bool            m_bWinOnce;

    // ���� ī�� ������ ���� ��� ������
    CTileScript*             m_pTargetTile; // Ÿ������ ���õǾ��� Ÿ��
    int                      m_iCardAttack; // ���õǾ��� ���� ī���� ���ݷ�
    WAPON_TYPE               m_eWaponType;  // � ����� ���� �޴���

    // ��� ° ������ �˱� ���� ī����
    int                     m_iTurnCount;   // �̹����� �� ��° ������ ����Ѵ�. 

    bool                    m_bCardDiscTurnOnce;    // ī�� �Ҹ� �Ͽ��� �����ð��� �ֱ�����
    float                   m_fAccCardDiscTime;

    // �� �����Լ�
    bool                    m_bMakeDeckOnce;    // �� ������ �� ���Ӹ��� �� �� ����Ǿ�� �Ѵ�.

public:
    // ������ �ּҸ� ������ִ� �Լ�
    void RegisterPlayer(CGameObject* _Player) { m_pPlayer = _Player; }
    void RegisterTileMgrScript(CTileMgrScript* _pScript) { m_pTileMgrScript = _pScript; }
    void RegisterCardMgrScript(CCardMgrScript* _pScript) { m_pCardMgrScript = _pScript; }
    void RegisterMonsterScript(CMonsterScript* _Target) { m_vecMonsterScript.push_back(_Target); }
    void RegisterUIMgrScript(CUIMgrScript* _pScript) { m_pUIMgrScript = _pScript; }
    void RegisterLvChangerScript(CLevelChangerScript* _pScript) { m_pLvChangerScript = _pScript; }

    // ��ȯ���ִ� �Լ�
    CGameObject* GetPlayer() { return m_pPlayer; }
    CTileMgrScript* GetTileMgrScript() { return m_pTileMgrScript; }
    CCardMgrScript* GetCardMgrScript() { return m_pCardMgrScript; }
    CPlayerScript* GetPlayerScript() { return (CPlayerScript*)(m_pPlayer->GetScript((UINT)PLAYERSCRIPT)); }
    CUIMgrScript* GetUIMgrScript() { return m_pUIMgrScript; }

    // �׽�Ʈ ���� �������ִ� �Լ�
    void SetTestMod() { m_bTestNow = true; }

    // ���� ���� ��ȯ�Ѵ�.
    int GetCurTurnCount() { return m_iTurnCount; }

    // ���õ� ī�尡 �߻��ϸ�, bool���� �������ִ� �Լ�
    void SetCardSelected(bool _Select);

    // ���õ� ī���� ���� �������� �Լ�
    tCardState GetSelectedCardState();

    // ���õ� ī���� Ÿ���� �������� �Լ�
    CARD_TYPE GetSelectedCardType();

    // ī�� ��� �� ī�带 �Ҹ��ߴٴ� ���� �˷��ִ� �Լ�
    void UsedSelectedCard();

    // ������ ��ο� �Ҷ� ������ �Լ�
    
    // ��ο� �Ҷ�, ī�� ǥ�Ⱚ���� �ٲ��ִ� �Լ�
   

public:
    // Player ���� ���� �Լ�
    void PlayerTurnPlay();

    // Player�� Ÿ�Ϻ����� ��û�� ��, ����ϴ� �Լ�
    void PlayerChangeTile(CTileScript* _TargetTile);

    // Monster ������ �������ִ� �Լ�
    void SetMonsterTurn() { m_eTurnType = TURN_TYPE::MONSTER; }

    // Card ���� ������ �������ִ� �Լ�
    void SetCardDisCTurn() { m_eTurnType = TURN_TYPE::DISCARD; m_bCardDiscTurnOnce = true; m_fAccCardDiscTime = 0.f; }

public:
    // Player�� ���� ó�� �Լ�
    void TargetMonHit();

private:
    // ���� �ѹ� ������ �Լ�
    int GetRandNum() { std::srand(time(NULL)); return std::rand() % 37; }

    // Win Logo�� ������ �ش�.
    void CreateWinLogo();
    void CreateWinSound();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CGameMgrScript);
public:
    CGameMgrScript();
    ~CGameMgrScript();
};