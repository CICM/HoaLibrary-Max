/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.exchanger~.cpp
 @name      hoa.2d.exchanger~
 @realname  hoa.2d.exchanger~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 Renumbers and normalizes the channels.
 
 @description
 <o>hoa.2d.exchanger~</o> renumbers and normalizes the channels.
 
 @discussion
 The library default normalization is SN3D and the numbering is ACN (cf tutorials). The exchangers will always convert from or to the standard.
 The exchanger can take one or two arguments for the numbering and the normalization.
 Numbering : toFurseMalham/toSID/fromFurseMalham/fromSID
 Normaliaztion : toMaxN/toN2D/fromMaxN/fromN2D
 The standards : toBFormat (aka. toFurseMalham & toMaxN)/toDaniel (aka. toSID & toN2D)/fromBFormat (aka. fromFurseMalham & fromMaxN)/fromDaniel (aka. fromSID & fromN2D)
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.3d.exchanger~, hoa.2d.decoder~, hoa.2d.encoder~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_2d_exchanger
{
    t_pxobject                  f_obj;
    t_sample*                   f_ins;
    t_sample*                   f_outs;
    Exchanger<Hoa2d, t_sample>* f_exchanger;
} t_hoa_2d_exchanger;

t_class *hoa_2d_exchanger_class;

t_hoa_err hoa_getinfos(t_hoa_2d_exchanger* x, t_hoa_boxinfos* boxinfos)
{
    boxinfos->object_type = HOA_OBJECT_2D;
    boxinfos->autoconnect_inputs = boxinfos->autoconnect_outputs = x->f_exchanger->getNumberOfHarmonics();
    boxinfos->autoconnect_inputs_type = boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
    return HOA_ERR_NONE;
}

t_hoa_err hoa_get_output_harmonic_order(t_hoa_2d_exchanger* x, ulong index, long* order)
{
    *order = x->f_exchanger->getOutputHarmonicOrder(index);
    return HOA_ERR_NONE;
}

void hoa_2d_exchanger_assist(t_hoa_2d_exchanger *x, void *b, long m, long a, char *s)
{
    if(m == ASSIST_INLET)
        sprintf(s,"(signal) %s", x->f_exchanger->getHarmonicName(a, true).c_str());
    else
        sprintf(s,"(signal) %s", x->f_exchanger->getHarmonicName(a, false).c_str());
}

