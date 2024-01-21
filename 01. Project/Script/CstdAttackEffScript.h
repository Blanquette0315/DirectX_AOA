#pragma once
#include <Engine/CScript.h>

class CstdAttackEffScript :
    public CScript
{
private:

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CstdAttackEffScript);
public:
    CstdAttackEffScript();
    ~CstdAttackEffScript();
};

