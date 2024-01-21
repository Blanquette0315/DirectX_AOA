#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CEventMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/GlobalComponent.h>
#include <Engine/CAnimation2D.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"
#include "CPlayerScript.h"
#include "CBloodEffectScript.h"

CMonsterScript::CMonsterScript(int _ScriptType)
	: CScript(_ScriptType)
	, m_eObjType(OBJECT_TYPE::MONSTER)
	, m_pOwnerTile(nullptr)
	, m_fAccTime(0.f)
	, m_bfAccMotionTime(0.f)
	, m_bTempDead(false)
	, m_ActionOnce(false)
	, m_bStun(false)
	, m_eMonsterPattern(MONSTER_PATTERN::NONE)
	, m_bMotionEnd(false)
	, m_bRotToPlayer(false)
	, m_bAnimStart(false)
	, m_bMonMove(false)
	, m_fAccMoveTime(0.f)
{
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::begin()
{
	// GameMgr에 등록하기
	CGameObject* pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)(pGameMgr->GetScript((UINT)GAMEMGRSCRIPT));
	m_pGameMgrScript->RegisterMonsterScript(this);

	// UI에서 Monster 정보 보기
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State MAX_HP", &m_tMonsterInfo.Max_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State CUR_HP", &m_tMonsterInfo.Cur_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State ATTACK", &m_tMonsterInfo.Attack);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State DEF   ", &m_tMonsterInfo.Deffence);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State Range ", &m_tMonsterInfo.Range);

	// 몬스터 포지션 이니셜
	PosInit();

	// 처음 진행하면 IDLE Animation을 실행해준다.
	PlayIDLEAnim();
}

void CMonsterScript::tick()
{
	// 애니메이션 모션 Test용도
	// 현재 진행하던 애니메이션이 끝났으면 IDLE 상태로 되돌려 준다.
	if (GetOwner()->Animator2D()->GetCurAnimation()->Is_End() && !m_bTempDead)
	{
		// 스턴일 동안은 예외처리
		if (!m_bStun)
		{
			PlayIDLEAnim();

			if (m_bAnimStart)
			{
				m_bTurnEnd = true;
				m_bAnimStart = false;
			}
		}
	}

	if (m_bMonMove)
	{
		if (m_fAccMoveTime < 0.1f)
		{
			m_fAccMoveTime += DT;
		}
		else if (m_fAccMoveTime >= 0.1f)
		{
			m_bTurnEnd = true;
			m_bMonMove = false;
			m_fAccMoveTime = 0.f;
		}
	}

	// 죽지 않았을 때만 플레이어를 바라보아야 한다.
	if (!m_bTempDead)
	{
		SetMonsterDir();
	}

	// 스턴이 걸렸다면 카운트를 세다가 풀리는 시점에 풀어준다.
	if (m_bStun)
	{
		PlayStunAnim();
		// 약속된 턴이 지나면 스턴을 풀어준다.
		if (m_iStunTurn == GetGameMgrScript()->GetCurTurnCount())
		{
			m_bStun = false;
		}
	}

	// 히트일때 DT를 세다가 슬로우 효과를 헤제 해주어야 한다.
	if (m_bHit)
	{
		// 누적시간 계산
		m_bfAccMotionTime += DT;

		if (m_bfAccMotionTime > 0.1)
		{
			CTimeMgr::GetInst()->ReleaseSlow();
			m_bfAccMotionTime = 0.f;
			m_bHit = false;
		}
	}
}



void CMonsterScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CMonsterScript::Overlap(CCollider2D* _pOther)
{
}

void CMonsterScript::EndOverlap(CCollider2D* _pOther)
{
}



void CMonsterScript::CreateHitSound()
{
	srand(time(nullptr));
	int iRand = rand() % 4;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_1.wav");
		pSound->Play(1, 1.0f, true);
	}
	else if (iRand == 1)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_2.wav");
		pSound->Play(1, 1.0f, true);
	}
	else if (iRand == 2)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_3.wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_4.wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CMonsterScript::CreateHitShieldSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\sword_hit_impact_heavy_03.wav");
		pSound->Play(1, 1.0f, true);
	}
	else if (iRand == 1)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\sword_hit_impact_heavy_05.wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CMonsterScript::CreateSlashSound()
{
	srand(time(nullptr));
	int iRand = rand() % 3;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_0(SH101_Sword Attack_Hollywood_Fienup_004).wav");
		pSound->Play(1, 1.0f, true);
	}
	else if (iRand == 1)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_1(whoosh_weapon_knife_swing_01).wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_2(whoosh_weapon_knife_swing_04).wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CMonsterScript::CreateHitEff()
{
	Ptr<CPrefab> pHitPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\HitEffect_Prefab_0.pref");

	CGameObject* pHitEffect = pHitPrefab->Instantiate();

	// 생성 후 begin 호출
	pHitEffect->begin();

	Vec3 vMonsterScale = GetOwner()->Transform()->GetRelativeScale();
	float y = vMonsterScale.y * 0.5f * -1.f;

	// 랜더를 위한 z 값 지정
	// 회전을 했는지에 따라 z값을 다르게 넣어주어야 한다.
	if (m_bRotToPlayer)
	{
		pHitEffect->Transform()->SetRelativePos(0.f, y, 2.f);
	}
	else
	{
		pHitEffect->Transform()->SetRelativePos(0.f, y, -2.f);
	}
	

	// 랜덤한 z축회전 입력
	srand(time(nullptr));
	int deg = rand() % 180;
	float rad = deg / XM_2PI;
	pHitEffect->Transform()->SetRelativeRotation(0.f, 0.f, rad);

	// 자식으로 설정한다.
	GetOwner()->AddChild(pHitEffect);

	// 레이어 설정
	pHitEffect->SetAllLayer(11);
}

