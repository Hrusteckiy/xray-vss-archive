#pragma	once

class	smapvis		: public	R_feedback
{
public:
	enum			{
		state_counting	= 0,
		state_working	= 1,
		state_usingTC	= 3,
	}							state;
	xr_vector<IRender_Visual*>	invisible;

	u32							frame_sleep;
	u32							test_count;
	u32							test_current;
	IRender_Visual*				testQ_V;
	u32							testQ_id;
	u32							testQ_frame;
public:
	smapvis			();
	~smapvis		();

	void			invalidate	(light* L);
	void			begin		(light* L);	// should be called before 'marker++' and before graph-build
	void			end			(light* L);
	void			mark		(light* L);
	void			flushoccq	(light* L);	// should be called when no rendering of light is supposed
	IC	bool		sleep		()			{ return Device.dwFrame > frame_sleep; }

	virtual		void	rfeedback_static	(IRender_Visual*	V);
};
