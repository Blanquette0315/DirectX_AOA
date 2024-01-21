#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CMaterial.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CLevel.h>
#include <Engine/CSound.h>

#include "CGameMgrScript.h"
#include "CCardMgrScript.h"
#include "CTileScript.h"
#include "CMonsterScript.h"

#include "CPlayerRightHandScript.h"
#include "CPlayerLeftHandScript.h"
#include "CPlayerHandScript.h"

#include "CMissileScript.h"
#include "CWaponScript.h"

#include "CUIMgrScript.h"
#include "CBloodEffectScript.h"
#include "CUnderAttackScript.h"

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_eObjType(OBJECT_TYPE::PLAYER)
	, m_pOwnerTile(nullptr)
	, m_pGameMgrScript(nullptr)
	, m_fAccTime(0.f)
	, m_fAccel(500.f)
	, m_fSpeed(320.f)
	, m_bAtteck(false)
	, m_bLockRot(false)
	, m_bRot(false)
	, m_bMove(false)
	, m_bHit(false)
	, m_pMoveTarget(nullptr)
	, m_vDifPos{}
	, m_fTargetDestance(0.f)
	, m_fAccDestance(0.f)
	, m_fSeta(0.f)
	, m_iStep(0)
	, m_bStepOnce(false)
	, m_iPlayerDebuf(0.f)
	, m_arrInven{}
	, m_bSelectedCard(false)
	, m_bOneMoretick(false)
	, m_OnceEffect(false)
	, m_bSlowROnce(false)
	, m_bArrowHit(false)
{
	// 플레이어의 초기 정보 넣어주기
	m_tPlayerInfo.Max_HP = 100;
	m_tPlayerInfo.Cur_HP = m_tPlayerInfo.Max_HP;
	m_tPlayerInfo.RightAttack = 0;
	m_tPlayerInfo.LeftAttack = 0;
	m_tPlayerInfo.Deffence = 0;
	m_tPlayerInfo.Max_Acction = 3;
	m_tPlayerInfo.Cur_Acction = m_tPlayerInfo.Max_Acction;
	m_tPlayerInfo.DrowCount = 6;
	m_fSpeed = 320.f;

	// Player가 지닌 기본 카드를 배열에 넣어주기
	DefaultCardSetting();

	// 플레이어의 기본 무기 설정
	// 왼손
	tPlayer_Wapon PlayerWapon = {};
	PlayerWapon.eWaponType = WAPON_TYPE::SWORD;
	PlayerWapon.eWaponDir = CARD_DIR_TYPE::LEFT;
	PlayerWapon.iATT = 4;
	PlayerWapon.iDff = 0;

	m_tLefetWapon = PlayerWapon;
	m_eLeftWaponType = m_tLefetWapon.eWaponType;

	//오른손
	PlayerWapon.eWaponType = WAPON_TYPE::SHIELD;
	PlayerWapon.eWaponDir = CARD_DIR_TYPE::RIGHT;
	PlayerWapon.iATT = 0;
	PlayerWapon.iDff = 2;

	m_tRightWapon = PlayerWapon;
	m_eRightWaponType = m_tRightWapon.eWaponType;

	// 플레이어에게 회복 아이템 넣어주기
	tItem_Info PlayerItem = {};

	PlayerItem.eItemType = ITEM_TYPE::ITEM;

	tItem tempITem = {};
	tempITem.eItemType = ITEM_NAME::HPPOTION;

	PlayerItem.tItem = tempITem;
	PlayerItem.bExist = true;

	m_arrInven[0] = PlayerItem;

	// 두번째 아이템 넣어주기
	PlayerItem = {};
	PlayerWapon.eWaponDir = CARD_DIR_TYPE::RIGHT;
	PlayerWapon.eWaponType = WAPON_TYPE::SWORD;
	PlayerWapon.iATT = 4;
	PlayerWapon.iDff = 0;

	PlayerItem.tWapon = PlayerWapon;
	PlayerItem.bExist = true;

	m_arrInven[1] = PlayerItem;
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	// UI에서 Player 정보 보기
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player Move Speed ", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player Accel      ", &m_fAccel);

	AddScriptParam(SCRIPT_PARAM::INT, "Player State MAX_HP ", &m_tPlayerInfo.Max_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State CUR_HP ", &m_tPlayerInfo.Cur_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State RATTACK", &m_tPlayerInfo.RightAttack);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State LATTACK", &m_tPlayerInfo.LeftAttack);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State DEF    ", &m_tPlayerInfo.Deffence);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State MAX_ACT", &m_tPlayerInfo.Max_Acction);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State CUR_ACT", &m_tPlayerInfo.Cur_Acction);

	// GameMgr에 플레이어 등록하기
	CGameObject* pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)(pGameMgr->GetScript(GAMEMGRSCRIPT));
	m_pGameMgrScript->RegisterPlayer(GetOwner());

	// 플레이어 포지션 이니셜
	PosInit();

	// 플레이어가 위치한 타일 설정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_0"));

	// Tile의 위치에 맞도록 Position 변경
	PosOnTile();

	// 임시로 무기 카드 만들어 주기 나중에는 아레나에 진입할 때만 진행되어야 한다.
	// 카드 생성은 각각의 슬롯에서 해야한다.
	//CreateWaponCard();

	// 처음 진행하면 IDLE Animation을 실행해준다.
	IDLEAnimPlay();
}

