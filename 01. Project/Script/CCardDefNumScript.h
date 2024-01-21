#pragma once
#include "CUIScript.h"

class CCardDefNumScript :
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

    CLONE(CCardDefNumScript);
public:
    CCardDefNumScript();
    ~CCardDefNumScript();
};

