#include "pch.h"
#include "CMainBGMScript.h"

#include <Engine/CTimeMgr.h>

#include <Engine/CResMgr.h>
#include <Engine/CSound.h>


CMainBGMScript::CMainBGMScript()
	: CScript(MAINBGMSCRIPT)
	, m_fAccTime(0.f)
{
}

CMainBGMScript::~CMainBGMScript()
{
}

void CMainBGMScript::begin()
{
	m_fAccTime = 0.f;

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\BGM_01_Theme_v1.wav");
	pSound->Play(0, 0.9f, false);
}

void CMainBGMScript::tick()
{
}