#pragma once
#include <Engine/CScript.h>

class CLevelChangerScript :
    public CScript
{
private:

public:
    virtual void LevelChange() {};

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CLevelChangerScript);
public:
    CLevelChangerScript(int SCRIPT);
    ~CLevelChangerScript();
};