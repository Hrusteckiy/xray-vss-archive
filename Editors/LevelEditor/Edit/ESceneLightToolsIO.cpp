#include "stdafx.h"
#pragma hdrstop

#include "ESceneLightTools.h"
#include "elight.h"
#include "scene.h"
#include "ui_main.h"

// chunks
static const u16 LIGHT_TOOLS_VERSION  	= 0x0000;
//----------------------------------------------------
enum{
    CHUNK_VERSION			= 0x1001ul,
    CHUNK_LCONTROLS			= 0x1002ul,
    CHUNK_LCONTROLS_LAST	= 0x1003ul,
    CHUNK_FLAGS				= 0x1004ul,
    CHUNK_HEMI				= 0x1005ul,
    CHUNK_SUN_SHADOW		= 0x1006ul,
    CHUNK_HEMI2				= 0x1007ul,
};
//----------------------------------------------------

bool ESceneLightTools::Load(IReader& F)
{
	u16 version 	= 0;
    if(F.r_chunk(CHUNK_VERSION,&version))
        if( version!=LIGHT_TOOLS_VERSION ){
            ELog.DlgMsg( mtError, "%s tools: Unsupported version.",ClassDesc());
            return false;
        }

	if (!inherited::Load(F)) return false;

    if (F.find_chunk(CHUNK_FLAGS))
    	m_Flags.set	(F.r_u32());

    if (F.find_chunk(CHUNK_HEMI)){
     	m_HemiQuality		= F.r_u8();
    }
    if (F.find_chunk(CHUNK_HEMI2)){
		m_HemiControl		= F.r_u32();
    }

    if (F.find_chunk(CHUNK_SUN_SHADOW)){
     	m_SunShadowQuality	= F.r_u8();
        F.r_fvector2		(m_SunShadowDir);
    }
    
    if (F.find_chunk(CHUNK_LCONTROLS_LAST))
		lcontrol_last_idx	= F.r_u32();
    
	IReader* R 		= F.open_chunk(CHUNK_LCONTROLS); 
    if (R){
        while (!R->eof()){
            AnsiString	l_name;
            R->r_stringZ(l_name);
            u32 l_idx	= R->r_u32();
            AppendLightControl(l_name.c_str(),&l_idx);
        }
        R->close		();
    }

    return true;
}
//----------------------------------------------------

void ESceneLightTools::Save(IWriter& F)
{
	inherited::Save	(F);

	F.w_chunk		(CHUNK_VERSION,(u16*)&LIGHT_TOOLS_VERSION,sizeof(LIGHT_TOOLS_VERSION));

	F.open_chunk	(CHUNK_FLAGS);
    F.w_u32			(m_Flags.get());
	F.close_chunk	();

	F.open_chunk	(CHUNK_HEMI);
    F.w_u8			(m_HemiQuality);
    F.close_chunk	();

	F.open_chunk	(CHUNK_HEMI2);
	F.w_u32			(m_HemiControl);
    F.close_chunk	();

	F.open_chunk	(CHUNK_SUN_SHADOW);
    F.w_u8			(m_SunShadowQuality);
    F.w_fvector2	(m_SunShadowDir);
    F.close_chunk	();

	F.open_chunk	(CHUNK_LCONTROLS_LAST);
	F.w_u32			(lcontrol_last_idx);
    F.close_chunk	();

	F.open_chunk	(CHUNK_LCONTROLS);
	ATokenIt		_I 	= lcontrols.begin();
    ATokenIt		_E 	= lcontrols.end();
    for (;_I!=_E; _I++){
        F.w_stringZ	(_I->name.c_str());
        F.w_u32		(_I->id);
    }
    F.close_chunk	();
}
//----------------------------------------------------
 
bool ESceneLightTools::LoadSelection(IReader& F)
{
	u16 version 	= 0;
    R_ASSERT(F.r_chunk(CHUNK_VERSION,&version));
    if( version!=LIGHT_TOOLS_VERSION ){
        ELog.DlgMsg( mtError, "%s tools: Unsupported version.",ClassDesc());
        return false;
    }

	return inherited::LoadSelection(F);
}
//----------------------------------------------------

void ESceneLightTools::SaveSelection(IWriter& F)
{
	F.w_chunk		(CHUNK_VERSION,(u16*)&LIGHT_TOOLS_VERSION,sizeof(LIGHT_TOOLS_VERSION));
    
	inherited::SaveSelection(F);
}
//----------------------------------------------------


