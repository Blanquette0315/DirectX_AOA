#pragma once
#include <Engine/CScript.h>

class CHitEffectScript :
    public CScript
{
private:
    Vec3 m_vOriginScale;    // ���� ������

    float m_fAccTime;       // ���� �ð�

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CHitEffectScript);
public:
    CHitEffectScript();
    ~CHitEffectScript();
};

