////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_ALife.h
//	Created 	: 19.09.2002
//  Modified 	: 04.06.2003
//	Author		: Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
//	Description : Server objects monsters for ALife simulator
////////////////////////////////////////////////////////////////////////////

#ifndef xrServer_Objects_ALife_MonstersH
#define xrServer_Objects_ALife_MonstersH

#include "xrServer_Objects_ALife.h"
#include "xrServer_Objects_ALife_Items.h"
#include "character_info_defs.h"

#pragma warning(push)
#pragma warning(disable:4005)

SERVER_ENTITY_DECLARE_BEGIN0(CSE_ALifeTraderAbstract)
	enum eTraderFlags {
		eTraderFlagInfiniteAmmo		= u32(1) << 0,
		eTraderFlagDummy			= u32(-1),
	};
	float							m_fCumulativeItemMass;
	int								m_iCumulativeItemVolume;
	u32								m_dwMoney;
	float							m_fMaxItemMass;
	ALife::PERSONAL_EVENT_P_VECTOR	m_tpEvents;
	Flags32							m_trader_flags;

	////////////////////////////////////////////////////
	//character profile info
#ifndef  AI_COMPILER
	PROFILE_ID						character_profile		();
	void							set_character_profile	(PROFILE_ID);
	SPECIFIC_CHARACTER_ID			specific_character		();
	void							set_specific_character	(SPECIFIC_CHARACTER_ID);
#endif

	CHARACTER_COMMUNITY_INDEX		m_community_index;
	CHARACTER_REPUTATION_VALUE		m_reputation;
	CHARACTER_RANK_VALUE			m_rank;
		
#ifdef XRGAME_EXPORTS
	//��� ������ � relation system
	u16								object_id	() const;
	CHARACTER_COMMUNITY_INDEX		Community	() const;
	LPCSTR							CommunityName () const;
	CHARACTER_RANK_VALUE			Rank		() const;
	CHARACTER_REPUTATION_VALUE		Reputation	() const;

private:
//	bool							m_character_profile_init;

#endif

	PROFILE_ID						m_sCharacterProfile;
	SPECIFIC_CHARACTER_ID			m_SpecificCharacter;

	//�������� ������ ����������� ����������
	xr_vector<SPECIFIC_CHARACTER_ID> m_CheckedCharacters;
	xr_vector<SPECIFIC_CHARACTER_ID> m_DefaultCharacters;

public:	
									CSE_ALifeTraderAbstract(LPCSTR caSection);
	virtual							~CSE_ALifeTraderAbstract();
	// we need this to prevent virtual inheritance :-(
	virtual CSE_Abstract			*base					() = 0;
	virtual const CSE_Abstract		*base					() const = 0;
	virtual CSE_Abstract			*init					();
	virtual CSE_Abstract			*cast_abstract			() {return 0;};
	virtual CSE_ALifeTraderAbstract	*cast_trader_abstract	() {return this;};
	// end of the virtual inheritance dependant code
			void __stdcall			OnChangeProfile			(PropValue* sender);

#ifdef XRGAME_EXPORTS
			void					attach					(CSE_ALifeInventoryItem *tpALifeInventoryItem,	bool		bALifeRequest,	bool bAddChildren = true);
			void					detach					(CSE_ALifeInventoryItem *tpALifeInventoryItem,	ALife::OBJECT_IT	*I = 0,	bool bALifeRequest = true,	bool bRemoveChildren = true);
#ifdef DEBUG
			bool					check_inventory_consistency	();
#endif
			void					vfInitInventory			();
	virtual void					spawn_supplies			();
