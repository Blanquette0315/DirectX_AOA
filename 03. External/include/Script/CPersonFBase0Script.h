#pragma once
#include <Engine/CScript.h>

class CPersonFBase0Script :
    public CScript
{
private:
    Vec3    m_vOriginPos;   // ���� ������
    float   m_fSpeed;       // �̵� ���ǵ�
    float   m_fDistance;    // �̵� �Ÿ�

    float   m_fAccDis;      // �̵� ���� �Ÿ�
    bool    m_bUp;          // ���� �̵��ؾ��ϴ��� Ȯ���ϴ� ����

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