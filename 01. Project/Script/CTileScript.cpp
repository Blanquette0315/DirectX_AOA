#include "pch.h"
#include "CTileScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CKeyMgr.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"
#include "CMouseCheckScript.h"
#include "CPlayerScript.h"

CTileScript::CTileScript()
	: CScript(TILESCRIPT)
	, m_eObjType(OBJECT_TYPE::NONE)
	, m_arrAdjacentTile{}
	, m_pTargetObj(nullptr)
	, m_bSelected(false)
	, m_bAdjacentSelected(false)
	, AdjacentSelectOnce(false)
	, DynamicSelectOnce(false)
	, m_bSelectedCard(false)
	, m_pPrevTileScript(nullptr)
	, m_fFromParent(0.f)
	, m_fToDest(0.f)
	, m_fFinal(0.f)
	, m_bOpen(false)
	, m_bClosed(false)
	, m_bPath(false)
{
}

CTileScript::~CTileScript()
{
}

void CTileScript::begin()
{
	AdjacentTileinit();
}

void CTileScript::tick()
{
	// ���콺�� ���� ���� ���� �˻�
	CMouseCheckScript* pMouseChScript = (CMouseCheckScript*)(GetOwner()->GetScript((UINT)MOUSECHECKSCRIPT));
	m_bSelected = pMouseChScript->Is_MouseOverlap();

	// ���� ���� ī�尡 �ִٸ�,
	//if (m_bSelectedCard)
	//{
	//	// ������ ���� Ÿ���� ������־�� �Ѵ�.
	//	CheckAdjTile();
	//}
	//else
	//{
	//	ReleaseAdjTile();
	//}

	// �ش� �ڵ�� ���߿� Ÿ�� info�� ����� �� Ȱ���� ��
	//else
	//{
	//	// �ش� Ÿ���� ���� ���̶��, ���� Ÿ���� ������ش�.
	//	if (m_bSelected)
	//	{
	//		// ���� ���� ���콺�� ���� ��� ���� Ÿ���� ������ֱ� ���� ���� Ÿ�ϵ鿡 ������ üũ���ش�.
	//		CheckAdjTile();
	//	}
	//	// ���콺�� �����Ǹ�, ����Ÿ�Ͽ� ������ ����Ÿ���� �������ش�.
	//	else
	//	{

	//		ReleaseAdjTile();
	//	}
	//}
	

	// ������ ���� Ÿ���� ��� ������ �ٲ� ��������� ������ش�.
	RenderAdjTile();
	
}




void CTileScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CTileScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}


// ��ǥ�� Ÿ���� ã�ƴ޶�� �ϴ� �Լ�
void CTileScript::CalculateCost(CTileScript* _pCurNode, CTileScript* _pOrigin)
{
	// pPrevTileScript
	// ������ ���� Ÿ�Ϸ� �Է����� ���� Ÿ���� �־��ش�.
	_pCurNode->m_pPrevTileScript = _pOrigin;

	// m_fFromParent
	// ������ ���� �������� ��������
	Vec3 CurTileWPos = _pCurNode->GetOwner()->Transform()->GetRelativePos();

	Vec3 OriTileWPos = _pOrigin->GetOwner()->Transform()->GetRelativePos();
	float COx = abs(CurTileWPos.x - OriTileWPos.x);
	float COy = abs(CurTileWPos.y - OriTileWPos.y);

	// ���� ������ ���α����� �Ÿ� �־��ֱ�
	_pCurNode->m_fFromParent = abs(sqrt((COx * COx + COy * COy))) + _pOrigin->m_fFromParent;


	// m_fToDest
	// ���κ��� Ÿ�ٱ����� �Ÿ� �־��ֱ�
	// ����� ���� ������ ��������
	CTileMgrScript* pTileMgrScript = (CTileMgrScript*)(GetOwner()->GetParent()->GetScript((UINT)TILEMGRSCRIPT));
	CTileScript* pTargetTile = (CTileScript*)pTileMgrScript->GetGameMgrScript()->GetPlayerScript()->GetTile();
	Vec3 TargetWPos = pTargetTile->GetOwner()->Transform()->GetRelativePos();

	float CTx = abs(CurTileWPos.x - TargetWPos.x);
	float CTy = abs(CurTileWPos.y - TargetWPos.y);

	_pCurNode->m_fToDest = abs(sqrt((CTx * CTx + CTy * CTy)));

	// m_fFinal
	// ���� �ΰ��� ���� �� �־��ֱ�
	_pCurNode->m_fFinal = _pCurNode->m_fFromParent + _pCurNode->m_fToDest;

}


