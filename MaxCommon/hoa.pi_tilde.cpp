/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.pi~.cpp
 @name      hoa.pi~
 @realname  hoa.pi~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 π signal constant.
 
 @description
 <o>hoa.pi~</o> is for people that never remember more than 4 decimals or want to initialize a good π number.
 
 @discussion
 <o>hoa.pi~</o> is for people that never remember more than 4 decimals or want to initialize a good π number.
 
 @category ambisonics, hoa objects, maths, msp
 
 @seealso hoa.pi
 */

#include "HoaCommon.max.h"

typedef struct _hoa_pi_sig
{	
	t_pxobject  f_ob;
	double      f_value;
    double      f_phase;
} t_hoa_pi_sig;

t_class *hoa_pi_sig_class;

void *hoa_pi_sig_new(t_symbol *s, int argc, t_atom *argv)
{
	t_hoa_pi_sig *x = (t_hoa_pi_sig *)object_alloc(hoa_pi_sig_class);
    
    if (x)
    {
        // @arg 0 @name float @optional 1 @type float @digest π multiplier
		// @description First argument is the π multiplier
        
        x->f_value = 1.0f;
        x->f_phase = 1.0f;
        
        if (atom_gettype(argv) == A_LONG)
            x->f_value = atom_getlong(argv);
        else if (atom_gettype(argv) == A_FLOAT)
            x->f_value = atom_getfloat(argv);
        
        dsp_setup((t_pxobject *)x, 2);
        outlet_new(x, "signal");
    }
    
	return(x);
}

t_hoa_err hoa_getinfos(t_hoa_pi_sig* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_STANDARD;
	boxinfos->autoconnect_inputs = 0;
	boxinfos->autoconnect_outputs = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_pi_sig_perform64(t_hoa_pi_sig *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_value = ins[0][i];
        outs[0][i] = HOA_PI * ins[0][i];
    }
}

void hoa_pi_sig_perform64_phase(t_hoa_pi_sig *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_phase = Math<double>::wrap(ins[1][i], 0.0f, 1.0f);
        outs[0][i] = HOA_PI * x->f_value * x->f_phase;
    }
}

void hoa_pi_sig_perform64_offset(t_hoa_pi_sig *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
        outs[0][i] = HOA_PI * x->f_value * x->f_phase;
}

void hoa_pi_sig_dsp64(t_hoa_pi_sig *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(count[0])
        object_method(dsp64, gensym("dsp_add64"), x, hoa_pi_sig_perform64, 0, NULL);
    else if(count[1])
        object_method(dsp64, gensym("dsp_add64"), x, hoa_pi_sig_perform64_phase, 0, NULL);
    else
        object_method(dsp64, gensym("dsp_add64"), x, hoa_pi_sig_perform64_offset, 0, NULL);
}

void hoa_pi_sig_assist(t_hoa_pi_sig *x, void *b, long m, long a, char *s)
{
    // @out 0 @type signal @digest output result
	if (m == ASSIST_OUTLET)
		sprintf(s,"(signal) \u03C0 Result");
	else
    {
        if(a)
            sprintf(s,"(signal/float) \u03C0 Phase");       // @in 1 @type signal/float @digest set phase (wrapped between 0. and 1.)
        else
            sprintf(s,"(signal/float) \u03C0 Multiplier");  // @in 0 @type signal/float @digest set π multiplier
    }
}

void hoa_pi_sig_int(t_hoa_pi_sig *x, long n)
{
    if (proxy_getinlet((t_object*)x))
        x->f_phase = n != 0;
    else
    {
        x->f_value = n;
        x->f_phase = 1;
    }
}


void hoa_pi_sig_float(t_hoa_pi_sig *x, double n) 
{
    if (proxy_getinlet((t_object*)x))
        x->f_phase = Math<double>::wrap(n, 0.0f, 1.0f);
    else
    {
        x->f_value = n;
        x->f_phase = 1;
    }
}

#ifdef HOA_PACKED_LIB
int hoa_pi_sig_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.pi~", (method)hoa_pi_sig_new, (method)dsp_free, sizeof(t_hoa_pi_sig), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.pi~", (char *)"hoa.pi~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    class_addmethod(c, (method)hoa_pi_sig_dsp64,	"dsp64",    A_CANT, 0);
    class_addmethod(c, (method)hoa_pi_sig_assist,	"assist",	A_CANT, 0);
    
    // @method int @digest Set π multiplier or phase
    // @description The <m>int</m> message set pi multiplier in the first inlet, the phase in the second one.
    // @marg 0 @name value @optional 0 @type int
    class_addmethod(c, (method)hoa_pi_sig_int,		"int",		A_LONG, 0);
    
    // @method float @digest Set π multiplier or phase
    // @description The <m>float</m> message set pi multiplier in the first inlet, the phase in the second one.
    // @marg 0 @name value @optional 0 @type float
    class_addmethod(c, (method)hoa_pi_sig_float,	"float",	A_FLOAT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    hoa_pi_sig_class = c;
    return 0;
}