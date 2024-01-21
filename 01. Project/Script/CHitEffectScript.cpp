#include "pch.h"
#include "CHitEffectScript.h"

#include <Engine/CTimeMgr.h>

CHitEffectScript::CHitEffectScript()
	: CScript(HITEFFECTSCRIPT)
	, m_fAccTime(0.f)
{
}

CHitEffectScript::~CHitEffectScript()
{
}

void CHitEffectScript::begin()
{
	m_vOriginScale = Transform()->GetRelativeScale();
}

void CHitEffectScript::tick()
{
	// 누적 시간 계산
	m_fAccTime += DT;
	if (m_fAccTime <= 0.55)
	{
		Vec3 vScale = m_vOriginScale;
		vScale.x -= m_vOriginScale.x * m_fAccTime;
		vScale.y -= m_vOriginScale.y * m_fAccTime;

		Transform()->SetRelativeScale(vScale);
	}
	else if (m_fAccTime > 0.55)
	{
		GetOwner()->Destroy();
	}
}