float CTileScript::CloneCalculateCost(CTileScript* _pOrigin, Vec3 _CurNodePos)
{
	// fFromParent
	// ���κ��� Ÿ�ٱ����� �Ÿ�
	Vec3 CurTileWPos = _CurNodePos;
	Vec3 OriTileWPos = _pOrigin->GetOwner()->Transform()->GetRelativePos();

	float COx = abs(CurTileWPos.x - OriTileWPos.x);
	float COy = abs(CurTileWPos.y - OriTileWPos.y);

	// ���ۺ����� ���α��� �Ÿ�
	float fFromParent = abs(sqrt((COx * COx + COy * COy))) + _pOrigin->m_fFromParent;

	// fToDest
	// ����� ���� ������ ��������
	CTileMgrScript* pTileMgrScript = (CTileMgrScript*)(GetOwner()->GetParent()->GetScript((UINT)TILEMGRSCRIPT));
	CTileScript* pTargetTile = (CTileScript*)pTileMgrScript->GetGameMgrScript()->GetPlayerScript()->GetTile();
	Vec3 TargetWPos = pTargetTile->GetOwner()->Transform()->GetRelativePos();

	float CTx = abs(CurTileWPos.x - TargetWPos.x);
	float CTy = abs(CurTileWPos.y - TargetWPos.y);

	float fToDest = abs(sqrt((CTx * CTx + CTy * CTy)));

	return fFromParent + fToDest;
}


void CTileScript::AddOpenList()
{
	// ���� Ÿ���� �־��ش�. Push_OpenList
	for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
	{
		// ���� Ÿ���� ��� nullptr�� ��� �������� �ʴ´�.
		if (nullptr != m_arrAdjacentTile[i])
		{
			CTileScript* pTileScript = (CTileScript*)(m_arrAdjacentTile[i]->GetScript((UINT)TILESCRIPT));

			// �̹� CloseList�� ��������� �ĺ��� �־����� �ʴ´�.
			if (pTileScript->m_bClosed == true)
				continue;
			// ���Ͱ� �̹� Ÿ�Ͽ� ������ ��� �̵��ϸ� �ȵǱ� ������ OpenList�� ���� �ʴ´�.
			if (OBJECT_TYPE::MONSTER == pTileScript->GetTileTargetType())
				continue;

			// ���� ����Ʈ�� �ֱ�
			if (false == pTileScript->Is_OpenList())
			{
				CalculateCost(pTileScript, this);
				CTileMgrScript* pTileMgrScript = (CTileMgrScript*)(GetOwner()->GetParent()->GetScript((UINT)TILEMGRSCRIPT));
				// OpenList�� �־��ֱ�
				pTileScript->SetOpen();
				pTileMgrScript->Push_OpenList(pTileScript);
			}
			else // �̹� OpenList�� �ִ� ���,
			{
				// ����� ����ؼ� �� ȿ���� ���� ������ ��ü�� �ش�.
				Vec3 CurTileWPos = pTileScript->GetOwner()->Transform()->GetRelativePos();
				float TempFinal = CloneCalculateCost(this, CurTileWPos);

				if (pTileScript->m_fFinal > TempFinal)
				{
					// �ش� Ÿ���� ��� �缳��
					CalculateCost(pTileScript, this);

					// ���¸���Ʈ(�켱����ť) �缳��
					CTileMgrScript* pTileMgrScript = (CTileMgrScript*)(GetOwner()->GetParent()->GetScript((UINT)TILEMGRSCRIPT));
					pTileMgrScript->Rebuild();
				}
			}
		}
	}
}

void CTileScript::AstarInit()
{
	m_pPrevTileScript = nullptr;
	m_fFromParent = 0.f;
	m_fToDest = 0.f;
	m_fFinal = 0.f;
	m_bOpen = false;
	m_bClosed = false;
	m_bPath = false;
}




