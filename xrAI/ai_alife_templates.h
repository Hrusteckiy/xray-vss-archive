////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_alife_templates.h
//	Created 	: 21.01.2003
//  Modified 	: 21.01.2003
//	Author		: Dmitriy Iassenev
//	Description : A-Life templates
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_AI_ALIFE_TEMPLATES
#define XRAY_AI_ALIFE_TEMPLATES

#include "ai_alife_space.h"
using namespace ALife;

template <class T>
void free_vector(xr_vector<T *> &tpVector)
{
	xr_vector<T *>::iterator		I = tpVector.begin();
	xr_vector<T *>::iterator		E = tpVector.end();
	for ( ; I != E; I++)
		xr_delete				(*I);
};

template <class T1, class T2>
void free_map(xr_map<T1,T2 *> &tpMap)
{
	xr_map<T1,T2 *>::iterator		I = tpMap.begin();
	xr_map<T1,T2 *>::iterator		E = tpMap.end();
	for ( ; I != E; I++)
		xr_delete					((*I).second);
};

// server objects
IC void save_bool_vector(xr_vector<bool> &baVector, NET_Packet &tNetPacket)
{
	tNetPacket.w_u32			(baVector.size());
	xr_vector<bool>::iterator 		I = baVector.begin();
	xr_vector<bool>::iterator 		E = baVector.end();
	u32							dwMask = 0;
	if (I != E) {
		for (int j=0; I != E; I++, j++) {
			if (j >= 32) {
				tNetPacket.w_u32(dwMask);
				dwMask = 0;
				j = 0;
			}
			if (*I)
				dwMask |= u32(1) << j;
		}
		tNetPacket.w_u32		(dwMask);
	}
};

IC void load_bool_vector(xr_vector<bool> &baVector, NET_Packet &tNetPacket)
{
	u32							dwDummy;
	tNetPacket.r_u32			(dwDummy);
	baVector.resize				(dwDummy);
	xr_vector<bool>::iterator 		I = baVector.begin();
	xr_vector<bool>::iterator 		E = baVector.end();
	u32							dwMask = 0;
	for (int j=32; I != E; I++, j++) {
		if (j >= 32) {
			tNetPacket.r_u32(dwMask);
			j = 0;
		}
		*I = !!(dwMask & (u32(1) << j));
	}
};

template <class T>
void save_base_vector(xr_vector<T> &tpVector, NET_Packet &tNetPacket)
{
	tNetPacket.w_u32			(tpVector.size());
	xr_vector<T>::iterator			I = tpVector.begin();
	xr_vector<T>::iterator			E = tpVector.end();
	for ( ; I != E; I++)
		tNetPacket.w			(I,sizeof(*I));
};

template <class T>
void load_base_vector(xr_vector<T> &tpVector, NET_Packet &tNetPacket)
{
	u32							dwDummy;
	tNetPacket.r_u32			(dwDummy);
	tpVector.resize				(dwDummy);
	xr_vector<T>::iterator			I = tpVector.begin();
	xr_vector<T>::iterator			E = tpVector.end();
	for ( ; I != E; I++)
		tNetPacket.r		(I,sizeof(*I));
};

template <class T>
void init_vector(xr_vector<T *> &tpVector, LPCSTR caSection)
{
	xr_vector<T *>::iterator		I = tpVector.begin();
	xr_vector<T *>::iterator		E = tpVector.end();
	for ( ; I != E; I++) {
		*I = xr_new<T>			();
		(*I)->Init				(caSection);
	}
};

template <class T>
void save_vector(xr_vector<T *> &tpVector, NET_Packet &tNetPacket)
{
	tNetPacket.w_u32			(tpVector.size());
	xr_vector<T *>::iterator		I = tpVector.begin();
	xr_vector<T *>::iterator		E = tpVector.end();
	for ( ; I != E; I++)
		(*I)->UPDATE_Write		(tNetPacket);
};

template <class T>
void load_vector(xr_vector<T *> &tpVector, NET_Packet &tNetPacket)
{
	u32							dwDummy;
	tNetPacket.r_u32			(dwDummy);
	tpVector.resize				(dwDummy);
	xr_vector<T *>::iterator		I = tpVector.begin();
	xr_vector<T *>::iterator		E = tpVector.end();
	for ( ; I != E; I++) {
		*I = xr_new<T>			();
		(*I)->UPDATE_Read		(tNetPacket);
	}
};

template <class T1, class T2>
void save_map(xr_map<T1,T2 *> &tpMap, NET_Packet &tNetPacket)
{
	tNetPacket.w_u32		(tpMap.size());
	xr_map<T1,T2 *>::iterator		I = tpMap.begin();
	xr_map<T1,T2 *>::iterator		E = tpMap.end();
	for ( ; I != E; I++)
		(*I).second->Save		(tMemoryStream);
};

template <class T1, class T2>
void load_map(xr_map<T1,T2 *> &tpMap, NET_Packet &tNetPacket, T1 tfGetKey(const T2 *))
{
	tpMap.clear					();
	u32							dwCount	= tFileStream.Rdword();
	for (int i=0 ; i<(int)dwCount; i++) {
		T2						*T = xr_new<T2>();
		T->Load					(tFileStream);
		tpMap.insert			(make_pair(tfGetKey(T),T));
	}
};
#endif