////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_time_manager.h
//	Created 	: 05.01.2003
//  Modified 	: 12.05.2004
//	Author		: Dmitriy Iassenev
//	Description : ALife time manager class
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "object_interfaces.h"
#include "alife_space.h"

class CALifeTimeManager {
private:
	ALife::_TIME_ID					m_game_time;
	ALife::_TIME_ID					m_last_surge_time;
	ALife::_TIME_ID					m_next_surge_time;
	float							m_time_factor;
	float							m_normal_time_factor;
	u32								m_start_time;
	u64								m_surge_interval;

private:
	ALife::_TIME_ID					m_start_game_time;

public:
									CALifeTimeManager		(LPCSTR		section);
	virtual							~CALifeTimeManager		();
	virtual void					save					(IWriter	&memory_stream);
	virtual void					load					(IReader	&file_stream);
			void					init					(LPCSTR		section);
	IC		void					set_time_factor			(float		time_factor);
	IC		void					last_surge_time			(ALife::_TIME_ID last_surge_time);
	IC		ALife::_TIME_ID			game_time				() const;
	IC		float					time_factor				() const;
	IC		float					normal_time_factor		() const;
	IC		ALife::_TIME_ID			next_surge_time			() const;
	IC		ALife::_TIME_ID			last_surge_time			() const;
};

#include "alife_time_manager_inline.h"