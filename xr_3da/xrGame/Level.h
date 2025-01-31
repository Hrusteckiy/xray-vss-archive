// Level.h: interface for the CLevel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVEL_H__38F63863_DB0C_494B_AFAB_C495876EC671__INCLUDED_)
#define AFX_LEVEL_H__38F63863_DB0C_494B_AFAB_C495876EC671__INCLUDED_
#pragma once

#include "../igame_level.h"
#include "net_client.h"
#include "infoportiondefs.h"
#include "script_export_space.h"
#include "../StatGraph.h"
#include "xrMessages.h"

class	CHUDManager;
class	CParticlesObject;
class	xrServer;
class	game_cl_GameState;
class	NET_Queue_Event;
class	CSE_Abstract;
class	CPatrolPathStorage;
class	CSpaceRestrictionManager;
class	CSeniorityHierarchyHolder;
class	CClientSpawnManager;
class	CGameObject;
class	CAutosaveManager;
class	CPHCommander;
class	CLevelDebug;

#define DEFAULT_FOV				90.f

const int maxRP					= 64;
const int maxTeams				= 32;

class CFogOfWar;
class CBulletManager;
class CMapManager;

class CLevel					: public IGame_Level, public IPureClient
{
private:
#ifdef DEBUG
	bool						m_bSynchronization;
#endif
protected:
	typedef IGame_Level			inherited;
	
	// patrol path storage
	CPatrolPathStorage			*m_patrol_path_storage;
	// movement restriction manager
	CSpaceRestrictionManager	*m_space_restriction_manager;
	// seniority hierarchy holder
	CSeniorityHierarchyHolder	*m_seniority_hierarchy_holder;
	// client spawn_manager
	CClientSpawnManager			*m_client_spawn_manager;
	// autosave manager
	CAutosaveManager			*m_autosave_manager;

	CPHCommander				*m_ph_commander;
	CPHCommander				*m_ph_commander_scripts;
	
	// level name
	shared_str					m_name;
	// Local events
	EVENT						eChangeRP;
	EVENT						eDemoPlay;
	EVENT						eChangeTrack;
	EVENT						eEnvironment;
	EVENT						eEntitySpawn;

	CStatGraph					*pStatGraph;
	
public:
	
#ifdef DEBUG
	// level debugger
	CLevelDebug					*m_level_debug;
#endif

public:
	////////////// network ////////////////////////
	u32							GetInterpolationSteps	();
	void						SetInterpolationSteps	(u32 InterpSteps);
	bool						InterpolationDisabled	();
	void						ReculcInterpolationSteps();
	u32							GetNumCrSteps			() const	{return m_dwNumSteps; };
	void						SetNumCrSteps			( u32 NumSteps );
	static void __stdcall		PhisStepsCallback		( u32 Time0, u32 Time1 );
	bool						In_NetCorrectionPrediction	() {return m_bIn_CrPr;};
private:
	BOOL						m_bNeed_CrPr;
	u32							m_dwNumSteps;
	bool						m_bIn_CrPr;

	DEF_VECTOR					(OBJECTS_LIST, CGameObject*);

	OBJECTS_LIST				pObjects4CrPr;
	OBJECTS_LIST				pActors4CrPr;

	CObject*					pCurrentControlEntity;
public:
	void						AddObject_To_Objects4CrPr	(CGameObject* pObj);
	void						AddActor_To_Actors4CrPr		(CGameObject* pActor);

	CObject*					CurrentControlEntity	( void ) const		{ return pCurrentControlEntity; }
	void						SetControlEntity		( CObject* O  )		{ pCurrentControlEntity=O; }
private:
	
	void						make_NetCorrectionPrediction	();

	u32							m_dwDeltaUpdate ;
	u32							m_dwLastNetUpdateTime;
	void						UpdateDeltaUpd					( u32 LastTime );
	void						BlockCheatLoad					()				;

	BOOL						Connect2Server					(LPCSTR options);
private:
	bool						m_bConnectResultReceived;
	bool						m_bConnectResult;
	xr_string					m_sConnectResult;
public:	
	void						OnGameSpyChallenge				(NET_Packet* P);
	void						OnBuildVersionChallenge			();
	void						OnConnectResult					(NET_Packet* P);
public:
	//////////////////////////////////////////////	
	// static particles
	DEFINE_VECTOR				(CParticlesObject*,POVec,POIt);
	POVec						m_StaticParticles;

