////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_ALife_Monsters_script2.cpp
//	Created 	: 19.09.2002
//  Modified 	: 04.06.2003
//	Author		: Dmitriy Iassenev
//	Description : Server monsters for ALife simulator, script export, the second part
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xrServer_Objects_ALife_Monsters.h"
#include "script_space.h"
#include "xrServer_script_macroses.h"

using namespace luabind;


void CSE_ALifeMonsterAbstract::script_register(lua_State *L)
{
	module(L)[
		luabind_class_monster2(
			CSE_ALifeMonsterAbstract,
			"cse_alife_monster_abstract",
			CSE_ALifeCreatureAbstract,
			CSE_ALifeSchedulable
		)
	];
}

void CSE_ALifeCreatureActor::script_register(lua_State *L)
{
	module(L)[
		luabind_class_creature3(
			CSE_ALifeCreatureActor,
			"cse_alife_creature_actor",
			CSE_ALifeCreatureAbstract,
			CSE_ALifeTraderAbstract,
			CSE_PHSkeleton
		)
	];
}

void CSE_ALifeTorridZone::script_register(lua_State *L)
{
	module(L)[
		luabind_class_alife2(
			CSE_ALifeTorridZone,
			"cse_torrid_zone",
			CSE_ALifeCustomZone,
			CSE_Motion
			)
	];
}

void CSE_ALifeZoneVisual::script_register(lua_State *L)
{
	module(L)[
		luabind_class_alife2(
			CSE_ALifeZoneVisual,
			"cse_zone_visual",
			CSE_ALifeAnomalousZone,
			CSE_Visual
			)
	];
}

void CSE_ALifeCreaturePhantom::script_register(lua_State *L)
{
	module(L)[
		luabind_class_creature1(
			CSE_ALifeCreaturePhantom,
			"cse_alife_creature_phantom",
			CSE_ALifeCreatureAbstract
			)
	];
}

