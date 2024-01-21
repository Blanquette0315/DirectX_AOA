#pragma once
#include "CMonsterScript.h"
class CChargeScript :
    public CMonsterScript
{
private:
	TILE_DIR	m_eTargetDIR;	// ������ �����ؾ��� ����
	bool		m_bCharge;		// ���� �غ� �������� Ȯ���Ѵ�.

public:
	// ���� AI
	virtual void MosnsterAIPlay() override;

	virtual void Hit(int ATK, WAPON_TYPE _eWaponType) override;

	virtual void Move() override;

	virtual void Charge() override;

	virtual void Rush() override;

public:
	// IDLE �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayIDLEAnim() override;

	// ��� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayDefAnim() override;

	// ���� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayAttackAnim() override;

	// ���� ���� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayDeadAnim() override;

	// ���� �ִϸ��̼����� ��ȯ�� �ش�.
	virtual void PlayStunAnim() override;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _pOther) override;
	virtual void Overlap(CCollider2D* _pOther) override;
	virtual void EndOverlap(CCollider2D* _pOther) override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CChargeScript);
public:
	CChargeScript();
	~CChargeScript();
};