#include "pch.h"
#include "CLv1MonSpawnScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CLevel.h>

CLv1MonSpawnScript::CLv1MonSpawnScript()
	: CScript(LV1MONSPAWNSCRIPT)
{
}

CLv1MonSpawnScript::~CLv1MonSpawnScript()
{
}

void CLv1MonSpawnScript::begin()
{
	CreateChallenger(16);

	CreateCrossBow(14);

	CreateCrossBow(21);
}

void CLv1MonSpawnScript::tick()
{
}

void CLv1MonSpawnScript::CreateCrossBow(int _TileIdx)
{
	// Monster 스폰 관련
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\CrossBow_Prefab_0.pref");

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

void CLv1MonSpawnScript::CreateChallenger(int _TileIdx)
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