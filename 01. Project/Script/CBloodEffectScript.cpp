#include "pch.h"
#include "CBloodEffectScript.h"

#include <Engine/CAnimation2D.h>

CBloodEffectScript::CBloodEffectScript()
	: CScript(BLOODEFFECTSCRIPT)
	, m_bRotOwner(false)
{
}

CBloodEffectScript::~CBloodEffectScript()
{
}

void CBloodEffectScript::begin()
{
}

void CBloodEffectScript::tick()
{
	// 애니메이션이 끝나면 삭제
	if (Animator2D()->GetCurAnimation()->Is_End())
	{
		GetOwner()->Destroy();
	}
}

void CBloodEffectScript::PlayRandAnim()
{
	srand(time(nullptr));
	int i = rand() % 2;

	if (i == 0)
	{
		Transform()->SetRelativePos(Vec3(20.f, 22.5f, -2.f));
		Animator2D()->Play(L"Eff_Blood", false);
	}
	else if (i == 1)
	{
		Transform()->SetRelativePos(Vec3(-20.f, 22.5f, -2.f));
		Animator2D()->Play(L"Eff_Blood_1", false);
	}

	// 피 효과를 생성한 오브젝트가 만약 회전을 한 상태였다면, -2가 아니라 2를 해주어야 한다.
	if (m_bRotOwner)
	{
		Transform()->SetRelativePos_Z(2.f);
	}
}