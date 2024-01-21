#pragma once
#include "CMonsterScript.h"
class CRefBeetleScript :
    public CMonsterScript
{
private:

public:
	// 몬스터 AI
	virtual void MosnsterAIPlay() override;

	virtual void Hit(int ATK, WAPON_TYPE _eWaponType) override;

	virtual void Move() override;

public:
	// IDLE 애니메이션으로 전환해 준다.
	virtual void PlayIDLEAnim() override;

	// 방어 애니메이션으로 전환해 준다.
	virtual void PlayDefAnim() override;

	// 공격 애니메이션으로 전환해 준다.
	virtual void PlayAttackAnim() override;

	// 죽은 상태 애니메이션으로 전환해 준다.
	virtual void PlayDeadAnim() override;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _pOther) override;
	virtual void Overlap(CCollider2D* _pOther) override;
	virtual void EndOverlap(CCollider2D* _pOther) override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

    CLONE(CRefBeetleScript);
public:
    CRefBeetleScript();
    ~CRefBeetleScript();
};