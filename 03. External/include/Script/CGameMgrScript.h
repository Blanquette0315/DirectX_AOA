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
    CGameObject*            m_pPlayer;          //Player의 주소
    CTileMgrScript*         m_pTileMgrScript;   // TileMgrScript의 주소
    CCardMgrScript*         m_pCardMgrScript;   // CardMgrScript의 주소
    CUIMgrScript*           m_pUIMgrScript;     // UIMgrScript의 주소

    CLevelChangerScript*    m_pLvChangerScript; // LevelChangerScript 주소

    vector<CMonsterScript*> m_vecMonsterScript; // 필드에 존재하는 Monster의 Script의 주소를 담는 vector
    TURN_TYPE               m_eTurnType;        // 현재 누구 차례인지를 알기 위한 변수

    bool            m_bTestNow; // 테스트 진행 중일 때는 클리어 검사를 하지 않는다.

    bool            m_bCardSelected;    // 선택된 카드가 있는지 확인하는 변수

    bool            m_RefreshStateOnce; // 플레이어가 지닌 버프, 공격력등을 합산해서 카드에 넣어준다. (이 일은 Drow할때 한번 실행되어야 하므로 Once 변수가 필요하다.
                                        // 턴을 마칠때, 다시 false로 바꿔둘 것이다.

    float           m_fWinAcctime;
    bool            m_bWinOnce;

    // 공격 카드 진행을 위한 멤버 변수들
    CTileScript*             m_pTargetTile; // 타겟으로 선택되었던 타일
    int                      m_iCardAttack; // 선택되었던 공격 카드의 공격력
    WAPON_TYPE               m_eWaponType;  // 어떤 무기로 공격 받는지

    // 몇번 째 턴인지 알기 위한 카운팅
    int                     m_iTurnCount;   // 이번턴이 몇 번째 턴인지 기록한다. 

    bool                    m_bCardDiscTurnOnce;    // 카드 소멸 턴에서 유예시간을 주기위해
    float                   m_fAccCardDiscTime;

    // 덱 관련함수
    bool                    m_bMakeDeckOnce;    // 덱 만들기는 매 게임마다 한 번 진행되어야 한다.

public:
    // 각각의 주소를 등록해주는 함수
    void RegisterPlayer(CGameObject* _Player) { m_pPlayer = _Player; }
    void RegisterTileMgrScript(CTileMgrScript* _pScript) { m_pTileMgrScript = _pScript; }
    void RegisterCardMgrScript(CCardMgrScript* _pScript) { m_pCardMgrScript = _pScript; }
    void RegisterMonsterScript(CMonsterScript* _Target) { m_vecMonsterScript.push_back(_Target); }
    void RegisterUIMgrScript(CUIMgrScript* _pScript) { m_pUIMgrScript = _pScript; }
    void RegisterLvChangerScript(CLevelChangerScript* _pScript) { m_pLvChangerScript = _pScript; }

    // 반환해주는 함수
    CGameObject* GetPlayer() { return m_pPlayer; }
    CTileMgrScript* GetTileMgrScript() { return m_pTileMgrScript; }
    CCardMgrScript* GetCardMgrScript() { return m_pCardMgrScript; }
    CPlayerScript* GetPlayerScript() { return (CPlayerScript*)(m_pPlayer->GetScript((UINT)PLAYERSCRIPT)); }
    CUIMgrScript* GetUIMgrScript() { return m_pUIMgrScript; }

    // 테스트 모드로 셋팅해주는 함수
    void SetTestMod() { m_bTestNow = true; }

    // 현재 턴을 반환한다.
    int GetCurTurnCount() { return m_iTurnCount; }

    // 선택된 카드가 발생하면, bool값을 셋팅해주는 함수
    void SetCardSelected(bool _Select);

    // 선택된 카드의 값을 가져오는 함수
    tCardState GetSelectedCardState();

    // 선택된 카드의 타입을 가져오는 함수
    CARD_TYPE GetSelectedCardType();

    // 카드 사용 후 카드를 소모했다는 것을 알려주는 함수
    void UsedSelectedCard();

    // 덱에서 드로우 할때 실행할 함수
    
    // 드로우 할때, 카드 표기값으르 바꿔주는 함수
   

public:
    // Player 차례 진행 함수
    void PlayerTurnPlay();

    // Player가 타일변경을 요청할 때, 사용하는 함수
    void PlayerChangeTile(CTileScript* _TargetTile);

    // Monster 턴으로 변경해주는 함수
    void SetMonsterTurn() { m_eTurnType = TURN_TYPE::MONSTER; }

    // Card 정리 턴으로 변경해주는 함수
    void SetCardDisCTurn() { m_eTurnType = TURN_TYPE::DISCARD; m_bCardDiscTurnOnce = true; m_fAccCardDiscTime = 0.f; }

public:
    // Player의 공격 처리 함수
    void TargetMonHit();

private:
    // 랜덤 넘버 얻어오는 함수
    int GetRandNum() { std::srand(time(NULL)); return std::rand() % 37; }

    // Win Logo를 생성해 준다.
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