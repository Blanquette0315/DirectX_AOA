#pragma once
#include <Engine/CScript.h>

class CTLv1MonSpawnScript :
    public CScript
{
private:

public:
    void CreateFast(int _TileIdx);

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTLv1MonSpawnScript);
public:
    CTLv1MonSpawnScript();
    ~CTLv1MonSpawnScript();
};