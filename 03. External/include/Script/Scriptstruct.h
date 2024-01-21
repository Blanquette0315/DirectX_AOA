#pragma once

// 카드 상태값 구조체
struct tCardState
{
	int Range;
	int Attack;
	int Deffence;
	int Cost;
};

// Player 스탯 구조체
struct tPlayerInfo
{
	int Max_HP;			// Player의 최대 체력
	int Cur_HP;			// Player의 현재 체력
	int RightAttack;	// Player의 공격력 : 공격력은 Player가 지닌 무기에 따라 바뀌어야 한다.
	int LeftAttack;
	int Deffence;		// Player의 방어력 : 방어력은 Card를 사용할 때 오른다.
	int Max_Acction;	// Player의 최대 행동력
	int Cur_Acction;	// Player의 현재 남은 행동력

	int DrowCount;		// Player가 카드를 얼마만큼 드로우 할지
};

// 몬스터 정보 구조체
struct tMonsterInfo
{
	int Max_HP;			// Monster의 최대 체력
	int Cur_HP;			// Monster의 현재 체력
	int Attack;			// Monster의 공격력
	int Deffence;		// Mnster의 방어력
	int Range;			// Monster의 공격 범위
	int iTile;
};

#include "CTileScript.h"
// TileMgr로 부터 방향과 거리를 받을 때 사용할 구조체
struct tTargetTile_Info
{
	CTileScript* pNextTile;		// 다음으로 이동해야할 타일 위치
	int			 iTargetRange;	// 타겟까지의 거리
};


// Player가 지닌 카드 리스트를 구현해주기 위한 구조체
struct tPlayerCardInfo
{
	CARD_TYPE		CardName;	// 해당 카드의 이름
	bool			Reinforce;	// 해당 카드의 강화 여부 확인
	CARD_DIR_TYPE	CardDir;	// 해당 카드가 우측인지 좌측인지
	// 카드가 소멸하는 카드인지도 표기해주면 좋을 것같다.
};

// 무기 객체를 생성하기 위한 구조체
struct tPlayer_Wapon
{
	WAPON_TYPE			eWaponType;		// 무기 타입
	CARD_DIR_TYPE		eWaponDir;		// 어느 손에 장착했는지 방향
	int					iATT;			// 공격력
	int					iDff;			// 방어력
	// 강화 관련
};

// 아이템 객체 생성을 위한 구조체
struct tItem
{
	ITEM_NAME eItemType;	// 어떤 아이템인지
};

// 인밴토리에 존재하는 아이템 정보
struct tItem_Info
{
	ITEM_TYPE		eItemType;		// 아이템의 종류

	tPlayer_Wapon	tWapon;			// 무기 관련 정보
	tItem			tItem;			// 인벤토리에 있는 아이템

	bool			bExist = false;	// 아이템이 존재하는지의 여부 기본적으로 false를 넣어준다.
};