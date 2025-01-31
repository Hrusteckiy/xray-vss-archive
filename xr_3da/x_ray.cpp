//-----------------------------------------------------------------------------
// File: x_ray.cpp
//
// Programmers:
//	Oles		- Oles Shishkovtsov
//	AlexMX		- Alexander Maksimchuk
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "igame_level.h"
#include "igame_persistent.h"
#include "SkeletonAnimated.h"

#include "xr_input.h"
#include "xr_ioconsole.h"
#include "x_ray.h"
#include "std_classes.h"
#include "GameFont.h"
#include "resource.h"
#include "LightAnimLibrary.h"
#include "ispatial.h"
#include "CopyProtection.h"
#include "Text_Console.h"

//---------------------------------------------------------------------
BOOL	g_bIntroFinished			= FALSE;
extern	void	Intro				( void* fn );
extern	void	Intro_DSHOW			( void* fn );
extern	int PASCAL IntroDSHOW_wnd	(HINSTANCE hInstC, HINSTANCE hInstP, LPSTR lpCmdLine, int nCmdShow);
//---------------------------------------------------------------------
// 2446363
// umbt@ukr.net
//////////////////////////////////////////////////////////////////////////
struct _SoundProcessor	: public pureFrame
{
	virtual void OnFrame	( )
	{
		//Msg							("------------- sound: %d [%3.2f,%3.2f,%3.2f]",u32(Device.dwFrame),VPUSH(Device.vCameraPosition));
		Device.Statistic->Sound.Begin();
		::Sound->update				(Device.vCameraPosition,Device.vCameraDirection,Device.vCameraTop,Device.fTimeDelta);
		Device.Statistic->Sound.End	();
	}
}	SoundProcessor;

//////////////////////////////////////////////////////////////////////////
// global variables
ENGINE_API	CApplication*	pApp			= NULL;
static		HWND			logoWindow		= NULL;

int				doLauncher					();
void			doBenchmark					();
ENGINE_API	bool			g_bBenchmark	= false;

// -------------------------------------------
// startup point
void InitEngine		()
{
	Engine.Initialize			( );
	while (!g_bIntroFinished)	Sleep	(100);
	Device.Initialize			( );
	CheckCopyProtection			( );
}

void InitSettings	()
{
	string_path					fname; 
	FS.update_path				(fname,"$game_config$","system.ltx");
	pSettings					= xr_new<CInifile>	(fname,TRUE);
}
void InitConsole	()
{
	if (strstr(Core.Params, "-dedicated") && !strstr(Core.Params, "-notextconsole"))
	{
		Console						= xr_new<CTextConsole>	();		
	}
	else
	{
		Console						= xr_new<CConsole>	();
	}
	Console->Initialize			( );
}

void InitInput		()
{
	BOOL bCaptureInput			= !strstr(Core.Params,"-i");

	pInput						= xr_new<CInput>		(bCaptureInput);
}
void destroyInput	()
{
	xr_delete					( pInput		);
}
void InitSound		()
{
	CSound_manager_interface::_create					(u64(Device.m_hWnd));
}
void destroySound	()
{
	CSound_manager_interface::_destroy				( );
}
void destroySettings()
{
	xr_delete					( pSettings		);
}
void destroyConsole	()
{
	Console->Destroy			( );
	xr_delete					(Console);
}
void destroyEngine	()
{
	Device.Destroy				( );
	Engine.Destroy				( );
}

