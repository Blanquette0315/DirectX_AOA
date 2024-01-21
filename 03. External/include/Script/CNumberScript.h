#pragma once
#include <Engine/CScript.h>

class CNumberScript :
    public CScript
{
private:
    int m_iNumber;

public:
    void SetNumber(int _Num) { m_iNumber = _Num; }

private:
    void RenderNum();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CNumberScript);
public:
    CNumberScript();
    ~CNumberScript();
};