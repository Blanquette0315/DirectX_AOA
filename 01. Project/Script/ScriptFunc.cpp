#include "pch.h"
#include "ScriptFunc.h"

CMonsterScript* GetMonsterScript(CGameObject* _Obj)
{
	if (_Obj->GetScript(FASTSCRIPT))
	{
		return (CMonsterScript*)_Obj->GetScript(FASTSCRIPT);
	}
	if (_Obj->GetScript(REFBEETLESCRIPT))
	{
		return (CMonsterScript*)_Obj->GetScript(REFBEETLESCRIPT);
	}
	if (_Obj->GetScript(DEMONDOGSCRIPT))
	{
		return (CMonsterScript*)_Obj->GetScript(DEMONDOGSCRIPT);
	}
	if (_Obj->GetScript(CHARGESCRIPT))
	{
		return (CMonsterScript*)_Obj->GetScript(CHARGESCRIPT);
	}
	if (_Obj->GetScript(CHALLENGERSCRIPT))
	{
		return (CMonsterScript*)_Obj->GetScript(CHALLENGERSCRIPT);
	}
	if (_Obj->GetScript(CROSSBOWSCRIPT))
	{
		return (CMonsterScript*)_Obj->GetScript(CROSSBOWSCRIPT);
	}
	if (_Obj->GetScript(ICEREAPERSCRIPT))
	{
		return (CMonsterScript*)_Obj->GetScript(ICEREAPERSCRIPT);
	}
	return nullptr;
}

CCardScript* GetCardScript(CGameObject* _Obj)
{
	if (_Obj->GetScript(CARDATTACKSCRIPT))
	{
		return (CCardScript*)_Obj->GetScript(CARDATTACKSCRIPT);
	}
	if (_Obj->GetScript(CARDDEFFENCESCRIPT))
	{
		return (CCardScript*)_Obj->GetScript(CARDDEFFENCESCRIPT);
	}
	if (_Obj->GetScript(CARDMOVESCRIPT))
	{
		return (CCardScript*)_Obj->GetScript(CARDMOVESCRIPT);
	}
	return nullptr;
}

CWaponScript* GetWaponScript(CGameObject* _Obj)
{
	if (_Obj->GetScript(NORMALKATANASCRIPT))
	{
		return (CWaponScript*)_Obj->GetScript(NORMALKATANASCRIPT);
	}
	if (_Obj->GetScript(OLDSWORDSCRIPT))
	{
		return (CWaponScript*)_Obj->GetScript(OLDSWORDSCRIPT);
	}
	if (_Obj->GetScript(OLDSHIELDSCRIPT))
	{
		return (CWaponScript*)_Obj->GetScript(OLDSHIELDSCRIPT);
	}
	return nullptr;
}



CSlotScript* GetSlotScript(CGameObject* _Obj)
{
	if (_Obj->GetScript(LEFTWAPONSLOTSCRIPT))
	{
		return (CSlotScript*)_Obj->GetScript(LEFTWAPONSLOTSCRIPT);
	}
	if (_Obj->GetScript(RIGHTWAPONSLOTSCRIPT))
	{
		return (CSlotScript*)_Obj->GetScript(RIGHTWAPONSLOTSCRIPT);
	}
	if (_Obj->GetScript(BAGSLOT0SCRIPT))
	{
		return (CSlotScript*)_Obj->GetScript(BAGSLOT0SCRIPT);
	}
	if (_Obj->GetScript(BAGSLOT1SCRIPT))
	{
		return (CSlotScript*)_Obj->GetScript(BAGSLOT1SCRIPT);
	}

	return nullptr;
}
