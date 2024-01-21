#include "pch.h"
#include "CTileMgrScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CCardMgrScript.h"

#include "CTileScript.h"

#include "CPlayerScript.h"
#include "CMonsterScript.h"

CTileMgrScript::CTileMgrScript()
	: CScript(TILEMGRSCRIPT)
	, m_pGameMgr(nullptr)
	, m_pGameMgrScript(nullptr)
	, m_bCardSelected(false)
{
}

CTileMgrScript::~CTileMgrScript()
{
}

void CTileMgrScript::begin()
{
	// GameMgr에 등록하기
	m_pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)(m_pGameMgr->GetScript(GAMEMGRSCRIPT));
	m_pGameMgrScript->RegisterTileMgrScript(this);
}

void CTileMgrScript::tick()
{
}

void CTileMgrScript::SaveToFile(FILE* _File)
{
}

void CTileMgrScript::LoadFromFile(FILE* _File)
{
}

CTileScript* CTileMgrScript::SelectInValidTile()
{
	for (int i = 0; i < m_vecValidTile.size(); ++i)
	{
		if (m_vecValidTile[i]->Is_Selected())
		{
			return m_vecValidTile[i];
		}
	}

	return nullptr;
}


tTargetTile_Info CTileMgrScript::FindAstar(CTileScript* _ToTile)
{
	// 전달할 때 사용할 구조체 만들기
	tTargetTile_Info TargetTile_Info = {};
	

	// 현제 타일을 CloseList에 추가해준다.
	CTileScript* pCurTileScript = _ToTile;
	pCurTileScript->SetClosed();

	while (true)
	{
		// 인접 타일을 OpenList에 넣는다.
		pCurTileScript->AddOpenList();

		// OpenList에서 가장 효율이 좋은 노드를 꺼낸다.
		pCurTileScript = m_OpenList.top();
		pCurTileScript->SetClosed();

		m_OpenList.pop();

		// 목적지에 도착한 경우 AStar 종료
		if (pCurTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
		{
			break;
		}
	}

	// 다시 시작점 직전까지 역추적한다.
	// 따라서 Range는 추가적으로 +1을 더 해주어야 한다.
	while (true)
	{
		if (pCurTileScript->GetPrevTileScript() == _ToTile)
		{
			break;
		}
		else
		{
			pCurTileScript = pCurTileScript->GetPrevTileScript();
			TargetTile_Info.iTargetRange += 1;

			// 테스트용으로 노란 발판으로 바꾸어 보기
			//pCurTileScript->SetAdjacentSelect(true);
		}
	}

	// 전달할 구조체 채워서 전달하기
	TargetTile_Info.iTargetRange += 1;
	TargetTile_Info.pNextTile = pCurTileScript;

	return TargetTile_Info;
}

void CTileMgrScript::Rebuild()
{
	priority_queue<CTileScript*, vector<CTileScript*>, Compare> tempQueue;

	while (!m_OpenList.empty())
	{
		tempQueue.push(m_OpenList.top());
		m_OpenList.pop();
	}

	m_OpenList.swap(tempQueue);
}

void CTileMgrScript::AstarInit()
{
	// OpenList 초기화
	while (!m_OpenList.empty())
	{
		m_OpenList.pop();
	}

	// 모든 타일의 Astar 멤버들 초기화
	vector<CGameObject*> vecChild = GetOwner()->GetChildObject();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		CTileScript* pTileScript = (CTileScript*)(vecChild[i]->GetScript((UINT)TILESCRIPT));
		pTileScript->AstarInit();
	}
}


void CTileMgrScript::ShowRange(int _iRange)
{
	// Player가 지닌 발판에게 전달
	CPlayerScript* pPlayerScript = (CPlayerScript*)(m_pGameMgrScript->GetPlayer()->GetScript((UINT)PLAYERSCRIPT));
	CTileScript* pTileScript = (CTileScript*)(pPlayerScript->GetTile()->GetScript((UINT)TILESCRIPT));

	if (_iRange == 0)
	{
		// 범위가 0이라는 것은 타일의 목표 방향이 SELF라는 것을 의미한다.
		pTileScript->RegisterAdjTile(_iRange, TILE_DIR::SELF);
		pTileScript->SetSelectedCard(true);
		pTileScript->SetAdjacentSelect(true);
	}
	else
	{
		// 유효한 타일로 등록 받기
		pTileScript->RegisterAdjTile(_iRange, TILE_DIR::ALL);
		pTileScript->SetSelectedCard(true);

		ValidTileSetAdj(true);
	}
}

// Player뿐만이 아니라 Monster도 해당 함수를 통해 이동하도록 구조를 잡아주어야 한다.
void CTileMgrScript::ObjectChangeTile(CGameObject* _TargetObj, CTileScript* _TargetTile, OBJECT_TYPE _eObjType)
{
	switch (_eObjType)
	{
	case OBJECT_TYPE::PLAYER: 
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)(_TargetObj->GetScript((UINT)PLAYERSCRIPT));

		// 이전에 플레이어가 서있던 타일은 소유 타일을 해제해 준다.
		CTileScript* pTileScript = (CTileScript*)(pPlayerScript->GetTile()->GetScript((UINT)TILESCRIPT));
		pTileScript->ReleaseTargetObj();
		pTileScript->SetSelectedCard(false);

		ValidTileSetAdj(false);

		// 플레이어가 위치한 타일을 바꿔준다.
		pPlayerScript->SetOwnerTile(_TargetTile->GetOwner());
		pPlayerScript->PosOnTile();
		break;
	}
		
	case OBJECT_TYPE::MONSTER:
	{
		CMonsterScript* pMonsterScript = GetMonsterScript(_TargetObj);

		// 이전에 몬스터가 서있던 타일은 소유 타일을 해제해 준다.
		CTileScript* pTileScript = (CTileScript*)(pMonsterScript->GetTile()->GetScript((UINT)TILESCRIPT));
		pTileScript->ReleaseTargetObj();

		// 몬스터가 위치한 타일을 바꿔준다.
		pMonsterScript->SetOwnerTile(_TargetTile->GetOwner());
		//pMonsterScript->PosOnTile();

		break;
	}
	}
	
}

void CTileMgrScript::ValidTileSetAdj(bool _OnOff)
{
	for (size_t i = 0; i < m_vecValidTile.size(); ++i)
	{
		m_vecValidTile[i]->SetAdjacentSelect(_OnOff);
	}
}