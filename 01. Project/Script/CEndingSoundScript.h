#pragma once
#include <Engine/CScript.h>

class CEndingSoundScript :
    public CScript
{
private:

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CEndingSoundScript);
public:
    CEndingSoundScript();
    ~CEndingSoundScript();
};