////////////////////////////////////////////////////////////////////////////
//	Module 		: space_restriction_holder_inline.h
//	Created 	: 17.08.2004
//  Modified 	: 27.08.2004
//	Author		: Dmitriy Iassenev
//	Description : Space restriction holder inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

IC	CSpaceRestrictionHolder::CSpaceRestrictionHolder	()
{
	m_default_out_restrictions	= "";
	m_default_in_restrictions	= "";
}

IC	shared_str	CSpaceRestrictionHolder::normalize_string	(shared_str space_restrictors)
{
	string4096				m_temp_string;
	u32						n = _GetItemCount(*space_restrictors);
	if (n < 2)
		return				(space_restrictors);

	m_temp.resize			(n);
	for (u32 i=0; i<n ;++i)
		m_temp[i]			= shared_str(_GetItem(*space_restrictors,i,m_temp_string));

	std::sort				(m_temp.begin(),m_temp.end());

	strcpy					(m_temp_string,"");
	xr_vector<shared_str>::const_iterator	I = m_temp.begin(), B = I;
	xr_vector<shared_str>::const_iterator	E = m_temp.end();
	for ( ; I != E; ++I) {
		if (I != B)
			strcat			(m_temp_string,",");
		strcat				(m_temp_string,**I);
	}

	return					(shared_str(m_temp_string));
}

IC	shared_str	CSpaceRestrictionHolder::default_out_restrictions	() const
{
	return					(m_default_out_restrictions);
}

IC	shared_str	CSpaceRestrictionHolder::default_in_restrictions	() const
{
	return					(m_default_in_restrictions);
}
