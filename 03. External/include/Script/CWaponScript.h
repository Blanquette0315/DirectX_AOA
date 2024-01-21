#pragma once
#include <Engine/CScript.h>
class CWaponScript :
    public CScript
{
private:
    WAPON_TYPE m_eWaponType;    // 해당 무기의 종류

protected:
    int        m_iAtk;          // 무기의 공격력
    int        m_iDef;          // 무기의 방어력

public:
    // 무기의 타입을 가져와보는 함수
    WAPON_TYPE GetWaponType() { return m_eWaponType; }

    // 무기의 공격력을 가져오는 함수
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

