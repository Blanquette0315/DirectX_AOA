#include "pch.h"
#include "CLvChanger0Script.h"

#include <Engine/CLevelMgr.h>

CLvChanger0Script::CLvChanger0Script()
	: CLevelChangerScript(LVCHANGER0SCRIPT)
{
}

CLvChanger0Script::~CLvChanger0Script()
{
}

void CLvChanger0Script::begin()
{
	CLevelChangerScript::begin();
}

void CLvChanger0Script::tick()
{
}

void CLvChanger0Script::LevelChange()
{
	CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\Level_1.lv");
	ChangeLevelAndPlay(Level);
}