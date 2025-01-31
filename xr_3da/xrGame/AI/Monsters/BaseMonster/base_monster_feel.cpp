////////////////////////////////////////////////////////////////////////////
//	Module 		: base_monster_feel.cpp
//	Created 	: 26.05.2003
//  Modified 	: 26.05.2003
//	Author		: Serge Zhem
//	Description : Visibility and look for all the biting monsters
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "base_monster.h"
#include "../../../actor.h"
#include "../../../ActorEffector.h"
#include "../ai_monster_effector.h"
#include "../../../hudmanager.h"
#include "../../../clsid_game.h"
#include "../../../../skeletonanimated.h"
#include "../../../sound_player.h"
#include "../../../level.h"
#include "../../../script_callback_ex.h"
#include "../../../script_game_object.h"
#include "../../../game_object_space.h"
#include "../../../ai_monster_space.h"

void CBaseMonster::feel_sound_new(CObject* who, int eType, CSound_UserDataPtr user_data, const Fvector &Position, float power)
{
	if (!g_Alive())		return;

	// ignore my sounds
	if (this == who)	return;

	if (user_data)
		user_data->accept	(sound_user_data_visitor());
	
	// ignore sounds if not from enemies
	CEntityAlive* entity = smart_cast<CEntityAlive*> (who);
	if (entity && (!EnemyMan.is_enemy(entity))) return;

	// ignore unknown sounds
	if (eType == 0xffffffff) return;

	// ignore distant sounds
	if (this->Position().distance_to(Position) > db().m_max_hear_dist)	return;
	
	if ((eType & SOUND_TYPE_WEAPON_SHOOTING) == SOUND_TYPE_WEAPON_SHOOTING) power = 1.f;

	// execute callback
	sound_callback	(who,eType,Position,power);
	
	// register in sound memory
	if (power >= db().m_fSoundThreshold) {
		SoundMemory.HearSound(who,eType,Position,power,m_current_update);
 	}
}

void CBaseMonster::HitEntity(const CEntity *pEntity, float fDamage, float impulse, Fvector &dir)
{
	if (!g_Alive()) return;
	if (!pEntity || pEntity->getDestroy()) return;

	if (!EnemyMan.get_enemy()) return;

	if (EnemyMan.get_enemy() == pEntity) {
		Fvector position_in_bone_space;
		position_in_bone_space.set(0.f,0.f,0.f);

		// ������� �� ��������� ��������� � ������� ������� ����������� ��������
		Fvector hit_dir;
		XFORM().transform_dir(hit_dir,dir);
		hit_dir.normalize();

		CEntity		*pEntityNC	= const_cast<CEntity*>(pEntity);
		VERIFY		(pEntityNC);
		pEntityNC->Hit(fDamage,hit_dir,this, smart_cast<CKinematics*>(pEntityNC->Visual())->LL_GetBoneRoot(),position_in_bone_space,impulse);

		if (smart_cast<CActor *>(pEntityNC)) {
			//HUD().GetUI()->UIMainIngameWnd.PlayClawsAnimation	("monster");
			SetAttackEffector									();
		}

		Morale.on_attack_success();
		
		m_time_last_attack_success	= Device.dwTimeGlobal;

		if (!pEntity->g_Alive()) on_kill_enemy(pEntity);
	}
}


BOOL  CBaseMonster::feel_vision_isRelevant(CObject* O)
{
	if (!g_Alive())					return FALSE;
	if (0==smart_cast<CEntity*>(O))	return FALSE;
	
	if ((O->spatial.type & STYPE_VISIBLEFORAI) != STYPE_VISIBLEFORAI) return FALSE;
	
	// ���� ����, �� ������ �� �����
	if (m_bSleep) return FALSE;
	
	// ���� �� ���� - �� �����
	CEntityAlive* entity = smart_cast<CEntityAlive*> (O);
	if (entity && entity->g_Alive()) {
		if (!EnemyMan.is_enemy(entity)) {
			// ���� ����� ����� - ��������� ������� � ���� ������
			CBaseMonster *monster = smart_cast<CBaseMonster *>(entity);
			if (monster) EnemyMan.transfer_enemy(monster);
			return FALSE;
		}
	}

	return TRUE;
}

void CBaseMonster::HitSignal(float amount, Fvector& vLocalDir, CObject* who, s16 element)
{
	if (!g_Alive()) return;
	
	feel_sound_new(who,SOUND_TYPE_WEAPON_SHOOTING,0,who->Position(),1.f);
	if (g_Alive()) sound().play(MonsterSpace::eMonsterSoundTakeDamage);

	if (element < 0) return;

	// ���������� ����������� ���� (����� || ��� || ���� || �����)
	float yaw,pitch;
	vLocalDir.getHP(yaw,pitch);
	
	yaw = angle_normalize(yaw);
	
	EHitSide hit_side = eSideFront;
	if ((yaw >= PI_DIV_4) && (yaw <= 3*PI_DIV_4)) hit_side = eSideLeft;
	else if ((yaw >= 3 * PI_DIV_4) && (yaw <= 5*PI_DIV_4)) hit_side = eSideBack;
	else if ((yaw >= 5 * PI_DIV_4) && (yaw <= 7*PI_DIV_4)) hit_side = eSideRight;

	MotionMan.FX_Play	(hit_side, 1.0f);

	HitMemory.add_hit	(who,hit_side);

	Morale.on_hit		();

	callback(GameObject::eHit)(
		lua_game_object(), 
		amount,
		vLocalDir,
		smart_cast<const CGameObject*>(who)->lua_game_object(),
		element
	);

	// ���� ������� - �������� ��� �����
	CEntityAlive	*obj = smart_cast<CEntityAlive*>(who);
	if (obj && (tfGetRelationType(obj) == ALife::eRelationTypeNeutral)) EnemyMan.add_enemy(obj);
}

void CBaseMonster::SetAttackEffector() 
{
	CActor *pA = smart_cast<CActor *>(Level().CurrentEntity());
	if (pA) {
		pA->EffectorManager().AddEffector(xr_new<CMonsterEffectorHit>(db().m_attack_effector.ce_time,db().m_attack_effector.ce_amplitude,db().m_attack_effector.ce_period_number,db().m_attack_effector.ce_power));
		Level().Cameras.AddEffector(xr_new<CMonsterEffector>(db().m_attack_effector.ppi, db().m_attack_effector.time, db().m_attack_effector.time_attack, db().m_attack_effector.time_release));
	}
}

void CBaseMonster::PsyHit(const CGameObject *object, float value) 
{
	NET_Packet		P;
	
	u_EventGen		(P,GE_HIT, object->ID());				// 
	P.w_u16			(ID());									// own
	P.w_u16			(ID());									// own
	P.w_dir			(Fvector().set(0.f,1.f,0.f));			// direction
	P.w_float		(value);								// hit value	
	P.w_s16			(BI_NONE);								// bone
	P.w_vec3		(Fvector().set(0.f,0.f,0.f));			
	P.w_float		(0.f);									
	P.w_u16			(u16(ALife::eHitTypeTelepatic));
	u_EventSend		(P);
}
