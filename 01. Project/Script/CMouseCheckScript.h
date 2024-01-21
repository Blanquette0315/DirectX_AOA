#pragma once
#include <Engine/CScript.h>
class CMouseCheckScript :
    public CScript
{
private:
    Vec2 m_vMousePos;

    bool m_bMouseOverlap;   // 마우스가 오브젝트의 위에 올라와 있는지 확인해주는 함수

public:
    // 마우스가 오브젝트에 오버랩 되어 있는지 알아오는 함수
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

