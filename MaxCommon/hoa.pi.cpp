/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.pi.cpp
 @name      hoa.pi
 @realname  hoa.pi
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 π constant.
 
 @description
 <o>hoa.pi</o> is for people that never remember more than 4 decimals or want to initialize a good Pi number.
 
 @discussion
 <o>hoa.pi</o> is for people that never remember more than 4 decimals or want to initialize a good Pi number.
 
 @category ambisonics, hoa objects, maths
 
 @seealso hoa.pi~
 */

#include "HoaCommon.max.h"

typedef struct _hoa_pi
{	
	t_object    f_ob;
	double      f_value;	
	void*       f_outlet;
} t_hoa_pi;

void hoa_pi_bang(t_hoa_pi *x);
void hoa_pi_int(t_hoa_pi *x, long n);
void hoa_pi_float(t_hoa_pi *x, double n) ;
void hoa_pi_assist(t_hoa_pi *x, void *b, long m, long a, char *s);
void *hoa_pi_new(t_symbol *s, int argc, t_atom *argv);

t_hoa_err hoa_getinfos(t_hoa_pi* x, t_hoa_boxinfos* boxinfos);

t_class *hoa_pi_class;

#ifdef HOA_PACKED_LIB
int hoa_pi_main(void)
#else
int C74_EXPORT main(void)
#endif
{
	t_class *c;
	
	c = class_new("hoa.pi", (method)hoa_pi_new, (method)NULL, sizeof(t_hoa_pi), 0L, A_GIMME, 0);
	class_setname((char *)"hoa.pi", (char *)"hoa.pi");
    
	hoa_initclass(c, (method)hoa_getinfos);
    
    class_addmethod(c, (method)hoa_pi_assist,	"assist",	A_CANT, 0);
	
    // @method bang @digest Compute and output result
	// @description The <m>bang</m> message compute and output result : pi * factor.
    class_addmethod(c, (method)hoa_pi_bang,		"bang",		0);
    
    // @method int @digest Set pi multiplier, and compute result
	// @description The <m>int</m> message et pi multiplier, and compute the result
	// @marg 0 @name value @optional 0 @type int
    class_addmethod(c, (method)hoa_pi_int,		"int",		A_LONG, 0);
    
    // @method float @digest Set pi multiplier, and compute result
	// @description The <m>float</m> message et pi multiplier, and compute the result
	// @marg 0 @name value @optional 0 @type float
	class_addmethod(c, (method)hoa_pi_float,	"float",	A_FLOAT, 0);
    
    // @method dblclick @digest Compute and output result
	// @description The <m>dblclick</m> on the object compute and output result
	class_addmethod(c, (method)hoa_pi_bang,		"dblclick",	A_CANT, 0);

	class_register(CLASS_BOX, c);
	hoa_pi_class = c;
	return 0;
}

void *hoa_pi_new(t_symbol *s, int argc, t_atom *argv)
{
	t_hoa_pi *x = NULL;
    x = (t_hoa_pi *)object_alloc(hoa_pi_class);
    if (x)
	{
        // @arg 0 @name multiplier @optional 1 @type float @digest π multiplier
		// @description First argument is the π multiplier
        
        x->f_value = 1.;
        if (atom_gettype(argv) == A_LONG)
            x->f_value = atom_getlong(argv);
        else if (atom_gettype(argv) == A_FLOAT)
            x->f_value = atom_getfloat(argv);
        
        x->f_outlet = floatout(x);
        
        attr_args_process(x, argc, argv);
    }
	
	return(x);
}

t_hoa_err hoa_getinfos(t_hoa_pi* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_STANDARD;
	boxinfos->autoconnect_inputs = 0;
	boxinfos->autoconnect_outputs = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_pi_assist(t_hoa_pi *x, void *b, long m, long a, char *s)
{
    // @in 0 @type bang/int/float @digest bang to output π, set the π multiplier
	if (m == ASSIST_INLET)
		sprintf(s,"(bang/int/float) Compute");
    
    // @out 0 @type float @digest output result
	else
		sprintf(s,"(float) \u03C0 * %.2f", x->f_value);
}

void hoa_pi_bang(t_hoa_pi *x) 
{
	outlet_float(x->f_outlet, PI * x->f_value);
}


void hoa_pi_int(t_hoa_pi *x, long n)
{
	x->f_value = n;
	hoa_pi_bang(x);
}


void hoa_pi_float(t_hoa_pi *x, double n) 
{
	x->f_value = n;
	hoa_pi_bang(x);
}


