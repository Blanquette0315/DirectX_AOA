#pragma once
#include <Engine/CScript.h>

class CBloodEffectScript :
    public CScript
{
private:
    bool m_bRotOwner;

public:
    // 피 애니메이션 두개 중 랜덤하게 실행한다.
    void PlayRandAnim();

    // 피 파티클을 생성한 쪽이 z축 회전이 일어났는지 셋팅해주는 함수
    void SetRotOwner(bool _Rot) { m_bRotOwner = _Rot; };

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBloodEffectScript);
public:
    CBloodEffectScript();
    ~CBloodEffectScript();
};

