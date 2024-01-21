#pragma once
#include <Engine/CScript.h>

class CBloodEffectScript :
    public CScript
{
private:
    bool m_bRotOwner;

public:
    // �� �ִϸ��̼� �ΰ� �� �����ϰ� �����Ѵ�.
    void PlayRandAnim();

    // �� ��ƼŬ�� ������ ���� z�� ȸ���� �Ͼ���� �������ִ� �Լ�
    void SetRotOwner(bool _Rot) { m_bRotOwner = _Rot; };

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBloodEffectScript);
public:
    CBloodEffectScript();
    ~CBloodEffectScript();
};

