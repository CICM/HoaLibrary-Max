/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.rotate~.cpp
 @name      hoa.2d.rotate~
 @realname  hoa.2d.rotate~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 2d ambisonic sound field rotation external
 
 @description
 <o>hoa.2d.rotate~</o> applies a rotation on the ambisonic sound field.
 
 @discussion
 <o>hoa.2d.rotate~</o> applies a rotation on the ambisonic sound field.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.wider~, hoa.2d.encoder~, hoa.2d.decoder~, hoa.process~, hoa.mirror~, hoa.2d.space, hoa.2d.recomposer, hoa.2d.meter~, hoa.2d.scope~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_rotate 
{
	t_pxobject                  f_ob;
    Rotate<Hoa2d, t_sample>*    f_rotate;
	t_sample*                   f_ins;
    t_sample*                   f_outs;
    
} t_hoa_rotate;

t_class *hoa_rotate_class;

void *hoa_rotate_new(t_symbol *s, long argc, t_atom *argv)
{
	// @arg 0 @name decomposition-order @optional 0 @type int @digest The ambisonic order of decomposition
	// @description First argument is the ambisonic order of decomposition.
	
	t_hoa_rotate *x = NULL;
	ulong order = 1;

    x = (t_hoa_rotate *)object_alloc(hoa_rotate_class);
	if (x)
	{		
		if(argc && atom_gettype(argv) == A_LONG)
			order = max<long>(atom_getlong(argv), 1);
		
		x->f_rotate = new Rotate<Hoa2d, t_sample>(order);
		
		dsp_setup((t_pxobject *)x, x->f_rotate->getNumberOfHarmonics() + 1);
		for (int i = 0; i < x->f_rotate->getNumberOfHarmonics(); i++)
			outlet_new(x, "signal");
        
		x->f_ins = new t_sample[x->f_rotate->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs = new t_sample[x->f_rotate->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
	}

	return (x);
}

t_hoa_err hoa_getinfos(t_hoa_rotate* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs = x->f_rotate->getNumberOfHarmonics();
	boxinfos->autoconnect_outputs = x->f_rotate->getNumberOfHarmonics();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	return HOA_ERR_NONE;
}

void hoa_rotate_float(t_hoa_rotate *x, double f)
{
    x->f_rotate->setYaw(f);
}

void hoa_rotate_int(t_hoa_rotate *x, long n)
{
    x->f_rotate->setYaw(n);
}

void hoa_rotate_perform64_yaw(t_hoa_rotate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numouts);
    }
	for(int i = 0; i < sampleframes; i++)
    {
        x->f_rotate->setYaw(ins[numins-1][i]);
        x->f_rotate->process(x->f_ins + numouts * i, x->f_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_rotate_perform64(t_hoa_rotate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numouts);
    }
	for(int i = 0; i < sampleframes; i++)
    {
        x->f_rotate->process(x->f_ins + numouts * i, x->f_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_rotate_dsp64(t_hoa_rotate *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(count[x->f_rotate->getNumberOfHarmonics()])
        object_method(dsp64, gensym("dsp_add64"), x, hoa_rotate_perform64_yaw, 0, NULL);
    else
        object_method(dsp64, gensym("dsp_add64"), x, hoa_rotate_perform64, 0, NULL);
}

void hoa_rotate_assist(t_hoa_rotate *x, void *b, long m, long a, char *s)
{
	if(a == x->f_rotate->getNumberOfHarmonics())
	{
        sprintf(s,"(signal/float) Rotation");
	}
	else 
	{
		sprintf(s,"(signal) %s", x->f_rotate->getHarmonicName(a).c_str());
	}
}


void hoa_rotate_free(t_hoa_rotate *x) 
{
	dsp_free((t_pxobject *)x);
	delete x->f_rotate;
    delete [] x->f_ins;
	delete [] x->f_outs;
}

#ifdef HOA_PACKED_LIB
int hoa_2d_rotate_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.rotate~", (method)hoa_rotate_new, (method)hoa_rotate_free, (long)sizeof(t_hoa_rotate), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.rotate~", (char *)"hoa.2d.rotate~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of circular harmonic signals to be rotated, rotation angle.
    // @description Array of circular harmonic signals to be optimized. Set the angle of the rotation of the soundfield in radian in the last inlet at signal rate. The rotation angle is anti-clockwise and wrapped between 0. and 2π.
    class_addmethod(c, (method)hoa_rotate_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_rotate_assist,		"assist",	A_CANT, 0);
    
    // @method float @digest Set the rotation angle.
    // @description Set the angle of the rotation of the soundfield in radian in the last inlet at control rate. The rotation angle is anti-clockwise and wrapped between 0. and 2π.
    class_addmethod(c, (method)hoa_rotate_float,		"float",	A_FLOAT, 0);
    
    // @method int @digest Set the rotation angle.
    // @description Set the angle of the rotation of the soundfield in radian in the last inlet at control rate. The rotation angle is anti-clockwise and wrapped between 0. and 2π.
    class_addmethod(c, (method)hoa_rotate_int,			"int",		A_LONG, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.rotate~"));
    hoa_rotate_class = c;
    
    return 0;
}