#pragma once
#include <Engine/CScript.h>

class CGameMgrScript;

class CMonsterScript :
	public CScript
{
private:
	CGameMgrScript* m_pGameMgrScript;	// GameMgrScript의 주소
	CGameObject* m_pOwnerTile;		// 몬스터가 서있는 타일의 주소
	OBJECT_TYPE		m_eObjType;			// 오브젝트 타입 = Monster이다.
	Vec2			m_vMonsterPos;		// Monster의 기본 포지션값.
	float			m_fAccTime;			// 누적 시간

	bool			m_bTempDead;		// 임시 죽음 상태를 표시해 준다. Object를 Dead로 하면, 실제 삭제하기 위한 Dead이고, 해당 Dead상태는 죽은 시체를 출력해주기 위함이다.
	bool			m_bStun;			// 스턴 상태인지를 나타낸다.

	int				m_iStunTurn;		// 몇번째 턴에 끝나는지 계산해둔다.

	float			m_bfAccMotionTime;
	bool			m_bMotionEnd;
	bool			m_bRotToPlayer;
	bool			m_bHit;

protected:
	tMonsterInfo	 m_tMonsterInfo;		// 몬스터의 정보
	MONSTER_PATTERN  m_eMonsterPattern;		// Monster가 수행해야할 패턴
	bool			 m_ActionOnce;			// 패턴은 한번만 실행되어야 한다.
	bool			 m_bTurnEnd;			// 턴을 마칠 수 있는 상태인지 확인하는 bool값
	tTargetTile_Info m_tAstarInfo;			// Astar 결과 값 저장하기

	bool			 m_bAnimStart;
	bool			 m_bMonMove;
	float			 m_fAccMoveTime;

	

public:
	// 몬스터의 스테이터스를 가져온다.
	int GetMonMaxHP() { return m_tMonsterInfo.Max_HP; }
	int GetMonCurHP() { return m_tMonsterInfo.Cur_HP; }
	int GetMonCurDef() { return m_tMonsterInfo.Deffence; }
	int GetMonAtt() { return m_tMonsterInfo.Attack; }

	bool GetMonRot() { return m_bRotToPlayer; }

public:
	// 사운드 관련 함수
	void CreateHitSound();
	void CreateHitShieldSound();
	void CreateSlashSound();

	// 이펙트 관련 함수
	void CreateHitEff();

	void CreatePunchHitEff();

	void CreateBloodEff();

	void CreateShieldParticle();

public:
	CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }
	CGameObject* GetOwnerTile() { return m_pOwnerTile; }
	CTileScript* GetOwnerTileScript() { return (CTileScript*)(GetOwnerTile()->GetScript((UINT)TILESCRIPT)); }

public:
	// 몬스터가 이번 차례 때 수행해야할 패턴을 정해준다.
	virtual void MosnsterAIPlay() {};

	// 몬스터가 이번 차례 때 수행해야할 패턴을 실행해준다.
	void MonsterAction();

	// 몬스터가 이번 차례 때 수행해야할 일들을 모두 마쳤는지 검사한다.
	bool MonsterTurnCheck() { return m_bTurnEnd; }

	// 몬스터의 패턴을 반환한다.
	MONSTER_PATTERN GetMonsterPattern() { return m_eMonsterPattern; }

public:
	// 임시 죽음 상태 확인 요청
	bool GetTempDead() { return m_bTempDead; }

	// 죽은 것으로 바꾸어준다. : 단순히 bool 값만 바뀌는게 아니라 OwnerTile과의 관계도 끊어줄 것이다.
	// ㄴ OwnerTile을 nullptr로 변경, OwnerTile에 접근해 ReleaseTargetObj 호출을 해준다.
	void SetTempDead();

	// 몬스터가 스턴 상태인지를 반환 받는다.
	bool IS_MonStun() { return m_bStun; }

	// IDLE 애니메이션으로 전환해 준다.
	virtual void PlayIDLEAnim() {};
	// 방어 애니메이션으로 전환해 준다.
	virtual void PlayDefAnim() {}
	// 공격 애니메이션으로 전환해 준다.
	virtual void PlayAttackAnim() {};
	// 죽은 상태 애니메이션으로 전환해 준다.
	virtual void PlayDeadAnim() {};
	// 스턴 상태 애니메이션으로 전환해 준다.
	virtual void PlayStunAnim() {};

	// 몬스터가 바라보는 방향을 정한다.
	void SetMonsterDir();

public:
	virtual void Hit(int ATK, WAPON_TYPE _eWaponType);

	virtual void Attack();

	virtual void Move() {};

	virtual void Restrain() {};

	virtual void Run() {};

	virtual void Sheild() {};

	virtual void Charge() {};

	virtual void Rush() {};

	virtual void Reload() {};

	virtual void Boss_Charge() {};

	virtual void Boss_Rush() {};

public:
	// 몬스터가 위치한 타일 주소 설정 함수 : 설정을 하면 해당 타일에도 몬스터를 등록해준다.
	void SetOwnerTile(CGameObject* _Tile);

	// 몬스터가 위치한 타일 주소 반환 함수
	CGameObject* GetTile() { return m_pOwnerTile; }

public:
	// 몬스터 포지션 이니셜 : 몬스터 발이 기준이 되도록 포지션을 초기화 해준다.
	void PosInit();

	// 몬스터가 서있는 타일로 포지션을 다시 잡아준다.
	void PosOnTile();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _pOther) override;
	virtual void Overlap(CCollider2D* _pOther) override;
	virtual void EndOverlap(CCollider2D* _pOther) override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

    CLONE(CMonsterScript);
public:
    CMonsterScript(int _ScriptType);
    ~CMonsterScript();
};

