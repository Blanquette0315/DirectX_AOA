#pragma once
#include "CUIScript.h"

class CICardAttNumScript :
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
    
    CLONE(CICardAttNumScript);
public:
    CICardAttNumScript();
    ~CICardAttNumScript();
};