#endif
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeTraderAbstract)
#define script_type_list save_type_list(CSE_ALifeTraderAbstract)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeTrader,CSE_ALifeDynamicObjectVisual,CSE_ALifeTraderAbstract)
	ALife::_ORGANIZATION_ID				m_tOrgID;
	ALife::ARTEFACT_TRADER_ORDER_MAP	m_tpOrderedArtefacts;
	ALife::TRADER_SUPPLY_VECTOR			m_tpSupplies;


									CSE_ALifeTrader			(LPCSTR caSection);
	virtual							~CSE_ALifeTrader		();
	virtual	bool					interactive				() const;
	virtual CSE_Abstract			*init					();
	virtual CSE_Abstract			*base					();
	virtual const CSE_Abstract		*base					() const;
	virtual void					on_surge				();
	int 							supplies_count;
			void 	__stdcall		OnSuppliesCountChange	(PropValue* sender);
	virtual bool					natural_weapon			() const {return false;}
	virtual bool					natural_detector		() const {return false;}

#ifdef XRGAME_EXPORTS
			u32						dwfGetItemCost			(CSE_ALifeInventoryItem *tpALifeInventoryItem);
	virtual void					spawn_supplies			();
#endif
	virtual CSE_Abstract			*cast_abstract			() {return this;};
	virtual CSE_ALifeTraderAbstract	*cast_trader_abstract	() {return this;};
	virtual CSE_ALifeTrader			*cast_trader			() {return this;};
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeTrader)
#define script_type_list save_type_list(CSE_ALifeTrader)

SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeCustomZone,CSE_ALifeSpaceRestrictor)
	f32								m_maxPower;
	f32								m_attn;
	u32								m_period;
	ALife::EAnomalousZoneType		m_tAnomalyType;
	ALife::EHitType					m_tHitType;
	
									CSE_ALifeCustomZone		(LPCSTR caSection);
	virtual							~CSE_ALifeCustomZone	();
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeCustomZone)
#define script_type_list save_type_list(CSE_ALifeCustomZone)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeAnomalousZone,CSE_ALifeCustomZone,CSE_ALifeSchedulable)
	float							m_fRadius;
	float							m_fBirthProbability;
	u16								m_wItemCount;
	float							*m_faWeights;
	string64						*m_cppArtefactSections;
	u16								m_wArtefactSpawnCount;
	u32								m_dwStartIndex;
	float							m_fStartPower;
	float							m_min_start_power;
	float							m_max_start_power;
	float							m_power_artefact_factor;
	u32								m_ef_anomaly_type;
	u32								m_ef_weapon_type;

									CSE_ALifeAnomalousZone	(LPCSTR caSection);
	virtual							~CSE_ALifeAnomalousZone	();
	virtual CSE_Abstract			*init					();
	virtual CSE_Abstract			*base					();
	virtual const CSE_Abstract		*base					() const;
	virtual CSE_Abstract			*cast_abstract			() {return this;};
	virtual CSE_ALifeSchedulable	*cast_schedulable		() {return this;};
	virtual CSE_ALifeAnomalousZone	*cast_anomalous_zone	() {return this;};
	virtual u32						ef_anomaly_type			() const;
	virtual u32						ef_weapon_type			() const;
	virtual u32						ef_creature_type		() const;
#ifndef XRGAME_EXPORTS
	virtual	void					update					()	{};
#else
	virtual void					on_spawn				();
	virtual	void					update					();
	virtual	CSE_ALifeItemWeapon		*tpfGetBestWeapon		(ALife::EHitType		&tHitType,				float &fHitPower);
	virtual	ALife::EMeetActionType	tfGetActionType			(CSE_ALifeSchedulable	*tpALifeSchedulable,	int iGroupIndex, bool bMutualDetection);
	virtual bool					bfActive				();
	virtual CSE_ALifeDynamicObject	*tpfGetBestDetector		();
#endif
	virtual bool					need_update				(CSE_ALifeDynamicObject *object);
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeAnomalousZone)
#define script_type_list save_type_list(CSE_ALifeAnomalousZone)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeTorridZone,CSE_ALifeCustomZone,CSE_Motion)
									CSE_ALifeTorridZone		(LPCSTR caSection);
	virtual							~CSE_ALifeTorridZone	();
	virtual CSE_Motion*	__stdcall	motion					();
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeTorridZone)
#define script_type_list save_type_list(CSE_ALifeTorridZone)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeZoneVisual,CSE_ALifeAnomalousZone,CSE_Visual)
shared_str attack_animation;
CSE_ALifeZoneVisual	(LPCSTR caSection);
virtual							~CSE_ALifeZoneVisual	();
virtual CSE_Visual* __stdcall	visual					();
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeZoneVisual)
#define script_type_list save_type_list(CSE_ALifeZoneVisual)

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------

SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeCreatureAbstract,CSE_ALifeDynamicObjectVisual)
	u8								s_team;
	u8								s_squad;
	u8								s_group;
	float							fHealth;
	float							m_fMorale;
	float							m_fAccuracy;
	float							m_fIntelligence;

	u32								timestamp;				// server(game) timestamp
	u8								flags;
	float							o_model;				// model yaw
	SRotation						o_torso;				// torso in world coords
	bool							m_bDeathIsProcessed;

	xr_vector<ALife::_OBJECT_ID>	m_dynamic_out_restrictions;
	xr_vector<ALife::_OBJECT_ID>	m_dynamic_in_restrictions;

	u32								m_ef_creature_type;
	u32								m_ef_weapon_type;
	u32								m_ef_detector_type;

	ALife::_OBJECT_ID				m_killer_id;
									
									CSE_ALifeCreatureAbstract(LPCSTR caSection);
	virtual							~CSE_ALifeCreatureAbstract();
	virtual u8						g_team					();
	virtual u8						g_squad					();
	virtual u8						g_group					();
	IC		float					g_Health				() const								{ return fHealth;}
	IC		float					g_Alive					() const								{ return (g_Health() <= 0.f);}
	virtual bool					used_ai_locations		() const;
	virtual bool					can_switch_online		() const;
	virtual bool					can_switch_offline		() const;
	virtual u32						ef_creature_type		() const;
	virtual u32						ef_weapon_type			() const;
	virtual u32						ef_detector_type		() const;
#ifdef XRGAME_EXPORTS
	virtual void					on_spawn				();
#endif
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeCreatureAbstract)
#define script_type_list save_type_list(CSE_ALifeCreatureAbstract)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeMonsterAbstract,CSE_ALifeCreatureAbstract,CSE_ALifeSchedulable)
	GameGraph::_GRAPH_ID				m_tNextGraphID;
	GameGraph::_GRAPH_ID				m_tPrevGraphID;
	float								m_fGoingSpeed;
	float								m_fCurSpeed;
	float								m_fDistanceFromPoint;
	float								m_fDistanceToPoint;
	GameGraph::TERRAIN_VECTOR			m_tpaTerrain;
	float								m_fMaxHealthValue;
	float								m_fRetreatThreshold;
	float								m_fEyeRange;
	float								m_fHitPower;
	ALife::EHitType						m_tHitType;
	shared_str							m_out_space_restrictors;
	shared_str							m_in_space_restrictors;
	svector<float,ALife::eHitTypeMax>	m_fpImmunityFactors;
	
									CSE_ALifeMonsterAbstract(LPCSTR					caSection);
	virtual							~CSE_ALifeMonsterAbstract();
	IC		float					g_MaxHealth				()	const									{ return m_fMaxHealthValue;	}
	virtual CSE_Abstract			*init					();
	virtual CSE_Abstract			*base					();
	virtual const CSE_Abstract		*base					() const;
	virtual CSE_Abstract			*cast_abstract			() {return this;};
	virtual CSE_ALifeSchedulable	*cast_schedulable		() {return this;};
	virtual CSE_ALifeMonsterAbstract*cast_monster_abstract	() {return this;};

	virtual u32						ef_creature_type		() const;
	virtual u32						ef_weapon_type			() const;
	virtual u32						ef_detector_type		() const;
#ifndef XRGAME_EXPORTS
	virtual	void					update					()	{};