void CPlayerScript::tick()
{
	// 애니메이션 모션 Test용도
	// 현재 진행하던 애니메이션이 끝났으면 IDLE 상태로 되돌려 준다.
	if (GetOwner()->Animator2D()->GetCurAnimation()->Is_End())
	{
		IDLEAnimPlay();
		m_bLockRot = false;
	}

	// 공격이 활성화 되었다면,
	if (m_bAtteck)
	{
		Attack_tick();
	}

	// 이동이 활성화 되었다면, 이동을 수행한다.
	if (m_bMove)
	{
		Move_tick();
	}

	// 본인이 공격 중이 아닐 때만 Hit가 진행되어야 한다.
	// RefBeetle의 반사 대미지를 입을 때 떄문.
	if (!m_bAtteck && m_bHit)
	{
		Hit_tick();
	}

	// Hit모션 테스트용
	if (KEY_TAP(KEY::H))
	{
		m_bHit = true;
	}

	// 마우스 포지션에 따른 Player의 회전은 카드가 선택된게 있을 때만 해야한다.
	//if (m_bSelectedCard)
	//{
	//	// 마우스 포지션 체크 용도
	//	Vec2 vMousePos = CKeyMgr::GetInst()->GetGWMousePos();
	//	Vec3 vPlayerPos = GetOwner()->Transform()->GetRelativePos();
	//	// 다른 애니메이션 동작 중일 때는 동작하면 안됨.
	//	if (!m_bLockRot && !m_bAtteck)
	//	{
	//		if (vPlayerPos.x < vMousePos.x)
	//		{
	//			// 마우스가 플레이어보다 우측에 있다는 것
	//			PlayerRotRight();
	//		}
	//		else
	//		{
	//			PlayerRotLeft();
	//		}
	//	}
	//}
	//else
	//{
	//	PlayerRotLeft();
	//}

	// 마우스 포지션 체크 용도
	Vec2 vMousePos = CKeyMgr::GetInst()->GetGWMousePos();
	Vec3 vPlayerPos = GetOwner()->Transform()->GetRelativePos();
	// 다른 애니메이션 동작 중일 때는 동작하면 안됨.
	if (!m_bLockRot && !m_bAtteck)
	{
		if (m_bSelectedCard)
		{
			if (vPlayerPos.x < vMousePos.x)
			{
				// 마우스가 플레이어보다 우측에 있다는 것
				PlayerRotRight();
			}
			else
			{
				PlayerRotLeft();
			}
		}
	}
}




void CPlayerScript::SetOwnerTile(CGameObject* _Tile)
{
	m_pOwnerTile = _Tile;
	CTileScript* m_pTileScript = (CTileScript*)m_pOwnerTile->GetScript((int)TILESCRIPT);
	m_pTileScript->SetTargetObj(GetOwner(), m_eObjType);
}

void CPlayerScript::PosInit()
{
	// 본인의 발을 기준으로 기준 변경해주기 (임시로 Scale의 1/2을 올려주는 거라 임시 방편)
	Vec3 vPlayerPos = Vec3(0.f, 0.f, 100.f);
	Vec3 vPlayerScale = GetOwner()->Transform()->GetRelativeScale();
	vPlayerPos.y += vPlayerScale.y * 0.5f;

	GetOwner()->Transform()->SetRelativePos(vPlayerPos);
	m_vPlayerPos.x = vPlayerPos.x;
	m_vPlayerPos.y = vPlayerPos.y;
}

void CPlayerScript::PosOnTile()
{
	// Player가 마우스 위치에 따라 회전하도록 풀어준다.
	m_bLockRot = false;

	Vec2 vPlayerPos = Vec2(m_vPlayerPos.x, m_vPlayerPos.y);
	Vec3 vTileWorldPos = m_pOwnerTile->Transform()->GetWorldPos();
	vPlayerPos.x += vTileWorldPos.x;
	vPlayerPos.y += vTileWorldPos.y;
	GetOwner()->Transform()->SetRelativePos_XY(vPlayerPos);
}

void CPlayerScript::PosOnTempTile(CTileScript* _TempTile)
{
	// Player가 마우스 위치에 따라 회전하지 않도록 막는다.
	m_bLockRot = true;

	// 임시로 이동되어야 할 타일이 원래 타일보다 우측인지 좌측인지를 확인해서 플레이어가 보는 방향을 정해준다.
	Vec3 OriginTilePos = m_pOwnerTile->Transform()->GetRelativePos();
	Vec3 TempTilePos = _TempTile->Transform()->GetRelativePos();

	// 임시 타일이 기존 타일보다 우측일 경우
	if (OriginTilePos.x < TempTilePos.x)
	{
		PlayerRotRight();
	}
	else
	{
		PlayerRotLeft();
	}

	Vec3 vPlayerPos = Vec3(m_vPlayerPos.x, m_vPlayerPos.y, 100);
	Vec3 vTileWorldPos = _TempTile->Transform()->GetWorldPos();
	vPlayerPos.x += vTileWorldPos.x;
	vPlayerPos.y += vTileWorldPos.y;
	GetOwner()->Transform()->SetRelativePos(vPlayerPos);
}

