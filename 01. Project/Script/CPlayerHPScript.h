#pragma once
#include "CUIScript.h"

class CPlayerScript;

class CPlayerHPScript :
    public CUIScript
{
private:
    CPlayerScript* m_pPlayerScript;

    int m_iMaxHP;
    int m_iCurHP;

private:
    // ���߿� ��ȭ�� ���� ���� ���� �ϵ��� ����ȭ�� ���־�� �Ѵ�.
    void RenderNumber();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerHPScript);
public:
    CPlayerHPScript();
    ~CPlayerHPScript();
};