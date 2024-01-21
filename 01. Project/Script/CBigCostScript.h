#pragma once
#include "CUIScript.h"

class CPlayerScript;

class CBigCostScript :
    public CUIScript
{
private:
    CPlayerScript* m_pPlayerScript;

private:
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBigCostScript);
public:
    CBigCostScript();
    ~CBigCostScript();
};