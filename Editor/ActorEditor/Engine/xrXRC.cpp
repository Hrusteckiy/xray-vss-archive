// xrXRC.cpp: implementation of the xrXRC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xrXRC.h"

ENGINE_API xrXRC XRC;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

xrXRC::xrXRC()
{
	CL = (CDB::COLLIDER*)cdb_collider_create();
}

xrXRC::~xrXRC()
{
	cdb_collider_destroy(CL);
}