void CTileScript::CheckAdjTile()
{
	if (false == AdjacentSelectOnce)
	{
		AdjacentSelectOnce = true;
		for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
		{
			// ���� Ÿ���� nullptr�� ��� ����ó���� ���־�� �Ѵ�.
			if (nullptr != m_arrAdjacentTile[i])
			{
				CTileScript* TargetScript = (CTileScript*)(m_arrAdjacentTile[i]->GetScript(TILESCRIPT));
				TargetScript->SetAdjacentSelect(true);
			}
		}
	}
}

void CTileScript::ReleaseAdjTile()
{
	if (true == AdjacentSelectOnce)
	{
		AdjacentSelectOnce = false;
		for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
		{
			// ���� Ÿ���� nullptr�� ��� ����ó���� ���־�� �Ѵ�.
			if (nullptr != m_arrAdjacentTile[i])
			{
				CTileScript* TargetScript = (CTileScript*)(m_arrAdjacentTile[i]->GetScript(TILESCRIPT));
				TargetScript->SetAdjacentSelect(false);
			}
		}
	}
}

void CTileScript::RenderAdjTile()
{
	// ����Ÿ�Ϸ� ���õ� ���
	if (m_bAdjacentSelected && !DynamicSelectOnce)
	{
		DynamicSelectOnce = true;
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 1.f, 0.f, 0.5f));
	}

	// ������ ���� Ÿ�Ϸν� ������ ���� �Ǿ��ٸ� �ٽ� ���� ������ �������� �ش�.
	else if (!m_bAdjacentSelected && DynamicSelectOnce)
	{
		DynamicSelectOnce = false;
		GetOwner()->MeshRender()->GetSharedMaterial();
	}
}

void CTileScript::RegisterAdjTile(int _iCount, TILE_DIR _eTileDir)
{
	CTileMgrScript* pTileMgrScript = (CTileMgrScript*)(GetOwner()->GetParent()->GetScript((UINT)TILEMGRSCRIPT));

	if (_iCount > 0)
	{
		_iCount--;

		switch (_eTileDir)
		{
		case TILE_DIR::LEFT:
		{
			if(nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::LEFT])
			{

				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::LEFT]->GetScript((UINT)TILESCRIPT));
				pTileMgrScript->SetValidTile(pAdjTileScript);
				pAdjTileScript->RegisterAdjTile(_iCount, TILE_DIR::LEFT);
			}
		}
			break;
		case TILE_DIR::LEFTTOP:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::LEFTTOP])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::LEFTTOP]->GetScript((UINT)TILESCRIPT));
				pTileMgrScript->SetValidTile(pAdjTileScript);
				pAdjTileScript->RegisterAdjTile(_iCount, TILE_DIR::LEFTTOP);
			}
		}
			break;
		case TILE_DIR::RIGHTTOP:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTTOP])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTTOP]->GetScript((UINT)TILESCRIPT));
				pTileMgrScript->SetValidTile(pAdjTileScript);
				pAdjTileScript->RegisterAdjTile(_iCount, TILE_DIR::RIGHTTOP);
			}
		}
			break;
		case TILE_DIR::RIGHT:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::RIGHT])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::RIGHT]->GetScript((UINT)TILESCRIPT));
				pTileMgrScript->SetValidTile(pAdjTileScript);
				pAdjTileScript->RegisterAdjTile(_iCount, TILE_DIR::RIGHT);
			}
		}
			break;
		case TILE_DIR::RIGHTBOT:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTBOT])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTBOT]->GetScript((UINT)TILESCRIPT));
				pTileMgrScript->SetValidTile(pAdjTileScript);
				pAdjTileScript->RegisterAdjTile(_iCount, TILE_DIR::RIGHTBOT);
			}
		}
			break;
		case TILE_DIR::LEFTBOT:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::LEFTBOT])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::LEFTBOT]->GetScript((UINT)TILESCRIPT));
				pTileMgrScript->SetValidTile(pAdjTileScript);
				pAdjTileScript->RegisterAdjTile(_iCount, TILE_DIR::LEFTBOT);
			}
		}
			break;
		case TILE_DIR::ALL:
		{
			for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
			{
				if (nullptr != m_arrAdjacentTile[i])
				{
					CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[i]->GetScript((UINT)TILESCRIPT));
					pTileMgrScript->SetValidTile(pAdjTileScript);
					pAdjTileScript->RegisterAdjTile(_iCount, (TILE_DIR)i);
				}
			}
		}
			break;
		}
		
	}
	else if (_iCount == 0)
	{
		if (TILE_DIR::SELF == _eTileDir)
		{
			// Self�� ��� ������ ����Ѵ�.
			pTileMgrScript->SetValidTile(this);
		}
	}
}

