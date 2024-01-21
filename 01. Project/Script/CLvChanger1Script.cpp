#include "pch.h"
#include "CLvChanger1Script.h"

#include <Engine/CLevelMgr.h>

CLvChanger1Script::CLvChanger1Script()
	: CLevelChangerScript(LVCHANGER1SCRIPT)
{
}

CLvChanger1Script::~CLvChanger1Script()
{
}

void CLvChanger1Script::begin()
{
	CLevelChangerScript::begin();
}

void CLvChanger1Script::tick()
{
}

void CLvChanger1Script::LevelChange()
{
	// Ending Level로 변경해야함.
	CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\ENDLevel.lv");
	ChangeLevelAndPlay(Level);
}