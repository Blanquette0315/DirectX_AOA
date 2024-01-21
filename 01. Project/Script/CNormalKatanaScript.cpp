#include "pch.h"
#include "CNormalKatanaScript.h"

CNormalKatanaScript::CNormalKatanaScript()
	: CWaponScript(NORMALKATANASCRIPT, WAPON_TYPE::KATANA)
{
	// 무기의 기본 스텟 넣어주기
	m_iAtk = 6;
	m_iDef = 0;
}

CNormalKatanaScript::~CNormalKatanaScript()
{
}

void CNormalKatanaScript::begin()
{
}

void CNormalKatanaScript::tick()
{
}

void CNormalKatanaScript::SaveToFile(FILE* _File)
{
}

void CNormalKatanaScript::LoadFromFile(FILE* _File)
{
}