#pragma once
#include <Engine/CScript.h>

class CHitEffectScript :
    public CScript
{
private:
    Vec3 m_vOriginScale;    // 본래 스케일

    float m_fAccTime;       // 누적 시간

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CHitEffectScript);
public:
    CHitEffectScript();
    ~CHitEffectScript();
};