void execUserScript				( )
{
// Execute script
	strcpy						(Console->ConfigFile,"user.ltx");
	if (strstr(Core.Params,"-ltx ")) {
		string64				c_name;
		sscanf					(strstr(Core.Params,"-ltx ")+5,"%[^ ] ",c_name);
		strcpy					(Console->ConfigFile,c_name);
	}
	if (!FS.exist(Console->ConfigFile))
		strcpy					(Console->ConfigFile,"user.ltx");
	Console->ExecuteScript		(Console->ConfigFile);
}
void slowdownthread	( void* )
{
	Sleep	(30*1000);
	for (;;)	{
		if (Device.Statistic->fFPS<30) Sleep(1);
		if (Device.mt_bMustExit)	return;
		if (0==pSettings)			return;
		if (0==Console)				return;
		if (0==pInput)				return;
		if (0==pApp)				return;
	}
}
void CheckPrivilegySlowdown		( )
{
	//u32		crc_dima_ai	= 0x574b9c29;	//shared_str("dima-ai")._get()->dwCRC;	//Msg("%x",crc_dima_ai);	// 0x574b9c29
	//u32		crc_dima	= 0x38e00bc3;	//shared_str("dima")._get()->dwCRC;		//Msg("%x",crc_dima);		// 0x38e00bc3
	//u32		crc_shuttle	= 0xd64cb17b;	//shared_str("shuttle")._get()->dwCRC;	//Msg("%x",crc_shuttle);	// 0xd64cb17b
	//u32		crc_jim		= 0x3d3d5aef;	//shared_str("jim")._get()->dwCRC;		//Msg("%x",crc_jim);		// 0x3d3d5aef
	//u32		crc_london	= 0x09de56e5;	//shared_str("london")._get()->dwCRC;	//Msg("%x",crc_london);		// 0x09de56e5
	//u32		crc_dandy	= 0x430b37e7;	//shared_str("dandy")._get()->dwCRC;	//Msg("%x",crc_dandy);		// 0x430b37e7
#ifdef DEBUG
	BOOL	bDima	=	(shared_str(Core.CompName)._get()->dwCRC == 0x574b9c29) && (shared_str(Core.UserName)._get()->dwCRC==0x38e00bc3) ;
	BOOL	bJim	=	(shared_str(Core.CompName)._get()->dwCRC == 0xd64cb17b) && (shared_str(Core.UserName)._get()->dwCRC==0x3d3d5aef) ;
	BOOL	bDandy	=	(shared_str(Core.CompName)._get()->dwCRC == 0x09de56e5) && (shared_str(Core.UserName)._get()->dwCRC==0x430b37e7) ;
	if	(bDima || bJim || bDandy)	{
		Log			("! slowdown enabled for your pleasure :)");
		thread_spawn(slowdownthread,"slowdown",0,0);
	}
	if	(strstr(Core.Params,"-slowdown"))	{
		thread_spawn(slowdownthread,"slowdown",0,0);
	}
	if	(strstr(Core.Params,"-slowdown2x"))	{
		thread_spawn(slowdownthread,"slowdown",0,0);
		thread_spawn(slowdownthread,"slowdown",0,0);
	}
#endif
}

void Startup					( )
{
	execUserScript	();
	InitInput		();
	InitSound		();

	// ...command line for auto start
	{
		LPCSTR	pStartup			= strstr				(Core.Params,"-start ");
		if (pStartup)				Console->Execute		(pStartup+1);
	}
	{
		LPCSTR	pStartup			= strstr				(Core.Params,"-load ");
		if (pStartup)				Console->Execute		(pStartup+1);
	}

	// Initialize APP
	Device.Create				( );
	LALib.OnCreate				( );
	pApp						= xr_new<CApplication>	();
	g_pGamePersistent			= (IGame_Persistent*)	NEW_INSTANCE (CLSID_GAME_PERSISTANT);
	g_SpatialSpace				= xr_new<ISpatial_DB>	();
	g_SpatialSpacePhysic		= xr_new<ISpatial_DB>	();
	
	// Destroy LOGO
	DestroyWindow				(logoWindow);
	logoWindow					= NULL;

	// Main cycle
	CheckCopyProtection			( );
	Device.Run					( );

	// Destroy APP
	xr_delete					( g_SpatialSpacePhysic	);
	xr_delete					( g_SpatialSpace		);
	DEL_INSTANCE				( g_pGamePersistent		);
	xr_delete					( pApp					);
	Engine.Event.Dump			( );

	// Destroying
	destroySound();
	destroyInput();

	if(!g_bBenchmark)
		destroySettings();

	LALib.OnDestroy				( );
	
	if(!g_bBenchmark)
		destroyConsole();
	else
		Console->Reset();

	destroyEngine();

}

