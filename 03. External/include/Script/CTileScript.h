#pragma once
#include <Engine/CScript.h>

class CTileScript :
    public CScript
{
private:
    CGameObject*    m_arrAdjacentTile[(UINT)TILE_DIR::END]; // 본인의 인접 타일 주소 배열
    CGameObject*    m_pTargetObj;                           // 타일 위에 올라가 있는 오브젝트
    OBJECT_TYPE     m_eObjType;                             //오브젝트 타입

    TILE_STATE      m_eTileStateType;                       // 현재 타일의 상태 (이펙트 출력시 사용할 것)

    bool            m_bSelectedCard;                        // 카드가 선택되어 플레이어 타일이 대상 타일로 선택되었는지 확인하는 변수

    bool            m_bSelected;                            // 마우스가 타일위에 있어서 선택이 되었는지 확인하는 변수
    bool            m_bAdjacentSelected;                    // 본인이 인접 타일로써 선택이 되었는지를 체크

    // 특정 함수들이 한번만 수행되도록 하는 Once 변수들
    bool            AdjacentSelectOnce;
    bool            DynamicSelectOnce;

    // AStar 알고리즘에 쓰일 변수
    CTileScript* m_pPrevTileScript;   // 이전 타일의 주소
    float		 m_fFromParent;     // 이전 타일에서 현재 타일까지의 거리
    float		 m_fToDest;	        // 현재 타일에서 목적지 까지의 거리
    float		 m_fFinal;		    // 위에 두 값을 합친 값(우선순위 기준)

    bool		 m_bOpen;           // OpenList 에 들어갔는지
    bool		 m_bClosed;         // ClostList 에 들어있는지

    bool		 m_bPath;

public:
    // 타일이 선택되었는지 체크해주는 함수
    bool Is_Selected() { return m_bSelected; }

    // 카드가 선택되어 Player 타일이 선택된 타일로 넣어주는 함수
    void SetSelectedCard(bool _Selected) { m_bSelectedCard = _Selected; }

    // 인접 타일을 TileMgr에 등록해주는 함수
    void RegisterAdjTile(int _iCount, TILE_DIR _eTileDir);

    // 인접 타일로 선택해주는 함수 : 이게 켜진상태가 되면, 타일이 노란색으로 보임
    void SetAdjacentSelect(bool _Selected) { m_bAdjacentSelected = _Selected; }

public:
    // Tile위에 위치해있는 TargetObj를 받는다.
    void SetTargetObj(CGameObject* _TargetObj, OBJECT_TYPE _ObjType) { m_pTargetObj = _TargetObj; m_eObjType = _ObjType; }

    // Tile위에 있는 Object를 반환해 준다.
    CGameObject* GetTargetObject() { return m_pTargetObj; }

    // Tile 위에 위치해있는 TargetObj의 Type을 반환해주는 함수
    OBJECT_TYPE GetTileTargetType() { return m_eObjType; }

    // Tile위에 위치해있는 소유 오브젝트를 제거해 주는 함수
    void ReleaseTargetObj() { m_pTargetObj = nullptr; m_eObjType = OBJECT_TYPE::NONE; }

    // Tile의 인접 타일에 Player가 있는지 확인해 준다.
    bool FindAdjPlayer();

    // 몬스터가 도망가기 위한 함수
    // Tile의 인접 타일중 Player 반대 방향의 Tile을 알려주는 함수
    // 만약 타일이 존재하지 않는다면, 차선책을 준다. 그래도 없으면, 혹은 주위에 Player가 없으면 nullptr을 반환한다.
    CTileScript* GetOppPlayerTile();

    // 본인의 인접 타일 방향 중 사거리 내에 플레이어가 있는지 확인하는 함수
    // 있다면 해당 TileScript를 반환한다.
    CTileScript* FindPlayerinRange(int _Range);
    CTileScript* CheckPlayerinRange(int _Range, TILE_DIR _TileDir);

    // 본인의 인접 타일 방향 중 사거리 내에 플레이어가 있는지 확인하는 함수
    // 있다면 진행해야하는 방향을 알려준다.
    TILE_DIR FindPlayerinRange_DIR(int _Range);

private:
    // 인접 타일을 찾아서 배열에 저장해주는 함수
    void AdjacentTileinit();

    // 본인의 인접타일에 인접타일로 선택되었다는 것을 알려주기
    void CheckAdjTile();

    // 본인의 인접타일에게 인접 타일로 선택된 것을 해제해주는 함수
    void ReleaseAdjTile();

    // 본인이 인접 타일일 경우 인접 타일로 표시해주는 함수
    void RenderAdjTile();


private:
    // Astar 알고리즘 함수들
    // 현재 스크립트의 비용을 계산해준다.
    void CalculateCost(CTileScript* _pCurNode, CTileScript* _pOrigin);

    // 임시 대상 스크립트의 비용을 계산해준다.
    float CloneCalculateCost(CTileScript* _pOrigin, Vec3 _CurNodePos);

    // 본인의 인접한 타일들을 조건별로 OpenList에 추가해준다.
    void AddOpenList();

public:
    // Astar의 최종 비용을 반환해 준다.
    float GetFinal() { return m_fFinal; }

    // CloseList에 포함됨을 체크해준다.
    void SetClosed() { m_bClosed = true; }

    // OpenList에 들어갔는지 확인한다.
    bool Is_OpenList() { return m_bOpen; }

    // OpenList에 추가된 후 호출해 체크해준다.
    void SetOpen() { m_bOpen = true; }

    // 본인의 이전 타일을 반환해준다.
    CTileScript* GetPrevTileScript() { return m_pPrevTileScript; }

    // Astar에 필요한 멤버들 초기화 함수
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

