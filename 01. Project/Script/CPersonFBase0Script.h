#pragma once
#include <Engine/CScript.h>

class CPersonFBase0Script :
    public CScript
{
private:
    Vec3    m_vOriginPos;   // 원래 포지션
    float   m_fSpeed;       // 이동 스피드
    float   m_fDistance;    // 이동 거리

    float   m_fAccDis;      // 이동 누적 거리
    bool    m_bUp;          // 위로 이동해야하는지 확인하는 변수

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPersonFBase0Script);
public:
    CPersonFBase0Script();
    ~CPersonFBase0Script();
};