#include "pch.h"
#include "CCrossBowScript.h"

#include <Engine/CEventMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"
#include "CMissileScript.h"
#include "CPlayerScript.h"

CCrossBowScript::CCrossBowScript()
	: CMonsterScript(CROSSBOWSCRIPT)
	, m_Prefab(nullptr)
	, m_pTargetTileScript(nullptr)
	, m_bBowLoading(true)
{
	// ������ �⺻ ���� �־��ֱ�
	//HP 12~18
	m_tMonsterInfo.Max_HP = 12;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 10;
	m_tMonsterInfo.Deffence = 0;
	m_tMonsterInfo.Range = 2;
}

CCrossBowScript::~CCrossBowScript()
{
}

void CCrossBowScript::begin()
{
	CMonsterScript::begin();

	// ���Ͱ� ��ġ�� Ÿ�� �����ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_3"));

	// Tile�� ��ġ�� �µ��� Position ����
	PosOnTile();

	// ȭ�� ������ ã�Ƽ� �ּ� �����ϱ�
	m_Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Arrow_Prefab_0.pref");
}

void CCrossBowScript::tick()
{
	CMonsterScript::tick();
}




void CCrossBowScript::CreateArrowShootEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_ArrowShoot_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// ȸ�� ���ֱ�
	Vec3 TargetPos = GetGameMgrScript()->GetPlayerScript()->GetTile()->Transform()->GetWorldPos();
	TargetPos.y += Transform()->GetRelativeScale().y * 0.5f;

	Vec3 OriginPos = Transform()->GetRelativePos();

	// ���� ���ϱ�
	Vec2 vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// �� ���� ������ ���� ���ϱ�
	float fSeta = atan2f(vDifPos.y, vDifPos.x);

	// ������ŭ ����Ʈ ȸ��
	pEffect->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, fSeta));

	// ���� ���� ����
	Vec3 vMonPos = Transform()->GetRelativePos();
	vMonPos.x += 75.f * cos(fSeta);
	vMonPos.y += 75.f * sin(fSeta);


	pEffect->Transform()->SetRelativePos(vMonPos);
	pEffect->Transform()->SetRelativePos_Z(0.f);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// ����Ʈ �ִϸ��̼� ����
	pEffect->Animator2D()->Play(L"Eff_ArrowShoot", false);
}

void CCrossBowScript::CreateArrowReloadSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_draw_stretch2_08.wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_draw_stretch2_09.wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CCrossBowScript::CreateArrowShootSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_shoot_type1_03.wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_shoot_type1_04.wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CCrossBowScript::MosnsterAIPlay()
{
	// �̹� ���ʶ� �ؾ������� ��ġ�� �ʾ����� �ٽ� false�� �־��ش�.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// �÷��̾ ���� �ȿ� ���Դ��� Ȯ���Ѵ�.
	// ��ȯ���� Ÿ���� ȭ���� �����ؾ��ϴ� Ÿ��.
	m_pTargetTileScript = GetOwnerTileScript()->FindPlayerinRange(m_tMonsterInfo.Range);
	if (m_pTargetTileScript != nullptr)
	{
		// ȭ���� ������ �Ǿ��ִ��� Ȯ���Ѵ�.
		if (m_bBowLoading)
		{
			// ���� ���� ������ ���� �ൿ ���ϱ�
			m_eMonsterPattern = MONSTER_PATTERN::ATTACK;
		}
		else
		{
			// ������ �ϱ�
			m_eMonsterPattern = MONSTER_PATTERN::RELOAD;
		}
		
	}
	else
	{
		// TileMgr���� Player���� �Ÿ��� ������ ��û�Ѵ�.
		m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

		// �÷��̾���� �Ÿ��� Ȯ���� ��ȿ�Ÿ� �ȿ� ���Դ��� Ȯ���Ѵ�.
		// ���� �������� �÷��̾ �� �ָ� �ִٸ�,
		if (1 < m_tAstarInfo.iTargetRange)
		{
			// �̵� ���� ������ ���� �ൿ ���ϱ�
			GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
			m_eMonsterPattern = MONSTER_PATTERN::MOVE;
		}
	}
}

void CCrossBowScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
}

void CCrossBowScript::Move()
{
	PosOnTile();
	m_bMonMove = true;
}

void CCrossBowScript::Attack()
{
	PlayAttackAnim();

	// ȿ���� �ֱ�
	CreateArrowShootSound();

	CreateArrowShootEff();

	// ȭ�� ������ �̴Ͻÿ���Ʈ
	CGameObject* pArrow = m_Prefab->Instantiate();
	pArrow->Transform()->SetRelativePos(Transform()->GetRelativePos());
	// ȭ�쿡�� �ڽ��� ��ǥ�� Ÿ�� �������ֱ�
	CMissileScript* pMScript = (CMissileScript*)(pArrow->GetScript((UINT)MISSILESCRIPT));
	pMScript->SetTargetTile(m_pTargetTileScript);
	pMScript->SetOwnerPos(Transform()->GetRelativeScale());
	pMScript->SetATK(m_tMonsterInfo.Attack);
	pMScript->CalculateTarget();

	tEvent tEVN = {};
	tEVN.eType = EVENT_TYPE::CREATE_OBJECT;
	tEVN.wParam = (DWORD_PTR)pArrow;
	tEVN.lParam = 4;

	CEventMgr::GetInst()->AddEvent(tEVN);

	m_bBowLoading = false;
	m_bAnimStart = true;
}

void CCrossBowScript::Reload()
{
	PlayDefAnim();

	// ȿ���� �ֱ�
	CreateArrowReloadSound();

	m_bBowLoading = true;
	m_bAnimStart = true;
}

void CCrossBowScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_CrossBow", true);
}

void CCrossBowScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_CrossBow", true);
}

void CCrossBowScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_CrossBow", true);
}

void CCrossBowScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_CrossBow", true);
}




void CCrossBowScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CCrossBowScript::Overlap(CCollider2D* _pOther)
{
}

void CCrossBowScript::EndOverlap(CCollider2D* _pOther)
{
}

void CCrossBowScript::SaveToFile(FILE* _File)
{
}

void CCrossBowScript::LoadFromFile(FILE* _File)
{
}
