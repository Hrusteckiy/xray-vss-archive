#include "stdafx.h"
#include "dxerr.h"

// *****************************************************************************************
// Error handling

//----------------------------- FLAGS
static struct _DF {
	char *	name;
	u32	mask;
} DF[] = {
	{"rsFullscreen",	rsFullscreen	},
	{"rsClearBB",		rsClearBB 		},
	{"rsNoVSync",		rsNoVSync 		},
	{"rsWireframe",		rsWireframe		},
    {NULL,0}
};

void CRenderDevice::DumpFlags()
{
	Log("- Dumping device flags");
	_DF *p = DF;
	while (p->name) {
		Msg("* %20s %s",p->name,psDeviceFlags.test(p->mask)?"on":"off");
		p++;
	}
}
