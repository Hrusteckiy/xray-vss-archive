// Blender_Vertex.cpp: implementation of the CBlender_Vertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#include "Blender_Vertex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlender_Vertex::CBlender_Vertex()
{
	description.CLS		= B_VERT;
}

CBlender_Vertex::~CBlender_Vertex()
{
	
}

void	CBlender_Vertex::Save	( IWriter& fs	)
{
	IBlender::Save	(fs);
}

void	CBlender_Vertex::Load	( IReader& fs, u16 version	)
{
	IBlender::Load	(fs,version);
}

void CBlender_Vertex::Compile	(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	if (C.bEditor)
	{
		// Editor shader
		C.PassBegin		();
		{
			C.PassSET_ZB			(TRUE,TRUE);
			C.PassSET_Blend			(FALSE,D3DBLEND_ONE,D3DBLEND_ZERO,	FALSE,0);
			C.PassSET_LightFog		(TRUE,TRUE);
			
			// Stage0 - Base texture
			C.StageBegin			();
			C.StageSET_Color		(D3DTA_TEXTURE,	  D3DTOP_MODULATE,	D3DTA_DIFFUSE);
			C.StageSET_Alpha		(D3DTA_TEXTURE,	  D3DTOP_MODULATE,	D3DTA_DIFFUSE);
			C.Stage_Texture			(oT_Name);
			C.Stage_Matrix			(oT_xform,0);
			C.Stage_Constant		("$null");
			C.StageEnd				();
		}
		C.PassEnd			();
	} else {
		switch (C.iElement)
		{
		case SE_R1_NORMAL_HQ:
		case SE_R1_NORMAL_LQ:
			// Level view
			if (C.bDetail)
			{
				C.r_Pass	("vert_dt","vert_dt",TRUE);
				C.r_Sampler	("s_base",	C.L_textures[0]);
				C.r_Sampler	("s_detail",C.detail_texture);
				C.r_End		();
			} else	{
				C.r_Pass	("vert","vert",TRUE);
				C.r_Sampler	("s_base",C.L_textures[0]);
				C.r_End		();
			}
			break;
		case SE_R1_LPOINT:
			break;
		case SE_R1_LSPOT:
			C.r_Pass		("vert_spot","add_spot",FALSE,TRUE,FALSE,TRUE,D3DBLEND_ONE,D3DBLEND_ONE,TRUE);
			C.r_Sampler		("s_base",	C.L_textures[0]);
			C.r_Sampler_clf	("s_lmap",	"internal\\internal_light_att",		true);
			C.r_Sampler_clf	("s_att",	"internal\\internal_light_attclip"		);
			C.r_End			();
			break;
		case SE_R1_LMODELS:
			// Lighting only
			C.r_Pass		("vert_l","vert_l",FALSE);
			C.r_Sampler		("s_base",C.L_textures[0]);
			C.r_End			();
			break;
		}
	}
}
