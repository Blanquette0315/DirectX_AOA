#pragma once
#include "CUIScript.h"

class CActMonHPBarScript :
    public CUIScript
{
private:
    float m_iMaxHP;
    float m_iCurHP;

private:
    // ���߿� ��ȭ�� ���� ���� ���� �ϵ��� ����ȭ�� ���־�� �Ѵ�.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CActMonHPBarScript);
public:
    CActMonHPBarScript();
    ~CActMonHPBarScript();
};