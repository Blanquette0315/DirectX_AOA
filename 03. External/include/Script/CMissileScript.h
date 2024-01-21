#pragma once
#include <Engine/CScript.h>

class CTileScript;

class CMissileScript :
    public CScript
{
private:
    CTileScript*    m_TargetScript;
    Vec3            m_pOwnerScale;

    Vec2            m_vDifPos;
    float           m_fAccDestance;
    float           m_fTargetDestance;
    float           m_fSeta;
    float           m_fSpeed;
    float           m_fAccel;

    int             m_iATK;

public:
    // DT���� �̵��ؾ��� Vec2�� ������ִ� �Լ�
    Vec2 CalculateDTPos();

    // ��ǥ���� Ÿ�ϱ����� ����, ������ �˾Ƴ��� �Լ�
    void CalculateTarget();

public:
    // �̵��ؾ��� Ÿ���� �������� ���� �޴´�.
    void SetTargetTile(CTileScript* _Script) { m_TargetScript = _Script; }

    // ȭ���� �� ����� ������ ������ �����Ѵ�.
    void SetOwnerPos(Vec3 _OwnerScale) { m_pOwnerScale = _OwnerScale; }

    // ȭ���� ������� ��ȯ�Ѵ�.
    int GetATK() { return m_iATK; }
    //ȭ���� ������� �����Ѵ�.
    void SetATK(int _ATK) { m_iATK = _ATK; }
public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};

