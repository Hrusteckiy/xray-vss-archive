#pragma once

#pragma pack(push,4)
struct	vis_data
{
	Fsphere		sphere;			//
	Fbox		box;			//
	u32			frame;			// when it was tested, and accepted
	u32			hom_frame;		// when to perform test - shedule
	u32			hom_tested;		// when it was last time tested

	IC void		clear			()
	{
		sphere.P.set	(0,0,0);
		sphere.R		= 0;
		box.invalidate	();
		frame			= 0;
		hom_frame		= 0;
		hom_tested		= 0;
	}
};
#pragma pack(pop)
