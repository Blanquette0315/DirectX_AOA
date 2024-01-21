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
	// LCTRL과 N을 누르면 다음 씬으로 체인지
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::N))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\TLevel_4.lv");
		ChangeLevelAndPlay(Level);
	}

	// LCTRL과 R을 누르면 몬스터 다시 생성
	//if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::R))
	//{
	//	CreateChallenger(8);
	//}

	// LCRTL과 I를 누르면 다시 메인으로 나가진다.
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::I))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\MainLevel.lv");
		ChangeLevelAndPlay(Level);
	}
}

void CTLv3MonSpawnScript::CreateChallenger(int _TileIdx)
{
	// Monster 스폰 관련
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Challenger_Prefab_0.pref");

	wstring TargetTileName = L"Tile_";
	TargetTileName += std::to_wstring(_TileIdx);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CGameObject* pTile = pCurLevel->FindObjectByName(TargetTileName);

	CTileScript* pTileScript = (CTileScript*)pTile->GetScript((UINT)TILESCRIPT);

	// 오브젝트를 생성해 배치해준다.
	CGameObject* pMonster = MonPrefab->Instantiate();

	// begin 호출해주기
	pMonster->begin();

	CMonsterScript* pMonsterScript = GetMonsterScript(pMonster);
	// 원래 타일 해제
	pMonsterScript->GetOwnerTileScript()->ReleaseTargetObj();

	// 타일 지정
	pMonsterScript->SetOwnerTile(pTile);
	pMonsterScript->PosOnTile();


	tEvent tEvt = {};
	tEvt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvt.wParam = (DWORD_PTR)pMonster;
	tEvt.lParam = (DWORD_PTR)3;

	CEventMgr::GetInst()->AddEvent(tEvt);
}