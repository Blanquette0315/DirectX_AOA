#pragma once

#define RESTRAIN_DEBUF 0x01

// ����� ����
enum class DEBUF_TYPE
{
    RESTRAIN,
};

// Ÿ�� ���� ����
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

// ������Ʈ Ÿ�� ����
enum class OBJECT_TYPE
{
    NONE,
    PLAYER,
    MONSTER,
    ITEM,
    END,
};

// Ÿ�� ���� ����
enum class TILE_STATE
{
    END,
};


// ī���� ����
enum class CARD_TYPE
{
    ATTACK,
    DEFFENCE,
    MOVE,
};

// GameRuleMgr���� ����� ���� ����
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

// ������ Ÿ��
enum class WAPON_TYPE
{
    KATANA,
    SWORD,
    SHIELD,
};

// ī���� �޼� ������ Ÿ��
enum class CARD_DIR_TYPE
{
    RIGHT,
    LEFT,
};

// Monster�� ���� ����
enum class MONSTER_PATTERN
{
    ATTACK,     // ����
    STUN,       // ����
    RESTRAIN,   // �ӹ�
    CHARGE,     // ��¡
    RUSH,       // ����
    RELOAD,     // ������
    MOVE,       // �̵�
    SHEILD,     // ����
    REFLACT,    // �ݻ�
    RUN,        // ����

    BOSS_CHARGE, // ������ ����
    BOSS_RUSH,   // ������ ����

    NONE,       // ����Ʈ �ƹ��͵� �ƴ� ����
};

// �������� �̸�
enum class ITEM_NAME
{
    HPPOTION,   // ü�� ����
};

// ������ ����
enum class ITEM_TYPE
{
    WAPON,  // ���� ��������
    ITEM,   // �Ҹ������ ��������
};