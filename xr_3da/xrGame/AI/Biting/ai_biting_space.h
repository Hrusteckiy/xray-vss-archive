////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_biting_space.h
//	Created 	: 26.05.2003
//  Modified 	: 26.05.2003
//	Author		: Dmitriy Iassenev
//	Description : Biting types and structures
////////////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_DYNAMIC_OBJECTS 			32
#define MAX_DYNAMIC_SOUNDS  			32
#define MAX_HURT_COUNT					32

#define MAX_TIME_RANGE_SEARCH			5000.f

#define R2D(x)							(angle_normalize(x)*180.f/PI)

#define SILENCE

#ifndef SILENCE
	#define OUT_TEXT(S)						if (g_Alive()) Msg(S);
#else
	#define OUT_TEXT(S)						;
#endif

namespace AI_Biting {
	enum EBodyState {
		eBodyStateLie = 0,
		eBodyStateCrouch,
		eBodyStateStand,
	};

	enum EMovementType {
		eMovementTypeWalk = 0,
		eMovementTypeRun,
		eMovementTypeStand,
	};

	enum EStateType {
		eStateTypeDanger = 0,
		eStateTypeNormal,
		eStateTypePanic,
	};

	enum EMovementDirection {
		eMovementDirectionForward = 0,
		eMovementDirectionBack,
		eMovementDirectionLeft,
		eMovementDirectionRight,
	};

	enum EDirectionType {
		eDirectionTypeForward = 0,
		eDirectionTypeForwardDodge,
		eDirectionTypeForwardCover,
		eDirectionTypeBack,
		eDirectionTypeBackDodge,
		eDirectionTypeBackCover,
	};

	enum EPathState {
		ePathStateSearchNode = 0,
		ePathStateBuildNodePath,
		ePathStateBuildTravelLine,
	};

	enum EPathType {
		ePathTypeStraight = 0,
		ePathTypeCriteria,
		ePathTypeStraightCriteria,
	};

	enum EActionState {
		eActionStateRun = 0,
		eActionStateStand,
		eActionStateDontWatch,
		eActionStateWatch,
		eActionStateWatchGo,
		eActionStateWatchLook,
	};

	enum EPostureAnim {
		ePostureStand = 0,
		ePostureSit,
		ePostureLie
	};

	enum EActionAnim {
		eActionIdle = 0,
		eActionWalkFwd,
		eActionWalkBkwd,
		eActionTurnLeft,
		eActionTurnRight
	};

	enum EMotionType {
		eNothing = 0,
		
		eStandUp,
		eSitDown,
		eLieDown,
		eMoveFwd,
		eMoveBkwd,
		eTurnLeft,
		eTurnRight,	
		eRun
	};

};

