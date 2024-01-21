#include "pch.h"
#include "CChargeScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"

CChargeScript::CChargeScript()
	: CMonsterScript(CHARGESCRIPT)
{
	// 몬스터의 기본 스텟 넣어주기
	// HP는 20~30
	m_tMonsterInfo.Max_HP = 3;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 15;
	m_tMonsterInfo.Deffence = 0;
	m_tMonsterInfo.Range = 2;
}

CChargeScript::~CChargeScript()
{
}


void CChargeScript::begin()
{
	CMonsterScript::begin();

	// 몬스터가 위치할 타일 정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_23"));

	// Tile의 위치에 맞도록 Position 변경
	PosOnTile();
}

void CChargeScript::tick()
{
	CMonsterScript::tick();
}



void CChargeScript::MosnsterAIPlay()
{
	// 이번 차례때 해야할일을 마치지 않았으니 다시 false를 넣어준다.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// 돌진 준비중이 아닐 때만 동작해야 한다.
	if (!m_bCharge)
	{
		// 공격 방향과 공격 사거리를 생각해서 플레이어가 존재하는지 확인한다.
		m_eTargetDIR = GetOwnerTileScript()->FindPlayerinRange_DIR(m_tMonsterInfo.Range);
		// 반환이 nullptr일 경우 범위내에 플레이어가 없다는 것이다.
		if (m_eTargetDIR != TILE_DIR::END)
		{
			// 플레이어가 범위 내에 있었다면, 돌진을 준비한다.
			m_eMonsterPattern = MONSTER_PATTERN::CHARGE;
		}
		// 범위 내에 플레이어가 없다면, 이동한다.
		else
		{
			// TileMgr에게 Player와의 거리와 방향을 요청한다.
			m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

			// 플레이어와의 거리를 확인해 유효거리 안에 들어왔는지 확인한다.
			// 공격 범위보다 플레이어가 더 멀리 있다면,
			if (m_tMonsterInfo.Range < 1)
			{
				// 이동 패턴 실행을 위한 행동 정하기
				GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
				m_eMonsterPattern = MONSTER_PATTERN::MOVE;
			}
		}
	}
	// 돌진 준비 중이었다면, 돌진을 실행한다.
	else
	{
		// 돌진 패턴 실행을 위한 행동 정하기
		m_eMonsterPattern = MONSTER_PATTERN::RUSH;
	}
}

void CChargeScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
}

void CChargeScript::Move()
{
	PosOnTile();
	m_bTurnEnd = true;
}

void CChargeScript::Charge()
{
	// 방어 장면을 차징과 히트 두개로 사용할 예정이다.
	PlayDefAnim();
	m_bTurnEnd = true;
}

void CChargeScript::Rush()
{
	PlayAttackAnim();
}

void CChargeScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_Charge", true);
}

void CChargeScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_Charge", true);
}

void CChargeScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_Charge", true);
}

void CChargeScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_Charge", true);
}

void CChargeScript::PlayStunAnim()
{
	Animator2D()->Play(L"Stun_Charge", true);
}



void CChargeScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CChargeScript::Overlap(CCollider2D* _pOther)
{
}

void CChargeScript::EndOverlap(CCollider2D* _pOther)
{
}

void CChargeScript::SaveToFile(FILE* _File)
{
}

void CChargeScript::LoadFromFile(FILE* _File)
{
}
