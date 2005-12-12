#pragma once

#include "iinputreceiver.h"
//#include "CameraManager.h"
#include "xr_object_list.h"
#include "xr_area.h"

// refs
class ENGINE_API CCameraManager;
class ENGINE_API CCursor;
class ENGINE_API CCustomHUD;
class ENGINE_API ISpatial;
namespace Feel { class ENGINE_API Sound; }

//-----------------------------------------------------------------------------------------------------------
class ENGINE_API	IGame_Level		: 
	public DLL_Pure,
	public IInputReceiver,
	public pureRender,
	public pureFrame,
	public IEventReceiver
{
protected:
	// Network interface
	CObject*					pCurrentEntity;
	CObject*					pCurrentViewEntity;
   
	// Static sounds
	xr_vector<ref_sound>		Sounds_Random;
	u32							Sounds_Random_dwNextTime;
	BOOL						Sounds_Random_Enabled;
	CCameraManager*				m_pCameras;
public:
	CObjectList					Objects; 
	CObjectSpace				ObjectSpace;
	CCameraManager&				Cameras			()				{return *m_pCameras;};

	BOOL						bReady;

	CInifile*					pLevel;
	CCustomHUD*					pHUD;
public:	// deferred sound events
	struct	_esound_delegate	{
		Feel::Sound*			dest	;
		ref_sound_data_ptr		source	;
		float					power	;
	};
	xr_vector<_esound_delegate>	snd_Events;
public:
	// Main, global functions
	IGame_Level					();
	virtual ~IGame_Level		();

	virtual shared_str			name					() const = 0;

	virtual BOOL				net_Start				( LPCSTR op_server, LPCSTR op_client)	= 0;
	virtual void				net_Load				( LPCSTR name )							= 0;
	virtual void				net_Save				( LPCSTR name )							= 0;
	virtual void				net_Stop				( );
	virtual void				net_Update				( )										= 0;

	virtual BOOL				Load					( u32 dwNum );
	virtual BOOL				Load_GameSpecific_Before( )										{ return TRUE; };		// before object loading
	virtual BOOL				Load_GameSpecific_After	( )										{ return TRUE; };		// after object loading
	virtual void				Load_GameSpecific_CFORM	( CDB::TRI* T, u32 count )				= 0;

	virtual void				OnFrame					( void );
	virtual void				OnRender				( void );

	// Main interface
	CObject*					CurrentEntity			( void ) const							{ return pCurrentEntity;				}
	CObject*					CurrentViewEntity		( void ) const							{ return pCurrentViewEntity;			}
	void						SetEntity				( CObject* O  )							{ pCurrentEntity=pCurrentViewEntity=O;	}
	void						SetViewEntity			( CObject* O  )							{ pCurrentViewEntity=O;					}
	
	void						SoundEvent_Register		( ref_sound_data_ptr S, float range );
	void						SoundEvent_Dispatch		( );

	// Loader interface
	ref_shader					LL_CreateShader			(int S, int T, int M, int C);
	void						LL_CheckTextures		();
};

//-----------------------------------------------------------------------------------------------------------
extern ENGINE_API	IGame_Level*	g_pGameLevel;

template <typename _class_type>
	void relcase_register	(_class_type *self, void (xr_stdcall _class_type::* function_to_bind)(CObject*))
	{
		g_pGameLevel->Objects.relcase_register	(
			CObjectList::RELCASE_CALLBACK (
				self,
				function_to_bind)
		);
	}

template <typename _class_type>
	void relcase_unregister	(_class_type *self, void (xr_stdcall _class_type::* function_to_bind)(CObject*))
	{
		g_pGameLevel->Objects.relcase_unregister	(
			CObjectList::RELCASE_CALLBACK (
				self,
				function_to_bind)
		);
	}
