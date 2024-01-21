#pragma once
#include <Engine/CScript.h>

class CTLv5MonSpawnScript :
    public CScript
{
private:

public:
    void CreateDemonDog(int _TileIdx);

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTLv5MonSpawnScript);
public:
    CTLv5MonSpawnScript();
    ~CTLv5MonSpawnScript();
};