void CMonsterScript::CreatePunchHitEff()
{
	Ptr<CPrefab> pHitPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_PunchHit_Prefab_0.pref");

	CGameObject* pHitEffect = pHitPrefab->Instantiate();

	Vec3 vMonsterScale = GetOwner()->Transform()->GetRelativeScale();
	float y = vMonsterScale.y * 0.25f;

	Vec3 vMonPos = Transform()->GetRelativePos();
	vMonPos.y -= y;
	vMonPos.z = 0.f;

	pHitEffect->Transform()->SetRelativePos(vMonPos);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pHitEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// 애니메이션 실행
	pHitEffect->Animator2D()->Play(L"Eff_PunchHit", false);
}

void CMonsterScript::CreateBloodEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Blood_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// 자식으로 설정한다.
	GetOwner()->AddChild(pEffect);

	// 레이어 설정
	pEffect->SetAllLayer(11);

	// 피 효과 애니메이션 랜덤 진행
	CBloodEffectScript* pBloodScript = (CBloodEffectScript*)pEffect->GetScript((UINT)BLOODEFFECTSCRIPT);
	
	// 몬스터가 회전을 했는지 넣어주는 함수 호출
	pBloodScript->SetRotOwner(m_bRotToPlayer);
	
	pBloodScript->PlayRandAnim();
}

void CMonsterScript::CreateShieldParticle()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Par_Shield_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// 생성 지점 설정해주기
	Vec3 vParitclePos = Transform()->GetRelativePos();

	vParitclePos.y -= 20.f;
	vParitclePos.z = 0.f;

	pEffect->Transform()->SetRelativePos(vParitclePos);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);
}

void CMonsterScript::MonsterAction()
{
	if (!m_ActionOnce)
	{
		m_ActionOnce = true;

		switch (m_eMonsterPattern)
		{
		case MONSTER_PATTERN::ATTACK:
		{
			Attack();
			break;
		}

		case MONSTER_PATTERN::STUN:
			break;

		case MONSTER_PATTERN::RESTRAIN:
		{
			Restrain();
			break;
		}
			
		case MONSTER_PATTERN::CHARGE:
		{
			Charge();
			break;
		}
			
		case MONSTER_PATTERN::RUSH:
		{
			Rush();
			break;
		}
			
		case MONSTER_PATTERN::RELOAD:
		{
			Reload();
			break;
		}

		case MONSTER_PATTERN::MOVE:
		{
			Move();
			break;
		}

		case MONSTER_PATTERN::SHEILD:
		{
			Sheild();
			break;
		}

		case MONSTER_PATTERN::RUN:
		{
			Run();
			break;
		}

		case MONSTER_PATTERN::BOSS_CHARGE:
		{
			Boss_Charge();
			break;
		}

		case MONSTER_PATTERN::BOSS_RUSH:
		{
			Boss_Rush();
			break;
		}

		}
	}
}

void CMonsterScript::SetTempDead()
{
	CTileScript* pTileScript = (CTileScript*)m_pOwnerTile->GetScript((UINT)TILESCRIPT);
	pTileScript->ReleaseTargetObj();
	m_pOwnerTile = nullptr;
}

void CMonsterScript::SetMonsterDir()
{
	// 몬스터가 바라보는 방향 정하기
	Vec3 vPlayerWPos = m_pGameMgrScript->GetPlayer()->Transform()->GetWorldPos();
	if (GetOwner()->Transform()->GetWorldPos().x < vPlayerWPos.x)
	{
		// 우측을 바라보도록 회전한다.
		GetOwner()->Transform()->SetRelativeRotation(0.f, 3.14f, 0.f);
		m_bRotToPlayer = true;
	}
	else
	{
		// 왼쪽을 바라보도록 회전한다.
		GetOwner()->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
		m_bRotToPlayer = false;
	}
}

void CMonsterScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	m_bHit = true;

	int iATT = ATK;

	// DT 슬로우 요청
	tEvent tevt = {};
	tevt.eType = EVENT_TYPE::SLOWTIME;
	CEventMgr::GetInst()->AddEvent(tevt);


	// 플레이어가 공격한 무기에 알맞게 피격 이펙트가 출력되어야 한다.
	// 피격 이펙트 생성
	switch (_eWaponType)
	{
	case WAPON_TYPE::KATANA:
	case WAPON_TYPE::SWORD:
	{
		CreateHitEff();
		break;
	}
	case WAPON_TYPE::SHIELD:
	{
		CreatePunchHitEff();
		break;
	}

	}

	// 피 효과 생성
	CreateBloodEff();
	

	// 몬스터의 방어가 있을 경우 방어를 지운다.
	if (m_tMonsterInfo.Deffence != 0)
	{
		// 이번 피해량이 방어 보다 낮을 경우
		if (m_tMonsterInfo.Deffence < ATK)
		{
			iATT -= m_tMonsterInfo.Deffence;
			m_tMonsterInfo.Deffence = 0;

			// HP 깎기
			if (m_tMonsterInfo.Cur_HP != 0)
			{
				if (m_tMonsterInfo.Cur_HP > iATT)
				{
					m_tMonsterInfo.Cur_HP -= iATT;

					// 효과음 재생
					CreateHitSound();
				}
				// 플레이어의 현재 HP가 받은 피해량 보다 작다면 0을 준다.
				else
				{
					m_tMonsterInfo.Cur_HP = 0;

					// 효과음 재생
					CreateHitSound();
				}
			}

			// 뿐만 아니라 이번 공격때, 쉴드가 깨졌다는 뜻이므로 스턴에 걸린다.
			m_iStunTurn = GetGameMgrScript()->GetCurTurnCount();
			m_iStunTurn += 2;
			m_bStun = true;
		}
		// 피해량과 방어력이 같은 경우
		else if (m_tMonsterInfo.Deffence == ATK)
		{
			m_tMonsterInfo.Deffence -= iATT;
			iATT = 0;

			// 효과음 재생
			CreateHitShieldSound();

			// 뿐만 아니라 이번 공격때, 쉴드가 깨졌다는 뜻이므로 스턴에 걸린다.
			m_iStunTurn = GetGameMgrScript()->GetCurTurnCount();
			m_iStunTurn += 2;
			m_bStun = true;
		}
		// 피해량 보다 방어력이 더 높을 경우
		else
		{
			m_tMonsterInfo.Deffence -= iATT;
			iATT = 0;

			// 효과음 재생
			CreateHitShieldSound();
		}
	}
	// 방어가 없을 경우
	else
	{
		// HP 깎기
		if (m_tMonsterInfo.Cur_HP != 0)
		{
			if (m_tMonsterInfo.Cur_HP > iATT)
			{
				m_tMonsterInfo.Cur_HP -= iATT;

				// 효과음 재생
				CreateHitSound();
			}
			// 플레이어의 현재 HP가 받은 피해량 보다 작다면 0을 준다.
			else
			{
				m_tMonsterInfo.Cur_HP = 0;

				// 효과음 재생
				CreateHitSound();
			}
		}
	}

	// Monster의 현재 HP가 0보다 클 경우 방어 모션을 한번 실행해 준다.
	if (0 < m_tMonsterInfo.Cur_HP)
	{
		// 스턴인데, 방어 모션이 나오는 것은 이상하므로 예외처리를 해준다.
		if (!m_bStun)
		{
			PlayDefAnim();
		}
	}

	// Monster의 현재 HP가 0이 되면, 죽은 상태로 변경한다.
	else if (0 == m_tMonsterInfo.Cur_HP)
	{
		// 죽은 상태로 변경
		SetTempDead();

		// 죽었을 때, z축 포지션 설정
		Transform()->SetRelativePos_Z(600.f);

		// 스턴을 풀어주어야 모션이 제대로 나온다..
		m_bStun = false;
		PlayDeadAnim();
		m_bTempDead = true;
	}
}

void CMonsterScript::Attack()
{
	m_pGameMgrScript->GetPlayerScript()->Hit(m_tMonsterInfo.Attack);
	PlayAttackAnim();

	m_bAnimStart = true;
}

void CMonsterScript::SetOwnerTile(CGameObject* _Tile)
{
	m_pOwnerTile = _Tile;
	CTileScript* m_pTileScript = (CTileScript*)m_pOwnerTile->GetScript((int)TILESCRIPT);
	m_pTileScript->SetTargetObj(GetOwner(), m_eObjType);
}

void CMonsterScript::PosInit()
{
	// 본인의 발을 기준으로 기준 변경해주기 (임시로 Scale의 1/2을 올려주는 거라 임시 방편)
	Vec3 vMonsterPos = Vec3(0.f, 0.f, 100.f);
	Vec3 vMonsterScale = GetOwner()->Transform()->GetRelativeScale();
	vMonsterPos.y += vMonsterScale.y * 0.5f;

	GetOwner()->Transform()->SetRelativePos(vMonsterPos);
	m_vMonsterPos.x = vMonsterPos.x;
	m_vMonsterPos.y = vMonsterPos.y;
}

void CMonsterScript::PosOnTile()
{
	Vec3 vMonsterPos = Vec3(m_vMonsterPos.x, m_vMonsterPos.y, 100);
	Vec3 vTileWorldPos = m_pOwnerTile->Transform()->GetWorldPos();
	vMonsterPos.x += vTileWorldPos.x;
	vMonsterPos.y += vTileWorldPos.y;
	GetOwner()->Transform()->SetRelativePos(vMonsterPos);
}


void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}