#pragma once
#include "CUIScript.h"

class CCardAttNumScript :
    public CUIScript
{
private:
    int m_iAttack;

private:
    // ���߿� ��ȭ�� ���� ���� ���� �ϵ��� ����ȭ�� ���־�� �Ѵ�.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CCardAttNumScript);
public:
    CCardAttNumScript();
    ~CCardAttNumScript();
};