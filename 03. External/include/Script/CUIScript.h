#pragma once
#include <Engine/CScript.h>

class CMouseCheckScript;
class CUIMgrScript;

class CUIScript :
    public CScript
{
private:
    
protected:
    CMouseCheckScript* m_pMouseChScript;
    CUIMgrScript* m_pUIMgrScript;

    bool                m_bMouseOn;
    bool                m_bLbtnPressed;

public:
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLBtnDown() { return m_bLbtnPressed; }

    // ���콺 üũ ��� �Լ�
    virtual void MouseCheck();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CUIScript);
public:
    CUIScript(int _Script);
    ~CUIScript();

    friend class CSlotScript;
};