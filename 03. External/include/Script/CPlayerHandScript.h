#pragma once
#include <Engine/CScript.h>

class CPlayerScript;

class CPlayerHandScript :
    public CScript
{
private:
	bool	m_bAttack;
	bool	m_bIDLE;

	WAPON_TYPE		m_eWaponType;
	CARD_DIR_TYPE	m_eAttackDir;

public:
	// 오른손 애니메이션 실행 함수
	void AttackAnimPlay(WAPON_TYPE _WAPONTYPE, CARD_DIR_TYPE _DIRTYPE);
	void IDLEAnimPlayer(WAPON_TYPE _WAPONTYPE);

	void StopAnim();

	// PlayerScript를 가져오는 함수
	CPlayerScript* GetPlayerScript() { return (CPlayerScript*)(GetOwner()->GetParent()->GetScript((UINT)PLAYERSCRIPT)); }

public:
	// 오른손 애니메이션 z축관리 진행 tick함수
	void Attack_Katana_tick();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerHandScript);
public:
    CPlayerHandScript();
    ~CPlayerHandScript();
};