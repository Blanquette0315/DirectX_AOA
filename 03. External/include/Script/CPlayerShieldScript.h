#pragma once
#include "CUIScript.h"

class CPlayerShieldScript :
    public CUIScript
{
private:
    int m_iCurDef;

private:
    // ���߿� ��ȭ�� ���� ���� ���� �ϵ��� ����ȭ�� ���־�� �Ѵ�.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CPlayerShieldScript);
public:
    CPlayerShieldScript();
    ~CPlayerShieldScript();
};