void hoa_2d_exchanger_perform(t_hoa_2d_exchanger *x, t_object *dsp, t_sample **ins, long nins, t_sample **outs, long numouts, long sampleframes, long f,void *up)
{
    for(long i = 0; i < nins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, nins);
    }
    for(long i = 0; i < sampleframes; i++)
    {
         x->f_exchanger->process(x->f_ins + nins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_2d_exchanger_dsp(t_hoa_2d_exchanger *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add64"), x, (method)hoa_2d_exchanger_perform, 0, NULL);
}

void hoa_2d_exchanger_free(t_hoa_2d_exchanger *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_exchanger;
    delete [] x->f_ins;
    delete [] x->f_outs;
}

void *hoa_2d_exchanger_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name decomposition-order @optional 0 @type int @digest The ambisonic order of decomposition
    // @description First argument is the ambisonic order of decomposition.
    
    // @arg 1 @name numbering @optional 1 @type symbol @digest The numbering format.
    // @description numbering format can be "toFurseMalham", "toSID", "fromFurseMalham", "fromSID"
    
    // @arg 2 @name normalisation @optional 1 @type symbol @digest The normalisation format.
    // @description numbering format can be "toMaxN", "toN2D", "fromMaxN", "fromN2D".
    // Numbering and Normalisation args can be combined into a single as follows : "toBFormat" (aka. toFurseMalham & toMaxN), "toDaniel" (aka. toSID & toN2D), "fromBFormat" (aka. fromFurseMalham & fromMaxN), "fromDaniel" (aka. fromSID & fromN2D).
    
    t_hoa_2d_exchanger *x = (t_hoa_2d_exchanger *)object_alloc(hoa_2d_exchanger_class);
    if (x)
    {
        long order = 1l;
        
        if(argc && argv && atom_gettype(argv) == A_LONG)
            order = Math<long>::clip(atom_getlong(argv), 1l, 63l);
        
        Exchanger<Hoa2d, t_sample>::Numbering       numb = Exchanger<Hoa2d, t_sample>::ACN;
        Exchanger<Hoa2d, t_sample>::Normalization   norm = Exchanger<Hoa2d, t_sample>::SN2D;
        
        for(int i = 1; i < 3; i++)
        {
            if(argc > i && atom_gettype(argv+i) == A_SYM)
            {
                if(atom_getsym(argv+i) == gensym("toMaxN"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::toMaxN;
                }
                else if(atom_getsym(argv+i) == gensym("toN2D"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::toN2D;
                }
                else if(atom_getsym(argv+i) == gensym("fromMaxN"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::fromMaxN;
                }
                else if(atom_getsym(argv+i) == gensym("fromN2D"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::fromN2D;
                }
                else if(atom_getsym(argv+i) == gensym("toFurseMalham"))
                {
                    numb = Exchanger<Hoa2d, t_sample>::toFurseMalham;
                }
                else if(atom_getsym(argv+i) == gensym("toSID"))
                {
                    numb = Exchanger<Hoa2d, t_sample>::toSID;
                }
                else if(atom_getsym(argv+i) == gensym("fromFurseMalham"))
                {
                    numb = Exchanger<Hoa2d, t_sample>::fromFurseMalham;
                }
                else if(atom_getsym(argv+i) == gensym("fromSID"))
                {
                    numb = Exchanger<Hoa2d, t_sample>::fromSID;
                }
                else if(atom_getsym(argv+i) == gensym("toBFormat"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::toMaxN;
                    numb = Exchanger<Hoa2d, t_sample>::toFurseMalham;
                }
                else if(atom_getsym(argv+i) == gensym("fromBFormat"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::fromMaxN;
                    numb = Exchanger<Hoa2d, t_sample>::fromFurseMalham;
                }
                else if(atom_getsym(argv+i) == gensym("toDaniel"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::toN2D;
                    numb = Exchanger<Hoa2d, t_sample>::toSID;
                }
                else if(atom_getsym(argv+i) == gensym("fromDaniel"))
                {
                    norm = Exchanger<Hoa2d, t_sample>::fromN2D;
                    numb = Exchanger<Hoa2d, t_sample>::fromSID;
                }
                else
                {
                    object_error((t_object*)x, "%s : wrong symbol.", atom_getsym(argv+i)->s_name);
                }
            }
        }
        
        x->f_exchanger = new Exchanger<Hoa2d, t_sample>(order);
        x->f_exchanger->setNormalization(norm);
        x->f_exchanger->setNumbering(numb);
        
        dsp_setup((t_pxobject *)x, x->f_exchanger->getNumberOfHarmonics());
        for(int i = 0; i < x->f_exchanger->getNumberOfHarmonics(); i++)
            outlet_new(x, "signal");
        
        x->f_ins = new t_sample[x->f_exchanger->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs = new t_sample[x->f_exchanger->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        return x;
    }
    return NULL;
}

#ifdef HOA_PACKED_LIB
int hoa_2d_exchanger_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.exchanger~", (method)hoa_2d_exchanger_new, (method)hoa_2d_exchanger_free, (long)sizeof(t_hoa_2d_exchanger), 0L, A_GIMME, 0);
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of circular harmonic signals.
    // @description Array of circular harmonic signals.
    class_addmethod(c, (method)hoa_2d_exchanger_dsp,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_exchanger_assist,		"assist",	A_CANT, 0);
    
    // @internal : used by hoa.connect object to color patchlines depending on exchanger numbering config.
    class_addmethod(c, (method)hoa_get_output_harmonic_order, "hoa_get_output_harmonic_order", A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    hoa_2d_exchanger_class = c;
    
    return 0;
}