void CPlayerScript::IDLEAnimPlay()
{
	// 만약 오른쪽 무기가 카타나라면, 양손이라는 의미이다.
	if (m_eRightWaponType == WAPON_TYPE::KATANA)
	{
		Animator2D()->Play(L"IDLE_Both_HandBase", true);
		Transform()->SetRelativeScale(120.f, 60.f, 1.f); // 양손무기는 120, 60이다.

		// 개별 손 애니메이션 끄기
		GetRightHandScript()->StopAnim();
		GetLeftHandScript()->StopAnim();
	
		// 양손 IDLE 애니메이션 출력
		GetBothHandScript()->IDLEAnimPlayer(m_eRightWaponType);
	}
	else
	{
		Animator2D()->Play(L"IDLE_SIG_HandBase", true);
		Transform()->SetRelativeScale(80.f, 60.f, 1.f); // 한손무기는 80, 60이다.

		// 양손 애니메이션 끄기
		GetBothHandScript()->StopAnim();

		// 우측 무기별 애니메이션 실행
		GetRightHandScript()->IDLEAnimPlayer(m_eRightWaponType);

		// 좌측 무기별 애니메이션 실행
		GetLeftHandScript()->IDLEAnimPlayer(m_eLeftWaponType);
	}

	PlayerRotLeft();
}

void CPlayerScript::AttackAnimPlay(CARD_DIR_TYPE _CardDir)
{
	// 무기 종류에 알맞게 애니메이션을 실행해 주어야 한다.
	// 또, 공격 선택한 것이 왼쪽 무기인지, 오른쪽 무기인지에 따라 다르게 출력되어야 한다. 
	
	// 만약 오른쪽 무기가 카타나라면, 양손이라는 의미이다.
	if (m_eRightWaponType == WAPON_TYPE::KATANA)
	{
		Animator2D()->Play(L"Atteck_Both_HandBase", false);
		Transform()->SetRelativeScale(120.f, 60.f, 1.f); // 양손무기는 120, 60이다.

		// 개별 손 애니메이션 끄기
		GetRightHandScript()->StopAnim();
		GetLeftHandScript()->StopAnim();

		// 카타나 애니메이션 출력
		GetBothHandScript()->AttackAnimPlay(m_eRightWaponType, _CardDir);
	}
	else
	{
		Animator2D()->Play(L"Atteck_SIG_HandBase", false);
		Transform()->SetRelativeScale(80.f, 60.f, 1.f); // 한손무기는 80, 60이다.

		// 양손 애니메이션 끄기
		GetBothHandScript()->StopAnim();

		// 우측 무기별 애니메이션 실행
		GetRightHandScript()->RAttackAnimPlay(m_eRightWaponType, _CardDir);
		
		// 좌측 무기별 애니메이션 실행
		GetLeftHandScript()->LAttackAnimPlay(m_eLeftWaponType, _CardDir);
	}
	
}

void CPlayerScript::Attack(CTileScript* _pTargetTile, CARD_DIR_TYPE _CardDir)
{
	CalculateTarget(_pTargetTile);

	AttackAnimPlay(_CardDir);
	m_eDir = _CardDir; // 공격 이펙트 출력을 할때 사용하기 위해 받아둔다.
	m_bAtteck = true;

	// 이펙트 출력을 한번 진행하기 위해
	m_OnceEffect = true;
}

void CPlayerScript::Move(CTileScript* _pTargetTile)
{
	CalculateTarget(_pTargetTile);

	// 두 점 사이의 거리 구하기
	m_fTargetDestance = sqrt((m_vDifPos.x* m_vDifPos.x) + (m_vDifPos.y* m_vDifPos.y));

	m_bMove = true;
}

void CPlayerScript::Hit(int _iAtk)
{
	// 피격 모션을 실행하기 위해 true를 넣어준다.
	m_bHit = true;

	int iATT = _iAtk;
	// Player의 체력을 계산한다.
	// 나중에는 방어가 있는지를 먼저 확인해서 방어가 있다면, 방어가 먼저 까여야 한다.

	if (m_tPlayerInfo.Deffence != 0)
	{
		// 만약 남아있는 쉴드량 보다 공격력이 크다면,
		if (m_tPlayerInfo.Deffence < _iAtk)
		{
			iATT -= m_tPlayerInfo.Deffence;
			m_tPlayerInfo.Deffence = 0;

			// HP 깎기
			if (m_tPlayerInfo.Cur_HP != 0)
			{
				if (m_tPlayerInfo.Cur_HP > iATT)
				{
					m_tPlayerInfo.Cur_HP -= iATT;

					// 효과음 재생
					CreateHitSound();

					// UnderAttack 효과 요청
					AddEventUnderAttack();
				}
				// 플레이어의 현재 HP가 받은 피해량 보다 작다면 0을 준다.
				else
				{
					m_tPlayerInfo.Cur_HP = 0;

					// 효과음 재생
					CreateHitSound();

					// UnderAttack 효과 요청
					AddEventUnderAttack();
				}
			}
		}
		else
		{
			m_tPlayerInfo.Deffence -= iATT;
			iATT = 0;

			// 효과음 재생
			CreateHitShieldSound();
		}
	}
	else
	{
		if (m_tPlayerInfo.Cur_HP != 0)
		{
			if (m_tPlayerInfo.Cur_HP > iATT)
			{
				m_tPlayerInfo.Cur_HP -= iATT;

				// 효과음 재생
				CreateHitSound();

				// UnderAttack 효과 요청
				AddEventUnderAttack();
			}
			// 플레이어의 현재 HP가 받은 피해량 보다 작다면 0을 준다.
			else
			{
				m_tPlayerInfo.Cur_HP = 0;

				// 효과음 재생
				CreateHitSound();

				// UnderAttack 효과 요청
				AddEventUnderAttack();
			}
		}
	}

	// 피격 이펙트를 출력한다.
	if (!m_bArrowHit)
	{
		CreateHitEff();
	}

	// 피 이펙트를 출력한다.
	CreateBloodEff();

	// DT 슬로우 요청
	tEvent tevt = {};
	tevt.eType = EVENT_TYPE::SLOWTIME;
	CEventMgr::GetInst()->AddEvent(tevt);

	m_bSlowROnce = true;
}

