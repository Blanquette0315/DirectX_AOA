#include "pch.h"
#include "CEndingSoundScript.h"

CEndingSoundScript::CEndingSoundScript()
	: CScript(ENDINGSOUNDSCRIPT)
{
}

CEndingSoundScript::~CEndingSoundScript()
{
}

void CEndingSoundScript::begin()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\BGM_Ending.wav");
	pSound->Play(0, 0.9f, false);
}

void CEndingSoundScript::tick()
{
}