#include "pch.h"
#include "CIceReaperScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"

CIceReaperScript::CIceReaperScript()
	: CMonsterScript(ICEREAPERSCRIPT)
	, m_pTargetTileScript(nullptr)
	, m_bCharge(false)
{
	// ������ �⺻ ���� �־��ֱ�
	m_tMonsterInfo.Max_HP = 3; // 70~ 80
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 15;
	m_tMonsterInfo.Deffence = 0; // 20
	m_tMonsterInfo.Range = 1;
}

CIceReaperScript::~CIceReaperScript()
{
}

void CIceReaperScript::begin()
{
	CMonsterScript::begin();

	// ���Ͱ� ��ġ�� Ÿ�� �����ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_3"));

	// Tile�� ��ġ�� �µ��� Position ����
	PosOnTile();
}

void CIceReaperScript::tick()
{
	CMonsterScript::tick();
}



void CIceReaperScript::MosnsterAIPlay()
{
	// �̹� ���ʶ� �ؾ������� ��ġ�� �ʾ����� �ٽ� false�� �־��ش�.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// ���� ���� ���� �÷��̾ �ִ��� Ȯ���Ѵ�.
	m_pTargetTileScript = GetOwnerTileScript()->FindPlayerinRange(2);
	if (m_pTargetTileScript != nullptr)
	{
		if (false == m_bCharge)
		{
			// ���� �ȿ� �÷��̾ �ִٸ� ���ø� �غ��Ѵ�.
			m_eMonsterPattern = MONSTER_PATTERN::BOSS_CHARGE;
		}
	}
	// ���� ���� ���ٸ�, �̵��� �Ѵ�.
	else
	{
		// TileMgr���� Player���� �Ÿ��� ������ ��û�Ѵ�.
		m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

		// �÷��̾���� �Ÿ��� Ȯ���� ��ȿ�Ÿ� �ȿ� ���Դ��� Ȯ���Ѵ�.
		if (1 < m_tAstarInfo.iTargetRange)
		{
			// �̵� ���� ������ ���� �ൿ ���ϱ�
			GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
			m_eMonsterPattern = MONSTER_PATTERN::MOVE;
		}
	}

	// ������ ���� ���̾��ٸ�, ���ø� �Ѵ�.
	if (m_bCharge)
	{
		m_eMonsterPattern = MONSTER_PATTERN::BOSS_RUSH;
	}
}

void CIceReaperScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
}

void CIceReaperScript::Move()
{
	PosOnTile();
	m_bTurnEnd = true;
}

void CIceReaperScript::Boss_Charge()
{
	m_bCharge = true;
	m_bTurnEnd = true;
}

void CIceReaperScript::Boss_Rush()
{
	m_bTurnEnd = true;

	m_bCharge = false;
}

void CIceReaperScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_IceReaper", true);
}

void CIceReaperScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_IceReaper", true);
}

void CIceReaperScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_IceReaper", true);
}

void CIceReaperScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_IceReaper", true);
}





void CIceReaperScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CIceReaperScript::Overlap(CCollider2D* _pOther)
{
}

void CIceReaperScript::EndOverlap(CCollider2D* _pOther)
{
}

void CIceReaperScript::SaveToFile(FILE* _File)
{
}

void CIceReaperScript::LoadFromFile(FILE* _File)
{
}
