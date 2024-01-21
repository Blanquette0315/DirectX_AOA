#pragma once
#include <Engine/CScript.h>

class CstdParticleScript :
    public CScript
{
private:
    float   m_fAccTime;

public:

public:
    virtual void tick() override;


    CLONE(CstdParticleScript);
public:
    CstdParticleScript();
    ~CstdParticleScript();
};

