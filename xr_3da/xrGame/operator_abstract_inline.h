////////////////////////////////////////////////////////////////////////////
//	Module 		: operator_abstract_inline.h
//	Created 	: 24.02.2004
//  Modified 	: 24.02.2004
//	Author		: Dmitriy Iassenev
//	Description : Operator abstract inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION template<\
	typename _world_property,\
	typename _edge_value_type\
>

#define CAbstractOperator COperatorAbstract<_world_property,_edge_value_type>

TEMPLATE_SPECIALIZATION
IC	CAbstractOperator::COperatorAbstract	()
{
}

TEMPLATE_SPECIALIZATION
IC	CAbstractOperator::COperatorAbstract	(const CSConditionState &conditions, const CSConditionState &effects)
{
	m_conditions		= conditions;
	m_effects			= effects;
}

TEMPLATE_SPECIALIZATION
CAbstractOperator::~COperatorAbstract	()
{
}

TEMPLATE_SPECIALIZATION
void CAbstractOperator::Load						(LPCSTR section)
{
}

TEMPLATE_SPECIALIZATION
void CAbstractOperator::reinit						(bool clear_all)
{
}

TEMPLATE_SPECIALIZATION
void CAbstractOperator::reload						(LPCSTR section)
{
}

TEMPLATE_SPECIALIZATION
IC	const typename CAbstractOperator::CSConditionState	&CAbstractOperator::conditions	() const
{
	return				(m_conditions);
}

TEMPLATE_SPECIALIZATION
IC	const typename CAbstractOperator::CSConditionState	&CAbstractOperator::effects		() const
{
	return				(m_effects);
}

TEMPLATE_SPECIALIZATION
IC	void CAbstractOperator::add_condition	(const COperatorCondition &condition)
{
	m_conditions.add_condition	(condition);
}

TEMPLATE_SPECIALIZATION
IC	void CAbstractOperator::remove_condition(const typename COperatorCondition::_condition_type &condition)
{
	m_conditions.remove_condition(condition);
}

TEMPLATE_SPECIALIZATION
IC	void CAbstractOperator::add_effect		(const COperatorCondition &effect)
{
	m_effects.add_condition		(effect);
}

TEMPLATE_SPECIALIZATION
IC	void CAbstractOperator::remove_effect	(const typename COperatorCondition::_condition_type &effect)
{
	m_effects.remove_condition	(effect);
}

TEMPLATE_SPECIALIZATION
IC	bool CAbstractOperator::applicable		(const CSConditionState &condition, const CSConditionState &start, const CSConditionState &self_condition) const
{
	xr_vector<COperatorCondition>::const_iterator	i = self_condition.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	e = self_condition.conditions().end();
	xr_vector<COperatorCondition>::const_iterator	I = condition.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	E = condition.conditions().end();
	xr_vector<COperatorCondition>::const_iterator	J = start.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	EE = start.conditions().end();
	for ( ; (I != E) && (i != e); )
		if ((*I).condition() < (*i).condition())
			++I;
		else
			if ((*I).condition() > (*i).condition()) {
				while ((J != EE) && ((*J).condition() < (*i).condition()))
					++J;
				if ((J != EE) && ((*J).condition() == (*i).condition())) {
					if ((*J).value() != (*i).value())
						return	(false);
					++J;
				}
				++i;
			}
			else {
				if ((*I).value() != (*i).value())
					return	(false);
				++I;
				++i;
			}

	if (i == e)
		return				(true);

	for ( ; (J != EE) && (i != e); )
		if ((*J).condition() < (*i).condition())
			++J;
		else
			if ((*J).condition() > (*i).condition())
				++i;
			else {
				if ((*J).value() != (*i).value())
					return	(false);
				++J;
				++i;
			}
	return					(true);
}

TEMPLATE_SPECIALIZATION
IC	const typename CAbstractOperator::CSConditionState &CAbstractOperator::apply	(const CSConditionState &condition, const CSConditionState &self_condition, CSConditionState &result) const
{
	result.clear			();
	xr_vector<COperatorCondition>::const_iterator	i = self_condition.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	e = self_condition.conditions().end();
	xr_vector<COperatorCondition>::const_iterator	I = condition.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	E = condition.conditions().end();
	for ( ; (I != E) && (i != e); )
		if ((*I).condition() < (*i).condition()) {
			result.add_condition_back(*I);
			++I;
		}
		else
			if ((*I).condition() > (*i).condition()) {
				result.add_condition_back(*i);
				++i;
			}
			else {
				VERIFY			((*I).condition() == (*i).condition());
				result.add_condition_back(*i);
				++I;
				++i;
			}

	if (i == e) {
		for ( ; I != E; ++I)
			result.add_condition_back(*I);
	}
	else {
		for ( ; i != e; ++i)
			result.add_condition_back(*i);
	}

	return					(result);
}

TEMPLATE_SPECIALIZATION
IC	bool CAbstractOperator::apply_reverse	(const CSConditionState &condition, const CSConditionState &start, CSConditionState &result, const CSConditionState &self_condition) const
{
	result.clear			();
	bool					changed = false;
	xr_vector<COperatorCondition>::const_iterator	i = self_condition.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	e = self_condition.conditions().end();
	xr_vector<COperatorCondition>::const_iterator	I = condition.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	E = condition.conditions().end();
	xr_vector<COperatorCondition>::const_iterator	J = start.conditions().begin();
	xr_vector<COperatorCondition>::const_iterator	EE = start.conditions().end();
	for ( ; (I != E) && (i != e); )
		if ((*I).condition() < (*i).condition()) {
			while ((J != EE) && ((*J).condition() < (*I).condition()))
				++J;
			if ((J != EE) && ((*J).condition() == (*I).condition())) {
				VERIFY		((*J).value() == (*I).value());
				changed		= true;
				++J;
			}
			else
				result.add_condition_back(*I);
			++I;
		}
		else
			if ((*I).condition() > (*i).condition()) {
				result.add_condition_back(*i);
				++i;
			}
			else {
				if ((*I).value() != (*i).value())
					changed	= true;
				result.add_condition_back(*i);
				++I;
				++i;
			}

	if (I == E) {
		if (!changed)
			return			(false);
		for ( ; (i != e); ++i)
			result.add_condition_back(*i);
		return				(true);
	}

	for ( ; (J != EE) && (I != E); )
		if ((*J).condition() < (*I).condition())
			++J;
		else
			if ((*J).condition() > (*I).condition()) {
				result.add_condition_back(*I);
				++I;
			}
			else {
				VERIFY		((*J).value() == (*I).value());
				changed		= true;
				++J;
				++I;
			}

	if (!changed)
		return				(false);

	if ((J == EE) && (I != E))
		for ( ; (I != E); ++I)
			result.add_condition_back(*I);

	return					(true);
}

TEMPLATE_SPECIALIZATION
IC	_edge_value_type CAbstractOperator::weight	(const CSConditionState &condition0, const CSConditionState &condition1) const
{
	return					(_edge_value_type(1));
}

#undef TEMPLATE_SPECIALIZATION
#undef CAbstractOperator