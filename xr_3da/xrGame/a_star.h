////////////////////////////////////////////////////////////////////////////
//	Module 		: a_star.h
//	Created 	: 21.03.2002
//  Modified 	: 26.12.2002
//	Author		: Dmitriy Iassenev
//	Description : A* class
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "a_star_template.h"
#include "a_star_interfaces.h"

class CAStar {
private:
	SAIMapData												m_tAIGraphData;
	SAIMapData												m_tAIMapData;
	SAIMapDataL												m_tAIMapDataL;
	SAIMapDataE												m_tAIMapDataE;
	SAIMapDataF												m_tAIMapDataF;
public:
	
	SNode													*m_tpHeap;
	SIndexNode												*m_tpIndexes;
	u32														m_dwAStarStaticCounter;
	AI::DWORD_VECTOR										m_tpaNodes;

	CAStarSearch<CAIMapShortestPathNode,SAIMapData>			*m_tpMapPath;
	CAStarSearch<CAIMapLCDPathNode,SAIMapDataL>				*m_tpLCDPath;
	CAStarSearch<CAIMapEnemyPathNode,SAIMapDataE>			*m_tpEnemyPath;
	CAStarSearch<CAIMapEnemyPositionPathNode,SAIMapDataF>   *m_tpEnemyPositionPath;
	CAStarSearch<CAIGraphShortestPathNode,SAIMapData>       *m_tpGraphPath;
					CAStar(u32 dwMaxNodes);
	virtual			~CAStar();
			float	ffFindMinimalPath(u32 dwStartNode, u32 dwGoalNode);
			float	ffFindMinimalPath(u32 dwStartNode, u32 dwGoalNode, AI::DWORD_VECTOR &tpaNodes);
			float	ffFindMinimalPath(u32 dwStartNode, u32 dwGoalNode, AI::NodePath& Result, bool bUseMarks = false);
			float	ffFindOptimalPath(u32 dwStartNode, u32 dwGoalNode, AI::NodePath& Result, float fLightWeight = DEFAULT_LIGHT_WEIGHT, float fCoverWeight = DEFAULT_COVER_WEIGHT, float fDistanceWeight = DEFAULT_DISTANCE_WEIGHT, bool bUseMarks = false);
			float	ffFindOptimalPath(u32 dwStartNode, u32 dwGoalNode, AI::NodePath& Result, u32 dwEnemyNode, float fOptimalEnemyDistance, float fLightWeight = DEFAULT_LIGHT_WEIGHT, float fCoverWeight = DEFAULT_COVER_WEIGHT, float fDistanceWeight = DEFAULT_DISTANCE_WEIGHT, float fEnemyViewWeight = DEFAULT_ENEMY_VIEW_WEIGHT, bool bUseMarks = false);
			float	ffFindOptimalPath(u32 dwStartNode, u32 dwGoalNode, AI::NodePath& Result, Fvector tEnemyPosition, float fOptimalEnemyDistance, float fLightWeight, float fCoverWeight, float fDistanceWeight, float fEnemyViewWeight, bool bUseMarks = false);
};
