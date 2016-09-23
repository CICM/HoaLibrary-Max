/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __DEF_HOA_MAX__
#define __DEF_HOA_MAX__

#include "ThirdParty/HoaLibrary/Sources/Hoa.hpp"

extern "C"
{
#include <ext.h>
#include <ext_obex.h>
#include <z_dsp.h>
#include <ext_common.h>
#include <jpatcher_api.h>
#include <jgraphics.h>
#include <jpatcher_syms.h>
#include <ext_dictionary.h>
#include <buffer.h>
#include <ext_globalsymbol.h>
#include <commonsyms.h>
#include <ext_parameter.h>
#include <jpatcher_syms.h>
}

#include "hoa.max_commonsyms.h"
#include "hoa.max.math.h"

#define HOA_MAX_PLANEWAVES      SYS_MAXSIGS
#define HOA_MAXBLKSIZE          8192
#define HOA_UI_BORDERTHICKNESS  1
#define HOA_UI_CORNERSIZE       8
#define HOA_CONTRAST_LIGHTER    0.06f
#define HOA_CONTRAST_DARKER     0.14f
#define HOA_DISPLAY_NPOINTS     360

static t_symbol* _sym_is_hoa	   = gensym("is_hoa");
static t_symbol* _sym_hoa_version  = gensym("(v2.2)");
static t_symbol* _sym_credit_line1 = gensym("by Julien Colafrancesco, Pierre Guillot & Eliott Paris");
#ifdef _WINDOWS
		static t_symbol* _sym_credit_line2 = gensym("(c) 2012 - 2015, CICM | University of Paris 8");
#else
		static t_symbol* _sym_credit_line2 = gensym("\u00a9 2012 - 2015, CICM | University of Paris 8");
#endif

typedef long t_hoa_err;		///< an integer value suitable to be returned as an hoa error code  @ingroup misc

#define atom_isNumber(av) (atom_gettype(av) == A_LONG || atom_gettype(av) == A_FLOAT)

/**
 Various errors definitions returned by Hoa functions
 @ingroup hoa_max
 */
typedef enum {
	HOA_ERR_ZERO			=					 0,
	HOA_ERR_NONE			=					-1,
	HOA_ERR_OUT_OF_MEMORY	=					-2,
	HOA_ERR_NOT_IMPLEMENTED =					-3,
	HOA_ERR_FAIL			=					-4,
	HOA_ERR_FILE_NOT_FOUND  =					-5,
} e_hoa_err_type;

/**
 The type of connection an Hoa object may have
 @ingroup hoa_max
 */
typedef enum {
	HOA_CONNECT_TYPE_STANDARD =		0,	///< a standard connection type
	HOA_CONNECT_TYPE_AMBISONICS =	1,	///< an ambisonic connection type
	HOA_CONNECT_TYPE_PLANEWAVES =	2,	///< a planewave connection type
} e_hoa_connect_type;

/**
 Is the Hoa object standard or a 2D, 3D decated one
 @ingroup hoa_max
 */
typedef enum {
	HOA_OBJECT_STANDARD =			0, ///< a standard object type
	HOA_OBJECT_2D		=			1, ///< a 2d object type
	HOA_OBJECT_3D		=			2, ///< a 3d object type
} e_hoa_object_type;

/**
 The data structure to describe an hoa box in max.
 @ingroup hoa_max
 */
typedef struct _hoa_boxinfos
{
	e_hoa_object_type object_type;					///< is the object a 2d/3d one ?
	int autoconnect_inputs;							///< number of inputs that can be autoconnected
	int autoconnect_outputs;						///< number of outputs that can be autoconnected
	e_hoa_connect_type autoconnect_inputs_type;		///< the type of inputs (planewaves, ambisonic2D, ambisonic3D..)
	e_hoa_connect_type autoconnect_outputs_type;	///< the type of outputs (planewaves, ambisonic2D, ambisonic3D..)
} t_hoa_boxinfos;

/**
 Print to the Max console some credits for the HoaLibrary
 @ingroup hoa_max
 */
void hoa_print_credit();

long object_is_hoa(t_object* o);

void hoa_boxinfos_init(t_hoa_boxinfos* boxinfos);

t_hoa_err hoa_not_implemented_method();
t_hoa_err hoa_initclass(t_class* c, method hoabox_getinfos);
t_jrgba rgba_addContrast(t_jrgba color, double contrast);

template<class T> class SharedPtr
{
private:
    T* p;
    unsigned* c;
    template<class U> friend class SharedPtr;
public:
    inline SharedPtr() noexcept : p(), c() {}
    inline explicit SharedPtr(T* s) noexcept :p(s), c(new unsigned(1)) {}
    inline SharedPtr(const SharedPtr& s) noexcept :p(s.p), c(s.c) { if(c) ++*c; }
    inline SharedPtr& operator=(const SharedPtr& s)  noexcept
    { if(this!=&s) { clear(); p=s.p; c=s.c; if(c) ++*c; } return *this; }
    template<class U> inline SharedPtr(const SharedPtr<U>& s) noexcept : p(s.p), c(s.c) { if(c) ++*c; }
    inline ~SharedPtr() noexcept {clear();}
    inline void clear() noexcept
    {
        if(c)
        {
            if(*c==1) delete p;
            if(!--*c) delete c;
        }
        c=0; p=0;
    }
    
    inline T* get() const noexcept {return c ? p: 0;}
    inline T* operator->() const noexcept {return c ? p: 0;}
    inline T& operator*() const noexcept {return *get();}
};

#endif
