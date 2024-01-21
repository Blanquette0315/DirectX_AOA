#pragma once
#include "CUIScript.h"

class CHandCardLimitScript :
    public CUIScript
{
private:
   

private:
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CHandCardLimitScript);
public:
    CHandCardLimitScript();
    ~CHandCardLimitScript();
};