static BOOL CALLBACK logDlgProc( HWND hw, UINT msg, WPARAM wp, LPARAM lp )
{
	switch( msg ){
		case WM_DESTROY:
			break;
		case WM_CLOSE:
			DestroyWindow( hw );
			break;
		case WM_COMMAND:
			if( LOWORD(wp)==IDCANCEL )
				DestroyWindow( hw );
			break;
		default:
			return FALSE;
	}
	return TRUE;
}
/*
void	test_rtc	()
{
	CStatTimer		tMc,tM,tC,tD;
	u32				bytes=0;
	tMc.FrameStart	();
	tM.FrameStart	();
	tC.FrameStart	();
	tD.FrameStart	();
	::Random.seed	(0x12071980);
	for		(u32 test=0; test<10000; test++)
	{
		u32			in_size			= ::Random.randI(1024,256*1024);
		u32			out_size_max	= rtc_csize		(in_size);
		u8*			p_in			= xr_alloc<u8>	(in_size);
		u8*			p_in_tst		= xr_alloc<u8>	(in_size);
		u8*			p_out			= xr_alloc<u8>	(out_size_max);
		for (u32 git=0; git<in_size; git++)			p_in[git] = (u8)::Random.randI	(8);	// garbage
		bytes		+= in_size;

		tMc.Begin	();
		memcpy		(p_in_tst,p_in,in_size);
		tMc.End		();

		tM.Begin	();
		Memory.mem_copy(p_in_tst,p_in,in_size);
		tM.End		();

		tC.Begin	();
		u32			out_size		= rtc_compress	(p_out,out_size_max,p_in,in_size);
		tC.End		();

		tD.Begin	();
		u32			in_size_tst		= rtc_decompress(p_in_tst,in_size,p_out,out_size);
		tD.End		();

		// sanity check
		R_ASSERT	(in_size == in_size_tst);
		for (u32 tit=0; tit<in_size; tit++)			R_ASSERT(p_in[tit] == p_in_tst[tit]);	// garbage

		xr_free		(p_out);
		xr_free		(p_in_tst);
		xr_free		(p_in);
	}
	tMc.FrameEnd	();	float rMc		= 1000.f*(float(bytes)/tMc.result)/(1024.f*1024.f);
	tM.FrameEnd		(); float rM		= 1000.f*(float(bytes)/tM.result)/(1024.f*1024.f);
	tC.FrameEnd		(); float rC		= 1000.f*(float(bytes)/tC.result)/(1024.f*1024.f);
	tD.FrameEnd		(); float rD		= 1000.f*(float(bytes)/tD.result)/(1024.f*1024.f);
	Msg				("* memcpy:        %5.2f M/s (%3.1f%%)",rMc,100.f*rMc/rMc);
	Msg				("* mm-memcpy:     %5.2f M/s (%3.1f%%)",rM,100.f*rM/rMc);
	Msg				("* compression:   %5.2f M/s (%3.1f%%)",rC,100.f*rC/rMc);
	Msg				("* decompression: %5.2f M/s (%3.1f%%)",rD,100.f*rD/rMc);
}
*/
extern void	testbed	(void);

