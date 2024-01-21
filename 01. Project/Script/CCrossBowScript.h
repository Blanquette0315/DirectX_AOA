#pragma once
#include "CMonsterScript.h"
class CCrossBowScript :
    public CMonsterScript
{
private:
	Ptr<CPrefab> m_Prefab;	// 화살 프리펩 주소를 지니는 변수
	CTileScript* m_pTargetTileScript;
	bool		 m_bBowLoading;	// 화살이 장전 되어있는지 확인하는 변수

public:
	void CreateArrowShootEff();

	void CreateArrowReloadSound();
	void CreateArrowShootSound();

public:
	// 몬스터 AI
	virtual void MosnsterAIPlay() override;

	virtual void Hit(int ATK, WAPON_TYPE _eWaponType) override;

	virtual void Move() override;

	virtual void Attack() override;

	virtual void Reload() override;

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

	CLONE(CCrossBowScript);
public:
	CCrossBowScript();
	~CCrossBowScript();
};

