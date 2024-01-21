#include "pch.h"
#include "CTLv5MonSpawnScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CKeyMgr.h>

#include <Engine/CLevel.h>

#include "CGameMgrScript.h"

CTLv5MonSpawnScript::CTLv5MonSpawnScript()
	: CScript(TLV5MONSPAWNSCRIPT)
{
}

CTLv5MonSpawnScript::~CTLv5MonSpawnScript()
{
}

void CTLv5MonSpawnScript::begin()
{
	CGameMgrScript* pGameMgrScript = (CGameMgrScript*)GetOwner()->GetScript((UINT)GAMEMGRSCRIPT);
	pGameMgrScript->SetTestMod();

	CreateDemonDog(8);
}

void CTLv5MonSpawnScript::tick()
{
	// LCTRL�� R�� ������ ���� �ٽ� ����
	//if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::R))
	//{
	//	CreateDemonDog(8);
	//}

	// LCRTL�� I�� ������ �ٽ� �������� ��������.
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::I))
	{
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\MainLevel.lv");
		ChangeLevelAndPlay(Level);
	}
}

void CTLv5MonSpawnScript::CreateDemonDog(int _TileIdx)
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