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
	CGameObject*			m_pOwnerTile;	// 플레이어가 서있는 타일의 주소
	CGameMgrScript*			m_pGameMgrScript;	// GameMgrScript의 주소
	Vec2					m_vPlayerPos;	// 플레이어 기본 포지션 값.
	tPlayerInfo				m_tPlayerInfo;	// 플레이어 정보 (스탯)

	OBJECT_TYPE				m_eObjType;		// Palyer의 오브젝트 타입은 Player이다.

	float					m_fAccTime;		// 누적 시간
	float					m_fAccel;		// 플레이어의 가속도
	float					m_fSpeed;		// 플레이어의 속도

	bool					m_bLockRot;		// Player의 회전을 막아준다.
	bool					m_bRot;			// Player가 회전을 했는지 안했는지 알 수 있는 변수
	bool					m_bAtteck;		// 플레이어가 공격중인지 확인하는 bool 변수
	bool					m_bMove;		// Player의 이동 모션을 수행해야하는지 확인
	bool					m_bHit;			// Player의 피격모션을 수행해야 하는지 확인

	// 플레이어가 지닌 무기 관련 변수
	tPlayer_Wapon			m_tRightWapon;		// 오른손 무기 정보
	tPlayer_Wapon			m_tLefetWapon;		// 왼손 무기 정보

	CWaponScript*			m_pRightWapon;		// 오른손 무기의 스크립트 주소
	CWaponScript*			m_pLeftWapon;		// 왼손 무기의 스크립트 주소
	WAPON_TYPE				m_eRightWaponType;	// 오른손 무기의 타입
	WAPON_TYPE				m_eLeftWaponType;	// 왼손 무기의 타입

	// 플레이어가 지닌 인밴토리 아이템 관련 변수
	tItem_Info				m_arrInven[3];		// 플레이어의 인밴토리 우선 3개짜리로 사용한다.

	// 카드 관련 변수
	vector<tPlayerCardInfo>	m_arrCardList;		// Player가 지닌 카드들의 정보 집합

	// 이동 관련 멤버
	CTileScript*			m_pMoveTarget;		// Player가 이동할 타겟 타일의 주소
	Vec2					m_vDifPos;			// 두 지점간 차이
	float					m_fTargetDestance;	// 목표 이동 거리
	float					m_fAccDestance;		// 누적 이동 거리
	float					m_fSeta;			// 사잇각

	int						m_iStep;			// 모션 진행 스텝
	bool					m_bStepOnce;

	bool					m_bSelectedCard;
	bool					m_bOneMoretick;

	// 이펙트 관련 변수
	CARD_DIR_TYPE			m_eDir;
	bool					m_OnceEffect;
	bool					m_bSlowROnce;
	bool					m_bArrowHit;

	// Player의 디버프 및 버프
	int						m_iPlayerDebuf;		// Player의 디버프
												// 순서 대로 속박 > 화상

	Ptr<CPrefab> m_Prefab;

public:
	// 사운트 관련 함수
	void CreatePunchSound();
	void CreateSlashSound();
	void CreateHitSound();
	void CreateHitShieldSound();

	void CreateAddShieldSound();
	
	void AddEventUnderAttack();

	void MoveSound();

public:
	// 이펙트 관련 함수
	void CreateHitEff();
	void CreateArrowHitEff(Vec3 TargetPos);

	void CreateBloodEff();

	void CreateSlashEff();
	void CreatePunchEff();

	void CreateShieldParticle();

	// 무기에 알맞는 공격 이펙트 출력해주는 함수
	void CreateAttackEff();

	void SetSelectedCard(bool _IsSelectedCard) { m_bSelectedCard = _IsSelectedCard; }

public:
	// 아이템 관련 함수 : 가방 인덱스를 인자로 받아서 해당 인덱스의 요소를 반환한다.
	tItem_Info GetItemInfo(int _Idx) { return m_arrInven[_Idx]; }

	// 아이템 정보를 넣어주는 함수
	void SetItemInfo(int _Idx, tItem_Info _info) { m_arrInven[_Idx] = _info; }

public:
	// 무기 관련 함수
	// 플레이어가 지닌 무기 데이터를 토대로 무기 카드를 생성해 준다.
	void CreateWaponCard();

public:
	// 덱 관련 함수
	// 기본 카드 넣기
	void DefaultCardSetting();

	// 덱 리스트 전달
	const vector<tPlayerCardInfo>& GetCardList() { return m_arrCardList; }

public:
	int GetMAXHP() { return m_tPlayerInfo.Max_HP; }
	int GetCURHP() { return m_tPlayerInfo.Cur_HP; }
	int GetMaxAct() { return m_tPlayerInfo.Max_Acction; }
	int GetCurAct() { return m_tPlayerInfo.Cur_Acction; }
	int GetDrowCount() { return m_tPlayerInfo.DrowCount; }
	int GetCurDef() { return m_tPlayerInfo.Deffence; }

	// 회복 아이템 사용에 따른 Player의 체력 회복 함수
	void RecoveryCurHP(int _Recovery);

	// Player의 행동력 차감 함수
	void DefCurAct(int _Cost) { m_tPlayerInfo.Cur_Acction -= _Cost; }
	// Player의 행동력 초기화 함수
	void ResetCurAct() { m_tPlayerInfo.Cur_Acction = m_tPlayerInfo.Max_Acction; }

	//Player의 공격력 가져오기
	int GetPlayerRightAtt() { return m_tPlayerInfo.RightAttack; }
	int GetPlayerLeftAtt() { return m_tPlayerInfo.LeftAttack; }

	// Player의 공격력 세팅하기
	void SetPlayerRightAtt(int _Att) { m_tPlayerInfo.RightAttack = _Att; }
	void SetPlayerLeftAtt(int _Att) { m_tPlayerInfo.LeftAttack = _Att; }
	