bool CTileScript::FindAdjPlayer()
{
	for (size_t i = 0; i < (UINT)TILE_DIR::END; ++i)
	{
		if (m_arrAdjacentTile[i] != nullptr)
		{
			CTileScript* pTileScript = (CTileScript*)(m_arrAdjacentTile[i]->GetScript((UINT)TILESCRIPT));
			if (pTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
			{
				return true;
			}
		}
	}
	return false;
}

CTileScript* CTileScript::GetOppPlayerTile()
{
	TILE_DIR TempDir = TILE_DIR::END;
	for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
	{
		if (m_arrAdjacentTile[i] != nullptr)
		{
			// Player Ÿ���� ã�� ������ ����� �д�.
			CTileScript* pTileScript = (CTileScript*)(m_arrAdjacentTile[i]->GetScript((UINT)TILESCRIPT));
			if (pTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
			{
				TempDir = (TILE_DIR)i;
			}
		}
	}

	// ���� for���� ��ġ�� ���Ծ END���, ���� Ÿ�Ͽ� Player�� ���ٴ� ���̹Ƿ� nullptr�� ��ȯ�Ѵ�.
	if (TempDir == TILE_DIR::END)
		return nullptr;

	TILE_DIR arrTempDir[3] = {};

	// Player������ ã�Ҵٸ� �ݴ� ������ ã�´�.
	switch (TempDir)
	{
	case TILE_DIR::LEFT:
	{
		arrTempDir[0] = TILE_DIR::RIGHT;
		arrTempDir[1] = TILE_DIR::RIGHTBOT;
		arrTempDir[2] = TILE_DIR::RIGHTTOP;
		break;
	}
		
	case TILE_DIR::LEFTTOP:
	{
		arrTempDir[0] = TILE_DIR::RIGHTBOT;
		arrTempDir[1] = TILE_DIR::LEFTBOT;
		arrTempDir[2] = TILE_DIR::RIGHT;
		break;
	}

	case TILE_DIR::RIGHTTOP:
	{
		arrTempDir[0] = TILE_DIR::LEFTBOT;
		arrTempDir[1] = TILE_DIR::RIGHTBOT;
		arrTempDir[2] = TILE_DIR::LEFT;
		break;
	}

	case TILE_DIR::RIGHT:
	{
		arrTempDir[0] = TILE_DIR::LEFT;
		arrTempDir[1] = TILE_DIR::LEFTBOT;
		arrTempDir[2] = TILE_DIR::LEFTTOP;
		break;
	}

	case TILE_DIR::RIGHTBOT:
	{
		arrTempDir[0] = TILE_DIR::LEFTTOP;
		arrTempDir[1] = TILE_DIR::LEFT;
		arrTempDir[2] = TILE_DIR::RIGHTTOP;
		break;
	}

	case TILE_DIR::LEFTBOT:
	{
		arrTempDir[0] = TILE_DIR::RIGHTTOP;
		arrTempDir[1] = TILE_DIR::RIGHT;
		arrTempDir[2] = TILE_DIR::LEFTTOP;
		break;
	}
	}

	// ���� �ӽ� ������ Ȯ���� ������ �����ϴٸ�, �̵��Ѵ�.
	for (int j = 0; j < 3; ++j)
	{
		if (m_arrAdjacentTile[(UINT)arrTempDir[j]] != nullptr)
		{
			CTileScript* pTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)arrTempDir[j]]->GetScript((UINT)TILESCRIPT));
			
			// ����Ÿ�� ���� ���ͳ� �÷��̾ �����ϸ� �ȵȴ�.
			if (!(pTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
				&& !(pTileScript->GetTileTargetType() == OBJECT_TYPE::MONSTER))
			{
				return pTileScript;
			}
		}
	}

	// ���� Ÿ���߿� �̵��� �� �ִ� �� Ÿ���� ���ٸ�, nullptr�� ��ȯ�Ѵ�.
	return nullptr;
}