	game_cl_GameState			*game;
	BOOL						game_configured;
	NET_Queue_Event				*game_events;
	xr_deque<CSE_Abstract*>		game_spawn_queue;
	xrServer*					Server;

public:
	// sounds
	xr_vector<ref_sound*>		static_Sounds;

	// startup options
	shared_str					m_caServerOptions;
	shared_str					m_caClientOptions;

	// Starting/Loading
	virtual BOOL				net_Start				( LPCSTR op_server, LPCSTR op_client);
	virtual void				net_Load				( LPCSTR name );
	virtual void				net_Save				( LPCSTR name );
	virtual void				net_Stop				( );
	virtual BOOL				net_Start_client		( LPCSTR name );
	virtual void				net_Update				( );


	virtual BOOL				Load_GameSpecific_Before( );
	virtual BOOL				Load_GameSpecific_After ( );
	virtual void				Load_GameSpecific_CFORM	( CDB::TRI* T, u32 count );

	// Events
	virtual void				OnEvent					( EVENT E, u64 P1, u64 P2 );
	virtual void				OnFrame					( void );
	virtual void				OnRender				( );
	void						ProcessGameEvents		( );

	// Input
	virtual	void				IR_OnKeyboardPress		( int btn );
	virtual void				IR_OnKeyboardRelease	( int btn );
	virtual void				IR_OnKeyboardHold		( int btn );
	virtual void				IR_OnMousePress			( int btn );
	virtual void				IR_OnMouseRelease		( int btn );
	virtual void				IR_OnMouseHold			( int btn );
	virtual void				IR_OnMouseMove			( int, int);
	virtual void				IR_OnMouseStop			( int, int);
	virtual void				IR_OnMouseWheel			( int direction);
	
			int					get_RPID				(LPCSTR name);


	// Game
	void						InitializeClientGame	(NET_Packet& P);
	void						ClientReceive			();
	void						ClientSend				();
	void						ClientSave				();
			u32					Objects_net_Save		(NET_Packet* _Packet, u32 start, u32 count);
	virtual	void				Send					(NET_Packet& P, u32 dwFlags=DPNSEND_GUARANTEED, u32 dwTimeout=0);
	
	void						g_cl_Spawn				(LPCSTR name, u8 rp, u16 flags);		// only ask server
	void						g_sv_Spawn				(CSE_Abstract* E);					// server reply/command spawning
	
	// Save/Load/State
	void						SLS_Load				(LPCSTR name);		// Game Load
	void						SLS_Default				();					// Default/Editor Load
	
	IC CSpaceRestrictionManager		&space_restriction_manager	();
	IC CSeniorityHierarchyHolder	&seniority_holder			();
	IC CClientSpawnManager			&client_spawn_manager		();
	IC CAutosaveManager				&autosave_manager			();

	IC CPHCommander					&ph_commander				();
	IC CPHCommander					&ph_commander_scripts		();
	// C/D
	CLevel();
	virtual ~CLevel();

	//��������� �������� ������
	virtual shared_str			name					() const;

	//gets the time from the game simulation
	
	//���������� ����� � ������������ ������������ ������ ����
	ALife::_TIME_ID		GetGameTime				();
	//���������� ����� ��� ������������� � ������������ ������������ ������ ����
	ALife::_TIME_ID		GetEnvironmentGameTime	();
	//������� ����� � ����������������� ����
	void				GetGameDateTime			(u32& year, u32& month, u32& day, u32& hours, u32& mins, u32& secs, u32& milisecs);

	float				GetGameTimeFactor		();
	void				SetGameTimeFactor		(const float fTimeFactor);
	void				SetGameTimeFactor		(ALife::_TIME_ID GameTime, const float fTimeFactor);
	void				SetEnvironmentGameTimeFactor		(ALife::_TIME_ID GameTime, const float fTimeFactor);
//	void				SetGameTime				(ALife::_TIME_ID GameTime);