void CPlayerScript::Attack_tick()
{
	// 마우스 위치에 따른 플레이어 회전을 막기 위한 true값 넣기
	m_bLockRot = true;

	// 누적시간 계산
	m_fAccTime += DT;

	Vec2 DTVec2 = CalculateDTPos();

	// 이동한 누적 거리
	m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));

	// 0.45초까지는 공격 반대 방향으로 진행
	if (m_fAccTime < 0.45f)
	{
		// 목표 거리 설정
		m_fTargetDestance = 20.f;

		// 누적거리가 아직 도달하지 않았으면,
		if (abs(m_fTargetDestance) > abs(m_fAccDestance))
		{
			// 반대 방향으로 목표 거리 까지 이동
			MoveOppoDir(DTVec2.x, DTVec2.y);
		}
		// 목표 이동거리를 넘어 섰을 경우
		else
		{
			// 최대 이동 거리 제한
			PosOnTile();
			MoveOppoDir(20.f * cos(m_fSeta), 20.f * sin(m_fSeta));
		}
	}
	// 0.55초까지는 대기
	else if (m_fAccTime >= 0.45f && m_fAccTime < 0.55f)
	{
		// 목표 이동 거리 재지정 (20 + 40)
		m_fTargetDestance = 60.f;

		// 누적 이동거리 초기화
		m_fAccDestance = 0.f;
	}
	// 대기 시간이 지나면, 원래 방향으로 공격 진행
	else
	{
		if (m_fTargetDestance == 60.f)
		{
			// 공격 이펙트 생성 : 한번만 생성되어야 한다.
			if (m_OnceEffect)
			{
				CreateAttackEff();
				m_OnceEffect = false;
			}

			if (abs(m_fTargetDestance) > abs(m_fAccDestance))
			{
				// 원래 진행 방향으로 이동
				MoveOriginDir(DTVec2.x, DTVec2.y);
			}
			// 만약 공격 목표 거리까지 이동을 했다면,
			else
			{
				// Player가 목표를 벗어나지 않도록 이동 제한
				// 원래 포지션을 가져와서
				PosOnTile();
				MoveOriginDir(40.f * cos(m_fSeta), 40.f * sin(m_fSeta));

				// 목표 이동 거리 재지정 (40)
				m_fTargetDestance = 40.f;

				// 누적 이동거리 초기화
				m_fAccDestance = 0.f;

				// 몬스터의 피격 계산 수행
				m_pGameMgrScript->TargetMonHit();
			}
		}
		else if (m_fTargetDestance == 40.f)
		{
			if (abs(m_fTargetDestance) > abs(m_fAccDestance))
			{
				MoveOppoDir(DTVec2.x, DTVec2.y);
			}

			// 만약 Player의 원래 포지션과 동일해 지면 멈춘다.
			else
			{
				PosOnTile();
				m_bLockRot = true;
				m_bAtteck = false;
				MoveInit();
			}
		}
	}
}

void CPlayerScript::Move_tick()
{
	// 이번 tick동안 목표까지 이동해야할 x,y성분 계산
	Vec2 DTVec2 = CalculateDTPos();

	// 이동한 누적 거리
	m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));


	// 목표 이동 거리와 DT동안 이동거리 누적값을 확인해서 어떻게 Transform을 옮길지 정한다.
	// 만약 이번에 이동하고나서의 거리 누적값이 목표보다 크다면,
	if (abs(m_fTargetDestance) < abs(m_fAccDestance))
	{
		// 목표지점으로 Player의 위치를 옮기고, 움직임을 끝낸다.
		// PlayerChangeTile()을 타고 들어가면 변경된 타일위로 포지션을 잡아주도록 되어있다.
		m_pGameMgrScript->PlayerChangeTile(m_pMoveTarget);
		MoveInit();
		m_bMove = false;
	}
	// 아니라면, DT만큼 이동해야할 포지션으로 이동한다.
	else
	{
		MoveOriginDir(DTVec2.x, DTVec2.y);
	}
}