CTileScript* CTileScript::FindPlayerinRange(int _Range)
{
	CTileScript* pPlayerScript = nullptr;
	for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
	{
		pPlayerScript = CheckPlayerinRange(_Range, (TILE_DIR)i);
		if (nullptr != pPlayerScript)
		{
			return pPlayerScript;
		}
	}

	if (pPlayerScript == nullptr)
	{
		return nullptr;
	}
}

CTileScript* CTileScript::CheckPlayerinRange(int _Range, TILE_DIR _TileDir)
{
	int _iCount = _Range;

	if (_iCount > 0)
	{
		_iCount--;

		switch (_TileDir)
		{
		case TILE_DIR::LEFT:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::LEFT])
			{

				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::LEFT]->GetScript((UINT)TILESCRIPT));
				if (pAdjTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
				{
					return pAdjTileScript;
				}
				else
				{
					pAdjTileScript->CheckPlayerinRange(_iCount, TILE_DIR::LEFT);
				}
			}
			else
			{
				return nullptr;
			}
		}
		break;
		case TILE_DIR::LEFTTOP:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::LEFTTOP])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::LEFTTOP]->GetScript((UINT)TILESCRIPT));
				if (pAdjTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
				{
					return pAdjTileScript;
				}
				else
				{
					pAdjTileScript->CheckPlayerinRange(_iCount, TILE_DIR::LEFTTOP);
				}
			}
			else
			{
				return nullptr;
			}
		}
		break;
		case TILE_DIR::RIGHTTOP:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTTOP])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTTOP]->GetScript((UINT)TILESCRIPT));
				if (pAdjTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
				{
					return pAdjTileScript;
				}
				else
				{
					pAdjTileScript->CheckPlayerinRange(_iCount, TILE_DIR::RIGHTTOP);
				}
			}
			else
			{
				return nullptr;
			}
		}
		break;
		case TILE_DIR::RIGHT:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::RIGHT])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::RIGHT]->GetScript((UINT)TILESCRIPT));
				if (pAdjTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
				{
					return pAdjTileScript;
				}
				else
				{
					pAdjTileScript->CheckPlayerinRange(_iCount, TILE_DIR::RIGHT);
				}
			}
			else
			{
				return nullptr;
			}
		}
		break;
		case TILE_DIR::RIGHTBOT:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTBOT])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::RIGHTBOT]->GetScript((UINT)TILESCRIPT));
				if (pAdjTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
				{
					return pAdjTileScript;
				}
				else
				{
					pAdjTileScript->CheckPlayerinRange(_iCount, TILE_DIR::RIGHTBOT);
				}
			}
			else
			{
				return nullptr;
			}
		}
		break;
		case TILE_DIR::LEFTBOT:
		{
			if (nullptr != m_arrAdjacentTile[(UINT)TILE_DIR::LEFTBOT])
			{
				CTileScript* pAdjTileScript = (CTileScript*)(m_arrAdjacentTile[(UINT)TILE_DIR::LEFTBOT]->GetScript((UINT)TILESCRIPT));
				if (pAdjTileScript->GetTileTargetType() == OBJECT_TYPE::PLAYER)
				{
					return pAdjTileScript;
				}
				else
				{
					pAdjTileScript->CheckPlayerinRange(_iCount, TILE_DIR::LEFTBOT);
				}
			}
			else
			{
				return nullptr;
			}
		}
		break;
		}
	}

	// Count�� 0���� ������, ��ã�Ҵٴ� ���� ���ٴ� ���̹Ƿ� nullptr ��ȯ
	else
	{
		return nullptr;
	}
}

TILE_DIR CTileScript::FindPlayerinRange_DIR(int _Range)
{
	CTileScript* pPlayerScript = nullptr;
	for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
	{
		pPlayerScript = CheckPlayerinRange(_Range, (TILE_DIR)i);
		if (!pPlayerScript)
		{
			return (TILE_DIR)i;
		}
	}

	if (pPlayerScript == nullptr)
	{
		return TILE_DIR::END;
	}
}