	// gets current daytime [0..23]
	u8					GetDayTime();
	float				GetGameDayTimeSec();
	float				GetEnvironmentGameDayTimeSec();

protected:
//	CFogOfWar*			m_pFogOfWar;
public:
//	IC CFogOfWar&		FogOfWar() {return	*m_pFogOfWar;}

	//������ ������� �� �����, ������� ������������ � ������ ������
protected:	
	CMapManager *			m_map_manager;
//	LOCATIONS_PTR_VECTOR	m_MapLocationVector;
public:
	CMapManager&			MapManager					() {return *m_map_manager;}

/*
	void					AddEntityMapLocation		(const CGameObject* object, EMapLocationFlags location_type);
	void					AddObjectMapLocationIcon	(const CGameObject* object, EMapLocationFlags location_type, LPCSTR name, LPCSTR text, int icon_x, int icon_y);

	void					AddMapLocation				(const SMapLocation& map_location, EMapLocationFlags location_type);
	void					RemoveMapLocationByID		(u16 object_id, EMapLocationFlags location_type);
	void					RemoveMapLocationByInfo		(INFO_INDEX info_index);
	void					RemoveMapLocations			();
	SMapLocation*			GetMapLocationByID			(u16 object_id);
	void					UpdateMapLocation			();
*/
	//������ � ������
protected:	
	CBulletManager*		m_pBulletManager;
public:
	IC CBulletManager&	BulletManager() {return	*m_pBulletManager;}

	IC		const CPatrolPathStorage &patrol_paths		() const
	{
		VERIFY				(m_patrol_path_storage);
		return				(*m_patrol_path_storage);
	}

	//by Mad Max 
			bool			IsServer					();
			bool			IsClient					();
			CSE_Abstract	*spawn_item					(LPCSTR section, const Fvector &position, u32 level_vertex_id, u16 parent_id, bool return_item = false);
			
protected:
	u32		m_dwCL_PingDeltaSend;
	u32		m_dwCL_PingLastSendTime;
	u32		m_dwRealPing;
	virtual	void			SendPingMessage();
public:
	virtual	u32				GetRealPing() { return m_dwRealPing; };

	DECLARE_SCRIPT_REGISTER_FUNCTION
};
add_to_type_list(CLevel)
#undef script_type_list
#define script_type_list save_type_list(CLevel)

IC CLevel&				Level()		{ return *((CLevel*) g_pGameLevel);			}
IC game_cl_GameState&	Game()		{ return *Level().game;					}
	u32					GameID();
IC CHUDManager&			HUD()		{ return *((CHUDManager*)Level().pHUD);	}

#ifdef DEBUG
IC CLevelDebug&			DBG()		{return *((CLevelDebug*)Level().m_level_debug);}
#endif


IC CSpaceRestrictionManager	&CLevel::space_restriction_manager()
{
	VERIFY				(m_space_restriction_manager);
	return				(*m_space_restriction_manager);
}

IC CSeniorityHierarchyHolder &CLevel::seniority_holder()
{
	VERIFY				(m_seniority_hierarchy_holder);
	return				(*m_seniority_hierarchy_holder);
}

IC CClientSpawnManager &CLevel::client_spawn_manager()
{
	VERIFY				(m_client_spawn_manager);
	return				(*m_client_spawn_manager);
}

IC CAutosaveManager &CLevel::autosave_manager()
{
	VERIFY				(m_autosave_manager);
	return				(*m_autosave_manager);
}

IC	shared_str	CLevel::name	() const
{
	return				(m_name);
}

IC CPHCommander	& CLevel::ph_commander()
{
	VERIFY(m_ph_commander);
	return *m_ph_commander;
}
IC CPHCommander & CLevel::ph_commander_scripts()
{
	VERIFY(m_ph_commander_scripts);
	return *m_ph_commander_scripts;
}
//by Mad Max 
IC bool					OnServer()	{ return Level().IsServer();				}
IC bool					OnClient()	{ return Level().IsClient();				}

class  CPHWorld;
extern CPHWorld*				ph_world;
extern BOOL						g_bDebugEvents;


#endif // !defined(AFX_LEVEL_H__38F63863_DB0C_494B_AFAB_C495876EC671__INCLUDED_)
