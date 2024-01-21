#include "pch.h"
#include "CstdAttackEffScript.h"

#include <Engine/CAnimation2D.h>

CstdAttackEffScript::CstdAttackEffScript()
	: CScript(STDATTACKEFFSCRIPT)
{
}

CstdAttackEffScript::~CstdAttackEffScript()
{
}

void CstdAttackEffScript::begin()
{
}

void CstdAttackEffScript::tick()
{
	// �ִϸ��̼��� ������ ����
	if (Animator2D()->GetCurAnimation()->Is_End())
	{
		GetOwner()->Destroy();
	}
}