void CPlayerScript::Hit_tick()
{
	if (m_bOneMoretick)
	{
		m_bHit = false;
		m_bOneMoretick = false;
		return;
	}

	// 마우스 위치에 따른 플레이어 회전을 막기 위한 true값 넣기
	m_bLockRot = true;

	// 누적시간 계산
	m_fAccTime += DT;

	// 0.1초가 지나면,DTSlow 효과를 해제해 준다.
	if (m_bSlowROnce && m_fAccTime > 0.1f)
	{
		CTimeMgr::GetInst()->ReleaseSlow();
		m_bSlowROnce = false;
	}

	// 몬스터의 공격 진행 애니메이션이 0.55초이다.
	if (m_fAccTime < 0.55f)
	{
		if (!m_bStepOnce)
		{
			m_bStepOnce = true;
			// 첫 번째 모션일 때,
			if (m_iStep == 0)
			{
				// 90도 방향으로 5만큼 이동
				m_fSeta = 1.571;
				m_fTargetDestance = 5;
				m_fAccDestance = 0;
			}
			// 두 번째 모션
			else if (m_iStep == 1)
			{
				// -15도 방향으로 15 이동
				m_fTargetDestance = 15;
				m_fSeta = (-0.2618);
				m_fAccDestance = 0;
			}
			// 세 번째 모션
			else if (m_iStep == 2)
			{
				// -165도 방향으로 30 이동
				m_fTargetDestance = 30;
				m_fSeta = (-2.88);
				m_fAccDestance = 0;
			}
			// 네 번째 모션
			else if (m_iStep == 3)
			{
				// -15도 방향으로 15 이동
				m_fTargetDestance = 15;
				m_fSeta = (-0.2618);
				m_fAccDestance = 0;
			}
			// 다섯 번째 모션
			else if (m_iStep == 4)
			{
				// 105도 방향으로 15 이동
				m_fTargetDestance = 15;
				m_fSeta = 1.833;
				m_fAccDestance = 0;
			}
			// 여섯 번째 모션
			else if (m_iStep == 5)
			{
				// 0도 방향으로 8 이동
				m_fTargetDestance = 8;
				m_fSeta = 0;
				m_fAccDestance = 0;
			}
			// 일곱 번째 모션
			else if (m_iStep == 6)
			{
				// 180도 방향으로 8 이동
				m_fTargetDestance = 8;
				m_fSeta = 3.142;
				m_fAccDestance = 0;
			}
			// 여덟 번째 모션
			else if (m_iStep == 7)
			{
				// 0도 방향으로 8 이동
				m_fTargetDestance = 8;
				m_fSeta = 0;
				m_fAccDestance = 0;
			}
			// 아홉 번째 모션
			else if (m_iStep == 8)
			{
				// -45도 방향으로 10 이동
				m_fTargetDestance = 10;
				m_fSeta = (-0.7854);
				m_fAccDestance = 0;
			}
		}


		// 이번 tick동안 목표까지 이동해야할 x,y성분 계산
		Vec2 DTVec2 = CalculateDTPos();

		// 이동한 누적 거리
		m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));
		
		// 실제 이동 부분
		if (m_fTargetDestance > m_fAccDestance)
		{
			MoveOriginDir(DTVec2.x, DTVec2.y);
		}
		else
		{	
			// 목표 지점을 넘지 못하게 제한 후 다음 모션으로 변경
			PosOnTile();
			m_bLockRot = true;
			if (m_iStep == 0)
			{
				MoveOriginDir(5 * cos(m_fSeta), 5 * sin(m_fSeta));
				m_iStep = 1; // 다음 모션으로 변경
			}
			else if (m_iStep == 1)
			{
				MoveOriginDir(15 * cos(m_fSeta), 15 * sin(m_fSeta));
				m_iStep = 2; // 다음 모션으로 변경
			}
			else if (m_iStep == 2)
			{
				MoveOriginDir(30 * cos(m_fSeta), 30 * sin(m_fSeta));
				m_iStep = 3; // 다음 모션으로 변경
			}
			else if (m_iStep == 3)
			{
				MoveOriginDir(15 * cos(m_fSeta), 15 * sin(m_fSeta));
				m_iStep = 4; // 다음 모션으로 변경
			}
			else if (m_iStep == 4)
			{
				MoveOriginDir(15 * cos(m_fSeta), 15 * sin(m_fSeta));
				m_iStep = 5; // 다음 모션으로 변경
			}
			else if (m_iStep == 5)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				m_iStep = 6; // 다음 모션으로 변경
			}
			else if (m_iStep == 6)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				m_iStep = 7; // 다음 모션으로 변경
			}
			else if (m_iStep == 7)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				m_iStep = 8; // 다음 모션으로 변경
			}
			else if (m_iStep == 8)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				PosOnTile();
				m_bOneMoretick = true;
				MoveInit();
				m_iStep = 0;
				// CTimeMgr::GetInst()->ReleaseSlow();
			}

			m_bStepOnce = false;
		}
	}
}

void CPlayerScript::MoveInit()
{
	m_pMoveTarget = nullptr;
	m_vDifPos = Vec2(0.f,0.f);
	m_fTargetDestance = 0.f;
	m_fAccDestance = 0.f;
	m_fSeta = 0.f;

	m_fSpeed = 320.f;

	// 공격 모션 진행시 사용되는 누적 시간초기화
	m_fAccTime = 0.f;
}

void CPlayerScript::MoveOriginDir(float _x, float _y)
{
	// 플레이어의 포지션을 가져온다.
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	// 원래방향으로 이동하도록 지정
	vPlayerPos.x += _x;
	vPlayerPos.y += _y;

	// 포지션을 입력해준다.
	Transform()->SetRelativePos(vPlayerPos);
}