// video
/*
static	HINSTANCE	g_hInstance		;
static	HINSTANCE	g_hPrevInstance	;
static	int			g_nCmdShow		;
void	__cdecl		intro_dshow_x	(void*)
{
	IntroDSHOW_wnd		(g_hInstance,g_hPrevInstance,"GameData\\Stalker_Intro.avi",g_nCmdShow);
	g_bIntroFinished	= TRUE	;
}
*/

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     char *    lpCmdLine,
                     int       nCmdShow)
{
	// Title window
	logoWindow = CreateDialog	(GetModuleHandle(NULL),	MAKEINTRESOURCE(IDD_STARTUP), 0, logDlgProc );

	// AVI
	g_bIntroFinished		= TRUE;
	/*
	if (!IsDebuggerPresent())
	{
		g_hInstance				= hInstance;
		g_hPrevInstance			= hPrevInstance;
		g_nCmdShow				= nCmdShow;
		thread_spawn			(Intro_DSHOW,"intro",0,"GameData\\Stalker_Intro.avi");
		Sleep					(100);
	}
	*/

	// Core
	Core._initialize		("xray",NULL);
	
	FPU::m24r				();

	// auth
	{
		xr_vector<xr_string>	ignore, test	;
		ignore.push_back		(xr_string("user"));
		test.push_back			(xr_string(".ltx"));
		test.push_back			(xr_string(".script"));
		test.push_back			(xr_string(".exe"));
		test.push_back			(xr_string(".dll"));
		FS.auth_generate		(ignore,test)	;
	}
	// auth end 

//#ifdef DEBUG
//	testbed	();
//#endif

	InitEngine				();
	InitSettings			();
	InitConsole				();
	
	if(strstr(lpCmdLine, "-batch_benchmark")){
		doBenchmark();
		return 0;
	}

	if (strstr(lpCmdLine,"-launcher")) 
	{
		int l_res = doLauncher();
		if (l_res != 0)
			return 0;
	};
	Engine.External.Initialize	( );
	CheckPrivilegySlowdown		( );
	Startup	 					( );
	Core._destroy				( );
	return						0;
}

CApplication::CApplication()
{
	ll_dwReference	= 0;

	// events
	eQuit						= Engine.Event.Handler_Attach("KERNEL:quit",this);
	eStart						= Engine.Event.Handler_Attach("KERNEL:start",this);
	eStartLoad					= Engine.Event.Handler_Attach("KERNEL:load",this);
	eDisconnect					= Engine.Event.Handler_Attach("KERNEL:disconnect",this);

	// levels
	Level_Current				= 0;
	Level_Scan					( );

	// Font
	pFontSystem					= xr_new<CGameFont>	("startup_font",CGameFont::fsGradient|CGameFont::fsDeviceIndependent);
	Device.seqRender.Add		( pFontSystem, REG_PRIORITY_LOW-1000 );

	// Register us
	Device.seqFrame.Add			(this, REG_PRIORITY_HIGH+1000);
	if (psDeviceFlags.test(mtSound))	Device.seqFrameMT.Add		(&SoundProcessor);
	else								Device.seqFrame.Add			(&SoundProcessor);
	Console->Show				( );
}

CApplication::~CApplication()
{
	Console->Hide				( );

	Device.seqFrameMT.Remove	(&SoundProcessor);
	Device.seqFrame.Remove		(&SoundProcessor);
	Device.seqFrame.Remove		(this);

	// font
	Device.seqRender.Remove		( pFontSystem		);
	xr_delete					( pFontSystem		);

	// events
	Engine.Event.Handler_Detach	(eDisconnect,this);
	Engine.Event.Handler_Detach	(eStartLoad,this);
	Engine.Event.Handler_Detach	(eStart,this);
	Engine.Event.Handler_Detach	(eQuit,this);
}

