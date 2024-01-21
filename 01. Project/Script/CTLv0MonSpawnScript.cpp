#include "pch.h"
#include "CTLv0MonSpawnScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CLevelMgr.h>

#include "CGameMgrScript.h"

CTLv0MonSpawnScript::CTLv0MonSpawnScript()
	: CScript(TLV0MONSPAWNSCRIPT)
{
}

CTLv0MonSpawnScript::~CTLv0MonSpawnScript()
{
}

void CTLv0MonSpawnScript::begin()
{
	CGameMgrScript* pGameMgrScript = (CGameMgrScript*)GetOwner()->GetScript((UINT)GAMEMGRSCRIPT);
	pGameMgrScript->SetTestMod();
}

void CTLv0MonSpawnScript::tick()
{
	// LCTRL과 N을 누르면 다음 씬으로 체인지
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::N))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\TLevel_1.lv");
		ChangeLevelAndPlay(Level);
	}

	// LCRTL과 I를 누르면 다시 메인으로 나가진다.
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::I))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\MainLevel.lv");
		ChangeLevelAndPlay(Level);
	}
}