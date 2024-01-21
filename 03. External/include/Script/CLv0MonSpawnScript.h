#pragma once
#include <Engine/CScript.h>

class CLv0MonSpawnScript :
    public CScript
{
private:

public:
    void CreateFast(int _TileIdx);
    void CreateDemonDog(int _TileIdx);
    void CreateRefBeetle(int _TileIdx);

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CLv0MonSpawnScript);
public:
    CLv0MonSpawnScript();
    ~CLv0MonSpawnScript();
};