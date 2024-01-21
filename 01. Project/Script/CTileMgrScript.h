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
    CGameObject*            m_pGameMgr;             // 게임 매니져 오브젝트 주소
    CGameMgrScript*         m_pGameMgrScript;       // 게임 매니져 스크립트 주소
    vector<CTileScript*>    m_vecValidTile;         // 현재 플레이어 타일에서 유효한 타일들이 등록된 vector

    bool                    m_bCardSelected;

    priority_queue<CTileScript*, vector<CTileScript*>, Compare>	m_OpenList;

public:
    // Astar 알고리즘을 이용해 Player까지의 거리와 다음 이동할 최적의 타일을 찾아준다.
    tTargetTile_Info FindAstar(CTileScript* _ToTile);

    // OpenList에 등록해주는 함수 (Astar 알고리즘에서 사용한다.)
    void Push_OpenList(CTileScript* _TileScript) { m_OpenList.push(_TileScript); }

    // 오픈리스트 (우선순위큐) 재설정
    void Rebuild();

    // Astar에 필요한 모든 변수들을 초기화 해준다.
    void AstarInit();

public:
    CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }

    void SetCardSelected(bool _Select) { m_bCardSelected = _Select; }

    // 유효 거리 만큼 인접타일 표시해주는 함수
    void ShowRange(int _iRange);

    // 인접한 발판으로 등록되어 있는 타일들의 인접 선택 불 값을 변경해주는 함수
    void ValidTileSetAdj(bool _OnOff);

public:
    // Obj를 특정 타일로 변경시켜주는 함수
    void ObjectChangeTile(CGameObject* _TargetObj, CTileScript* _TargetTile, OBJECT_TYPE _eObjType);

    // 유효한 타일 등록하기
    void SetValidTile(CTileScript* _TargetTile) { m_vecValidTile.push_back(_TargetTile); }

    // 유효 타일로 등록된 것을 지워 준다.
    void ValidTileClear() { ValidTileSetAdj(false); m_vecValidTile.clear(); }

    // 유요한 타일내에서 선택된 타일을 반환해주는 함수
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

