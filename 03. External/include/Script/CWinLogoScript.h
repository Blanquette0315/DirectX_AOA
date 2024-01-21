#pragma once
#include "CUIScript.h"

class CWinLogoScript :
    public CUIScript
{
private:
    float   m_fAccTime;

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CWinLogoScript);
public:
    CWinLogoScript();
    ~CWinLogoScript();
};