/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.projector~.cpp
 @name      hoa.2d.projector~
 @realname  hoa.2d.projector~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 2d ambisonic projection external
 
 @description
 <o>hoa.2d.projector~</o> discretizes the ambisonic soundfield into an array of channels and gives an access to a new sound domain.
 
 @discussion
 <o>hoa.2d.projector~</o> discretizes the ambisonic soundfield into an array of channels and gives an access to a new sound domain.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.recomposer~, hoa.2d.recomposer, hoa.2d.space, hoa.fx.gain~, hoa.fx.delay~, hoa.2d.wider~, hoa.2d.encoder~, hoa.2d.decoder~, hoa.process~, hoa.2d.meter~, hoa.2d.scope~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_projector 
{
	t_pxobject                  f_ob;
    Projector<Hoa2d, t_sample>* f_projector;
    t_sample*                   f_ins;
    t_sample*                   f_outs;
    
} t_hoa_projector;

t_class *hoa_projector_class;

void *hoa_projector_new(t_symbol *s, long argc, t_atom *argv)
{
	// @arg 0 @name decomposition-order @optional 0 @type int @digest The ambisonic order of decomposition
	// @description First argument is the ambisonic order of decomposition.
	
	// @arg 1 @name number-of-channels @optional 0 @type int @digest The number of channels
	// @description Second argument sets the number of channels.
    
	t_hoa_projector *x = NULL;
    x = (t_hoa_projector *)object_alloc(hoa_projector_class);
	if (x)
	{
        ulong order = 1;
        ulong numberOfPlanewaves = 4;
        
		if(argc && atom_gettype(argv) == A_LONG)
			order = max<long>(atom_getlong(argv), 1);
        
        if(argc > 1 && atom_gettype(argv+1) == A_LONG)
			numberOfPlanewaves = max<long>(atom_getlong(argv+1), order * 2 + 1);
		
		x->f_projector = new Projector<Hoa2d, t_sample>(order, numberOfPlanewaves);
		
		dsp_setup((t_pxobject *)x, x->f_projector->getNumberOfHarmonics());
		for (int i = 0; i < x->f_projector->getNumberOfPlanewaves(); i++)
			outlet_new(x, "signal");
        
        x->f_ins = new t_sample[x->f_projector->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs = new t_sample[x->f_projector->getNumberOfPlanewaves() * HOA_MAXBLKSIZE];
	}

	return (x);
}

t_hoa_err hoa_getinfos(t_hoa_projector* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs = x->f_projector->getNumberOfHarmonics();
	boxinfos->autoconnect_outputs = x->f_projector->getNumberOfPlanewaves();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_PLANEWAVES;
	return HOA_ERR_NONE;
}

void hoa_projector_perform64(t_hoa_projector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
	for(int i = 0; i < sampleframes; i++)
    {
        x->f_projector->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_projector_dsp64(t_hoa_projector *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp64, gensym("dsp_add64"), x, hoa_projector_perform64, 0, NULL);
}

void hoa_projector_assist(t_hoa_projector *x, void *b, long m, long a, char *s)
{
    if(m == ASSIST_INLET)
        sprintf(s,"(signal) %s", x->f_projector->getHarmonicName(a).c_str());
    else
        sprintf(s,"(signal) %s", x->f_projector->getPlanewaveName(a).c_str());
}


void hoa_projector_free(t_hoa_projector *x) 
{
	dsp_free((t_pxobject *)x);
	delete x->f_projector;
    delete [] x->f_ins;
	delete [] x->f_outs;
}

#ifdef HOA_PACKED_LIB
int hoa_2d_projector_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.projector~", (method)hoa_projector_new, (method)hoa_projector_free, (long)sizeof(t_hoa_projector), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.projector~", (char *)"hoa.2d.projector~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of circular harmonic signals to be projected.
    // @description Array of circular harmonic signals to be projected.
    class_addmethod(c, (method)hoa_projector_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_projector_assist,    "assist",	A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.projector~"));
    hoa_projector_class = c;
    
    return 0;
}
