#pragma once
#include "CEntity.h"

class CComponent;
class CTransform;
class CRenderComponent;
class CMeshRender;
class CCollider2D;
class CAnimator2D;
class CCamera;
class CScript;
class CLight2D;
class CParticleSystem;
class CTileMap;

#define GET_COMPONENT(classname, CLASSNAME ) C##classname* classname() { return ( C##classname*)GetComponent(COMPONENT_TYPE::CLASSNAME); }

class CGameObject :
    public CEntity
{
private:
    CGameObject*                m_pParent;

    CComponent*                 m_arrCom[(UINT)COMPONENT_TYPE::END];
    vector<CScript*>            m_vecScripts;

    CRenderComponent*           m_pRenderComponent;

    int                         m_iLayerIdx;
    bool                        m_bDead;
    bool                        m_bRender;  // ������ �� �� ���� �����ִ� ���� �⺻�� true

protected:
    vector<CGameObject*>        m_vecChild;

public:
    void begin();
    void tick();
    void finaltick();
    void render();


// ������ ��ȯ
public:
    void ChangePrefab();

    // �������� ���� ������ �����ش�.
    void SetRender(bool _render) { m_bRender = _render; }

public:
    void AddComponent(CComponent* _pComponent);
    void ReleaseComponent(CComponent* _pComponent);
    CComponent* GetComponent(COMPONENT_TYPE _eComType) { return m_arrCom[(UINT)_eComType]; }
    void AddChild(CGameObject* _pChild);

    // CCardMgr������ ��� �ؾ��Ѵ�.
    void ClearChild() { m_vecChild.clear(); }

    template<typename T>
    T* GetScript();

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(TileMap, TILEMAP);

    CRenderComponent* GetRenderComponent() { return m_pRenderComponent; }

    CGameObject* GetParent() { return m_pParent; }
    const vector<CGameObject*>& GetChildObject()const { return m_vecChild; }

    // �ڽ� �̸����� �ڽ��� ã���ִ� ��� : ���� ������� �ҵ�.
    CGameObject* GetChildObject(const wstring& _strName);

    void DisconnectFromParent(); // �θ��� �ڽ� vector���� ������ ���ش�.

    const vector<CScript*>& GetScripts()const { return m_vecScripts; }

    // Script enum�� (Script�� ID)�� Script�� ã�Ƽ� ��ȯ�� �ش�.
    CScript* GetScript(int _ScriptType);

    int GetLayerIdx() { return m_iLayerIdx; }
    void SetLayerIdx(int _LayerIdx) { m_iLayerIdx = _LayerIdx; }
    void SetAllLayer(int _LayerIdx);

    bool IsDead() { return m_bDead; }
    void Destroy();

    CLONE(CGameObject);

public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    virtual ~CGameObject();

    friend class CLayer;
    friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScripts.size(); ++i)
    {
        T* pScript = dynamic_cast<T*>(m_vecScripts[i]);
        if (nullptr != pScript)
            return pScript;
    }
    return nullptr;
}