#else
	virtual	void					update					();
	virtual	CSE_ALifeItemWeapon		*tpfGetBestWeapon		(ALife::EHitType		&tHitType,				float	&fHitPower);
	virtual	ALife::EMeetActionType	tfGetActionType			(CSE_ALifeSchedulable	*tpALifeSchedulable,	int		iGroupIndex,	bool bMutualDetection);
	virtual bool					bfActive				();
	virtual CSE_ALifeDynamicObject	*tpfGetBestDetector		();
	virtual	void					vfDetachAll				(bool					bFictitious = false) {};
			void					vfCheckForPopulationChanges();
	virtual void					on_surge				();
#endif
	virtual bool					need_update				(CSE_ALifeDynamicObject *object);
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeMonsterAbstract)
#define script_type_list save_type_list(CSE_ALifeMonsterAbstract)

SERVER_ENTITY_DECLARE_BEGIN3(CSE_ALifeCreatureActor,CSE_ALifeCreatureAbstract,CSE_ALifeTraderAbstract,CSE_PHSkeleton)
	u16								mstate;
	Fvector							accel;
	Fvector							velocity;
	float							fArmor;
	u8								weapon;
	///////////////////////////////////////////
	u16								m_u16NumItems;
	u16								m_holderID;
//	DEF_DEQUE		(PH_STATES, SPHNetState); 
	SPHNetState						m_AliveState;
//	PH_STATES						m_DeadStates;

	// ����������� ������ - 6 float(������� �������� �����������) + m_u16NumItems*(7 u8) (������� � ������� �����)
	u8								m_BoneDataSize;
	char							m_DeadBodyData[1024];
	///////////////////////////////////////////
									CSE_ALifeCreatureActor	(LPCSTR caSection);
	virtual							~CSE_ALifeCreatureActor	();
	virtual CSE_Abstract			*base					();
	virtual const CSE_Abstract		*base					() const;
	virtual CSE_Abstract			*init					();
	virtual void					load					(NET_Packet &tNetPacket);
	virtual bool					can_save				()const{return true;}
	virtual bool					natural_weapon			() const {return false;}
	virtual bool					natural_detector		() const {return false;}
#ifdef XRGAME_EXPORTS
	virtual void					spawn_supplies			();
#endif
	virtual CSE_Abstract			*cast_abstract			() {return this;};
	virtual CSE_ALifeTraderAbstract	*cast_trader_abstract	() {return this;};
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeCreatureActor)
#define script_type_list save_type_list(CSE_ALifeCreatureActor)

SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeCreatureCrow,CSE_ALifeCreatureAbstract)
									CSE_ALifeCreatureCrow	(LPCSTR caSection);
	virtual							~CSE_ALifeCreatureCrow	();
	virtual bool					used_ai_locations		() const;
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeCreatureCrow)
#define script_type_list save_type_list(CSE_ALifeCreatureCrow)

SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeCreaturePhantom,CSE_ALifeCreatureAbstract)
									CSE_ALifeCreaturePhantom	(LPCSTR caSection);
	virtual							~CSE_ALifeCreaturePhantom	();
	virtual bool					used_ai_locations			() const;
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeCreaturePhantom)
#define script_type_list save_type_list(CSE_ALifeCreaturePhantom)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeMonsterRat,CSE_ALifeMonsterAbstract,CSE_ALifeInventoryItem)
	// Personal characteristics:
	float							fEyeFov;
	float							fEyeRange;
	float							fMinSpeed;
	float							fMaxSpeed;
	float							fAttackSpeed;
	float							fMaxPursuitRadius;
	float							fMaxHomeRadius;
	// morale
	float							fMoraleSuccessAttackQuant;
	float							fMoraleDeathQuant;
	float							fMoraleFearQuant;
	float							fMoraleRestoreQuant;
	u16								u16MoraleRestoreTimeInterval;
	float							fMoraleMinValue;
	float							fMoraleMaxValue;
	float							fMoraleNormalValue;
	// attack
	float							fHitPower;
	u16								u16HitInterval;
	float							fAttackDistance;
	float							fAttackAngle;
	float							fAttackSuccessProbability;

									CSE_ALifeMonsterRat	(LPCSTR caSection);				// constructor for variable initialization
	virtual							~CSE_ALifeMonsterRat();
	virtual bool					bfUseful();
	virtual CSE_Abstract			*init				();
	virtual CSE_Abstract			*base				();
	virtual const CSE_Abstract		*base				() const;
	virtual CSE_Abstract			*cast_abstract			() {return this;};
	virtual CSE_ALifeInventoryItem	*cast_inventory_item	() {return this;};
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeMonsterRat)
#define script_type_list save_type_list(CSE_ALifeMonsterRat)

SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeMonsterZombie,CSE_ALifeMonsterAbstract)
	// Personal characteristics:
	float							fEyeFov;
	float							fEyeRange;
	float							fMinSpeed;
	float							fMaxSpeed;
	float							fAttackSpeed;
	float							fMaxPursuitRadius;
	float							fMaxHomeRadius;
	// attack
	float							fHitPower;
	u16								u16HitInterval;
	float							fAttackDistance;
	float							fAttackAngle;

									CSE_ALifeMonsterZombie	(LPCSTR caSection);				// constructor for variable initialization
	virtual							~CSE_ALifeMonsterZombie	();
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeMonsterZombie)
#define script_type_list save_type_list(CSE_ALifeMonsterZombie)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeMonsterBase,CSE_ALifeMonsterAbstract,CSE_PHSkeleton)
									CSE_ALifeMonsterBase	(LPCSTR caSection);				// constructor for variable initialization
	virtual							~CSE_ALifeMonsterBase	();
	virtual	void					load					(NET_Packet &tNetPacket);
	virtual CSE_Abstract			*cast_abstract			() {return this;}
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeMonsterBase)
#define script_type_list save_type_list(CSE_ALifeMonsterBase)

//-------------------------------
SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeHumanAbstract,CSE_ALifeTraderAbstract,CSE_ALifeMonsterAbstract)
	DWORD_VECTOR					m_tpPath;
	u32								m_dwCurNode;
	GameGraph::_GRAPH_ID			m_tDestGraphPointIndex;								
	xr_vector<bool>					m_baVisitedVertices;
	ALife::ETaskState				m_tTaskState;
	u32								m_dwCurTaskLocation;
	u32								m_dwCurTaskID;
	float							m_fSearchSpeed;
	shared_str						m_caKnownCustomers;
	ALife::OBJECT_VECTOR			m_tpKnownCustomers;
	svector<char,5>					m_cpEquipmentPreferences;
	svector<char,4>					m_cpMainWeaponPreferences;
	u32								m_dwTotalMoney;
	float							m_fGoingSuccessProbability;
	float							m_fSearchSuccessProbability;
	float							m_detect_probability;

									CSE_ALifeHumanAbstract	(LPCSTR					caSection);
	virtual							~CSE_ALifeHumanAbstract	();
	virtual CSE_Abstract			*init					();
	virtual CSE_Abstract			*base					();
	virtual const CSE_Abstract		*base					() const;
	virtual void					on_surge				();
	virtual bool					natural_weapon			() const {return false;}
	virtual bool					natural_detector		() const {return false;}
