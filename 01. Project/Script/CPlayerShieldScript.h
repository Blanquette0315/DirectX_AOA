#pragma once
#include "CUIScript.h"

class CPlayerShieldScript :
    public CUIScript
{
private:
    int m_iCurDef;

private:
    // 나중에 변화가 있을 때만 동작 하도록 최적화를 해주어야 한다.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CPlayerShieldScript);
public:
    CPlayerShieldScript();
    ~CPlayerShieldScript();
};