void CPlayerScript::MoveOppoDir(float _x, float _y)
{
	// 플레이어의 포지션을 가져온다.
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	// 반대방향으로 이동하도록 지정
	vPlayerPos.x -= _x;
	vPlayerPos.y -= _y;

	// 포지션을 입력해준다.
	Transform()->SetRelativePos(vPlayerPos);
}

Vec2 CPlayerScript::CalculateDTPos()
{
	// (속도 + 가속도 * DT) * DT = 이번 DT동안 이동 거리
	m_fSpeed += m_fAccel * DT;
	float DestanceDT = m_fSpeed * DT;

	// 이번 DT동안 이동할 X축 = Rdt * cos(Seta)
	float xDT = DestanceDT * cos(m_fSeta);

	// 이번 DT동안 이동할 Y축 = Rdt * sin(Seta)
	float yDT = DestanceDT * sin(m_fSeta);

	return Vec2(xDT, yDT);
}

void CPlayerScript::CalculateTarget(CTileScript* _pTargetTile)
{
	m_pMoveTarget = _pTargetTile;
	Vec3 TargetPos = _pTargetTile->Transform()->GetWorldPos();
	// 실제 발판위에 위치해야할 플레이어의 목표 좌표로 변경한다.
	TargetPos.x += m_vPlayerPos.x;
	TargetPos.y += m_vPlayerPos.y;

	Vec3 OriginPos = GetOwner()->Transform()->GetRelativePos();

	// 차이 구하기
	m_vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// 두 점 사이의 각도 구하기
	m_fSeta = atan2f(m_vDifPos.y, m_vDifPos.x);
}

// =========================
// =    이펙트 관련 함수    =
// =========================

void CPlayerScript::CreateAttackEff()
{
	// 우측 공격 카드 였다면, 우측 장비 아이템을 확인해 알맞은 공격 이펙트를 출력해준다.
	if (m_eDir == CARD_DIR_TYPE::RIGHT)
	{
		switch (m_eRightWaponType)
		{
		case WAPON_TYPE::KATANA:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}
			
		case WAPON_TYPE::SWORD:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}
			
		case WAPON_TYPE::SHIELD:
		{
			CreatePunchEff();

			// 펀치 효과음 재생
			CreatePunchSound();
			break;
		}
			
		}
	}
	else
	{
		switch (m_eLeftWaponType)
		{
		case WAPON_TYPE::KATANA:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}

		case WAPON_TYPE::SWORD:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			CreatePunchEff();
			
			CreatePunchSound();
			break;
		}

		}
	}
}

void CPlayerScript::CreatePunchSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Punch\\Injured_Punch_4(bullet_impact_body_thump_03).wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Punch\\Injured_Punch_1(punch_grit_wet_impact_05).wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CPlayerScript::CreateSlashSound()
{
	srand(time(nullptr));
	int iRand = rand() % 3;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_0(SH101_Sword Attack_Hollywood_Fienup_004).wav");
		pSound->Play(1, 1.0f, true);
	}
	else if(iRand == 1)
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

void CPlayerScript::CreateHitSound()
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

void CPlayerScript::CreateHitShieldSound()
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

void CPlayerScript::CreateAddShieldSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Add\\collect_item_11.wav");
	pSound->Play(1, 1.0f, true);
}

void CPlayerScript::AddEventUnderAttack()
{
	CGameObject* pMainCam = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MainCamera");
	CUnderAttackScript* pEffScript = (CUnderAttackScript*)pMainCam->GetChildObject(L"Eff_UnderAttack")->GetScript((UINT)UNDERATTACKSCRIPT);
	pEffScript->RenderOn();
}

void CPlayerScript::MoveSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_0(SH101_Sword Attack_Hollywood_Fienup_004).wav");
	pSound->Play(1, 1.0f, true);
}

void CPlayerScript::CreateHitEff()
{
	Ptr<CPrefab> pHitPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\HitEffect_Prefab_0.pref");

	CGameObject* pHitEffect = pHitPrefab->Instantiate();

	// 생성 후 begin 호출
	pHitEffect->begin();

	// 랜더를 위한 z 값 지정
	pHitEffect->Transform()->SetRelativePos_Z(-3);

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

void CPlayerScript::CreateArrowHitEff(Vec3 TargetPos)
{
	m_bArrowHit = false;

	Ptr<CPrefab> pHitPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_ArrowHit_Prefab_0.pref");

	CGameObject* pHitEffect = pHitPrefab->Instantiate();

	Vec3 OriginPos = Transform()->GetRelativePos();

	// 차이 구하기
	Vec2 vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// 두 지점 사이의 각도 구하기
	float fSeta = atan2f(vDifPos.y, vDifPos.x);

	// 각도만큼 이펙트 회전
	pHitEffect->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, fSeta));

	// 생성 지점 지정
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	vPlayerPos.x += 35.f * cos(fSeta);
	vPlayerPos.y += 35.f * sin(fSeta);

	pHitEffect->Transform()->SetRelativePos(vPlayerPos);
	pHitEffect->Transform()->SetRelativePos_Z(0.f);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pHitEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);
	// 이펙트 애니메이션 실행
	pHitEffect->Animator2D()->Play(L"Eff_ArrowHit", false);
}

