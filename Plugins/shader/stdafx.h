// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// Third generation by Oles.

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once
#define WIN32_LEAN_AND_MEAN			// Exclude rarely-used stuff from Windows headers
#define STRICT						// Enable strict syntax
#define IDIRECTPLAY2_OR_GREATER

#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOCLIPBOARD
#define NODRAWTEXT
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOSERVICE
#define NOCOMM
#define NOHELP
#define NOPROFILER
#define NOMCX

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>
#include <tchar.h>

#define DIRECTSOUND_VERSION 0x0700
#define DIRECTINPUT_VERSION 0x0700

#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>
#include <dsound.h>
#include <dplobby.h>
#include <dplay.h>

#include <malloc.h>
#ifdef DEBUG
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <process.h>

/*
#ifndef DEBUG
#pragma inline_depth	( 254 )
#pragma inline_recursion( on )
#pragma intrinsic		(abs, fabs, fmod, sin, cos, tan, asin, acos, atan, sqrt, exp, log, log10, strcpy, strcat)
#define inline			__forceinline
#define _inline			__forceinline
#define __inline		__forceinline
#define IC				__forceinline
#else
#define IC				__forceinline
#endif
*/
#define IC				__forceinline

// Warnings
#pragma warning (disable : 4786 )		// too long names
#pragma warning (disable : 4503 )		// decorated name length exceeded
#pragma warning (disable : 4251 )		// object needs DLL interface

// stl
#include <algorithm>
#include <numeric>
#include <limits>
#include <vector>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <string>
using namespace std;

// Property-set support
#define PropertyG(a)	__declspec( property( get=a ) )
#define PropertyP(a)	__declspec( property( put=a ) )
#define PropertyGP(a,b)	__declspec( property( get=a, put=b ) )

// CLASS ID type
typedef unsigned __int64	CLASS_ID;
#define MK_CLSID(a,b,c,d,e,f,g,h) \
	CLASS_ID(	((CLASS_ID(a)<<CLASS_ID(24))|(CLASS_ID(b)<<CLASS_ID(16))|(CLASS_ID(c)<<CLASS_ID(8))|(CLASS_ID(d)))<<CLASS_ID(32) | \
				((CLASS_ID(e)<<CLASS_ID(24))|(CLASS_ID(f)<<CLASS_ID(16))|(CLASS_ID(g)<<CLASS_ID(8))|(CLASS_ID(h))) )

// Our headers
#define NO_XR_NETWORK
#define	NO_XR_VERTEX

#define ENGINE_API 

#ifdef DEBUG
#define R_ASSERT	_ASSERT
#define VERIFY		_ASSERT
#else
#define R_ASSERT
#define VERIFY  
#endif
#define NODEFAULT 

//#include "debugkernel.h"
#ifndef NO_MMGR
//#include "fmemorymanager.h"
#endif
//#include "defines.h"
//#include "pure.h"
#include "math.h"
#include "vector.h"
#ifndef NO_XRLOG
//#include "log.h"
#endif
//#include "device.h"
#include "ffileops.h"
// TODO: reference additional headers your program requires here

#pragma comment( lib, "imagehlp.lib"	)
#pragma comment( lib, "winmm.lib"		)

#pragma comment( lib, "d3d8.lib"		)
#pragma comment( lib, "d3dx8.lib"		)
#pragma comment( lib, "dxerr8.lib"		)

#pragma comment( lib, "dsound.lib"		)
#pragma comment( lib, "dinput.lib"		)
#pragma comment( lib, "dxguid.lib"		)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
