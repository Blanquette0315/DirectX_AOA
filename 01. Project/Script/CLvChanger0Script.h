#pragma once
#include "CLevelChangerScript.h"

class CLvChanger0Script :
    public CLevelChangerScript
{
private:

public:
    virtual void LevelChange() override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CLvChanger0Script);
public:
    CLvChanger0Script();
    ~CLvChanger0Script();
};