void CTileScript::AdjacentTileinit()
{
	// ������ Ÿ�� �ѹ� ã��
	wstring strTileName = GetOwner()->GetName();
	size_t Start = strTileName.rfind(L"_");
	wstring Number = strTileName.substr(Start + 1);

	int TileNumber = std::stoi(Number);

	// ������ Ÿ���� ã�ƿ��� ���� �ѹ� �迭 �����
	// ������ Ÿ���� ���� ��� -1�� �־��ش�.
	int arrAdjacentTileNum[6] = {};

	switch (TileNumber)
	{
	// ù��° ��
	case 0:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 7;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 10;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 4;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 25;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 22;
		break;
	}

	case 1:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 2;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 8;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 7;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 0;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 22;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 23;
		break;
	}

	case 2:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 3;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 9;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 8;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 23;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 24;
		break;
	}

	case 3:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 9;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 2;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 24;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = -1;
		break;
	}

	case 4:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 0;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 10;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 11;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 5;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 26;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 25;
		break;
	}

	case 5:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 4;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 11;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 12;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] =6;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 27;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 26;
		break;
	}

	case 6:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 5;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 12;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 27;
		break;
	}

	// �ι� ° ��

	case 7:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 8;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 14;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 13;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 10;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 0;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 1;
		break;
	}

	case 8:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 9;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 15;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 14;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 7;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 2;
		break;
	}

	case 9:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 15;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 8;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 2;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 3;
		break;
	}

	case 10:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 7;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 13;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 16;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 11;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 4;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 0;
		break;
	}

	case 11:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 10;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 16;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 17;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 12;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 5;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 4;
		break;
	}

	case 12:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 11;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 17;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 6;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 5;
		break;
	}

	case 13:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 14;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 18;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 20;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 16;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 10;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 7;
		break;
	}

	case 14:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 15;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 19;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 18;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 13;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 7;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 8;
		break;
	}

	case 15:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 19;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 14;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 8;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 9;
		break;
	}

	case 16:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 13;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 20;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 21;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 17;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 11;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 10;
		break;
	}

	case 17:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 16;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 21;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 12;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 11;
		break;
	}

	// ����° ��
	case 18:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 19;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 20;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 13;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 14;
		break;
	}

	case 19:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 18;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 14;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 15;
		break;
	}

	case 20:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 18;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 21;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 16;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 13;
		break;
	}

	case 21:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] =20;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 17;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 16;
		break;
	}

	// �� ��° ��
	case 22:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 23;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 0;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 25;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 28;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 29;
		break;
	}

	case 23:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 24;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 2;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 22;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 29;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 30;
		break;
	}

	case 24:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 3;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 2;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 23;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 30;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = -1;
		break;
	}

	case 25:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 22;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 0;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 4;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 26;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 31;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 28;
		break;
	}

	case 26:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 25;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 4;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 5;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 27;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 32;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 31;
		break;
	}

	case 27:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 26;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 5;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 6;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 32;
		break;
	}

	// �ټ� ��° ��
	case 28:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 29;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 22;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 25;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 31;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 35;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 33;
		break;
	}

	case 29:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 30;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 23;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 22;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 28;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 33;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 34;
		break;
	}

	case 30:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 24;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 23;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 29;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 34;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = -1;
		break;
	}

	case 31:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 28;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 25;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 26;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 32;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = 36;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 35;
		break;
	}

	case 32:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 31;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 26;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 27;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = 36;
		break;
	}

	// ���� ��° ��
	case 33:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 34;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 29;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 28;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 35;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = -1;
		break;
	}

	case 34:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 30;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 29;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 33;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = -1;
		break;
	}

	case 35:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 33;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 28;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 31;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = 36;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = -1;
		break;
	}

	case 36:
	{
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFT] = 35;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTTOP] = 31;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTTOP] = 32;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::RIGHTBOT] = -1;
		arrAdjacentTileNum[(UINT)TILE_DIR::LEFTBOT] = -1;
		break;
	}
	}

	// ã�� ���� �ѹ����� ���ڿ��� �ٲ㼭 Ÿ�� ������Ʈ ã�� �� ���� Ÿ�� �迭�� �־��ֱ�
	for (int i = 0; i < (UINT)TILE_DIR::END; ++i)
	{
		if (arrAdjacentTileNum[i] == -1)
		{
			m_arrAdjacentTile[i] = nullptr;
		}
		else
		{
			wstring TargetTileName = L"Tile_";
			TargetTileName += std::to_wstring(arrAdjacentTileNum[i]);

			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
			CGameObject* pTargetTile = pCurLevel->FindObjectByName(TargetTileName);
			m_arrAdjacentTile[i] = pTargetTile;
		}
	}
}