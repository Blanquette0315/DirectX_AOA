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
	// Fast�� ����
	CreateFast(8);

	CreateFast(14);

	CreateFast(5);

	// Demondog ����
	CreateDemonDog(16);

	// �ݻ��� ����
	CreateRefBeetle(28);

}

void CLv0MonSpawnScript::tick()
{
}

void CLv0MonSpawnScript::CreateFast(int _TileIdx)
{
	// Monster ���� ����
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Fast_Prefab_0.pref");

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

void CLv0MonSpawnScript::CreateDemonDog(int _TileIdx)
{
	// Monster ���� ����
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\DemonDog_Prefab_0.pref");

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

void CLv0MonSpawnScript::CreateRefBeetle(int _TileIdx)
{
	// Monster ���� ����
	Ptr<CPrefab> MonPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RefBeetle_Prefab_0.pref");

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