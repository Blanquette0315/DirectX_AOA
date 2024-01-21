#pragma once
#include "CWaponScript.h"
class COldShieldScript :
    public CWaponScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(COldShieldScript);
public:
    COldShieldScript();
    ~COldShieldScript();
};

