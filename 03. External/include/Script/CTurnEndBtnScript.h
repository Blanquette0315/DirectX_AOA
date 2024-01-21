#pragma once
#include "CUIScript.h"
class CTurnEndBtnScript :
    public CUIScript
{
private:

public:
    void CreateTurnEndSound();

public:
    virtual void begin();
    virtual void tick();

    CLONE(CTurnEndBtnScript);
public:
    CTurnEndBtnScript();
    ~CTurnEndBtnScript();
};

