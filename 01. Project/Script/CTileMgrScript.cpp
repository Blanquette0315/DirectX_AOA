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
	// GameMgr�� ����ϱ�
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
	// ������ �� ����� ����ü �����
	tTargetTile_Info TargetTile_Info = {};
	

	// ���� Ÿ���� CloseList�� �߰����ش�.
	CTileScript* pCurTileScript = _ToTile;
	pCurTileScript->SetClosed();

	while (true)
	{
		// ���� Ÿ���� OpenList�� �ִ´�.
		pCurTileScript->AddOpenList();

		// OpenList���� ���� ȿ���� ���� ��带 ������.
		pCurTileScript = m_OpenList.top();
		pCurTileScript->SetClosed();

		m_OpenList.pop();

		// �������� ������ ��� AStar ����
		if (pCurTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
		{
			break;
		}
	}

	// �ٽ� ������ �������� �������Ѵ�.
	// ���� Range�� �߰������� +1�� �� ���־�� �Ѵ�.
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

			// �׽�Ʈ������ ��� �������� �ٲپ� ����
			//pCurTileScript->SetAdjacentSelect(true);
		}
	}

	// ������ ����ü ä���� �����ϱ�
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
	// OpenList �ʱ�ȭ
	while (!m_OpenList.empty())
	{
		m_OpenList.pop();
	}

	// ��� Ÿ���� Astar ����� �ʱ�ȭ
	vector<CGameObject*> vecChild = GetOwner()->GetChildObject();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		CTileScript* pTileScript = (CTileScript*)(vecChild[i]->GetScript((UINT)TILESCRIPT));
		pTileScript->AstarInit();
	}
}


void CTileMgrScript::ShowRange(int _iRange)
{
	// Player�� ���� ���ǿ��� ����
	CPlayerScript* pPlayerScript = (CPlayerScript*)(m_pGameMgrScript->GetPlayer()->GetScript((UINT)PLAYERSCRIPT));
	CTileScript* pTileScript = (CTileScript*)(pPlayerScript->GetTile()->GetScript((UINT)TILESCRIPT));

	if (_iRange == 0)
	{
		// ������ 0�̶�� ���� Ÿ���� ��ǥ ������ SELF��� ���� �ǹ��Ѵ�.
		pTileScript->RegisterAdjTile(_iRange, TILE_DIR::SELF);
		pTileScript->SetSelectedCard(true);
		pTileScript->SetAdjacentSelect(true);
	}
	else
	{
		// ��ȿ�� Ÿ�Ϸ� ��� �ޱ�
		pTileScript->RegisterAdjTile(_iRange, TILE_DIR::ALL);
		pTileScript->SetSelectedCard(true);

		ValidTileSetAdj(true);
	}
}

// Player�Ӹ��� �ƴ϶� Monster�� �ش� �Լ��� ���� �̵��ϵ��� ������ ����־�� �Ѵ�.
void CTileMgrScript::ObjectChangeTile(CGameObject* _TargetObj, CTileScript* _TargetTile, OBJECT_TYPE _eObjType)
{
	switch (_eObjType)
	{
	case OBJECT_TYPE::PLAYER: 
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)(_TargetObj->GetScript((UINT)PLAYERSCRIPT));

		// ������ �÷��̾ ���ִ� Ÿ���� ���� Ÿ���� ������ �ش�.
		CTileScript* pTileScript = (CTileScript*)(pPlayerScript->GetTile()->GetScript((UINT)TILESCRIPT));
		pTileScript->ReleaseTargetObj();
		pTileScript->SetSelectedCard(false);

		ValidTileSetAdj(false);

		// �÷��̾ ��ġ�� Ÿ���� �ٲ��ش�.
		pPlayerScript->SetOwnerTile(_TargetTile->GetOwner());
		pPlayerScript->PosOnTile();
		break;
	}
		
	case OBJECT_TYPE::MONSTER:
	{
		CMonsterScript* pMonsterScript = GetMonsterScript(_TargetObj);

		// ������ ���Ͱ� ���ִ� Ÿ���� ���� Ÿ���� ������ �ش�.
		CTileScript* pTileScript = (CTileScript*)(pMonsterScript->GetTile()->GetScript((UINT)TILESCRIPT));
		pTileScript->ReleaseTargetObj();

		// ���Ͱ� ��ġ�� Ÿ���� �ٲ��ش�.
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