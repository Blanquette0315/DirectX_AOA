#pragma once

#include <Engine/CGameObject.h>
#include "CMonsterScript.h"
#include "CFastScript.h"
#include "CRefBeetleScript.h"
CMonsterScript* GetMonsterScript(CGameObject* _Obj);

#include "CCardScript.h"
CCardScript* GetCardScript(CGameObject* _Obj);

#include "CWaponScript.h"
CWaponScript* GetWaponScript(CGameObject* _Obj);

#include "CSlotScript.h"
CSlotScript* GetSlotScript(CGameObject* _Obj);