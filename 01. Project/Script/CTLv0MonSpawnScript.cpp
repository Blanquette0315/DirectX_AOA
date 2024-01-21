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
	// LCTRL�� N�� ������ ���� ������ ü����
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::N))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\TLevel_1.lv");
		ChangeLevelAndPlay(Level);
	}

	// LCRTL�� I�� ������ �ٽ� �������� ��������.
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::I))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\MainLevel.lv");
		ChangeLevelAndPlay(Level);
	}
}