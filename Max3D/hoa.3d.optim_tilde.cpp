/*
 // Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      hoa.3d.optim~.cpp
 @name      hoa.3d.optim~
 @realname  hoa.3d.optim~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 3d ambisonic optimization tool
 
 @description
 <o>hoa.3d.optim~</o> weigths the spherical harmonics signals depending on the ambisonic optimization. It can be "basic", for no optimization, "maxRe" or "inPhase".
 
 @discussion
 <o>hoa.3d.optim~</o> weigths the spherical harmonics signals depending on the ambisonic optimization. It can be "basic", for no optimization, "maxRe" or "inPhase".
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.optim~, hoa.3d.map~, hoa.3d.encoder~, hoa.3d.decoder~, hoa.3d.scope~, hoa.3d.wider~, hoa.3d.meter~
 */

#include "Hoa3D.max.h"

typedef struct _hoa_3d_optim 
{
	t_pxobject               f_ob;
    Optim<Hoa3d, t_sample>*  f_optim;
    t_sample*                f_ins;
    t_sample*                f_outs;
    t_symbol*                f_mode;
    
} t_hoa_3d_optim;

t_class *hoa_3d_optim_class;

t_hoa_err hoa_getinfos(t_hoa_3d_optim* x, t_hoa_boxinfos* boxinfos)
{
    boxinfos->object_type = HOA_OBJECT_3D;
    boxinfos->autoconnect_inputs = boxinfos->autoconnect_outputs = x->f_optim->getNumberOfHarmonics();
    boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_AMBISONICS;
    boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
    return HOA_ERR_NONE;
}

void hoa_3d_optim_assist(t_hoa_3d_optim *x, void *b, long m, long a, char *s)
{
    sprintf(s,"(signal) %s", x->f_optim->getHarmonicName(a).c_str());
}

void hoa_3d_optim_perform_basic(t_hoa_3d_optim *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins + i * sampleframes, 1);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_ins + i * sampleframes, 1, outs[i], 1);
    }
}

void hoa_3d_optim_perform_maxRe(t_hoa_3d_optim *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
    for(long i = 0; i < sampleframes; i++)
    {
        (static_cast<Optim<Hoa3d, t_sample>::MaxRe *>(x->f_optim))->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

extern void hoa_3d_optim_perform_inPhase(t_hoa_3d_optim *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
    for(long i = 0; i < sampleframes; i++)
    {
        (static_cast<Optim<Hoa3d, t_sample>::InPhase *>(x->f_optim))->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_3d_optim_dsp64(t_hoa_3d_optim *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(x->f_mode == hoa_sym_basic)
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_3d_optim_perform_basic, 0, NULL);
    else if(x->f_mode == hoa_sym_maxRe)
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_3d_optim_perform_maxRe, 0, NULL);
    else if(x->f_mode == hoa_sym_inPhase)
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_3d_optim_perform_inPhase, 0, NULL);
}

void hoa_3d_optim_basic(t_hoa_3d_optim *x)
{
    if(hoa_sym_basic != x->f_mode)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        ulong order = x->f_optim->getDecompositionOrder();
        delete x->f_optim;
        x->f_optim = new Optim<Hoa3d, t_sample>::Basic(order);
        x->f_mode = hoa_sym_basic;
    }
}

void hoa_3d_optim_maxre(t_hoa_3d_optim *x)
{
    if(hoa_sym_maxRe != x->f_mode)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        ulong order = x->f_optim->getDecompositionOrder();
        delete x->f_optim;
        x->f_optim = new Optim<Hoa3d, t_sample>::MaxRe(order);
        x->f_mode = hoa_sym_maxRe;
    }
}

void hoa_3d_optim_inphase(t_hoa_3d_optim *x)
{
    if(hoa_sym_inPhase != x->f_mode)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        ulong order = x->f_optim->getDecompositionOrder();
        delete x->f_optim;
        x->f_optim = new Optim<Hoa3d, t_sample>::InPhase(order);
        x->f_mode = hoa_sym_inPhase;
    }
}

void hoa_3d_optim_free(t_hoa_3d_optim *x) 
{
	dsp_free((t_pxobject *)x);
    delete x->f_optim;
    delete [] x->f_ins;
    delete [] x->f_outs;
}

void *hoa_3d_optim_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name decomposition-order @optional 0 @type int @digest The ambisonic order of decomposition
    // @description First argument is the ambisonic order of decomposition.
    
    // @arg 1 @name optimization-mode @optional 1 @type symbol @digest The optimization mode.
    // @description The optimization mode can be <b>basic</b> for no optimization (default), <b>maxRe</b> or <b>inPhase</b>.
    
    t_hoa_3d_optim *x = (t_hoa_3d_optim *)object_alloc(hoa_3d_optim_class);
    if (x)
    {
        ulong order = 1;
        if(argc && atom_gettype(argv) == A_LONG)
            order = max<long>(atom_getlong(argv), 1);
        
        x->f_mode   = hoa_sym_basic;
        x->f_optim  = new Optim<Hoa3d, t_sample>::InPhase(order);
        
        if(argc > 1 && argv+1 && atom_gettype(argv+1) == A_SYM)
        {
            if(atom_getsym(argv+1) == hoa_sym_maxRe)
                hoa_3d_optim_maxre(x);
            else if(atom_getsym(argv+1) != hoa_sym_inPhase)
                hoa_3d_optim_basic(x);
        }
        
        dsp_setup((t_pxobject *)x, x->f_optim->getNumberOfHarmonics());
        for (int i = 0; i < x->f_optim->getNumberOfHarmonics(); i++)
            outlet_new(x, "signal");
        
        x->f_ins = new t_sample[x->f_optim->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs = new t_sample[x->f_optim->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
    }
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_3d_optim_main(void)
#else
void ext_main(void *r)
#endif
{
    
    t_class *c;
    
    c = class_new("hoa.3d.optim~", (method)hoa_3d_optim_new, (method)hoa_3d_optim_free, (long)sizeof(t_hoa_3d_optim), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.3d.optim~", (char *)"hoa.3d.optim~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of spherical harmonics signals to be optimized.
    // @description Array of spherical harmonics signals to be optimized.
    class_addmethod(c, (method)hoa_3d_optim_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_3d_optim_assist,    "assist",	A_CANT, 0);
    
    // @method basic @digest Set the optimization mode to <b>basic</b>, does not apply any optimization.
    // @description Set the optimization mode to <b>basic</b>. This is particulary suitable when the listener is ideally placed at the center of the loudspeaker arrangement, or for diffused soundfields.
    class_addmethod(c, (method)hoa_3d_optim_basic,     "basic",	A_NOTHING, 0);
    
    // @method maxRe @digest Set the optimization mode to <b>maxRe</b>.
    // @description Set the optimization mode to <b>maxRe</b>. This is particulary suitable when the audience is confined at the center of the restitution area.
    class_addmethod(c, (method)hoa_3d_optim_maxre,     "maxRe",	A_NOTHING, 0);
    
    // @method maxRe @digest Set the optimization mode to <b>maxRe</b>.
    // @description Set the optimization mode to <b>maxRe</b>. This is particulary suitable when the audience covers all of the restitution area.
    class_addmethod(c, (method)hoa_3d_optim_inphase,   "inPhase",	A_NOTHING, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    
    hoa_3d_optim_class = c;
}
