#pragma once
#include "CUIScript.h"

class CICardDefNumScript :
    public CUIScript
{
private:
    int m_iDef;

private:
    // 나중에 변화가 있을 때만 동작 하도록 최적화를 해주어야 한다.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CICardDefNumScript);
public:
    CICardDefNumScript();
    ~CICardDefNumScript();
};