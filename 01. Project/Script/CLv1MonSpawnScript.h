#pragma once
#include <Engine/CScript.h>

class CLv1MonSpawnScript :
    public CScript
{
private:

public:
    void CreateCrossBow(int _TileIdx);
    void CreateChallenger(int _TileIdx);

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CLv1MonSpawnScript);
public:
    CLv1MonSpawnScript();
    ~CLv1MonSpawnScript();
};

