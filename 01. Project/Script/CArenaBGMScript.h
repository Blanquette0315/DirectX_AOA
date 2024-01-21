#pragma once
#include <Engine/CScript.h>

class CArenaBGMScript :
    public CScript
{
private:
    float m_fAccTime;
    int   m_iBgmChannel;
    int   m_iChannel;

public:
    void SoundStop();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CArenaBGMScript);
public:
    CArenaBGMScript();
    ~CArenaBGMScript();
};

