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
	// 몬스터의 기본 스텟 넣어주기
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

	// 몬스터가 위치할 타일 정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_3"));

	// Tile의 위치에 맞도록 Position 변경
	PosOnTile();
}

void CFastScript::tick()
{
	CMonsterScript::tick();
}



void CFastScript::MosnsterAIPlay()
{
	// 이번 차례때 해야할일을 마치지 않았으니 다시 false를 넣어준다.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// TileMgr에게 Player와의 거리와 방향을 요청한다.
	m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

	// 플레이어와의 거리를 확인해 유효거리 안에 들어왔는지 확인한다.
	// 공격 범위보다 플레이어가 더 멀리 있다면,
	if (m_tMonsterInfo.Range < m_tAstarInfo.iTargetRange)
	{
		// 이동 패턴 실행을 위한 행동 정하기
		GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
		m_eMonsterPattern = MONSTER_PATTERN::MOVE;
	}
	// 공격 범위에 들어왔다면,
	else
	{
		// 공격 패턴 실행을 위한 행동 정하기
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

	// 효과음 재생
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
