#include "pch.h"
#include "CExhasusPileScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"
#include "CCardMgrScript.h"

#include "CNumberScript.h"

CExhasusPileScript::CExhasusPileScript()
	: CUIScript(EXHASUSPILESCRIPT)
	, m_iExhasusSize(0)
{
}

CExhasusPileScript::~CExhasusPileScript()
{
}

void CExhasusPileScript::begin()
{
	CUIScript::begin();
}

void CExhasusPileScript::tick()
{
	CUIScript::tick();

	m_iExhasusSize = m_pUIMgrScript->GetGameMgrScript()->GetCardMgrScript()->GetExhust();

	RenderNumber();
}

void CExhasusPileScript::RenderNumber()
{
	// �Ҹ� ������ ���� ī���� ���� ������ش�.
	int Num0 = m_iExhasusSize / 10;

	// 10�� �ڸ��� ���� 0�� ��� ������� �ʱ� ����
	if (Num0 == 0)
	{
		Num0 = -1;
	}
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ExhustPileNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iExhasusSize % 10;
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ExhustPileNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}