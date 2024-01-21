#pragma once
#include "CCardScript.h"
class CCardMoveScript :
    public CCardScript
{
private:
    
public:

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CCardMoveScript);
public:
    CCardMoveScript();
    ~CCardMoveScript();
};

