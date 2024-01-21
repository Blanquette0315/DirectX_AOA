#pragma once
#include <Engine/CScript.h>

class CGameMgrScript;

class CMonsterScript :
	public CScript
{
private:
	CGameMgrScript* m_pGameMgrScript;	// GameMgrScript�� �ּ�
	CGameObject* m_pOwnerTile;		// ���Ͱ� ���ִ� Ÿ���� �ּ�
	OBJECT_TYPE		m_eObjType;			// ������Ʈ Ÿ�� = Monster�̴�.
	Vec2			m_vMonsterPos;		// Monster�� �⺻ �����ǰ�.
	float			m_fAccTime;			// ���� �ð�

	bool			m_bTempDead;		// �ӽ� ���� ���¸� ǥ���� �ش�. Object�� Dead�� �ϸ�, ���� �����ϱ� ���� Dead�̰�, �ش� Dead���´� ���� ��ü�� ������ֱ� �����̴�.
	bool			m_bStun;			// ���� ���������� ��Ÿ����.

	int				m_iStunTurn;		// ���° �Ͽ� �������� ����صд�.

	float			m_bfAccMotionTime;
	bool			m_bMotionEnd;
	bool			m_bRotToPlayer;
	bool			m_bHit;

protected:
	tMonsterInfo	 m_tMonsterInfo;		// ������ ����
	MONSTER_PATTERN  m_eMonsterPattern;		// Monster�� �����ؾ��� ����
	bool			 m_ActionOnce;			// ������ �ѹ��� ����Ǿ�� �Ѵ�.
	bool			 m_bTurnEnd;			// ���� ��ĥ �� �ִ� �������� Ȯ���ϴ� bool��
	tTargetTile_Info m_tAstarInfo;			// Astar ��� �� �����ϱ�

	bool			 m_bAnimStart;
	bool			 m_bMonMove;
	float			 m_fAccMoveTime;

	

public:
	// ������ �������ͽ��� �����´�.
	int GetMonMaxHP() { return m_tMonsterInfo.Max_HP; }
	int GetMonCurHP() { return m_tMonsterInfo.Cur_HP; }
	int GetMonCurDef() { return m_tMonsterInfo.Deffence; }
	int GetMonAtt() { return m_tMonsterInfo.Attack; }

	bool GetMonRot() { return m_bRotToPlayer; }

public:
	// ���� ���� �Լ�
	void CreateHitSound();
	void CreateHitShieldSound();
	void CreateSlashSound();

	// ����Ʈ ���� �Լ�
	void CreateHitEff();

	void CreatePunchHitEff();

	void CreateBloodEff();

	void CreateShieldParticle();

public:
	CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }
	CGameObject* GetOwnerTile() { return m_pOwnerTile; }
	CTileScript* GetOwnerTileScript() { return (CTileScript*)(GetOwnerTile()->GetScript((UINT)TILESCRIPT)); }

public:
	// ���Ͱ� �̹� ���� �� �����ؾ��� ������ �����ش�.
	virtual void MosnsterAIPlay() {};

	// ���Ͱ� �̹� ���� �� �����ؾ��� ������ �������ش�.
	void MonsterAction();

	// ���Ͱ� �̹� ���� �� �����ؾ��� �ϵ��� ��� ���ƴ��� �˻��Ѵ�.
	bool MonsterTurnCheck() { return m_bTurnEnd; }

	// ������ ������ ��ȯ�Ѵ�.
	MONSTER_PATTERN GetMonsterPattern() { return m_eMonsterPattern; }

public:
	// �ӽ� ���� ���� Ȯ�� ��û
	bool GetTempDead() { return m_bTempDead; }

	// ���� ������ �ٲپ��ش�. : �ܼ��� bool ���� �ٲ�°� �ƴ϶� OwnerTile���� ���赵 ������ ���̴�.
	// �� OwnerTile�� nullptr�� ����, OwnerTile�� ������ ReleaseTargetObj ȣ���� ���ش�.
	void SetTempDead();

	// ���Ͱ� ���� ���������� ��ȯ �޴´�.
	bool IS_MonStun() { return m_bStun; }

	// IDLE �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayIDLEAnim() {};
	// ��� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayDefAnim() {}
	// ���� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayAttackAnim() {};
	// ���� ���� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayDeadAnim() {};
	// ���� ���� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayStunAnim() {};

	// ���Ͱ� �ٶ󺸴� ������ ���Ѵ�.
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
	// ���Ͱ� ��ġ�� Ÿ�� �ּ� ���� �Լ� : ������ �ϸ� �ش� Ÿ�Ͽ��� ���͸� ������ش�.
	void SetOwnerTile(CGameObject* _Tile);

	// ���Ͱ� ��ġ�� Ÿ�� �ּ� ��ȯ �Լ�
	CGameObject* GetTile() { return m_pOwnerTile; }

public:
	// ���� ������ �̴ϼ� : ���� ���� ������ �ǵ��� �������� �ʱ�ȭ ���ش�.
	void PosInit();

	// ���Ͱ� ���ִ� Ÿ�Ϸ� �������� �ٽ� ����ش�.
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

