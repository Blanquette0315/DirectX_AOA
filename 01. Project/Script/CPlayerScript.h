#pragma once
#include <Engine/CScript.h>


class CGameMgrScript;
class CWaponScript;
class CPlayerRightHandScript;
class CPlayerLeftHandScript;
class CPlayerHandScript;

class CPlayerScript :
    public CScript
{
private:
	CGameObject*			m_pOwnerTile;	// �÷��̾ ���ִ� Ÿ���� �ּ�
	CGameMgrScript*			m_pGameMgrScript;	// GameMgrScript�� �ּ�
	Vec2					m_vPlayerPos;	// �÷��̾� �⺻ ������ ��.
	tPlayerInfo				m_tPlayerInfo;	// �÷��̾� ���� (����)

	OBJECT_TYPE				m_eObjType;		// Palyer�� ������Ʈ Ÿ���� Player�̴�.

	float					m_fAccTime;		// ���� �ð�
	float					m_fAccel;		// �÷��̾��� ���ӵ�
	float					m_fSpeed;		// �÷��̾��� �ӵ�

	bool					m_bLockRot;		// Player�� ȸ���� �����ش�.
	bool					m_bRot;			// Player�� ȸ���� �ߴ��� ���ߴ��� �� �� �ִ� ����
	bool					m_bAtteck;		// �÷��̾ ���������� Ȯ���ϴ� bool ����
	bool					m_bMove;		// Player�� �̵� ����� �����ؾ��ϴ��� Ȯ��
	bool					m_bHit;			// Player�� �ǰݸ���� �����ؾ� �ϴ��� Ȯ��

	// �÷��̾ ���� ���� ���� ����
	tPlayer_Wapon			m_tRightWapon;		// ������ ���� ����
	tPlayer_Wapon			m_tLefetWapon;		// �޼� ���� ����

	CWaponScript*			m_pRightWapon;		// ������ ������ ��ũ��Ʈ �ּ�
	CWaponScript*			m_pLeftWapon;		// �޼� ������ ��ũ��Ʈ �ּ�
	WAPON_TYPE				m_eRightWaponType;	// ������ ������ Ÿ��
	WAPON_TYPE				m_eLeftWaponType;	// �޼� ������ Ÿ��

	// �÷��̾ ���� �ι��丮 ������ ���� ����
	tItem_Info				m_arrInven[3];		// �÷��̾��� �ι��丮 �켱 3��¥���� ����Ѵ�.

	// ī�� ���� ����
	vector<tPlayerCardInfo>	m_arrCardList;		// Player�� ���� ī����� ���� ����

	// �̵� ���� ���
	CTileScript*			m_pMoveTarget;		// Player�� �̵��� Ÿ�� Ÿ���� �ּ�
	Vec2					m_vDifPos;			// �� ������ ����
	float					m_fTargetDestance;	// ��ǥ �̵� �Ÿ�
	float					m_fAccDestance;		// ���� �̵� �Ÿ�
	float					m_fSeta;			// ���հ�

	int						m_iStep;			// ��� ���� ����
	bool					m_bStepOnce;

	bool					m_bSelectedCard;
	bool					m_bOneMoretick;

	// ����Ʈ ���� ����
	CARD_DIR_TYPE			m_eDir;
	bool					m_OnceEffect;
	bool					m_bSlowROnce;
	bool					m_bArrowHit;

	// Player�� ����� �� ����
	int						m_iPlayerDebuf;		// Player�� �����
												// ���� ��� �ӹ� > ȭ��

	Ptr<CPrefab> m_Prefab;

public:
	// ���Ʈ ���� �Լ�
	void CreatePunchSound();
	void CreateSlashSound();
	void CreateHitSound();
	void CreateHitShieldSound();

	void CreateAddShieldSound();
	
	void AddEventUnderAttack();

	void MoveSound();

public:
	// ����Ʈ ���� �Լ�
	void CreateHitEff();
	void CreateArrowHitEff(Vec3 TargetPos);

	void CreateBloodEff();

	void CreateSlashEff();
	void CreatePunchEff();

	void CreateShieldParticle();

	// ���⿡ �˸´� ���� ����Ʈ ������ִ� �Լ�
	void CreateAttackEff();

	void SetSelectedCard(bool _IsSelectedCard) { m_bSelectedCard = _IsSelectedCard; }

public:
	// ������ ���� �Լ� : ���� �ε����� ���ڷ� �޾Ƽ� �ش� �ε����� ��Ҹ� ��ȯ�Ѵ�.
	tItem_Info GetItemInfo(int _Idx) { return m_arrInven[_Idx]; }

	// ������ ������ �־��ִ� �Լ�
	void SetItemInfo(int _Idx, tItem_Info _info) { m_arrInven[_Idx] = _info; }

public:
	// ���� ���� �Լ�
	// �÷��̾ ���� ���� �����͸� ���� ���� ī�带 ������ �ش�.
	void CreateWaponCard();

public:
	// �� ���� �Լ�
	// �⺻ ī�� �ֱ�
	void DefaultCardSetting();

	// �� ����Ʈ ����
	const vector<tPlayerCardInfo>& GetCardList() { return m_arrCardList; }

public:
	int GetMAXHP() { return m_tPlayerInfo.Max_HP; }
	int GetCURHP() { return m_tPlayerInfo.Cur_HP; }
	int GetMaxAct() { return m_tPlayerInfo.Max_Acction; }
	int GetCurAct() { return m_tPlayerInfo.Cur_Acction; }
	int GetDrowCount() { return m_tPlayerInfo.DrowCount; }
	int GetCurDef() { return m_tPlayerInfo.Deffence; }

	// ȸ�� ������ ��뿡 ���� Player�� ü�� ȸ�� �Լ�
	void RecoveryCurHP(int _Recovery);

	// Player�� �ൿ�� ���� �Լ�
	void DefCurAct(int _Cost) { m_tPlayerInfo.Cur_Acction -= _Cost; }
	// Player�� �ൿ�� �ʱ�ȭ �Լ�
	void ResetCurAct() { m_tPlayerInfo.Cur_Acction = m_tPlayerInfo.Max_Acction; }

	//Player�� ���ݷ� ��������
	int GetPlayerRightAtt() { return m_tPlayerInfo.RightAttack; }
	int GetPlayerLeftAtt() { return m_tPlayerInfo.LeftAttack; }

	// Player�� ���ݷ� �����ϱ�
	void SetPlayerRightAtt(int _Att) { m_tPlayerInfo.RightAttack = _Att; }
	void SetPlayerLeftAtt(int _Att) { m_tPlayerInfo.LeftAttack = _Att; }
	

public:
	// ����� Ÿ�Կ� ���� �÷��̾��� ����� ���¸� üũ���ش�.
	void SetDebuf(DEBUF_TYPE _eDeBufType);

	// ����� ���¸� Ȯ���غ��� �Լ�
	bool CheckDebuf(DEBUF_TYPE _eDeBufType);

	// ������� ���� ���ִ� �Լ�
	void ReleaseDebuf(DEBUF_TYPE _eDeBufType);

public:
	CGameObject* GetTile() { return m_pOwnerTile; }

	// Player�� ������ ��ũ��Ʈ ��������
	CPlayerRightHandScript* GetRightHandScript() { return (CPlayerRightHandScript*)(GetOwner()->GetChildObject(L"PlayerRightHand")->GetScript((UINT)PLAYERRIGHTHANDSCRIPT)); }
	// Player�� �޼� ��ũ��Ʈ ��������
	CPlayerLeftHandScript* GetLeftHandScript() {return (CPlayerLeftHandScript*)(GetOwner()->GetChildObject(L"PlayerLeftHand")->GetScript((UINT)PLAYERLEFTHANDSCRIPT)); }
	// Player�� ��� ��ũ��Ʈ ��������
	CPlayerHandScript* GetBothHandScript() { return (CPlayerHandScript*)(GetOwner()->GetChildObject(L"PlayerBothHand")->GetScript((UINT)PLAYERHANDSCRIPT)); }

	// Player�� ������ ���� Ÿ�� ��������
	WAPON_TYPE GetRightWaponType() { return m_eRightWaponType; }
	// Player�� �޼� ���� Ÿ�� ��������
	WAPON_TYPE GetLeftWaponType() { return m_eLeftWaponType; }

	// Player�� ���� Ÿ�� ����
	void SetRightWaponType(WAPON_TYPE _eType) { m_eRightWaponType = _eType; }
	void SetLeftWaponType(WAPON_TYPE _eType) { m_eLeftWaponType = _eType; }

	void SetRightWaponInfo(tPlayer_Wapon _Info) { m_tRightWapon = _Info; }
	void SetLeftWaponInfo(tPlayer_Wapon _Info) { m_tLefetWapon = _Info; }

	tPlayer_Wapon GetRightWaponInfo() { return m_tRightWapon; }
	tPlayer_Wapon GetLeftWaponInfo() { return m_tLefetWapon; }

	CWaponScript* GetPlayerRightWapon() { return m_pRightWapon; }
	CWaponScript* GetPlayerLeftWapon() { return m_pLeftWapon; }

public:
	// �÷��̾ ��ġ�� Ÿ�� �ּ� ���� �Լ� : ������ �ϸ� �ش� Ÿ�Ͽ��� �÷��̾ ������ش�.
	void SetOwnerTile(CGameObject* _Tile);

	// �÷��̾��� ���� ���� �Լ�
	void SetDeffence(int _Def) { m_tPlayerInfo.Deffence = _Def; }
	void AddDeffence(int _Def) { m_tPlayerInfo.Deffence += _Def; }

public:
	// �÷��̾� ������ �̴ϼ� : �÷��̾� ���� ������ �ǵ��� �������� �ʱ�ȭ ���ش�.
	void PosInit();
	
	// �÷��̾ ���ִ� Ÿ�Ϸ� �������� �ٽ� ����ش�.
	void PosOnTile();

	// �÷��̾��� �������� �ӽ� Ÿ�Ϸ� �Űܼ� �����ش�.
	void PosOnTempTile(CTileScript* _TempTile);

	// Player�� ȸ���� ����, Ȥ�� �Ұ������� �������ִ� �Լ�
	void PlayerLockRot(bool _Lock) { m_bLockRot = _Lock; }

	// Player�� �ٶ󺸴� ������ ���������� ������ �ش�.
	void PlayerRotRight() { Transform()->SetRelativeRotation(0.f, 3.14159f, 0.f); m_bRot = true; }

	// Player�� �ٶ󺸴� ������ �������� ������ �ش�.
	void PlayerRotLeft() { Transform()->SetRelativeRotation(0.f, 0.f, 0.f); m_bRot = false; }

	// Player�� ȸ���� �ߴ��� �˾ƿ��� �Լ�
	bool Is_Rot() { return m_bRot; }

public:
	// Player�� IDLE���� �ִϸ��̼� ����
	void IDLEAnimPlay();

	// Player�� Attack ���� �ִϸ��̼� ����
	void AttackAnimPlay(CARD_DIR_TYPE _CardDir);

public:
	// Player ���� ���� �Լ�
	void Attack(CTileScript* _pTargetTile, CARD_DIR_TYPE _CardDir);

	// Player �̵� ���� �Լ�
	void Move(CTileScript* _pTargetTile);

	// Player �ǰ� ���� �Լ�
	void Hit(int _iAtk);

	// Player�� �ǰݸ���� �������� Ȯ���ϴ� �Լ�
	// ������ �ް� ����� ������ false�� ��ȯ�ȴ�.
	bool Is_Hit() { return m_bHit; };

public:
	// Player�� ���� ���� tick�Լ�
	void Attack_tick();

	// Player�� �̵� ���� tick �Լ�
	void Move_tick();

	// Player�� �ǰ� ���� tick �Լ�
	void Hit_tick();

	// Player �̵� ���� �ʱ�ȭ �Լ�
	void MoveInit();

	// ���� �������� �̵��� ���ִ� �Լ�
	void MoveOriginDir(float _x, float _y);

	// ����� �ݴ� �������� �̵����ִ� �Լ�
	void MoveOppoDir(float _x, float _y);

	// DT���� �̵��ؾ��� Vec2�� ������ִ� �Լ�
	Vec2 CalculateDTPos();

	// Player�� ��ǥ���� Ÿ�ϱ����� ����, ������ �˾Ƴ��� �Լ�
	void CalculateTarget(CTileScript* _pTargetTile);

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _pOther) override;
	virtual void Overlap(CCollider2D* _pOther) override;
	virtual void EndOverlap(CCollider2D* _pOther) override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CPlayerScript);
public:
	CPlayerScript();
	~CPlayerScript();
};