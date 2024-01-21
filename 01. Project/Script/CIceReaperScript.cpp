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
	// 몬스터의 기본 스텟 넣어주기
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

	// 몬스터가 위치할 타일 정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_3"));

	// Tile의 위치에 맞도록 Position 변경
	PosOnTile();
}

void CIceReaperScript::tick()
{
	CMonsterScript::tick();
}



void CIceReaperScript::MosnsterAIPlay()
{
	// 이번 차례때 해야할일을 마치지 않았으니 다시 false를 넣어준다.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// 패턴 범위 내에 플레이어가 있는지 확인한다.
	m_pTargetTileScript = GetOwnerTileScript()->FindPlayerinRange(2);
	if (m_pTargetTileScript != nullptr)
	{
		if (false == m_bCharge)
		{
			// 범위 안에 플레이어가 있다면 러시를 준비한다.
			m_eMonsterPattern = MONSTER_PATTERN::BOSS_CHARGE;
		}
	}
	// 범위 내에 없다면, 이동을 한다.
	else
	{
		// TileMgr에게 Player와의 거리와 방향을 요청한다.
		m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

		// 플레이어와의 거리를 확인해 유효거리 안에 들어왔는지 확인한다.
		if (1 < m_tAstarInfo.iTargetRange)
		{
			// 이동 패턴 실행을 위한 행동 정하기
			GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
			m_eMonsterPattern = MONSTER_PATTERN::MOVE;
		}
	}

	// 이전에 차지 중이었다면, 러시를 한다.
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
