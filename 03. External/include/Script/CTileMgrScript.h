#pragma once
#include <Engine/CScript.h>

class CGameMgrScript;
class CTileScript;

// Open List
class Compare
{
public:
    bool operator() (CTileScript* _pLeft, CTileScript* _pRight)
    {
        return _pLeft->GetFinal() > _pRight->GetFinal();
    }
};

class CTileMgrScript :
    public CScript
{
private:
    CGameObject*            m_pGameMgr;             // ���� �Ŵ��� ������Ʈ �ּ�
    CGameMgrScript*         m_pGameMgrScript;       // ���� �Ŵ��� ��ũ��Ʈ �ּ�
    vector<CTileScript*>    m_vecValidTile;         // ���� �÷��̾� Ÿ�Ͽ��� ��ȿ�� Ÿ�ϵ��� ��ϵ� vector

    bool                    m_bCardSelected;

    priority_queue<CTileScript*, vector<CTileScript*>, Compare>	m_OpenList;

public:
    // Astar �˰����� �̿��� Player������ �Ÿ��� ���� �̵��� ������ Ÿ���� ã���ش�.
    tTargetTile_Info FindAstar(CTileScript* _ToTile);

    // OpenList�� ������ִ� �Լ� (Astar �˰��򿡼� ����Ѵ�.)
    void Push_OpenList(CTileScript* _TileScript) { m_OpenList.push(_TileScript); }

    // ���¸���Ʈ (�켱����ť) �缳��
    void Rebuild();

    // Astar�� �ʿ��� ��� �������� �ʱ�ȭ ���ش�.
    void AstarInit();

public:
    CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }

    void SetCardSelected(bool _Select) { m_bCardSelected = _Select; }

    // ��ȿ �Ÿ� ��ŭ ����Ÿ�� ǥ�����ִ� �Լ�
    void ShowRange(int _iRange);

    // ������ �������� ��ϵǾ� �ִ� Ÿ�ϵ��� ���� ���� �� ���� �������ִ� �Լ�
    void ValidTileSetAdj(bool _OnOff);

public:
    // Obj�� Ư�� Ÿ�Ϸ� ��������ִ� �Լ�
    void ObjectChangeTile(CGameObject* _TargetObj, CTileScript* _TargetTile, OBJECT_TYPE _eObjType);

    // ��ȿ�� Ÿ�� ����ϱ�
    void SetValidTile(CTileScript* _TargetTile) { m_vecValidTile.push_back(_TargetTile); }

    // ��ȿ Ÿ�Ϸ� ��ϵ� ���� ���� �ش�.
    void ValidTileClear() { ValidTileSetAdj(false); m_vecValidTile.clear(); }

    // ������ Ÿ�ϳ����� ���õ� Ÿ���� ��ȯ���ִ� �Լ�
    CTileScript* SelectInValidTile();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CTileMgrScript);
public:
    CTileMgrScript();
    ~CTileMgrScript();
};

