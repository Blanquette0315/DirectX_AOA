#pragma once
#include "CUIScript.h"

class CDrawPileScript :
    public CUIScript
{
private:
    int     m_iDeckSize;

private:
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CDrawPileScript);
public:
    CDrawPileScript();
    ~CDrawPileScript();
};

