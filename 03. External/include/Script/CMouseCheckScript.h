#pragma once
#include <Engine/CScript.h>
class CMouseCheckScript :
    public CScript
{
private:
    Vec2 m_vMousePos;

    bool m_bMouseOverlap;   // ���콺�� ������Ʈ�� ���� �ö�� �ִ��� Ȯ�����ִ� �Լ�

public:
    // ���콺�� ������Ʈ�� ������ �Ǿ� �ִ��� �˾ƿ��� �Լ�
    bool Is_MouseOverlap();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CMouseCheckScript);

public:
    CMouseCheckScript();
    ~CMouseCheckScript();
};