void CPlayerScript::CreateBloodEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Blood_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// 자식으로 설정한다.
	GetOwner()->AddChild(pEffect);

	// 레이어 설정
	pEffect->SetAllLayer(11);

	// 피 효과 애니메이션 랜덤 진행
	CBloodEffectScript* pBloodScript = (CBloodEffectScript*)pEffect->GetScript((UINT)BLOODEFFECTSCRIPT);
	pBloodScript->PlayRandAnim();
}

void CPlayerScript::CreateSlashEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Slash_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// 이펙트 회전
	Vec3 vRot = pEffect->Transform()->GetRelativeRotation();

	// y축을 회전해 뒤집어 준다.
	if (1.5707963268f > m_fSeta && m_fSeta > -1.5707963268f)
	{
		vRot.y = -3.1415926536f;
		vRot.z += m_fSeta;
	}

	// 정방향
	else
	{
		vRot.y = 0.f;
		vRot.z += m_fSeta;
	}

	pEffect->Transform()->SetRelativeRotation(vRot);

	// 이펙트 포지션 설정
	Vec3 vPos = m_pOwnerTile->Transform()->GetWorldPos();

	// 목표 이동량 = 85
	// 목표 지점까지의 세타를 구해서 적용해야 한다.
	if (m_fSeta == 0.f || m_fSeta == 3.14159274f)
	{
		vPos.x += cos(m_fSeta) * 35;
		vPos.y += sin(m_fSeta) * 35;
	}

	vPos.x += cos(m_fSeta) * 55;
	vPos.y += sin(m_fSeta) * 55;
	vPos.z = 0.f;

	pEffect->Transform()->SetRelativePos(vPos);
	

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// 이펙트 애니메이션 실행
	pEffect->Animator2D()->Play(L"Eff_Slash", false);
}

void CPlayerScript::CreatePunchEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Punch_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// 이펙트 회전
	Vec3 vRot = pEffect->Transform()->GetRelativeRotation();

	// y축을 회전해 뒤집어 준다.
	if (1.5707963268f > m_fSeta && m_fSeta > -1.5707963268f)
	{
		vRot.y = -3.1415926536f;
		vRot.z += m_fSeta;
	}

	// 정방향
	else
	{
		vRot.y = 0.f;
		vRot.z += m_fSeta;
	}

	pEffect->Transform()->SetRelativeRotation(vRot);

	// 이펙트 포지션 설정
	Vec3 vPos = m_pOwnerTile->Transform()->GetWorldPos();

	// 목표 이동량 = 85
	// 목표 지점까지의 세타를 구해서 적용해야 한다.
	if (m_fSeta == 0.f || m_fSeta == 3.14159274f)
	{
		vPos.x += cos(m_fSeta) * 35;
		vPos.y += sin(m_fSeta) * 35;
	}

	vPos.x += cos(m_fSeta) * 55;
	vPos.y += sin(m_fSeta) * 55;
	vPos.z = 0.f;

	pEffect->Transform()->SetRelativePos(vPos);


	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// 이펙트 애니메이션 실행
	pEffect->Animator2D()->Play(L"Eff_Punch", false);
}

void CPlayerScript::CreateShieldParticle()
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

// =========================
// =    무기들 관련 함수    =
// =========================

