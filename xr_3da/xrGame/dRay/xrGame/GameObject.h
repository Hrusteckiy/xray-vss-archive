// GameObject.h: interface for the CGameObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOBJECT_H__3DA72D03_C759_4688_AEBB_89FA812AA873__INCLUDED_)
#define AFX_GAMEOBJECT_H__3DA72D03_C759_4688_AEBB_89FA812AA873__INCLUDED_
#pragma once

#include "../xr_object.h"
#include "xrServer_Space.h"
#include "alife_space.h"
#include "UsableScriptObject.h"
#include "script_binder.h"
#include "Hit.h"

class CPhysicsShell;
class CSE_Abstract;
class CPHSynchronize;
class CScriptGameObject;
class CInventoryItem;
class CEntity;
class CEntityAlive;
class CInventoryOwner;
class CActor;
class CPhysicsShellHolder;
class CParticlesPlayer;
class CCustomZone;
class IInputReceiver;
class CArtefact;
class CCustomMonster;
class CAI_Stalker;
class CScriptEntity;
class CAI_ObjectLocation;
class CWeapon;
class CExplosive;
class CHolderCustom;
class CAttachmentOwner;
class CBaseMonster;

namespace GameObject {
	enum ECallbackType;
};

template <typename _return_type>
class CScriptCallbackEx;

