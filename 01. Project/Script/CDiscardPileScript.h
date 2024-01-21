#pragma once
#include "CUIScript.h"

class CDiscardPileScript :
    public CUIScript
{
private:
    int     m_iDiscardSize;

private:
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CDiscardPileScript);
public:
    CDiscardPileScript();
    ~CDiscardPileScript();
};