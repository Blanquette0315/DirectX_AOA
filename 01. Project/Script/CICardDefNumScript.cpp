#include "pch.h"
#include "CICardDefNumScript.h"

#include "CNumberScript.h"
#include "CItemCardScript.h"

CICardDefNumScript::CICardDefNumScript()
	: CUIScript(ICARDDEFNUMSCRIPT)
	, m_iDef(0)
{
}

CICardDefNumScript::~CICardDefNumScript()
{
}

void CICardDefNumScript::begin()
{
}

void CICardDefNumScript::tick()
{
	CItemCardScript* pItemCardScript = (CItemCardScript*)GetOwner()->GetParent()->GetScript((UINT)ITEMCARDSCRIPT);

	// ���õ� ������ ī�尡 ���ϰ� �ִ� ���� ��������
	tItem_Info tempInfo = pItemCardScript->GetItemInfo();

	m_iDef = tempInfo.tWapon.iDff;

	RenderNumber();
}

void CICardDefNumScript::RenderNumber()
{
	int Num0 = m_iDef;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ItemNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);
}