void CApplication::OnEvent(EVENT E, u64 P1, u64 P2)
{
	if (E==eQuit)	{
		PostQuitMessage	(0);
		
		for (u32 i=0; i<Levels.size(); i++)
		{
			xr_free(Levels[i].folder	);
			xr_free(Levels[i].name	);
		}
	} else if (E==eStart) {
		Console->Hide();
		LPSTR		op_server		= LPSTR	(P1);
		LPSTR		op_client		= LPSTR	(P2);
		R_ASSERT	(0==g_pGameLevel);
		R_ASSERT	(0!=g_pGamePersistent);
		g_pGamePersistent->Start	(op_server);
		g_pGameLevel= (IGame_Level*)	NEW_INSTANCE(CLSID_GAME_LEVEL);
		BOOL		result			= g_pGameLevel->net_Start(op_server,op_client);
		xr_free		(op_server);
		xr_free		(op_client);
		if (result)	{
			// start any console command
			if (strstr(Core.Params,"-$")) {
				string256				buf,cmd,param;
				sscanf					(strstr(Core.Params,"-$")+2,"%[^ ] %[^ ] ",cmd,param);
				strconcat				(buf,cmd," ",param);
				Console->Execute		(buf);
			}
		} else {
			Msg				("! Failed to start client. Check the connection or level existance.");
			DEL_INSTANCE	(g_pGameLevel);
		}
	} else if (E==eDisconnect) {
		if (g_pGameLevel) {
			Console->Hide			();
			g_pGameLevel->net_Stop	();
			DEL_INSTANCE			(g_pGameLevel);
			Console->Show			();
		}
		R_ASSERT			(0!=g_pGamePersistent);
		g_pGamePersistent->Disconnect();
	}
}

void CApplication::LoadBegin	()
{
	ll_dwReference++;
	if (1==ll_dwReference) {
		::Sound->set_volume (0.f);
		ll_hGeom.create		(FVF::F_TL, RCache.Vertex.Buffer(), RCache.QuadIB);
		ll_hLogo1.create	("font","ui\\ui_logo");
		ll_hLogo2.create	("font","ui\\ui_logo_nv");
		ll_hLogo			= ll_hLogo2;
		CheckCopyProtection	();
	}
}

void CApplication::LoadEnd		()
{
	ll_dwReference--;
	if (0==ll_dwReference)		{
		::Sound->set_volume		(1.f);
	}
}

void CApplication::LoadTitle	(char *S, char *S2)
{
	VERIFY(ll_dwReference);

	Device.dwFrame				+= 1;
	Device.Begin				();

	Log							(S,S2);

	if	(g_pGamePersistent->bDedicatedServer)	{
		Console->OnRender			();
	} else {
		// Draw logo
		u32	Offset;
		u32	C						= 0xffffffff;
		u32	_w						= Device.dwWidth;
		u32	_h						= Device.dwHeight;
		FVF::TL* pv					= (FVF::TL*) RCache.Vertex.Lock(4,ll_hGeom.stride(),Offset);
		pv->set						(EPS_S,				float(_h+EPS_S),	0+EPS_S, 1, C, 0, 1);	pv++;
		pv->set						(EPS_S,				EPS_S,				0+EPS_S, 1, C, 0, 0);	pv++;
		pv->set						(float(_w+EPS_S),	float(_h+EPS_S),	0+EPS_S, 1, C, 1, 1);	pv++;
		pv->set						(float(_w+EPS_S),	EPS_S,				0+EPS_S, 1, C, 1, 0);	pv++;
		RCache.Vertex.Unlock		(4,ll_hGeom.stride());

		RCache.set_Shader			(ll_hLogo);
		RCache.set_Geometry			(ll_hGeom);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);

		// Draw title
		R_ASSERT					(pFontSystem);
		pFontSystem->Clear			();
		pFontSystem->SetColor		(color_rgba(192,192,192,255));
		pFontSystem->SetAligment	(CGameFont::alCenter);
		char *F = "%s";
		if (S2) F="%s%s";
		pFontSystem->OutI			(0.f,0.93f,F,S,S2);
		pFontSystem->OnRender		();
	}

	Device.End					();
	CheckCopyProtection			();
}

void CApplication::LoadSwitch	()
{
	if (ll_hLogo == ll_hLogo1)	ll_hLogo = ll_hLogo2;
	else						ll_hLogo = ll_hLogo1;
}

void CApplication::OnFrame	( )
{
	/*
	CTimer	T;
	T.Start	();
	while	(T.GetElapsed_ms()<10);
	*/
	Engine.Event.OnFrame			();
	g_SpatialSpace->update			();
	g_SpatialSpacePhysic->update	();
	if (g_pGameLevel)				g_pGameLevel->SoundEvent_Dispatch	( );
}

