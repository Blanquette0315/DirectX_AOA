#pragma once
#include <Engine/CScript.h>

class CTLv0MonSpawnScript :
    public CScript
{
private:

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTLv0MonSpawnScript);
public:
    CTLv0MonSpawnScript();
    ~CTLv0MonSpawnScript();
};

