#include "pch.h"
#include "CTLv3MonSpawnScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CKeyMgr.h>

#include <Engine/CLevel.h>

#include "CGameMgrScript.h"

CTLv3MonSpawnScript::CTLv3MonSpawnScript()
	: CScript(TLV3MONSPAWNSCRIPT)
{
}

CTLv3MonSpawnScript::~CTLv3MonSpawnScript()
{
}

void CTLv3MonSpawnScript::begin()
{
	CGameMgrScript* pGameMgrScript = (CGameMgrScript*)GetOwner()->GetScript((UINT)GAMEMGRSCRIPT);
	pGameMgrScript->SetTestMod();

	CreateChallenger(8);
}

void CTLv3MonSpawnScript::tick()
{
	// LCTRL�� N�� ������ ���� ������ ü����
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::N))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\TLevel_4.lv");
		ChangeLevelAndPlay(Level);
	}

	// LCTRL�� R�� ������ ���� �ٽ� ����
	//if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::R))
	//{
	//	CreateChallenger(8);
	//}

	// LCRTL�� I�� ������ �ٽ� �������� ��������.
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::I))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\MainLevel.lv");
		ChangeLevelAndPlay(Level);
	}
}

void CTLv3MonSpawnScript::CreateChallenger(int _TileIdx)
{
	// Monster ���� ����
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Challenger_Prefab_0.pref");

	wstring TargetTileName = L"Tile_";
	TargetTileName += std::to_wstring(_TileIdx);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CGameObject* pTile = pCurLevel->FindObjectByName(TargetTileName);

	CTileScript* pTileScript = (CTileScript*)pTile->GetScript((UINT)TILESCRIPT);

	// ������Ʈ�� ������ ��ġ���ش�.
	CGameObject* pMonster = MonPrefab->Instantiate();

	// begin ȣ�����ֱ�
	pMonster->begin();

	CMonsterScript* pMonsterScript = GetMonsterScript(pMonster);
	// ���� Ÿ�� ����
	pMonsterScript->GetOwnerTileScript()->ReleaseTargetObj();

	// Ÿ�� ����
	pMonsterScript->SetOwnerTile(pTile);
	pMonsterScript->PosOnTile();


	tEvent tEvt = {};
	tEvt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvt.wParam = (DWORD_PTR)pMonster;
	tEvt.lParam = (DWORD_PTR)3;

	CEventMgr::GetInst()->AddEvent(tEvt);
}