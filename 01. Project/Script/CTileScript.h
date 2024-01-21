#pragma once
#include <Engine/CScript.h>

class CTileScript :
    public CScript
{
private:
    CGameObject*    m_arrAdjacentTile[(UINT)TILE_DIR::END]; // ������ ���� Ÿ�� �ּ� �迭
    CGameObject*    m_pTargetObj;                           // Ÿ�� ���� �ö� �ִ� ������Ʈ
    OBJECT_TYPE     m_eObjType;                             //������Ʈ Ÿ��

    TILE_STATE      m_eTileStateType;                       // ���� Ÿ���� ���� (����Ʈ ��½� ����� ��)

    bool            m_bSelectedCard;                        // ī�尡 ���õǾ� �÷��̾� Ÿ���� ��� Ÿ�Ϸ� ���õǾ����� Ȯ���ϴ� ����

    bool            m_bSelected;                            // ���콺�� Ÿ������ �־ ������ �Ǿ����� Ȯ���ϴ� ����
    bool            m_bAdjacentSelected;                    // ������ ���� Ÿ�Ϸν� ������ �Ǿ������� üũ

    // Ư�� �Լ����� �ѹ��� ����ǵ��� �ϴ� Once ������
    bool            AdjacentSelectOnce;
    bool            DynamicSelectOnce;

    // AStar �˰��� ���� ����
    CTileScript* m_pPrevTileScript;   // ���� Ÿ���� �ּ�
    float		 m_fFromParent;     // ���� Ÿ�Ͽ��� ���� Ÿ�ϱ����� �Ÿ�
    float		 m_fToDest;	        // ���� Ÿ�Ͽ��� ������ ������ �Ÿ�
    float		 m_fFinal;		    // ���� �� ���� ��ģ ��(�켱���� ����)

    bool		 m_bOpen;           // OpenList �� ������
    bool		 m_bClosed;         // ClostList �� ����ִ���

    bool		 m_bPath;

public:
    // Ÿ���� ���õǾ����� üũ���ִ� �Լ�
    bool Is_Selected() { return m_bSelected; }

    // ī�尡 ���õǾ� Player Ÿ���� ���õ� Ÿ�Ϸ� �־��ִ� �Լ�
    void SetSelectedCard(bool _Selected) { m_bSelectedCard = _Selected; }

    // ���� Ÿ���� TileMgr�� ������ִ� �Լ�
    void RegisterAdjTile(int _iCount, TILE_DIR _eTileDir);

    // ���� Ÿ�Ϸ� �������ִ� �Լ� : �̰� �������°� �Ǹ�, Ÿ���� ��������� ����
    void SetAdjacentSelect(bool _Selected) { m_bAdjacentSelected = _Selected; }

public:
    // Tile���� ��ġ���ִ� TargetObj�� �޴´�.
    void SetTargetObj(CGameObject* _TargetObj, OBJECT_TYPE _ObjType) { m_pTargetObj = _TargetObj; m_eObjType = _ObjType; }

    // Tile���� �ִ� Object�� ��ȯ�� �ش�.
    CGameObject* GetTargetObject() { return m_pTargetObj; }

    // Tile ���� ��ġ���ִ� TargetObj�� Type�� ��ȯ���ִ� �Լ�
    OBJECT_TYPE GetTileTargetType() { return m_eObjType; }

    // Tile���� ��ġ���ִ� ���� ������Ʈ�� ������ �ִ� �Լ�
    void ReleaseTargetObj() { m_pTargetObj = nullptr; m_eObjType = OBJECT_TYPE::NONE; }

    // Tile�� ���� Ÿ�Ͽ� Player�� �ִ��� Ȯ���� �ش�.
    bool FindAdjPlayer();

    // ���Ͱ� �������� ���� �Լ�
    // Tile�� ���� Ÿ���� Player �ݴ� ������ Tile�� �˷��ִ� �Լ�
    // ���� Ÿ���� �������� �ʴ´ٸ�, ����å�� �ش�. �׷��� ������, Ȥ�� ������ Player�� ������ nullptr�� ��ȯ�Ѵ�.
    CTileScript* GetOppPlayerTile();

    // ������ ���� Ÿ�� ���� �� ��Ÿ� ���� �÷��̾ �ִ��� Ȯ���ϴ� �Լ�
    // �ִٸ� �ش� TileScript�� ��ȯ�Ѵ�.
    CTileScript* FindPlayerinRange(int _Range);
    CTileScript* CheckPlayerinRange(int _Range, TILE_DIR _TileDir);

    // ������ ���� Ÿ�� ���� �� ��Ÿ� ���� �÷��̾ �ִ��� Ȯ���ϴ� �Լ�
    // �ִٸ� �����ؾ��ϴ� ������ �˷��ش�.
    TILE_DIR FindPlayerinRange_DIR(int _Range);

private:
    // ���� Ÿ���� ã�Ƽ� �迭�� �������ִ� �Լ�
    void AdjacentTileinit();

    // ������ ����Ÿ�Ͽ� ����Ÿ�Ϸ� ���õǾ��ٴ� ���� �˷��ֱ�
    void CheckAdjTile();

    // ������ ����Ÿ�Ͽ��� ���� Ÿ�Ϸ� ���õ� ���� �������ִ� �Լ�
    void ReleaseAdjTile();

    // ������ ���� Ÿ���� ��� ���� Ÿ�Ϸ� ǥ�����ִ� �Լ�
    void RenderAdjTile();


private:
    // Astar �˰��� �Լ���
    // ���� ��ũ��Ʈ�� ����� ������ش�.
    void CalculateCost(CTileScript* _pCurNode, CTileScript* _pOrigin);

    // �ӽ� ��� ��ũ��Ʈ�� ����� ������ش�.
    float CloneCalculateCost(CTileScript* _pOrigin, Vec3 _CurNodePos);

    // ������ ������ Ÿ�ϵ��� ���Ǻ��� OpenList�� �߰����ش�.
    void AddOpenList();

public:
    // Astar�� ���� ����� ��ȯ�� �ش�.
    float GetFinal() { return m_fFinal; }

    // CloseList�� ���Ե��� üũ���ش�.
    void SetClosed() { m_bClosed = true; }

    // OpenList�� ������ Ȯ���Ѵ�.
    bool Is_OpenList() { return m_bOpen; }

    // OpenList�� �߰��� �� ȣ���� üũ���ش�.
    void SetOpen() { m_bOpen = true; }

    // ������ ���� Ÿ���� ��ȯ���ش�.
    CTileScript* GetPrevTileScript() { return m_pPrevTileScript; }

    // Astar�� �ʿ��� ����� �ʱ�ȭ �Լ�
    void AstarInit();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CTileScript);
public:
    CTileScript();
    ~CTileScript();

    friend class CTileMgrScript;
};

