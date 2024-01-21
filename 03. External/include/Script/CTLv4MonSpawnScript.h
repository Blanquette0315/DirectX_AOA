#pragma once
#include <Engine/CScript.h>

class CTLv4MonSpawnScript :
    public CScript
{
private:

public:
    void CreateCrossBow(int _TileIdx);

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTLv4MonSpawnScript);
public:
    CTLv4MonSpawnScript();
    ~CTLv4MonSpawnScript();
};