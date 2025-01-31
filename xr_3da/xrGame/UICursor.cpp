// exxZERO Time Stamp AddIn. Document modified at : Thursday, March 07, 2002 14:11:58 , by user : Oles , from computer : OLES
#include "stdafx.h"
#include "uicursor.h"

#include "../CustomHUD.h"
#include "HUDManager.h"
#include "UI.h"

#define C_DEFAULT	D3DCOLOR_XRGB(0xff,0xff,0xff)

#define SENSITIVITY_DEFAULT 1.5f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUICursor::CUICursor()
{    
	m_fSensitivity	= SENSITIVITY_DEFAULT;
	bHoldMode		= false;
	bVisible		= false;
	vPos.set		(0.f,0.f);
	vHoldPos.set	(0,0);
	vDelta.set		(0,0);

	hShader.create	("hud\\cursor","ui\\ui_ani_cursor");
	m_si.SetShader(hShader);
}
//--------------------------------------------------------------------
CUICursor::~CUICursor	()
{
}
//--------------------------------------------------------------------
u32 last_render_frame = 0;
void CUICursor::Render	()
{
#ifdef DEBUG
	VERIFY(last_render_frame != Device.dwFrame);
	last_render_frame = Device.dwFrame;

	if(bDebug){
	CGameFont* F		= UI()->Font()->pFontDI;
	F->SetAligment		(CGameFont::alCenter);
	F->SetSizeI			(0.02f);
	F->OutSetI			(0.f,-0.9f);
	F->SetColor			(0xffffffff);
	Ivector2			pt = GetPos();
	F->OutNext			("%d-%d",pt.x, pt.y);

	if(bHoldMode)
		F->OutNext		("Hold Mode");
	};
#endif
	if(bHoldMode) return;
	// Convert to screen coords
	int cx	= int((vPos.x+1)/2*UI_BASE_WIDTH);
	int cy	= int((vPos.y+1)/2*UI_BASE_HEIGHT);

	m_si.SetPos(cx, cy);
	m_si.Render();
}
//--------------------------------------------------------------------

//move cursor to screen coordinates
void CUICursor::SetPos(int x, int y)
{
	vDelta.set		(0,0);
	vPos.x =(float)2*x/(float)UI_BASE_WIDTH - 1.0f;
	vPos.y =(float)2*y/(float)UI_BASE_HEIGHT - 1.0f;

	if(vPos.x<-1) vPos.x=-1;
	if(vPos.x>1) vPos.x=1;
	if(vPos.y<-1) vPos.y=-1;
	if(vPos.y>1) vPos.y=1;
}

void CUICursor::GetPos(int& x, int& y)
{
	x = (int)((vPos.x+1.0f)*(float)UI_BASE_WIDTH/2);
	y = (int)((vPos.y+1.0f)*(float)UI_BASE_HEIGHT/2);

}

Ivector2 CUICursor::GetPos()
{
	Ivector2 pt;

	int x, y;
	GetPos(x, y);
	pt.x = x;
	pt.y = y;

	return  pt;
}

Ivector2 CUICursor::GetPosDelta()
{
	Ivector2 res;
	res.x = iFloor( (vDelta.x/2.0f)*(float)UI_BASE_WIDTH );
	res.y = iFloor( (vDelta.y/2.0f)*(float)UI_BASE_HEIGHT );
	return res;
}

void CUICursor::HoldMode		(bool b)
{
	if(bHoldMode==b) return;
	if(b){
		vHoldPos = vPos;
	}else{
		vPos = vHoldPos;
	}
	bHoldMode = b;
}

void CUICursor::MoveBy(int dx, int dy)
{
	vDelta.x = (float)m_fSensitivity*dx/(float)UI_BASE_WIDTH * UI()->GetScaleX();
	vDelta.y = (float)m_fSensitivity*dy/(float)UI_BASE_HEIGHT * UI()->GetScaleY();

	if(!bHoldMode){
		vPos.x += vDelta.x;
		vPos.y += vDelta.y;
	}
	clamp(vPos.x, -1.f, 1.f);
	clamp(vPos.y, -1.f, 1.f);
}
