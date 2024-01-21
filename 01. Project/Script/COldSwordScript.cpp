#include "pch.h"
#include "COldSwordScript.h"

COldSwordScript::COldSwordScript()
	: CWaponScript(OLDSWORDSCRIPT, WAPON_TYPE::SWORD)
{
	// 기본 스텟 넣어주기
	m_iAtk = 4;
	m_iDef = 0;
}

COldSwordScript::~COldSwordScript()
{
}

void COldSwordScript::begin()
{
}

void COldSwordScript::tick()
{
}

void COldSwordScript::SaveToFile(FILE* _File)
{
}

void COldSwordScript::LoadFromFile(FILE* _File)
{
}