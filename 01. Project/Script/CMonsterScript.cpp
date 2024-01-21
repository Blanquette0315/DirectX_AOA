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
	// GameMgr�� ����ϱ�
	CGameObject* pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)(pGameMgr->GetScript((UINT)GAMEMGRSCRIPT));
	m_pGameMgrScript->RegisterMonsterScript(this);

	// UI���� Monster ���� ����
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State MAX_HP", &m_tMonsterInfo.Max_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State CUR_HP", &m_tMonsterInfo.Cur_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State ATTACK", &m_tMonsterInfo.Attack);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State DEF   ", &m_tMonsterInfo.Deffence);
	AddScriptParam(SCRIPT_PARAM::INT, "Monster State Range ", &m_tMonsterInfo.Range);

	// ���� ������ �̴ϼ�
	PosInit();

	// ó�� �����ϸ� IDLE Animation�� �������ش�.
	PlayIDLEAnim();
}

void CMonsterScript::tick()
{
	// �ִϸ��̼� ��� Test�뵵
	// ���� �����ϴ� �ִϸ��̼��� �������� IDLE ���·� �ǵ��� �ش�.
	if (GetOwner()->Animator2D()->GetCurAnimation()->Is_End() && !m_bTempDead)
	{
		// ������ ������ ����ó��
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

	// ���� �ʾ��� ���� �÷��̾ �ٶ󺸾ƾ� �Ѵ�.
	if (!m_bTempDead)
	{
		SetMonsterDir();
	}

	// ������ �ɷȴٸ� ī��Ʈ�� ���ٰ� Ǯ���� ������ Ǯ���ش�.
	if (m_bStun)
	{
		PlayStunAnim();
		// ��ӵ� ���� ������ ������ Ǯ���ش�.
		if (m_iStunTurn == GetGameMgrScript()->GetCurTurnCount())
		{
			m_bStun = false;
		}
	}

	// ��Ʈ�϶� DT�� ���ٰ� ���ο� ȿ���� ���� ���־�� �Ѵ�.
	if (m_bHit)
	{
		// �����ð� ���
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

	// ���� �� begin ȣ��
	pHitEffect->begin();

	Vec3 vMonsterScale = GetOwner()->Transform()->GetRelativeScale();
	float y = vMonsterScale.y * 0.5f * -1.f;

	// ������ ���� z �� ����
	// ȸ���� �ߴ����� ���� z���� �ٸ��� �־��־�� �Ѵ�.
	if (m_bRotToPlayer)
	{
		pHitEffect->Transform()->SetRelativePos(0.f, y, 2.f);
	}
	else
	{
		pHitEffect->Transform()->SetRelativePos(0.f, y, -2.f);
	}
	

	// ������ z��ȸ�� �Է�
	srand(time(nullptr));
	int deg = rand() % 180;
	float rad = deg / XM_2PI;
	pHitEffect->Transform()->SetRelativeRotation(0.f, 0.f, rad);

	// �ڽ����� �����Ѵ�.
	GetOwner()->AddChild(pHitEffect);

	// ���̾� ����
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

	// �ִϸ��̼� ����
	pHitEffect->Animator2D()->Play(L"Eff_PunchHit", false);
}

void CMonsterScript::CreateBloodEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Blood_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// �ڽ����� �����Ѵ�.
	GetOwner()->AddChild(pEffect);

	// ���̾� ����
	pEffect->SetAllLayer(11);

	// �� ȿ�� �ִϸ��̼� ���� ����
	CBloodEffectScript* pBloodScript = (CBloodEffectScript*)pEffect->GetScript((UINT)BLOODEFFECTSCRIPT);
	
	// ���Ͱ� ȸ���� �ߴ��� �־��ִ� �Լ� ȣ��
	pBloodScript->SetRotOwner(m_bRotToPlayer);
	
	pBloodScript->PlayRandAnim();
}

void CMonsterScript::CreateShieldParticle()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Par_Shield_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// ���� ���� �������ֱ�
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
	// ���Ͱ� �ٶ󺸴� ���� ���ϱ�
	Vec3 vPlayerWPos = m_pGameMgrScript->GetPlayer()->Transform()->GetWorldPos();
	if (GetOwner()->Transform()->GetWorldPos().x < vPlayerWPos.x)
	{
		// ������ �ٶ󺸵��� ȸ���Ѵ�.
		GetOwner()->Transform()->SetRelativeRotation(0.f, 3.14f, 0.f);
		m_bRotToPlayer = true;
	}
	else
	{
		// ������ �ٶ󺸵��� ȸ���Ѵ�.
		GetOwner()->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
		m_bRotToPlayer = false;
	}
}

void CMonsterScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	m_bHit = true;

	int iATT = ATK;

	// DT ���ο� ��û
	tEvent tevt = {};
	tevt.eType = EVENT_TYPE::SLOWTIME;
	CEventMgr::GetInst()->AddEvent(tevt);


	// �÷��̾ ������ ���⿡ �˸°� �ǰ� ����Ʈ�� ��µǾ�� �Ѵ�.
	// �ǰ� ����Ʈ ����
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

	// �� ȿ�� ����
	CreateBloodEff();
	

	// ������ �� ���� ��� �� �����.
	if (m_tMonsterInfo.Deffence != 0)
	{
		// �̹� ���ط��� ��� ���� ���� ���
		if (m_tMonsterInfo.Deffence < ATK)
		{
			iATT -= m_tMonsterInfo.Deffence;
			m_tMonsterInfo.Deffence = 0;

			// HP ���
			if (m_tMonsterInfo.Cur_HP != 0)
			{
				if (m_tMonsterInfo.Cur_HP > iATT)
				{
					m_tMonsterInfo.Cur_HP -= iATT;

					// ȿ���� ���
					CreateHitSound();
				}
				// �÷��̾��� ���� HP�� ���� ���ط� ���� �۴ٸ� 0�� �ش�.
				else
				{
					m_tMonsterInfo.Cur_HP = 0;

					// ȿ���� ���
					CreateHitSound();
				}
			}

			// �Ӹ� �ƴ϶� �̹� ���ݶ�, ���尡 �����ٴ� ���̹Ƿ� ���Ͽ� �ɸ���.
			m_iStunTurn = GetGameMgrScript()->GetCurTurnCount();
			m_iStunTurn += 2;
			m_bStun = true;
		}
		// ���ط��� ������ ���� ���
		else if (m_tMonsterInfo.Deffence == ATK)
		{
			m_tMonsterInfo.Deffence -= iATT;
			iATT = 0;

			// ȿ���� ���
			CreateHitShieldSound();

			// �Ӹ� �ƴ϶� �̹� ���ݶ�, ���尡 �����ٴ� ���̹Ƿ� ���Ͽ� �ɸ���.
			m_iStunTurn = GetGameMgrScript()->GetCurTurnCount();
			m_iStunTurn += 2;
			m_bStun = true;
		}
		// ���ط� ���� ������ �� ���� ���
		else
		{
			m_tMonsterInfo.Deffence -= iATT;
			iATT = 0;

			// ȿ���� ���
			CreateHitShieldSound();
		}
	}
	// �� ���� ���
	else
	{
		// HP ���
		if (m_tMonsterInfo.Cur_HP != 0)
		{
			if (m_tMonsterInfo.Cur_HP > iATT)
			{
				m_tMonsterInfo.Cur_HP -= iATT;

				// ȿ���� ���
				CreateHitSound();
			}
			// �÷��̾��� ���� HP�� ���� ���ط� ���� �۴ٸ� 0�� �ش�.
			else
			{
				m_tMonsterInfo.Cur_HP = 0;

				// ȿ���� ���
				CreateHitSound();
			}
		}
	}

	// Monster�� ���� HP�� 0���� Ŭ ��� ��� ����� �ѹ� ������ �ش�.
	if (0 < m_tMonsterInfo.Cur_HP)
	{
		// �����ε�, ��� ����� ������ ���� �̻��ϹǷ� ����ó���� ���ش�.
		if (!m_bStun)
		{
			PlayDefAnim();
		}
	}

	// Monster�� ���� HP�� 0�� �Ǹ�, ���� ���·� �����Ѵ�.
	else if (0 == m_tMonsterInfo.Cur_HP)
	{
		// ���� ���·� ����
		SetTempDead();

		// �׾��� ��, z�� ������ ����
		Transform()->SetRelativePos_Z(600.f);

		// ������ Ǯ���־�� ����� ����� ���´�..
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
	// ������ ���� �������� ���� �������ֱ� (�ӽ÷� Scale�� 1/2�� �÷��ִ� �Ŷ� �ӽ� ����)
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