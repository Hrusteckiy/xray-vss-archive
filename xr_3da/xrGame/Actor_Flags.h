#pragma once

#define AF_GODMODE			0x0001
#define AF_INVISIBLE		0x0002
#define AF_ALWAYSRUN		0x0004
#define AF_UNLIMITEDAMMO	0x0008
#define AF_RUN_BACKWARD		0x0010
#define AF_AUTOPICKUP		0x0020

extern Flags32 psActorFlags;

#include "game_base_space.h"
#include "Level.h"
IC BOOL		GodMode	()	
{ 
	if (GameID() == GAME_SINGLE) 
		return psActorFlags.test(AF_GODMODE); 
	return FALSE;	
}

