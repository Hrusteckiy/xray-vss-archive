// exxZERO Time Stamp AddIn. Document modified at : Thursday, March 07, 2002 14:13:00 , by user : Oles , from computer : OLES
// HUDCursor.cpp: implementation of the CHUDCursor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hudcursor.h"
#include "hudmanager.h"

#include "..\CustomHUD.h"
#include "Entity.h"

#define C_ON_ENEMY	D3DCOLOR_XRGB(0xff,0,0)
#define C_DEFAULT	D3DCOLOR_XRGB(0xff,0xff,0xff)
#define C_SIZE		0.025f
#define NEAR_LIM	0.5f


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHUDCursor::CHUDCursor()
{    
	hShader			= 0;
	Stream			= 0;
	OnDeviceCreate	();
	
	Device.seqDevCreate.Add		(this);
	Device.seqDevDestroy.Add	(this);
}

CHUDCursor::~CHUDCursor()
{
	Device.seqDevCreate.Remove	(this);
	Device.seqDevDestroy.Remove	(this);
	
	Stream			= 0;
	OnDeviceDestroy	();
}

void CHUDCursor::OnDeviceDestroy()
{
	Log			("- CHUDCursor");
	Device.Shader.Delete(hShader);
}
void CHUDCursor::OnDeviceCreate()
{
	REQ_CREATE	();
	Stream		= Device.Streams.Create	(FVF::F_TL,4);
	Log			("+ CHUDCursor");
	hShader		= Device.Shader.Create	("hud\\cursor","ui\\cursor",FALSE);
}

void CHUDCursor::Render()
{
	Fvector		p1,p2,dir;

	int			cur_team = (dynamic_cast<CEntity*>(pCreator->CurrentEntity()))->g_Team();

	p1	= Device.vCameraPosition;
	dir = Device.vCameraDirection;
	
	// Render cursor
	float		dist=Device.fFOV;
	
	pCreator->CurrentEntity()->setEnabled(false);
	DWORD C			= C_DEFAULT;
	Collide::ray_query	RQ;

	if (pCreator->ObjectSpace.RayPick( p1, dir, dist, RQ )){
		// if (RQ.O && (RQ.O->CLS_ID == CLSID_ENTITY) && (((CEntity*)RQ.O)->id_Team!=cur_team)) C = C_ON_ENEMY;
		dist = RQ.range;
	}
	pCreator->CurrentEntity()->setEnabled(true);
	
	if ( dist<NEAR_LIM) dist=NEAR_LIM;
	
	FVF::TL			PT;
	p2.mad			(p1,dir,dist);
	PT.transform	(p2,Device.mFullTransform);
	float			di_size = C_SIZE/powf(PT.p.w,.2f);

	if (psHUD_Flags&HUD_CROSSHAIR_DIST){
		pApp->pFont->Color	(C);
		pApp->pFont->Size	(di_size*1.5f);
		pApp->pFont->Out(PT.p.x,PT.p.y+di_size*2,"~%3.1f",dist);
	}
	if (RQ.O && (psHUD_Flags&HUD_INFO))
	{ 
		CFontSmall* F = Level().HUD()->pSmallFont;
		F->Color	(C);
		F->Size		(0.02f);
		F->Out		(PT.p.x,PT.p.y+di_size*4,"~%s",RQ.O->cName());
	}
	// actual rendering
	DWORD			vOffset;
	FVF::TL*	pv	= (FVF::TL*)Stream->Lock(4,vOffset);
	float			size= Device.dwWidth * di_size;
	
	// Convert to screen coords
	float cx        = Device._x2real(PT.p.x);
	float cy        = Device._y2real(PT.p.y);
	
	pv->set(cx - size, cy + size, PT.p.z, PT.p.w, C, 0, 1); pv++;
	pv->set(cx - size, cy - size, PT.p.z, PT.p.w, C, 0, 0); pv++;
	pv->set(cx + size, cy + size, PT.p.z, PT.p.w, C, 1, 1); pv++;
	pv->set(cx + size, cy - size, PT.p.z, PT.p.w, C, 1, 0); pv++;
	
	// unlock VB and Render it as triangle list
	Stream->Unlock			(4);
	Device.Shader.set_Shader(hShader);
	Device.Primitive.Draw	(Stream,4,2,vOffset,Device.Streams_QuadIB);
	// Device.Primitive.dbg_DrawLINE(precalc_identity,p1,p2,C);
}
