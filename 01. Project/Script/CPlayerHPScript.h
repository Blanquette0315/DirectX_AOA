#pragma once
#include "CUIScript.h"

class CPlayerScript;

class CPlayerHPScript :
    public CUIScript
{
private:
    CPlayerScript* m_pPlayerScript;

    int m_iMaxHP;
    int m_iCurHP;

private:
    // 나중에 변화가 있을 때만 동작 하도록 최적화를 해주어야 한다.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerHPScript);
public:
    CPlayerHPScript();
    ~CPlayerHPScript();
};