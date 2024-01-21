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
    // DT동안 이동해야할 Vec2를 계산해주는 함수
    Vec2 CalculateDTPos();

    // 목표로한 타일까지의 각도, 방향을 알아내는 함수
    void CalculateTarget();

public:
    // 이동해야할 타일의 포지션을 전달 받는다.
    void SetTargetTile(CTileScript* _Script) { m_TargetScript = _Script; }

    // 화살을 쏜 대상의 포지션 정보를 저장한다.
    void SetOwnerPos(Vec3 _OwnerScale) { m_pOwnerScale = _OwnerScale; }

    // 화살의 대미지를 반환한다.
    int GetATK() { return m_iATK; }
    //화살의 대미지를 셋팅한다.
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