#ifdef XRGAME_EXPORTS
	virtual	void					update					();
			// FSM
			void					vfChooseTask			();
			void					vfHealthCare			();
			void					vfBuySupplies			();
			void					vfGoToCustomer			();
			void					vfBringToCustomer		();
			void					vfGoToSOS				();
			void					vfSendSOS				();
			void					vfAccomplishTask		();
			void					vfSearchObject			();
			// FSM miscellanious
			void					vfChooseHumanTask		();
			bool					bfHealthIsGood			();
			bool					bfItemCanTreat			(CSE_ALifeInventoryItem	*tpALifeInventoryItem);
			void					vfUseItem				(CSE_ALifeInventoryItem	*tpALifeInventoryItem);
			bool					bfCanTreat				();
			bool					bfEnoughMoneyToTreat	();
			bool					bfEnoughTimeToTreat		();
			bool					bfEnoughEquipmentToGo	();
			bool					bfDistanceToTraderIsDanger();
			bool					bfEnoughMoneyToEquip	();
			// miscellanious
			bool					bfCheckIfTaskCompleted	(ALife::_TASK_ID		task_id, ALife::OBJECT_IT &I);
			bool					bfCheckIfTaskCompleted	(ALife::_TASK_ID		task_id);
			bool					bfCheckIfTaskCompleted	(ALife::OBJECT_IT		&I);
			bool					bfCheckIfTaskCompleted	();
			bool					similar_task			(const CALifeTask		*prev_task, const CALifeTask *new_task);
			void					vfCheckForDeletedEvents	();
			bool					bfChooseNextRoutePoint	();
			void					vfSetCurrentTask		(ALife::_TASK_ID		&tTaskID);
			u16						get_available_ammo_count(const CSE_ALifeItemWeapon	*tpALifeItemWeapon,		ALife::OBJECT_VECTOR	&tpObjectVector);
			u16						get_available_ammo_count(const CSE_ALifeItemWeapon	*tpALifeItemWeapon,		ALife::ITEM_P_VECTOR	&tpItemVector,		ALife::OBJECT_VECTOR	*tpObjectVector = 0);
			void					attach_available_ammo	(CSE_ALifeItemWeapon	*tpALifeItemWeapon,			ALife::ITEM_P_VECTOR	&tpItemVector,		ALife::OBJECT_VECTOR	*tpObjectVector = 0);
	virtual	CSE_ALifeItemWeapon		*tpfGetBestWeapon		(ALife::EHitType		&tHitType,					float					&fHitPower);
	virtual bool					bfPerformAttack			();
	virtual	void					vfUpdateWeaponAmmo		();
	virtual	void					vfProcessItems			();
	virtual	void					vfAttachItems			(ALife::ETakeType		tTakeType = ALife::eTakeTypeAll);
			bool					bfCanGetItem			(CSE_ALifeInventoryItem	*tpALifeInventoryItem);
	virtual	ALife::EMeetActionType	tfGetActionType			(CSE_ALifeSchedulable	*tpALifeSchedulable,		int						iGroupIndex,		bool			bMutualDetection);
			void					vfCollectAmmoBoxes		();
	virtual CSE_ALifeDynamicObject	*tpfGetBestDetector		();
	virtual	void					vfDetachAll				(bool					bFictitious = false);
			int						ifChooseEquipment		(ALife::OBJECT_VECTOR	*tpObjectVector = 0);
			int						ifChooseWeapon			(ALife::EWeaponPriorityType	tWeaponPriorityType,	ALife::OBJECT_VECTOR	*tpObjectVector = 0);
			int						ifChooseFood			(ALife::OBJECT_VECTOR	*tpObjectVector = 0);
			int						ifChooseMedikit			(ALife::OBJECT_VECTOR	*tpObjectVector = 0);
			int						ifChooseDetector		(ALife::OBJECT_VECTOR	*tpObjectVector = 0);
			int						ifChooseValuables		();
			bool					bfChooseFast			();
			void					vfChooseGroup			(CSE_ALifeGroupAbstract *tpALifeGroupAbstract);
	virtual void					spawn_supplies			();
#endif
	virtual CSE_Abstract			*cast_abstract			() {return this;};
	virtual CSE_ALifeTraderAbstract	*cast_trader_abstract	() {return this;};
	virtual CSE_ALifeHumanAbstract	*cast_human_abstract	() {return this;};
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeHumanAbstract)
#define script_type_list save_type_list(CSE_ALifeHumanAbstract)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeHumanStalker,CSE_ALifeHumanAbstract,CSE_PHSkeleton)
	BOOL							m_demo_mode;
	shared_str						m_start_dialog;

									CSE_ALifeHumanStalker	(LPCSTR caSection);
	virtual							~CSE_ALifeHumanStalker	();
	virtual	void					load					(NET_Packet &tNetPacket);
	virtual CSE_Abstract			*cast_abstract			() {return this;}
SERVER_ENTITY_DECLARE_END
add_to_type_list(CSE_ALifeHumanStalker)
#define script_type_list save_type_list(CSE_ALifeHumanStalker)

#pragma warning(pop)

#endif