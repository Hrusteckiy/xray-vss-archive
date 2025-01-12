//////////////////////////////////////////////////////////////////////////////////
//	Module			:	Mobility routines										//
//	File			:	mobil.h													//
//	Creation date	:	19.12.2001												//
//	Author			:	Dmitriy Iassenev										//
//////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////
// Used modules
//////////////////////////////////////////////////////////

#include "board.h"

//////////////////////////////////////////////////////////
// Macros declarations
//////////////////////////////////////////////////////////

#define MP_COUNT	48

//////////////////////////////////////////////////////////
// Type declarations
//////////////////////////////////////////////////////////

typedef struct tagTMPStructure {
	u16	*wpMPIndex;
	u16 wMPIndexChange;
} TMPStructure;

typedef struct tagTMPInfo {
	u8				ucCount;
	TMPStructure	taPatt[8];
} TMPInfo;

//////////////////////////////////////////////////////////
// Exported variables
//////////////////////////////////////////////////////////

extern ALIGN(16)	u16	waMPIndexes				[MP_COUNT];
extern				TMPInfo	taMPStructures		[BOARD_SIZE];
extern u8			ucaBlackAccessabilityMasks	[BOARD_SIZE];
extern u8			ucaWhiteAccessabilityMasks	[BOARD_SIZE];

//////////////////////////////////////////////////////////
// Exported functions
//////////////////////////////////////////////////////////

extern void vfLoadMobilityPatterns();
extern void vfInitMPIndexes();
extern void vfInitAccessabilityMasks();
