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
	// 몬스터의 기본 스텟 넣어주기
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

	// 몬스터가 위치할 타일 정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_2"));

	// Tile의 위치에 맞도록 Position 변경
	PosOnTile();
}

void CRefBeetleScript::tick()
{
	CMonsterScript::tick();
}



void CRefBeetleScript::MosnsterAIPlay()
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

void CRefBeetleScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
	GetGameMgrScript()->GetPlayerScript()->Hit(m_tMonsterInfo.Attack);
}

void CRefBeetleScript::Move()
{
	// 또 이제, Monster가 이동을 할때, 실제로 움직이는 모션이 나오도록 잡아주어야 한다.
	// 해당 코드는 중간 과정을 생략한 테스트용 코드
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

	// 효과음 재생
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
