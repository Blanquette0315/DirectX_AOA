#pragma once

#define RESTRAIN_DEBUF 0x01

// 디버프 종류
enum class DEBUF_TYPE
{
    RESTRAIN,
};

// 타일 방향 정보
enum class TILE_DIR
{
    LEFT,
    LEFTTOP,
    RIGHTTOP,
    RIGHT,
    RIGHTBOT,
    LEFTBOT,

    END,

    ALL,
    SELF,
};

// 오브젝트 타입 정보
enum class OBJECT_TYPE
{
    NONE,
    PLAYER,
    MONSTER,
    ITEM,
    END,
};

// 타일 상태 정보
enum class TILE_STATE
{
    END,
};


// 카드의 종류
enum class CARD_TYPE
{
    ATTACK,
    DEFFENCE,
    MOVE,
};

// GameRuleMgr에서 사용할 차례 종류
enum class TURN_TYPE
{
    TURNCOUNTING,
    DROWCARD,
    PLAYER,
    DISCARD,
    MONSTER,
    MONSTERACTION,
    CHECKMONSTER,
    WIN,
};

// 무기의 타입
enum class WAPON_TYPE
{
    KATANA,
    SWORD,
    SHIELD,
};

// 카드의 왼손 오른손 타입
enum class CARD_DIR_TYPE
{
    RIGHT,
    LEFT,
};

// Monster의 패턴 종류
enum class MONSTER_PATTERN
{
    ATTACK,     // 공격
    STUN,       // 스턴
    RESTRAIN,   // 속박
    CHARGE,     // 차징
    RUSH,       // 돌진
    RELOAD,     // 재장전
    MOVE,       // 이동
    SHEILD,     // 수비
    REFLACT,    // 반사
    RUN,        // 도망

    BOSS_CHARGE, // 보스몹 차지
    BOSS_RUSH,   // 보스몹 러시

    NONE,       // 디폴트 아무것도 아닌 상태
};

// 아이템의 이름
enum class ITEM_NAME
{
    HPPOTION,   // 체력 물약
};

// 아이템 종류
enum class ITEM_TYPE
{
    WAPON,  // 무기 종류인지
    ITEM,   // 소모아이템 종류인지
};