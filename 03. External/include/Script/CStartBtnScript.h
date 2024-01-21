#pragma once
#include "CUIScript.h"

class CStartBtnScript :
    public CUIScript
{
private:
    float m_fAccTime;
    bool  m_bSoundOnce;
    bool  m_bSelected;

public:
    void CreateBtnSound();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CStartBtnScript);
public:
    CStartBtnScript();
    ~CStartBtnScript();
};

