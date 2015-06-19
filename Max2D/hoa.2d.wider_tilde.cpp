/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.wider~.cpp
 @name      hoa.2d.wider~
 @realname  hoa.2d.wider~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 2d fractional ambisonic order simulator
 
 @description
 <o>hoa.2d.wider~</o> can be used to wide the diffusion of a localised sound. The order depending signals are weighted and appear in a logarithmic way to have linear changes.
 
 @discussion
 <o>hoa.2d.wider~</o> can be used to wide the diffusion of a localised sound. The order depending signals are weighted and appear in a logarithmic way to have linear changes.
 
 @category 2d, Ambisonics, MSP
 
 @seealso hoa.2d.rotate~, hoa.2d.encoder~, hoa.2d.decoder~, hoa.2d.map~, hoa.process~, hoa.2d.space, hoa.2d.recomposer, hoa.2d.meter~, hoa.2d.scope~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_2d_wider 
{
	t_pxobject              f_ob;
    Wider<Hoa2d, t_sample>* f_wider;
    t_sample*               f_ins;
    t_sample*               f_outs;
    
} t_hoa_2d_wider;

t_class *hoa_2d_wider_class;

void *hoa_2d_wider_new(t_symbol *s, long argc, t_atom *argv)
{
	// @arg 0 @name decomposition-order @optional 0 @type int @digest The ambisonic order of decomposition
	// @description First argument is the ambisonic order of decomposition.
	
    t_hoa_2d_wider *x = (t_hoa_2d_wider *)object_alloc(hoa_2d_wider_class);
	if (x)
	{
        int	order = 1;
		if(argc && atom_gettype(argv) == A_LONG)
			order = max<long>(atom_getlong(argv), 1);
        
        x->f_wider = new Wider<Hoa2d, t_sample>(order);
        dsp_setup((t_pxobject *)x, x->f_wider->getNumberOfHarmonics() + 1);
        for (int i = 0; i < x->f_wider->getNumberOfHarmonics(); i++)
            outlet_new(x, "signal");
        
        x->f_ins = new t_sample[x->f_wider->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs = new t_sample[x->f_wider->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
	}

	return (x);
}

t_hoa_err hoa_getinfos(t_hoa_2d_wider* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs = x->f_wider->getNumberOfHarmonics();
	boxinfos->autoconnect_outputs = x->f_wider->getNumberOfHarmonics();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	return HOA_ERR_NONE;
}

void hoa_2d_wider_float(t_hoa_2d_wider *x, double f)
{
    x->f_wider->setWidening(f);
}

void hoa_2d_wider_int(t_hoa_2d_wider *x, long n)
{
    x->f_wider->setWidening(n);
}

void hoa_2d_wider_perform(t_hoa_2d_wider *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins - 1; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins - 1);
    }
	for(int i = 0; i < sampleframes; i++)
    {
        x->f_wider->setWidening(ins[numins-1][i]);
        x->f_wider->process(x->f_ins + (numins - 1) * i, x->f_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_2d_wider_perform_offset(t_hoa_2d_wider *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
	for(int i = 0; i < sampleframes; i++)
    {
        x->f_wider->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_2d_wider_dsp64(t_hoa_2d_wider *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(count[x->f_wider->getNumberOfHarmonics()])
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_2d_wider_perform, 0, NULL);
    else
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_2d_wider_perform_offset, 0, NULL);
}

void hoa_2d_wider_assist(t_hoa_2d_wider *x, void *b, long m, long a, char *s)
{
	if(a == x->f_wider->getNumberOfHarmonics())
	{
        sprintf(s,"(signal/float) Widening Value");
	}
	else 
	{
		sprintf(s,"(signal) %s", x->f_wider->getHarmonicName(a).c_str());
	}
}


void hoa_2d_wider_free(t_hoa_2d_wider *x) 
{
	dsp_free((t_pxobject *)x);
	delete x->f_wider;
    delete [] x->f_ins;
    delete [] x->f_outs;
}

#ifdef HOA_PACKED_LIB
int hoa_2d_wider_main(void)
#else
void ext_main(void *r)
#endif
{
    
    t_class *c;
    
    c = class_new("hoa.2d.wider~", (method)hoa_2d_wider_new, (method)hoa_2d_wider_free, (long)sizeof(t_hoa_2d_wider), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.wider~", (char *)"hoa.2d.wider~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of circular harmonic signals to be processed, widening value.
    // @description Array of circular harmonic signals to be processed. Set the widening value in the last inlet at signal rate. The widening value is clipped between 0. and 1.
    class_addmethod(c, (method)hoa_2d_wider_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_wider_assist,     "assist",	A_CANT, 0);
    
    // @method float @digest Set the widening value.
    // @description Set the widening value in the last inlet at control rate. The widening value is clipped between 0. and 1.
    class_addmethod(c, (method)hoa_2d_wider_float,		"float",	A_FLOAT, 0);
    
    // @method int @digest Set the widening value.
    // @description Set the widening value in the last inlet at control rate. The widening value is clipped between 0. and 1.
    class_addmethod(c, (method)hoa_2d_wider_int,		"int",		A_LONG, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.wider~"));
    hoa_2d_wider_class = c;
}
