#include "pch.h"
#include "CLv0MonSpawnScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CLevel.h>

CLv0MonSpawnScript::CLv0MonSpawnScript()
	: CScript(LV0MONSPAWNSCRIPT)
{
}

CLv0MonSpawnScript::~CLv0MonSpawnScript()
{
}

void CLv0MonSpawnScript::begin()
{
	// Fast몹 생성
	CreateFast(8);

	CreateFast(14);

	CreateFast(5);

	// Demondog 생성
	CreateDemonDog(16);

	// 반사충 생성
	CreateRefBeetle(28);

}

void CLv0MonSpawnScript::tick()
{
}

void CLv0MonSpawnScript::CreateFast(int _TileIdx)
{
	// Monster 스폰 관련
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Fast_Prefab_0.pref");

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

void CLv0MonSpawnScript::CreateDemonDog(int _TileIdx)
{
	// Monster 스폰 관련
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\DemonDog_Prefab_0.pref");

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

void CLv0MonSpawnScript::CreateRefBeetle(int _TileIdx)
{
	// Monster 스폰 관련
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RefBeetle_Prefab_0.pref");

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