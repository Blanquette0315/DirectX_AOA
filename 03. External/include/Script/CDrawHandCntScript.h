#pragma once
#include "CUIScript.h"

class CDrawHandCntScript :
    public CUIScript
{
private:

private:
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CDrawHandCntScript);
public:
    CDrawHandCntScript();
    ~CDrawHandCntScript();
};

