#pragma once

#include "UIDialogWnd.h"
#include "../script_space_forward.h"
#include "../script_export_space.h"

struct SCallbackInfo;

namespace boost {
	template<typename Signature, typename Allocator>
	class function;
};

class CUIDialogWndEx :public CUIDialogWnd, public DLL_Pure
{
typedef boost::function<void(),std::allocator<void> > void_function;
typedef CUIDialogWnd				inherited;
typedef xr_vector<SCallbackInfo*>	CALLBACKS;
typedef CALLBACKS::iterator			CALLBACK_IT;

private:
			CALLBACKS			m_callbacks;
	virtual void				SendMessage			(CUIWindow* pWnd, s16 msg, void* pData = NULL);
			SCallbackInfo*		NewCallback			();
protected:
			bool				Load				(LPCSTR xml_name);

public:
			void				Register			(CUIWindow* pChild);
								CUIDialogWndEx		();
	virtual						~CUIDialogWndEx		();
			void				AddCallback			(LPCSTR control_id, s16 event, const luabind::functor<void> &lua_function);
			void				AddCallback			(LPCSTR control_id, s16 event, const luabind::functor<void> &functor, const luabind::object &object);
			void				AddCallback			(LPCSTR control_id, s16 event, const void_function &f);
	virtual bool				OnKeyboard			(int dik, EUIMessages keyboard_action);
			void				test();

template<typename T>
IC	T*	GetControl(LPCSTR name);

	DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CUIDialogWndEx)
#undef script_type_list
#define script_type_list save_type_list(CUIDialogWndEx)
