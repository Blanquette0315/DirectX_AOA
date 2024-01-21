#pragma once
#include <Engine/CScript.h>

class CUnderAttackScript :
    public CScript
{
private:
    bool    m_bRenderOnOff;
    bool    m_CurRender;
    float   m_fAccTime;

public:
    void RenderOn() { m_bRenderOnOff = true; m_fAccTime = 0.f; MeshRender()->GetSharedMaterial(); }
    void RenderOff() { m_bRenderOnOff = false; }

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CUnderAttackScript);
public:
    CUnderAttackScript();
    ~CUnderAttackScript();
};