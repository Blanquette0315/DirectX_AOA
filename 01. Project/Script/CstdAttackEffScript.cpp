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
	// 애니메이션이 끝나면 삭제
	if (Animator2D()->GetCurAnimation()->Is_End())
	{
		GetOwner()->Destroy();
	}
}