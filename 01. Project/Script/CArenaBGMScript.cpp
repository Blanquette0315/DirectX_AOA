#include "pch.h"
#include "CArenaBGMScript.h"

#include <Engine/CTimeMgr.h>

#include <Engine/CResMgr.h>
#include <Engine/CSound.h>

CArenaBGMScript::CArenaBGMScript()
	: CScript(ARENABGMSCRIPT)
	, m_fAccTime(0.f)
{
}

CArenaBGMScript::~CArenaBGMScript()
{
}

void CArenaBGMScript::SoundStop()
{
	// BGM을 꺼준다.
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\Battle_0.wav");
	pSound->Stop();
}

void CArenaBGMScript::begin()
{
	m_fAccTime = 0.f;

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\Battle_0.wav");
	m_iBgmChannel = pSound->Play(0, 0.8f, false);

	// 군중 소리 넣기
	pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\people_noisy_talking.wav");
	m_iChannel = pSound->Play(1, 1.f, true);
}

void CArenaBGMScript::tick()
{
	if (m_fAccTime < 8.f)
		m_fAccTime += DT;
	
	if (m_fAccTime > 3.f && m_fAccTime < 8.f)
	{
		float Ratio = (8.f - m_fAccTime) / 8.f;

		float Volume = 0.2f * Ratio;

		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\people_noisy_talking.wav");
		pSound->SetVolume(Volume, m_iChannel);
	}
	else if (m_fAccTime > 8.f)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\people_noisy_talking.wav");
		pSound->Stop();
	}
}