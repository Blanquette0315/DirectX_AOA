#include "pch.h"
#include "CRefBeetleScript.h"

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CTileScript.h"
#include "CPlayerScript.h"

CRefBeetleScript::CRefBeetleScript()
	: CMonsterScript(REFBEETLESCRIPT)
{
	// ������ �⺻ ���� �־��ֱ�
	m_tMonsterInfo.Max_HP = 16;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 2;
	m_tMonsterInfo.Deffence = 0;
	m_tMonsterInfo.Range = 1;
}

CRefBeetleScript::~CRefBeetleScript()
{
}

void CRefBeetleScript::begin()
{
	CMonsterScript::begin();

	// ���Ͱ� ��ġ�� Ÿ�� �����ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_2"));

	// Tile�� ��ġ�� �µ��� Position ����
	PosOnTile();
}

void CRefBeetleScript::tick()
{
	CMonsterScript::tick();
}



void CRefBeetleScript::MosnsterAIPlay()
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

void CRefBeetleScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
	GetGameMgrScript()->GetPlayerScript()->Hit(m_tMonsterInfo.Attack);
}

void CRefBeetleScript::Move()
{
	// �� ����, Monster�� �̵��� �Ҷ�, ������ �����̴� ����� �������� ����־�� �Ѵ�.
	// �ش� �ڵ�� �߰� ������ ������ �׽�Ʈ�� �ڵ�
	PosOnTile();
	//GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
	m_bMonMove = true;
}

void CRefBeetleScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_RefBeetle", true);
}

void CRefBeetleScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_RefBeetle", false);
}

void CRefBeetleScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_RefBeetle", false);

	// ȿ���� ���
	CreateSlashSound();
}

void CRefBeetleScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_RefBeetle", false);
}



void CRefBeetleScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CRefBeetleScript::Overlap(CCollider2D* _pOther)
{
}

void CRefBeetleScript::EndOverlap(CCollider2D* _pOther)
{
}

void CRefBeetleScript::SaveToFile(FILE* _File)
{
}

void CRefBeetleScript::LoadFromFile(FILE* _File)
{
}
