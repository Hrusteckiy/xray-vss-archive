//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "GameMtlLib.h"

void DestroySounds(SoundSVec4& lst)
{
	for (SoundS4It it=lst.begin(); it!=lst.end(); it++)	it->destroy();
}

void DestroyMarks(ShaderSVec4& lst)
{
	for (ShaderS4It it=lst.begin(); it!=lst.end(); it++)
		it->destroy();
}

void DestroyPSs(PSSVec4& lst)
{
//	for (ShaderS4It it=lst.begin(); it!=lst.end(); it++)
//		Device.Shader.Delete(*it);
}

void CreateSounds(SoundSVec4& lst, LPCSTR buf)
{
	string128 tmp;
	int cnt=_GetItemCount(buf);	R_ASSERT(cnt<=GAMEMTL_SUBITEM_COUNT);
	for (int k=0; k<cnt; k++){
		lst.push_back		(ref_sound());
		lst.back().create	(TRUE,	_GetItem(buf,k,tmp));
	}
}

void CreateMarks(ShaderSVec4& lst, LPCSTR buf)
{
	string128	tmp;
	int cnt		=_GetItemCount(buf);	R_ASSERT(cnt<=GAMEMTL_SUBITEM_COUNT);
	ref_shader	s;
	for (int k=0; k<cnt; k++)
	{
		s.create		("effects\\wallmark",_GetItem(buf,k,tmp));
		lst.push_back	(s);
	}
}

void CreatePSs(PSSVec4& lst, LPCSTR buf)
{
	//	string128 tmp;
	int cnt=_GetItemCount(buf);	R_ASSERT(cnt<=GAMEMTL_SUBITEM_COUNT);
	for (int k=0; k<cnt; k++)
		lst.push_back	(0);
}

SGameMtlPair::~SGameMtlPair()
{
	// destroy all media
	DestroySounds	(BreakingSounds);
	DestroySounds	(StepSounds);
	DestroySounds	(CollideSounds);
	DestroySounds	(HitSounds);
	DestroyPSs		(HitParticles);
	DestroyMarks	(HitMarks);
}

void SGameMtlPair::Load(IReader& fs)
{
	string128			buf;

	R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_PAIR));
    mtl0				= fs.r_u32();
    mtl1				= fs.r_u32();
    ID					= fs.r_u32();
    ID_parent			= fs.r_u32();
    OwnProps.set		(fs.r_u32());
 
    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_FLOTATION));
    fFlotation			= fs.r_float();

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_BREAKING));
    fs.r_stringZ			(buf); 		CreateSounds		(BreakingSounds,buf);
    
    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_STEP));
    fs.r_stringZ			(buf);		CreateSounds		(StepSounds,buf);
    
    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_COLLIDE));
    fs.r_stringZ			(buf);		CreateSounds		(CollideSounds,buf);
    
    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_HIT));
    fs.r_stringZ			(buf);		CreateSounds		(HitSounds,buf);
    fs.r_stringZ			(buf);		CreatePSs			(HitParticles,buf);
    fs.r_stringZ			(buf);		CreateMarks			(HitMarks,buf);
}