void CPlayerScript::CreateWaponCard()
{
	Ptr<CPrefab> pWaponPrefab = nullptr;

	// 카타나의 경우 오른손만 이용해 만들면 된다.
	if (m_tRightWapon.eWaponType == WAPON_TYPE::KATANA)
	{
		pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_Katana_Prefab_0.pref");

		CGameObject* pRWaopon = pWaponPrefab->Instantiate();

		// 랜더를 위한 z 값 지정
		pRWaopon->Transform()->SetRelativePos_Z(-1);

		// UIMgr의 슬롯의 자식으로 설정한다.
		m_pGameMgrScript->GetUIMgrScript()->GetOwner()->GetChildObject(L"RightWaponSlot")->AddChild(pRWaopon);

		// 레이어 설정
		pRWaopon->SetAllLayer(10);

		// 해당 무기의 스크립트 주소를 가져와 저장해둔다.
		if (nullptr != pRWaopon)
		{
			CWaponScript* pTargetScript = GetWaponScript(pRWaopon);

			if (nullptr != pTargetScript)
			{
				m_pRightWapon = pTargetScript;
			}
		}
		
		// 오른손 장착 무기 타입에 해당 무기의 타입을 저장한다.
		m_eRightWaponType = m_tRightWapon.eWaponType;
		m_eLeftWaponType = m_tRightWapon.eWaponType;
	}
	else
	{
		// 우측 무기 먼저 진행
		switch (m_tRightWapon.eWaponType)
		{
		case WAPON_TYPE::SWORD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_SwordBasic_Prefab_0.pref");
			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_ShieldBasic_Prefab_0.pref");
			break;
		}
		}

		// 얻어온 무기 객체 생성
		CGameObject* pRWaopon = pWaponPrefab->Instantiate();

		// 랜더를 위한 z 값 지정
		pRWaopon->Transform()->SetRelativePos_Z(-1);

		// UIMgr의 슬롯의 자식으로 설정한다.
		m_pGameMgrScript->GetUIMgrScript()->GetOwner()->GetChildObject(L"RightWaponSlot")->AddChild(pRWaopon);

		// 레이어 설정
		pRWaopon->SetAllLayer(10);

		// 해당 무기의 스크립트 주소를 가져와 저장해둔다.
		if (nullptr != pRWaopon)
		{
			CWaponScript* pTargetScript = GetWaponScript(pRWaopon);

			if (nullptr != pTargetScript)
			{
				m_pRightWapon = pTargetScript;
			}
		}

		// 오른손 장착 무기 타입에 해당 무기의 타입을 저장한다.
		m_eRightWaponType = m_tRightWapon.eWaponType;


		// 왼손 진행
		switch (m_tLefetWapon.eWaponType)
		{
		case WAPON_TYPE::SWORD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_SwordBasic_Prefab_0.pref");
			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_ShieldBasic_Prefab_0.pref");
			break;
		}
		}

		// 얻어온 무기 객체 생성
		CGameObject* pLWaopon = pWaponPrefab->Instantiate();

		// 랜더를 위한 z 값 지정
		pLWaopon->Transform()->SetRelativePos_Z(-1);

		// UIMgr의 슬롯의 자식으로 설정한다.
		m_pGameMgrScript->GetUIMgrScript()->GetOwner()->GetChildObject(L"LeftWaponSlot")->AddChild(pLWaopon);

		// 레이어 설정
		pLWaopon->SetAllLayer(10);

		// 해당 무기의 스크립트 주소를 가져와 저장해둔다.
		if (nullptr != pLWaopon)
		{
			CWaponScript* pTargetScript = GetWaponScript(pLWaopon);

			if (nullptr != pTargetScript)
			{
				m_pLeftWapon = pTargetScript;
			}
		}

		// 오른손 장착 무기 타입에 해당 무기의 타입을 저장한다.
		m_eLeftWaponType = m_tLefetWapon.eWaponType;
	}


}


// 회복 아이템 사용시 호출
void CPlayerScript::RecoveryCurHP(int _Recovery)
{
	m_tPlayerInfo.Cur_HP += _Recovery;

	// 최대 체력을 넘지 못하도록 방지
	if (m_tPlayerInfo.Cur_HP >= m_tPlayerInfo.Max_HP)
	{
		m_tPlayerInfo.Cur_HP = m_tPlayerInfo.Max_HP;
	}
}

// =========================
// =    카드덱 관련 함수    =
// =========================

void CPlayerScript::DefaultCardSetting()
{
	// 기본 공격 카드 3장 주기 (손 방향 별로)
	tPlayerCardInfo CardInfo = {};
	CardInfo.CardName = CARD_TYPE::ATTACK;
	CardInfo.Reinforce = false;
	CardInfo.CardDir = CARD_DIR_TYPE::RIGHT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}

	CardInfo.CardDir = CARD_DIR_TYPE::LEFT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}

	// 기본 방어 카드 3장 주기 (손 방향 별로)
	CardInfo.CardName = CARD_TYPE::DEFFENCE;
	CardInfo.Reinforce = false;
	CardInfo.CardDir = CARD_DIR_TYPE::RIGHT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}

	CardInfo.CardDir = CARD_DIR_TYPE::LEFT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}
}

// =========================
// =    디버프 관련 함수    =
// =========================

void CPlayerScript::SetDebuf(DEBUF_TYPE _eDeBufType)
{
	switch (_eDeBufType)
	{
	case DEBUF_TYPE::RESTRAIN:
	{
		m_iPlayerDebuf |= RESTRAIN_DEBUF;
		break;
	}
	}
}

bool CPlayerScript::CheckDebuf(DEBUF_TYPE _eDeBufType)
{
	switch (_eDeBufType)
	{
	case DEBUF_TYPE::RESTRAIN:
	{
		if (m_iPlayerDebuf & RESTRAIN_DEBUF)
			return true;
		else
			return false;

		break;
	}
	}
}

void CPlayerScript::ReleaseDebuf(DEBUF_TYPE _eDeBufType)
{
	switch (_eDeBufType)
	{
	case DEBUF_TYPE::RESTRAIN:
	{
		m_iPlayerDebuf &= ~RESTRAIN_DEBUF;
		break;
	}
	}
}




void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{
	// 맞은게 화살일 경우 피격 진행
	if (nullptr != _pOther->GetOwner()->GetScript((UINT)MISSILESCRIPT))
	{
		m_bArrowHit = true;

		// 화살은 부수고, 본인이 피격되기
		CMissileScript* pArrowScript = (CMissileScript*)_pOther->GetOwner()->GetScript((UINT)MISSILESCRIPT);
		Hit(pArrowScript->GetATK());

		CreateArrowHitEff(_pOther->Transform()->GetRelativePos());

		_pOther->GetOwner()->Destroy();
	}
}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	// 부모쪽에서 저장할 데이터가 있을 수 있으니 호출해준다.
	CScript::SaveToFile(_File);

	fwrite(&m_fSpeed, sizeof(float), 1, _File);

	SaveResourceRef(m_Prefab, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);

	fread(&m_fSpeed, sizeof(float), 1, _File);
	LoadResourceRef(m_Prefab, _File);
}

