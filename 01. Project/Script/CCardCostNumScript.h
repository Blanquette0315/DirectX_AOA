#pragma once
#include "CUIScript.h"

class CCardCostNumScript :
    public CUIScript
{
private:
    int m_iCost;

private:
    // ���߿� ��ȭ�� ���� ���� ���� �ϵ��� ����ȭ�� ���־�� �Ѵ�.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CCardCostNumScript);
public:
    CCardCostNumScript();
    ~CCardCostNumScript();
};

