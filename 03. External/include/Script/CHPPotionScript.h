#pragma once
#include <Engine/CScript.h>

class CHPPotionScript :
    public CScript
{
private:

public:
    void CreatePotionUseSound();

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CHPPotionScript);
public:
    CHPPotionScript();
    ~CHPPotionScript();
};