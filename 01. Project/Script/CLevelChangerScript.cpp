#include "pch.h"
#include "CLevelChangerScript.h"

#include "CGameMgrScript.h"

CLevelChangerScript::CLevelChangerScript(int SCRIPT)
	: CScript(SCRIPT)
{
}

CLevelChangerScript::~CLevelChangerScript()
{
}

void CLevelChangerScript::begin()
{
	CGameMgrScript* pGameMgrScript = (CGameMgrScript*)GetOwner()->GetScript((UINT)GAMEMGRSCRIPT);
	pGameMgrScript->RegisterLvChangerScript(this);
}

void CLevelChangerScript::tick()
{
}