void CApplication::Level_Scan()
{
	xr_vector<char*>*	folder	= FS.file_list_open	("$game_levels$",FS_ListFolders|FS_RootOnly);
	R_ASSERT			(folder&&folder->size());
	for (u32 i=0; i<folder->size(); i++)
	{
		string256	N1,N2,N3,N4;
		strconcat	(N1,(*folder)[i],"level");
		strconcat	(N2,(*folder)[i],"level.ltx");
		strconcat	(N3,(*folder)[i],"level.game");
		strconcat	(N4,(*folder)[i],"level.cform");
		if	(
			FS.exist("$game_levels$",N1)		&&
			FS.exist("$game_levels$",N2)		&&
			FS.exist("$game_levels$",N3)		&&
			FS.exist("$game_levels$",N4)	
			)
		{
			sLevelInfo			LI;
			LI.folder			= xr_strdup((*folder)[i]);
			LI.name				= 0;
			Levels.push_back	(LI);
		} else {
			// Msg		("! Level not compiled: %s",(*folder)[i]);
		}
	}
	FS.file_list_close	(folder);
}

void CApplication::Level_Set(u32 L)
{
	if (L>=Levels.size())	return;
	Level_Current = L;
	FS.get_path	("$level$")->_set	(Levels[L].folder);
	CheckCopyProtection		();
}

int CApplication::Level_ID(LPCSTR name)
{
	char buffer	[256];
	strconcat	(buffer,name,"\\");
	for (u32 I=0; I<Levels.size(); I++)
	{
		if (0==_stricmp(buffer,Levels[I].folder))	return int(I);
	}
	return -1;
}


//launcher stuff----------------------------
extern "C"{
	typedef int	 __cdecl LauncherFunc	(int);
}
HMODULE			hLauncher		= NULL;
LauncherFunc*	pLauncher		= NULL;

void InitLauncher(){
	if(hLauncher)
		return;
	hLauncher	= LoadLibrary	("xrLauncher.dll");
	if (0==hLauncher)	R_CHK			(GetLastError());
	R_ASSERT2		(hLauncher,"xrLauncher DLL raised exception during loading or there is no xrLauncher.dll at all");

	pLauncher = (LauncherFunc*)GetProcAddress(hLauncher,"RunXRLauncher");
	R_ASSERT2		(pLauncher,"Cannot obtain RunXRLauncher function from xrLauncher.dll");
};

void FreeLauncher(){
	if (hLauncher)	{ 
		FreeLibrary(hLauncher); 
		hLauncher = NULL; pLauncher = NULL; };
}

int doLauncher()
{
	execUserScript();
	InitLauncher();
	int res = pLauncher(0);
	FreeLauncher();
	if(res == 1) // do benchmark
		g_bBenchmark = true;

	if(g_bBenchmark){ //perform benchmark cycle
		doBenchmark();
	
		// InitLauncher	();
		// pLauncher	(2);	//show results
		// FreeLauncher	();

		Core._destroy			();
		return					(1);

	};
	if(res==8){//Quit
		Core._destroy			();
		return					(1);
	}
	return 0;

}

void doBenchmark()
{
	g_bBenchmark = true;
	string_path in_file;
	FS.update_path(in_file,"$server_root$","tmp_benchmark.ini");
	CInifile ini(in_file);
	int test_count = ini.line_count("benchmark");
	LPCSTR test_name,t;
	shared_str test_command;
	for(int i=0;i<test_count;++i){
		ini.r_line( "benchmark", i, &test_name, &t);
		
		test_command = ini.r_string_wb("benchmark",test_name);
		_strlwr				(strcpy(Core.Params,*test_command));
		
		if(i){
			ZeroMemory(&HW,sizeof(CHW));
			InitEngine();
		}

		Engine.External.Initialize	( );
		execUserScript();
		Startup	 				();
	}
}
