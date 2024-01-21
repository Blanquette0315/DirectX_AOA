#pragma once
#include <Engine/CScript.h>

class CGameMgrScript;
class CItemCardScript;

class CUIMgrScript :
    public CScript
{
private:
    CGameMgrScript* m_pGameMgrScript;

    CGameObject* m_pFocusedUI;
    CGameObject* m_pTargetUI;

    CItemCardScript* m_pSelectItemScript;  // ���õ� ������ ī���� �ּ�

    bool              m_bSelectedItem;      // �������� ���õǾ����� �˷��ش�.
    bool              m_bNext;

public:
    bool IS_SelectedSlot() { return m_bNext; }

public:
    // ������ ī����� �Լ���
    void SelectedItem(CItemCardScript* _TargetItem) { m_pSelectItemScript = _TargetItem; m_bSelectedItem = true; }

    // �������� ���õȰ� �ִ��� �������� �Լ�
    bool GetSelectedItem() { return m_bSelectedItem; }

    // GameMgr���� ���õ� �������� �ִ��� Ȯ���� ������ϱ� ���� ��ȿ�� �˻�
    bool IS_ItemVaild();

    // ��� ������ ��ũ��Ʈ ��������
    CItemCardScript* GetSelectedItemScript() { return m_pSelectItemScript; }

    // ��� ������ ī�� ���� �Լ�
    void ReleaseSelectedCard();

    // ������ ī�� ���� ȣ���� �Լ�
    void UseSelectedItemCard();

public:
    CGameMgrScript* GetGameMgrScript() { return m_pGameMgrScript; }

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CUIMgrScript);
public:
    CUIMgrScript();
    ~CUIMgrScript();
};