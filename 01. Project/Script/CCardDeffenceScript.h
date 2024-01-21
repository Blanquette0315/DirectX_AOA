#pragma once
#include "CCardScript.h"

class CCardDeffenceScript :
    public CCardScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CCardDeffenceScript);
public:
    CCardDeffenceScript();
    ~CCardDeffenceScript();
};