#include "pch.h"
#include "COldShieldScript.h"

COldShieldScript::COldShieldScript()
	: CWaponScript(OLDSHIELDSCRIPT, WAPON_TYPE::SHIELD)
{
	// �⺻ ���� �־��ֱ�
	m_iAtk = 0;
	m_iDef = 2;
}

COldShieldScript::~COldShieldScript()
{
}

void COldShieldScript::begin()
{
}

void COldShieldScript::tick()
{
}

void COldShieldScript::SaveToFile(FILE* _File)
{
}

void COldShieldScript::LoadFromFile(FILE* _File)
{
}