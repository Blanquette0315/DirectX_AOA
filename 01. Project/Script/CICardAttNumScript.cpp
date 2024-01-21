#include "pch.h"
#include "CICardAttNumScript.h"

#include "CNumberScript.h"
#include "CItemCardScript.h"

CICardAttNumScript::CICardAttNumScript()
	: CUIScript(ICARDATTNUMSCRIPT)
	, m_iAttack(0)
{
}

CICardAttNumScript::~CICardAttNumScript()
{
}

void CICardAttNumScript::begin()
{
}

void CICardAttNumScript::tick()
{
	CItemCardScript* pItemCardScript = (CItemCardScript*)GetOwner()->GetParent()->GetScript((UINT)ITEMCARDSCRIPT);

	// ���õ� ������ ī�尡 ���ϰ� �ִ� ���� ��������
	tItem_Info tempInfo = pItemCardScript->GetItemInfo();

	m_iAttack = tempInfo.tWapon.iATT;

	RenderNumber();
}

void CICardAttNumScript::RenderNumber()
{
	int Num0 = m_iAttack;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ItemNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);
}