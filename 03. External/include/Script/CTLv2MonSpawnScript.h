#pragma once
#include <Engine/CScript.h>

class CTLv2MonSpawnScript :
    public CScript
{
private:

public:
    void CreateRefBeetle(int _TileIdx);

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTLv2MonSpawnScript);
public:
    CTLv2MonSpawnScript();
    ~CTLv2MonSpawnScript();
};