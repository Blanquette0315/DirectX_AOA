#pragma once
#include "CUIScript.h"

class CICardDefNumScript :
    public CUIScript
{
private:
    int m_iDef;

private:
    // ���߿� ��ȭ�� ���� ���� ���� �ϵ��� ����ȭ�� ���־�� �Ѵ�.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CICardDefNumScript);
public:
    CICardDefNumScript();
    ~CICardDefNumScript();
};