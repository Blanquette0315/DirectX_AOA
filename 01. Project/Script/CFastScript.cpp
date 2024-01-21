#include "pch.h"
#include "CFastScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"

CFastScript::CFastScript()
	: CMonsterScript(FASTSCRIPT)
{
	// ������ �⺻ ���� �־��ֱ�
	m_tMonsterInfo.Max_HP = 6;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 5;
	m_tMonsterInfo.Deffence = 0;
	m_tMonsterInfo.Range = 1;
}

CFastScript::~CFastScript()
{
}

void CFastScript::begin()
{
	CMonsterScript::begin();

	// ���Ͱ� ��ġ�� Ÿ�� �����ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_3"));

	// Tile�� ��ġ�� �µ��� Position ����
	PosOnTile();
}

void CFastScript::tick()
{
	CMonsterScript::tick();
}



void CFastScript::MosnsterAIPlay()
{
	// �̹� ���ʶ� �ؾ������� ��ġ�� �ʾ����� �ٽ� false�� �־��ش�.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// TileMgr���� Player���� �Ÿ��� ������ ��û�Ѵ�.
	m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

	// �÷��̾���� �Ÿ��� Ȯ���� ��ȿ�Ÿ� �ȿ� ���Դ��� Ȯ���Ѵ�.
	// ���� �������� �÷��̾ �� �ָ� �ִٸ�,
	if (m_tMonsterInfo.Range < m_tAstarInfo.iTargetRange)
	{
		// �̵� ���� ������ ���� �ൿ ���ϱ�
		GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
		m_eMonsterPattern = MONSTER_PATTERN::MOVE;
	}
	// ���� ������ ���Դٸ�,
	else
	{
		// ���� ���� ������ ���� �ൿ ���ϱ�
		m_eMonsterPattern = MONSTER_PATTERN::ATTACK;
	}
}

void CFastScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
}

void CFastScript::Move()
{
	PosOnTile();
	m_bMonMove = true;
}

void CFastScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_Fast", true);
}

void CFastScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_Fast", true);
}

void CFastScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_Fast", true);

	// ȿ���� ���
	CreateSlashSound();
}

void CFastScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_Fast", true);
}



void CFastScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CFastScript::Overlap(CCollider2D* _pOther)
{
}

void CFastScript::EndOverlap(CCollider2D* _pOther)
{
}

void CFastScript::SaveToFile(FILE* _File)
{
}

void CFastScript::LoadFromFile(FILE* _File)
{
}
