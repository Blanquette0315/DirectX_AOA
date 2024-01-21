#include "pch.h"
#include "CstdParticleScript.h"

#include <Engine/CTimeMgr.h>

CstdParticleScript::CstdParticleScript()
	: CScript(STDPARTICLESCRIPT)
	, m_fAccTime(0.f)
{
}

CstdParticleScript::~CstdParticleScript()
{
}

void CstdParticleScript::tick()
{
	m_fAccTime += DT;

	if (m_fAccTime > 1.1f)
	{
		GetOwner()->Destroy();
	}
}