class CGameObject : 
	public CObject, 
	public CUsableScriptObject,
	public CScriptBinder
{
	typedef CObject inherited;
	bool					m_spawned;
	Flags32					m_server_flags;
	CAI_ObjectLocation		*m_ai_location;
	ALife::_STORY_ID		m_story_id;

protected:
	//����� �������� �������
	bool					m_bObjectRemoved;

public:
	//functions used for avoiding most of the smart_cast
	virtual CAttachmentOwner*			cast_attachment_owner		()						{return NULL;}
	virtual CInventoryOwner*			cast_inventory_owner		()						{return NULL;}
	virtual CInventoryItem*				cast_inventory_item			()						{return NULL;}
	virtual CEntity*					cast_entity					()						{return NULL;}
	virtual CEntityAlive*				cast_entity_alive			()						{return NULL;}
	virtual CActor*						cast_actor					()						{return NULL;}
	virtual CGameObject*				cast_game_object			()						{return this;}
	virtual CCustomZone*				cast_custom_zone			()						{return NULL;}
	virtual CPhysicsShellHolder*		cast_physics_shell_holder	()						{return NULL;}
	virtual IInputReceiver*				cast_input_receiver			()						{return NULL;}
	virtual CParticlesPlayer*			cast_particles_player		()						{return NULL;}
	virtual CArtefact*					cast_artefact				()						{return NULL;}
	virtual CCustomMonster*				cast_custom_monster			()						{return NULL;}
	virtual CAI_Stalker*				cast_stalker				()						{return NULL;}
	virtual CScriptEntity*				cast_script_entity			()						{return NULL;}
	virtual CWeapon*					cast_weapon					()						{return NULL;}
	virtual CExplosive*					cast_explosive				()						{return NULL;}
	virtual CSpaceRestrictor*			cast_restrictor				()						{return NULL;}
	virtual CAttachableItem*			cast_attachable_item		()						{return NULL;}
	virtual CHolderCustom*				cast_holder_custom			()						{return NULL;}
	virtual CBaseMonster*				cast_base_monster			()						{return NULL;}

public:
	virtual BOOL						feel_touch_on_contact	(CObject *)					{return TRUE;}
	virtual bool						use						(CGameObject* who_use)		{return CUsableScriptObject::use(who_use);};

public:
	CInifile				*m_ini_file;

	// Utilities
	static void				u_EventGen			(NET_Packet& P, u32 type, u32 dest	);
	static void				u_EventSend			(NET_Packet& P, BOOL sync=TRUE		);
	
	// Methods
	virtual void			Load				(LPCSTR section);
	virtual BOOL			net_Spawn			(CSE_Abstract* DC);
	virtual void			net_Destroy			();
	virtual	void			net_Relcase			( CObject* O );					//
	
	//object serialization
	virtual void			net_Save			(NET_Packet &net_packet);
	virtual void			net_Load			(IReader	&ireader);
	virtual BOOL			net_SaveRelevant	();
	virtual void			save				(NET_Packet &output_packet);
	virtual void			load				(IReader &input_packet);

	virtual BOOL			net_Relevant		()	{ return getLocal();	}	// send messages only if active and local
	virtual void			spatial_move		();
	virtual BOOL			Ready				()	{ return getReady();	}	// update only if active and fully initialized by/for network
//	virtual float			renderable_Ambient	();

	virtual void			shedule_Update		(u32 dt);	
	virtual bool			shedule_Needed		();

	virtual void			renderable_Render	();
	virtual void			OnEvent				(NET_Packet& P, u16 type);
	virtual	void			Hit					(SHit* pHDS) {};
	virtual void			SetHitInfo				(CObject* who, CObject* weapon, s16 element, Fvector Pos, Fvector Dir)	{};


	//������� ��� �������
	virtual LPCSTR			Name                () const;
	
	//virtual void			OnH_A_Independent	();
	virtual void			OnH_B_Chield		();
	virtual void			OnH_B_Independent	();

	virtual bool			IsVisibleForZones	() { return true; }
///////////////////////////////////////////////////////////////////////
	virtual bool			NeedToDestroyObject	() const;
	virtual void			DestroyObject		();
///////////////////////////////////////////////////////////////////////

	// Position stack
	virtual	SavedPosition	ps_Element			(u32 ID) const;

			void			setup_parent_ai_locations(bool assign_position = true);
			void			validate_ai_locations(bool decrement_reference = true);

	// Game-specific events
	CGameObject();
	virtual ~CGameObject();

	virtual BOOL			UsedAI_Locations				();
	virtual	bool			can_validate_position_on_spawn	(){return true;}
#ifdef DEBUG
	virtual void			OnRender			();
#endif

			void			init				();
	virtual	void			reinit				();
	virtual	void			reload				(LPCSTR section);
	///////////////////// network /////////////////////////////////////////
private:
	bool					m_bCrPr_Activated;
	u32						m_dwCrPr_ActivationStep;

public:
	virtual void			make_Interpolation	() {}; // interpolation from last visible to corrected position/rotation
	virtual void			PH_B_CrPr			() {}; // actions & operations before physic correction-prediction steps
	virtual void			PH_I_CrPr			() {}; // actions & operations after correction before prediction steps
#ifdef DEBUG
	virtual void			PH_Ch_CrPr			() {}; // 
	virtual	void			dbg_DrawSkeleton	();
#endif
	virtual void			PH_A_CrPr			() {}; // actions & operations after phisic correction-prediction steps
	virtual void			CrPr_SetActivationStep	(u32 Step)	{m_dwCrPr_ActivationStep = Step; };
	virtual u32				CrPr_GetActivationStep	()	{ return m_dwCrPr_ActivationStep; };
	virtual void			CrPr_SetActivated		(bool Activate)	{ m_bCrPr_Activated = Activate; };
	virtual bool			CrPr_IsActivated		()				{ return m_bCrPr_Activated; };
	///////////////////////////////////////////////////////////////////////
	virtual const SRotation	Orientation			() const
	{
		SRotation			rotation;
		float				h,p,b;
		XFORM().getHPB		(h,p,b);
		rotation.yaw		= h;
		rotation.pitch		= p;
		return				(rotation);
	};

	virtual bool			use_parent_ai_locations	() const
	{
		return				(true);
	}

public:
	typedef void __stdcall visual_callback(CKinematics *);
	typedef svector<visual_callback*,6>			CALLBACK_VECTOR;
	typedef CALLBACK_VECTOR::iterator			CALLBACK_VECTOR_IT;

	CALLBACK_VECTOR			m_visual_callback;

public:
			void			add_visual_callback		(visual_callback *callback);
			void			remove_visual_callback	(visual_callback *callback);
			void			SetKinematicsCallback	(bool set);

	IC		CALLBACK_VECTOR &visual_callbacks	()
	{
		return				(m_visual_callback);
	}


private:
	mutable CScriptGameObject	*m_lua_game_object;
	int						m_script_clsid;
public:
			CScriptGameObject	*lua_game_object() const;
			int				clsid			() const
	{
		THROW				(m_script_clsid >= 0);
		return				(m_script_clsid);
	}
public:
	IC		CInifile		*spawn_ini			()
	{
		return				(m_ini_file);
	}
protected:
	virtual	void			spawn_supplies		();

public:
	virtual void				on_reguested_spawn	(CObject *object);
	IC		CAI_ObjectLocation	&ai_location		() const
	{
		VERIFY				(m_ai_location);
		return				(*m_ai_location);
	}

private:
	u32						m_spawn_time;

public:
	IC		u32				spawn_time			() const
	{
		VERIFY				(m_spawned);
		return				(m_spawn_time);
	}

	IC		const ALife::_STORY_ID &story_id	() const
	{
		return				(m_story_id);
	}
	
public:
	virtual u32				ef_creature_type	() const;
	virtual u32				ef_equipment_type	() const;
	virtual u32				ef_main_weapon_type	() const;
	virtual u32				ef_anomaly_type		() const;
	virtual u32				ef_weapon_type		() const;
	virtual u32				ef_detector_type	() const;
	virtual bool			natural_weapon		() const {return true;}
	virtual bool			natural_detector	() const {return true;}
	virtual bool			use_center_to_aim	() const {return false;}

public:
	
	typedef CScriptCallbackEx<void> CScriptCallbackExVoid;

private:
	
	DEFINE_MAP				(GameObject::ECallbackType, CScriptCallbackExVoid, CALLBACK_MAP, CALLBACK_MAP_IT);
	CALLBACK_MAP			*m_callbacks;

public:
	CScriptCallbackExVoid	&callback			(GameObject::ECallbackType type) const;
	virtual	LPCSTR			visual_name			(CSE_Abstract *server_entity);
};

#endif // !defined(AFX_GAMEOBJECT_H__3DA72D03_C759_4688_AEBB_89FA812AA873__INCLUDED_)
