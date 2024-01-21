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
	// �ִϸ��̼��� ������ ����
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

	// �� ȿ���� ������ ������Ʈ�� ���� ȸ���� �� ���¿��ٸ�, -2�� �ƴ϶� 2�� ���־�� �Ѵ�.
	if (m_bRotOwner)
	{
		Transform()->SetRelativePos_Z(2.f);
	}
}