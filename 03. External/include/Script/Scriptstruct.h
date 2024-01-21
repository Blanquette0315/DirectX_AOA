#pragma once

// ī�� ���°� ����ü
struct tCardState
{
	int Range;
	int Attack;
	int Deffence;
	int Cost;
};

// Player ���� ����ü
struct tPlayerInfo
{
	int Max_HP;			// Player�� �ִ� ü��
	int Cur_HP;			// Player�� ���� ü��
	int RightAttack;	// Player�� ���ݷ� : ���ݷ��� Player�� ���� ���⿡ ���� �ٲ��� �Ѵ�.
	int LeftAttack;
	int Deffence;		// Player�� ���� : ������ Card�� ����� �� ������.
	int Max_Acction;	// Player�� �ִ� �ൿ��
	int Cur_Acction;	// Player�� ���� ���� �ൿ��

	int DrowCount;		// Player�� ī�带 �󸶸�ŭ ��ο� ����
};

// ���� ���� ����ü
struct tMonsterInfo
{
	int Max_HP;			// Monster�� �ִ� ü��
	int Cur_HP;			// Monster�� ���� ü��
	int Attack;			// Monster�� ���ݷ�
	int Deffence;		// Mnster�� ����
	int Range;			// Monster�� ���� ����
	int iTile;
};

#include "CTileScript.h"
// TileMgr�� ���� ����� �Ÿ��� ���� �� ����� ����ü
struct tTargetTile_Info
{
	CTileScript* pNextTile;		// �������� �̵��ؾ��� Ÿ�� ��ġ
	int			 iTargetRange;	// Ÿ�ٱ����� �Ÿ�
};


// Player�� ���� ī�� ����Ʈ�� �������ֱ� ���� ����ü
struct tPlayerCardInfo
{
	CARD_TYPE		CardName;	// �ش� ī���� �̸�
	bool			Reinforce;	// �ش� ī���� ��ȭ ���� Ȯ��
	CARD_DIR_TYPE	CardDir;	// �ش� ī�尡 �������� ��������
	// ī�尡 �Ҹ��ϴ� ī�������� ǥ�����ָ� ���� �Ͱ���.
};

// ���� ��ü�� �����ϱ� ���� ����ü
struct tPlayer_Wapon
{
	WAPON_TYPE			eWaponType;		// ���� Ÿ��
	CARD_DIR_TYPE		eWaponDir;		// ��� �տ� �����ߴ��� ����
	int					iATT;			// ���ݷ�
	int					iDff;			// ����
	// ��ȭ ����
};

// ������ ��ü ������ ���� ����ü
struct tItem
{
	ITEM_NAME eItemType;	// � ����������
};

// �ι��丮�� �����ϴ� ������ ����
struct tItem_Info
{
	ITEM_TYPE		eItemType;		// �������� ����

	tPlayer_Wapon	tWapon;			// ���� ���� ����
	tItem			tItem;			// �κ��丮�� �ִ� ������

	bool			bExist = false;	// �������� �����ϴ����� ���� �⺻������ false�� �־��ش�.
};