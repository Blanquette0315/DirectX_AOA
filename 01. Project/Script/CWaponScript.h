#pragma once
#include <Engine/CScript.h>
class CWaponScript :
    public CScript
{
private:
    WAPON_TYPE m_eWaponType;    // �ش� ������ ����

protected:
    int        m_iAtk;          // ������ ���ݷ�
    int        m_iDef;          // ������ ����

public:
    // ������ Ÿ���� �����ͺ��� �Լ�
    WAPON_TYPE GetWaponType() { return m_eWaponType; }

    // ������ ���ݷ��� �������� �Լ�
    int GetWaponATK() { return m_iAtk; }
    int GetWaponDEF() { return m_iDef; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CWaponScript);
public:
    CWaponScript(int _ScriptType, WAPON_TYPE _WaponType);
    ~CWaponScript();
};

