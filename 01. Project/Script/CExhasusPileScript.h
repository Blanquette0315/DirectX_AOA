#pragma once
#include "CUIScript.h"
class CExhasusPileScript :
    public CUIScript
{
private:
    int m_iExhasusSize;

private:
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CExhasusPileScript);
public:
    CExhasusPileScript();
    ~CExhasusPileScript();
};

