///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains compilation flags.
 *	\file		OPC_Settings.h
 *	\author		Pierre Terdiman
 *	\date		May, 12, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_SETTINGS_H__
#define __OPC_SETTINGS_H__

	//! Use CPU comparisons (comment that line to use standard FPU compares)
	#define OPC_CPU_COMPARE

	//! Use FCOMI / FCMOV on Pentium-Pro based processors (comment that line to use plain C++)
	#define OPC_USE_FCOMI

	//! Use epsilon value in tri-tri overlap test
	#define OPC_TRITRI_EPSILON_TEST

	//! Use epsilon value in ray-AABB overlap test
//	#define OPC_RAYAABB_EPSILON 0.00001f

	//! Use tree-coherence or not
	#define OPC_USE_TREE_COHERENCE

#endif //__OPC_SETTINGS_H__