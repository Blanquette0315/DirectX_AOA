#pragma once
#include "CUIScript.h"

class CAttackNumScript :
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

    CLONE(CAttackNumScript);
public:
    CAttackNumScript();
    ~CAttackNumScript();
};