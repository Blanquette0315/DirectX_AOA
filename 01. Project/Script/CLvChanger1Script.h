#pragma once
#include "CLevelChangerScript.h"

class CLvChanger1Script :
    public CLevelChangerScript
{
private:

public:
    virtual void LevelChange() override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CLvChanger1Script);
public:
    CLvChanger1Script();
    ~CLvChanger1Script();
};