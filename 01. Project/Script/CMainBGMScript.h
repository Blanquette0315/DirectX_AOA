#pragma once
#include <Engine/CScript.h>

class CMainBGMScript :
    public CScript
{
private:
    float m_fAccTime;

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMainBGMScript);
public:
    CMainBGMScript();
    ~CMainBGMScript();
};

