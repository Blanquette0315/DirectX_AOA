#pragma once
#include <Engine/CScript.h>

class CTLv3MonSpawnScript :
    public CScript
{
private:

public:
    void CreateChallenger(int _TileIdx);

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTLv3MonSpawnScript);
public:
    CTLv3MonSpawnScript();
    ~CTLv3MonSpawnScript();
};