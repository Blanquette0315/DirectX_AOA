#pragma once
#include "CMonsterScript.h"

class CTileScript;

class CIceReaperScript :
    public CMonsterScript
{
private:
	CTileScript*	m_pTargetTileScript;
	bool			m_bCharge;

public:
	// 몬스터 AI
	virtual void MosnsterAIPlay() override;

	virtual void Hit(int ATK, WAPON_TYPE _eWaponType) override;

	virtual void Move() override;

	virtual void Boss_Charge() override;

	virtual void Boss_Rush() override;

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

	CLONE(CIceReaperScript);
public:
	CIceReaperScript();
	~CIceReaperScript();
};