public:
	// 디버프 타입에 따라 플레이어의 디버프 상태를 체크해준다.
	void SetDebuf(DEBUF_TYPE _eDeBufType);

	// 디버프 상태를 확인해보는 함수
	bool CheckDebuf(DEBUF_TYPE _eDeBufType);

	// 디버프를 해제 해주는 함수
	void ReleaseDebuf(DEBUF_TYPE _eDeBufType);

public:
	CGameObject* GetTile() { return m_pOwnerTile; }

	// Player의 오른손 스크립트 가져오기
	CPlayerRightHandScript* GetRightHandScript() { return (CPlayerRightHandScript*)(GetOwner()->GetChildObject(L"PlayerRightHand")->GetScript((UINT)PLAYERRIGHTHANDSCRIPT)); }
	// Player의 왼손 스크립트 가져오기
	CPlayerLeftHandScript* GetLeftHandScript() {return (CPlayerLeftHandScript*)(GetOwner()->GetChildObject(L"PlayerLeftHand")->GetScript((UINT)PLAYERLEFTHANDSCRIPT)); }
	// Player의 양손 스크립트 가져오기
	CPlayerHandScript* GetBothHandScript() { return (CPlayerHandScript*)(GetOwner()->GetChildObject(L"PlayerBothHand")->GetScript((UINT)PLAYERHANDSCRIPT)); }

	// Player의 오른손 무기 타입 가져오기
	WAPON_TYPE GetRightWaponType() { return m_eRightWaponType; }
	// Player의 왼손 무기 타입 가져오기
	WAPON_TYPE GetLeftWaponType() { return m_eLeftWaponType; }

	// Player의 무기 타입 지정
	void SetRightWaponType(WAPON_TYPE _eType) { m_eRightWaponType = _eType; }
	void SetLeftWaponType(WAPON_TYPE _eType) { m_eLeftWaponType = _eType; }

	void SetRightWaponInfo(tPlayer_Wapon _Info) { m_tRightWapon = _Info; }
	void SetLeftWaponInfo(tPlayer_Wapon _Info) { m_tLefetWapon = _Info; }

	tPlayer_Wapon GetRightWaponInfo() { return m_tRightWapon; }
	tPlayer_Wapon GetLeftWaponInfo() { return m_tLefetWapon; }

	CWaponScript* GetPlayerRightWapon() { return m_pRightWapon; }
	CWaponScript* GetPlayerLeftWapon() { return m_pLeftWapon; }

public:
	// 플레이어가 위치한 타일 주소 설정 함수 : 설정을 하면 해당 타일에도 플레이어를 등록해준다.
	void SetOwnerTile(CGameObject* _Tile);

	// 플레이어의 스탯 관련 함수
	void SetDeffence(int _Def) { m_tPlayerInfo.Deffence = _Def; }
	void AddDeffence(int _Def) { m_tPlayerInfo.Deffence += _Def; }

public:
	// 플레이어 포지션 이니셜 : 플레이어 발이 기준이 되도록 포지션을 초기화 해준다.
	void PosInit();
	
	// 플레이어가 서있는 타일로 포지션을 다시 잡아준다.
	void PosOnTile();

	// 플레이어의 포지션을 임시 타일로 옮겨서 보여준다.
	void PosOnTempTile(CTileScript* _TempTile);

	// Player의 회전이 가능, 혹은 불가능으로 변경해주는 함수
	void PlayerLockRot(bool _Lock) { m_bLockRot = _Lock; }

	// Player가 바라보는 방향을 오른쪽으로 변경해 준다.
	void PlayerRotRight() { Transform()->SetRelativeRotation(0.f, 3.14159f, 0.f); m_bRot = true; }

	// Player가 바라보는 방향을 왼쪽으로 변경해 준다.
	void PlayerRotLeft() { Transform()->SetRelativeRotation(0.f, 0.f, 0.f); m_bRot = false; }

	// Player가 회전을 했는지 알아오는 함수
	bool Is_Rot() { return m_bRot; }

public:
	// Player의 IDLE상태 애니메이션 실행
	void IDLEAnimPlay();

	// Player의 Attack 상태 애니메이션 실행
	void AttackAnimPlay(CARD_DIR_TYPE _CardDir);

public:
	// Player 공격 동작 함수
	void Attack(CTileScript* _pTargetTile, CARD_DIR_TYPE _CardDir);

	// Player 이동 동작 함수
	void Move(CTileScript* _pTargetTile);

	// Player 피격 동작 함수
	void Hit(int _iAtk);

	// Player가 피격모션이 끝났는지 확인하는 함수
	// 공격을 받고 모션이 끝나면 false가 반환된다.
	bool Is_Hit() { return m_bHit; };

public:
	// Player의 공격 진행 tick함수
	void Attack_tick();

	// Player의 이동 진행 tick 함수
	void Move_tick();

	// Player의 피격 진행 tick 함수
	void Hit_tick();

	// Player 이동 관련 초기화 함수
	void MoveInit();

	// 진행 방향으로 이동을 해주는 함수
	void MoveOriginDir(float _x, float _y);

	// 진행과 반대 방향으로 이동해주는 함수
	void MoveOppoDir(float _x, float _y);

	// DT동안 이동해야할 Vec2를 계산해주는 함수
	Vec2 CalculateDTPos();

	// Player가 목표로한 타일까지의 각도, 방향을 알아내는 함수
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