#include "pch.h"
#include "CWaponScript.h"

CWaponScript::CWaponScript(int _ScriptType, WAPON_TYPE _WaponType)
	: CScript(_ScriptType)
	, m_eWaponType(_WaponType)
{
}

CWaponScript::~CWaponScript()
{
}

void CWaponScript::begin()
{
}

void CWaponScript::tick()
{
}



void CWaponScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CWaponScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}