//---------------------------------------------------------------------------
#ifndef SHCompilerToolsH
#define SHCompilerToolsH

#include "Shader_xrLC.h"
#include "SHToolsInterface.h"

// refs
class PropValue;                               

class CSHCompilerTools: public ISHTools
{
	bool __fastcall			ItemExist			(LPCSTR name){return !!FindItem(name);}
	Shader_xrLC*			FindItem			(LPCSTR name);
    Shader_xrLC_LIB			m_Library;

    ListItem*				m_Selected;
public:
    Shader_xrLC*			m_Shader;
    virtual BOOL __fastcall OnRemoveItem		(LPCSTR name, EItemType type); 
	virtual void __fastcall OnRenameItem		(LPCSTR old_full_name, LPCSTR new_full_name, EItemType type);
    virtual LPCSTR			AppendItem			(LPCSTR folder, LPCSTR parent=0);
	virtual void			FillItemList		();
public:
							CSHCompilerTools 	(ISHInit& init);
    virtual 				~CSHCompilerTools	();

    virtual LPCSTR			ToolsName			(){return "Compiler Shader";}

	virtual void			Reload				();
	virtual void			Load				();
	virtual void			Save				();

    virtual bool			OnCreate			();
    virtual void			OnDestroy			();
	virtual void 			OnActivate			();
	virtual void 			OnDeactivate		();

    // misc
    virtual void			ResetCurrentItem	();
    virtual void			SetCurrentItem		(LPCSTR name, bool bView);
    virtual void			ApplyChanges		(bool bForced=false);

	virtual void 			RealUpdateProperties();

	virtual void 			OnFrame				();
	virtual void 			OnRender			(){;}

    virtual void			OnDeviceCreate		(){;}
    virtual void			OnDeviceDestroy		(){;}
};
//---------------------------------------------------------------------------
#endif
