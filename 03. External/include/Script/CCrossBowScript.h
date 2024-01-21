#pragma once
#include "CMonsterScript.h"
class CCrossBowScript :
    public CMonsterScript
{
private:
	Ptr<CPrefab> m_Prefab;	// ȭ�� ������ �ּҸ� ���ϴ� ����
	CTileScript* m_pTargetTileScript;
	bool		 m_bBowLoading;	// ȭ���� ���� �Ǿ��ִ��� Ȯ���ϴ� ����

public:
	void CreateArrowShootEff();

	void CreateArrowReloadSound();
	void CreateArrowShootSound();

public:
	// ���� AI
	virtual void MosnsterAIPlay() override;

	virtual void Hit(int ATK, WAPON_TYPE _eWaponType) override;

	virtual void Move() override;

	virtual void Attack() override;

	virtual void Reload() override;

public:
	// IDLE �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayIDLEAnim() override;

	// ��� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayDefAnim() override;

	// ���� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayAttackAnim() override;

	// ���� ���� �ִϸ��̼����� ��ȯ�� �ش�.
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

