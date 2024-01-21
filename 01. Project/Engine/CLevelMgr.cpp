#include "pch.h"
#include "CLevelMgr.h"

#include "CEventMgr.h"
#include "CResMgr.h"
#include "CCollisionMgr.h"

#include "CLevel.h"
#include "CGameObject.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	/*if (nullptr != m_pCurLevel)
	{
		m_mapLevel.erase(m_pCurLevel->GetRelativePath());
		delete m_pCurLevel;
	}*/

	Safe_Del_Map<wstring, CLevel*>(m_mapLevel);
}

void CLevelMgr::init()
{
	// 임시 Level 제작하기
	m_pCurLevel = new CLevel;
	m_pCurLevel->SetName(L"Level 0");
	RegisterLevel(m_pCurLevel->GetName(), m_pCurLevel);
}

void CLevelMgr::progress()
{
	// 이제 tick이 안돌기 때문에 정지 상태에서 Layer들을 Clear해주지 않으면 계속 쌓이게 되는 문제가 발생한다.
	m_pCurLevel->ClearLayer();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();
	}

	// 정지 상태일 때도 finaltick을 돌려주어야 하는 이유는 각 컴포넌트에서 fianltick에서 행렬등을 연산해주어야
	// 렌더링을 해주기 때문에 finaltick은 돌려주어야 한다.
	m_pCurLevel->finaltick();
}

LEVEL_STATE CLevelMgr::GetLevelState()
{
	return m_pCurLevel->GetState();
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _name)
{
	return m_pCurLevel->FindObjectByName(_name);
}

void CLevelMgr::FindObjectByName(const wstring& _name, vector<CGameObject*>& _out)
{
	m_pCurLevel->FindObjectByName(_name, _out);
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	// 현재 레벨이 임시 레벨이었을 경우 지우기
	//if (L"Level 0" == m_pCurLevel->GetName())
	//{
	//	m_mapLevel.erase(L"Level 0");
	//	delete m_pCurLevel;
	//}

	if (nullptr != m_pCurLevel)
	{
		m_pCurLevel->SetState(LEVEL_STATE::STOP);
	}

	m_pCurLevel = _NextLevel;
	// 변경될 Level이 지닌 Collsion 충돌 설정 값으로 변경해준다.
	_NextLevel->RefreshCollsionSet();
}

void CLevelMgr::RegisterLevel(const wstring& _strKey, CLevel* _level)
{
	CLevel* pTemLevel = FindLevel(_strKey);
	assert(!pTemLevel);

	m_mapLevel.insert(make_pair(_strKey, _level));
}

void CLevelMgr::DeRegisterLevel(const wstring& _strKey)
{
	map<wstring, CLevel*>::const_iterator iter = m_mapLevel.find(_strKey);
	CLevel* TargetLevel = iter->second;
	m_mapLevel.erase(_strKey);

	delete TargetLevel;
}

CLevel* CLevelMgr::FindLevel(const wstring& _strKey)
{
	map<wstring, CLevel*>::const_iterator iter = m_mapLevel.find(_strKey);
	if (iter == m_mapLevel.end())
		return nullptr;
	else
		return iter->second;
}


void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	// 이미 플레이 상태인데, 플레이상태가 또 들어오면 안되기 때문에 예외처리를 해주었다.
	assert(!(m_pCurLevel->GetState() == _State));

	m_pCurLevel->SetState(_State);

	// 플레이 상태가 되면, 레벨이 지닌 모든 것들을 begin()을 호출해줄 것이다.
	if (LEVEL_STATE::PLAY == _State)
		m_pCurLevel->begin();
}
