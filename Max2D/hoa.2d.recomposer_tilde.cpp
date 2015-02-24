/*
 // Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      hoa.2d.recomposer~.cpp
 @name      hoa.2d.recomposer~
 @realname  hoa.2d.recomposer~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 Recomposes a 2d plane wave decomposition into circular harmonics
 
 @description
 <o>hoa.2d.recomposer~</o> recomposes a plane wave decomposition into circular harmonics. There's three modes, <b>fixe</b> (default), <b>fisheye</b> and <b>free</b>. The <o>hoa.2d.recomposer~</o> can be controlled either by message or with the help of its dedicated graphical user interface <o>hoa.2d.recomposer</o>.
 
 @discussion
 <o>hoa.2d.recomposer~</o> recomposes a plane wave decomposition into circular harmonics. There's three modes, <b>fixe</b> (default), <b>fisheye</b> and <b>free</b>. The <o>hoa.2d.recomposer~</o> can be controlled either by message or with the help of its dedicated graphical user interface <o>hoa.2d.recomposer</o>.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.projector~, hoa.2d.recomposer, hoa.2d.space, hoa.fx.gain~, hoa.fx.delay~, hoa.2d.encoder~, hoa.2d.wider~, hoa.2d.decoder~, hoa.process~, hoa.2d.meter~, hoa.2d.scope~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_recomposer
{
	t_pxobject              f_ob;
    
    Recomposer<Hoa2d, t_sample, hoa::Fixe>*     f_fixe;
    Recomposer<Hoa2d, t_sample, hoa::Fisheye>*  f_fisheye;
    Line<t_sample>                              f_line;
    Recomposer<Hoa2d, t_sample, hoa::Free>*     f_free;
    PolarLines<Hoa2d,t_sample>*                 f_lines;
    t_sample*                                   f_lines_vector;
    
    t_sample*                                   f_ins;
    t_sample*                                   f_outs;
    t_symbol*                                   f_mode;
    t_sample                                    f_ramp;
    
} t_hoa_recomposer;

t_class *hoa_recomposer_class;

t_hoa_err hoa_getinfos(t_hoa_recomposer* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs = x->f_fixe->getNumberOfPlanewaves();
	boxinfos->autoconnect_outputs = x->f_fixe->getNumberOfPlanewaves();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_PLANEWAVES;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	return HOA_ERR_NONE;
}

void hoa_recomposer_assist(t_hoa_recomposer *x, void *b, long m, long a, char *s)
{
    if(x->f_mode == hoa_sym_fixe)
    {
        if(m == ASSIST_OUTLET)
            sprintf(s,"(signal) %s", x->f_fixe->getHarmonicName(a).c_str());
        else
            sprintf(s,"(signal) %s", x->f_fixe->getPlanewaveName(a).c_str());
    }
    else if(x->f_mode == hoa_sym_fisheye)
    {
        if(m == ASSIST_OUTLET)
            sprintf(s,"(signal) %s", x->f_fisheye->getHarmonicName(a).c_str());
        else if(a == x->f_fisheye->getNumberOfPlanewaves())
            sprintf(s,"(signal/float) Fisheye factor");
        else
            sprintf(s,"(signal) %s", x->f_fisheye->getPlanewaveName(a).c_str());
    }
    else if(x->f_mode == hoa_sym_free)
    {
        if(m == ASSIST_OUTLET)
            sprintf(s,"(signal) %s", x->f_free->getHarmonicName(a).c_str());
        else
            sprintf(s,"(signal) %s", x->f_free->getPlanewaveName(a).c_str());
    }
}

t_max_err ramp_set(t_hoa_recomposer *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_isNumber(argv))
    {
        x->f_ramp = max<t_sample>(atom_getfloat(argv), 0);
        x->f_lines->setRamp(x->f_ramp / 1000. * sys_getsr());
    }
    
    return MAX_ERR_NONE;
}

void hoa_recomposer_float(t_hoa_recomposer *x, double d)
{
    if(x->f_mode == hoa_sym_fisheye)
    {
        x->f_line.setValue(Math<t_sample>::clip(d, 0., 1.));
    }
}

void hoa_recomposer_angle(t_hoa_recomposer *x, t_symbol *s, short ac, t_atom *av)
{
    if(ac && av && x->f_mode == hoa_sym_free)
    {
        for(int i = 0; i < x->f_free->getNumberOfPlanewaves() && i < ac; i++)
        {
            if(atom_isNumber(av+i))
            {
                x->f_lines->setAzimuth(i, atom_getfloat(av+i));
            }
        }
    }
}

void hoa_recomposer_wide(t_hoa_recomposer *x, t_symbol *s, short ac, t_atom *av)
{
    if(ac && av && x->f_mode == hoa_sym_free)
    {
        for(int i = 0; i < x->f_free->getNumberOfPlanewaves() && i < ac; i++)
        {
            if(atom_isNumber(av+i))
            {
                x->f_lines->setRadius(i, atom_getfloat(av+i));
            }
        }
    }
}

void hoa_recomposer_perform_fixe(t_hoa_recomposer *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_fixe->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_recomposer_perform_fisheye(t_hoa_recomposer *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    ulong numberOfPlanewaves = x->f_fisheye->getNumberOfPlanewaves();
    for(ulong i = 0; i < numberOfPlanewaves; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_ins+i, numberOfPlanewaves);
    }
    for(ulong i = 0; i < sampleframes; i++)
    {
        x->f_fisheye->setFisheye(ins[numberOfPlanewaves][i]);
        x->f_fisheye->process(x->f_ins + numberOfPlanewaves * i, x->f_outs + numouts * i);
    }
    for(ulong i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_recomposer_perform_fisheye_offset(t_hoa_recomposer *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    ulong numberOfPlanewaves = x->f_fisheye->getNumberOfPlanewaves();
    for(ulong i = 0; i < numberOfPlanewaves; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_ins+i, numberOfPlanewaves);
    }
    for(ulong i = 0; i < sampleframes; i++)
    {
        x->f_fisheye->setFisheye(x->f_line.process());
        x->f_fisheye->process(x->f_ins + numberOfPlanewaves * i, x->f_outs + numouts * i);
    }
    for(ulong i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_recomposer_perform_free(t_hoa_recomposer *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    int numberOfPlanewaves = x->f_free->getNumberOfPlanewaves();
    
    for(ulong i = 0; i < numberOfPlanewaves && i < numins; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_ins+i, numberOfPlanewaves);
    }
    for(ulong i = 0; i < sampleframes; i++)
    {
        x->f_lines->process(x->f_lines_vector);
        for(ulong j = 0; j < numberOfPlanewaves; j++)
            x->f_free->setWidening(j, x->f_lines_vector[j]);
        for(ulong j = 0; j < numberOfPlanewaves; j++)
            x->f_free->setAzimuth(j, x->f_lines_vector[j + numberOfPlanewaves]);
        x->f_free->process(x->f_ins + numberOfPlanewaves * i, x->f_outs + numouts * i);
    }
    for(ulong i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_recomposer_dsp(t_hoa_recomposer *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(x->f_mode == hoa_sym_fixe)
    {
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_recomposer_perform_fixe, 0, NULL);
    }
    else if(x->f_mode == hoa_sym_fisheye)
    {
        x->f_line.setRamp(x->f_ramp / 1000. * samplerate);
        if(count[x->f_fisheye->getNumberOfPlanewaves()])
            object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_recomposer_perform_fisheye, 0, NULL);
        else
            object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_recomposer_perform_fisheye_offset, 0, NULL);
    }
    else if(x->f_mode == hoa_sym_free)
    {
        x->f_lines->setRamp(x->f_ramp / 1000. * samplerate);
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_recomposer_perform_free, 0, NULL);
    }
}

void hoa_recomposer_free(t_hoa_recomposer *x)
{
    dsp_free((t_pxobject *)x);
    if(x->f_mode == hoa_sym_fixe)
    {
        delete x->f_fixe;
    }
    else if(x->f_mode == hoa_sym_fisheye)
    {
        delete x->f_fisheye;
    }
    else if(x->f_mode == hoa_sym_free)
    {
        delete x->f_free;
        delete x->f_lines;
        delete [] x->f_lines_vector;
    }
    delete [] x->f_ins;
    delete [] x->f_outs;
}

void *hoa_recomposer_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name ambisonic-order @optional 0 @type int @digest The ambisonic order of decomposition
    // @description First argument is the ambisonic order of decomposition.
    
    // @arg 1 @name number-of-channels @optional 0 @type int @digest The number of channels
    // @description Second argument sets the number of channels.
    
    // @arg 2 @name mode @optional 1 @type symbol @digest The recomposition mode
    // @description The third argument to <o>hoa.2d.recomposer~</o> object sets the recomposition mode. There is three modes :
    // <ul>
    // <li>In <b>fixe</b> mode the <o>hoa.2d.recomposer~</o> object simply recomposes a plane wave decomposition into circular harmonics</li>
    // <li><o>hoa.2d.recomposer~</o>, used in <b>fisheye</b> mode, allows you to perform an operation similar to the visual "fisheye" effect by distorsion of the sound field perspective to the front of the scene.</li>
    // <li>used in <b>free</b> mode, <o>hoa.2d.recomposer~</o> allows you to change the angles and the widening value for each channels. You can set these values directly with messages or with the use of the <o>hoa.2d.recomposer</o> GUI.</li>
    // </ul>
    
    t_hoa_recomposer *x = (t_hoa_recomposer *)object_alloc(hoa_recomposer_class);
    if (x)
    {
        ulong order = 1;
        int numberOfPlanewaves = 4;
        
        x->f_ramp       = 100;
        x->f_mode       = hoa_sym_fixe;
        
        if(argc && atom_gettype(argv) == A_LONG)
            order = max<ulong>(atom_getlong(argv), 1);
        
        if(argc > 1 && atom_gettype(argv+1) == A_LONG)
            numberOfPlanewaves = atom_getlong(argv+1);
        if(numberOfPlanewaves < order * 2 + 1)
            numberOfPlanewaves = order * 2 + 1;
        
        if (argc > 2 && atom_gettype(argv+2) == A_SYM)
        {
            t_symbol* mode = atom_getsym(argv+2);
            if (mode == hoa_sym_fisheye || mode == hoa_sym_free)
                x->f_mode = mode;
        }
        
        ulong numins = 0;
        ulong numouts = 0;
        
        if(x->f_mode == hoa_sym_fisheye)
        {
            x->f_fisheye    = new Recomposer<Hoa2d, t_sample, Fisheye>(order, numberOfPlanewaves);
            x->f_ins        = new t_sample[x->f_fisheye->getNumberOfPlanewaves() * HOA_MAXBLKSIZE];
            x->f_outs       = new t_sample[x->f_fisheye->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
            x->f_line.setRamp(0.1 * sys_getsr());
            x->f_line.setValue(0.f);
            numins = x->f_fisheye->getNumberOfPlanewaves() + 1;
            numouts = x->f_fisheye->getNumberOfHarmonics();
        }
        else if(x->f_mode == hoa_sym_free)
        {
            x->f_free       = new Recomposer<Hoa2d, t_sample, Free>(order, numberOfPlanewaves);
            x->f_lines      = new PolarLines<Hoa2d,t_sample>(x->f_free->getNumberOfPlanewaves());
            x->f_ins        = new t_sample[x->f_free->getNumberOfPlanewaves() * HOA_MAXBLKSIZE];
            x->f_outs       = new t_sample[x->f_free->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
            x->f_lines->setRamp(0.1 * sys_getsr());
            for(ulong i = 0; i < x->f_free->getNumberOfPlanewaves(); i++)
            {
                x->f_lines->setRadiusDirect(i, x->f_free->getWidening(i));
                x->f_lines->setAzimuthDirect(i, x->f_free->getAzimuth(i));
            }
            numins = x->f_free->getNumberOfPlanewaves();
            numouts = x->f_free->getNumberOfHarmonics();
            
            x->f_lines_vector   = new t_sample[x->f_free->getNumberOfPlanewaves() * 2];
        }
        else // (x->f_mode == hoa_sym_fixe)
        {
            x->f_fixe = new Recomposer<Hoa2d, t_sample, Fixe>(order, numberOfPlanewaves);
            x->f_ins  = new t_sample[x->f_fixe->getNumberOfPlanewaves() * HOA_MAXBLKSIZE];
            x->f_outs = new t_sample[x->f_fixe->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
            numins = x->f_fixe->getNumberOfPlanewaves();
            numouts = x->f_fixe->getNumberOfHarmonics();
        }
        
        dsp_setup((t_pxobject *)x, numins);
        for (int i = 0; i < numouts; i++)
            outlet_new(x, "signal");
        
        attr_args_process(x, argc, argv);
    }
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_2d_recomposer_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.recomposer~", (method)hoa_recomposer_new, (method)hoa_recomposer_free, (long)sizeof(t_hoa_recomposer), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.recomposer~", (char *)"hoa.2d.recomposer~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of plane wave signals to be recomposed, fisheye factor.
    // @description Array of plane wave signals to be recomposed, fisheye factor in the last inlet in <b>fisheye</b> mode.
    class_addmethod(c, (method)hoa_recomposer_dsp,      "dsp64",            A_CANT, 0);
    class_addmethod(c, (method)hoa_recomposer_assist,   "assist",           A_CANT, 0);
    
    // @method angles @digest Set the angles of channels.
    // @description Set the angles of channels. Angles are in radians, wrapped between 0. and 2π.
    class_addmethod(c, (method)hoa_recomposer_angle,    "angles",           A_GIMME,0);
    
    // @method wide @digest Set the widening value of channels.
    // @description Set the widening value of channels. Widening value is clipped between 0. (omnidirectional) and 1. (directional).
    class_addmethod(c, (method)hoa_recomposer_wide,     "wide",             A_GIMME,0);
    
    // @method float @digest Set the fisheye factor in the last inlet (only in <b>fisheye</b> mode).
    // @description Set the fisheye factor in the last inlet (only in <b>fisheye</b> mode).
    class_addmethod(c, (method)hoa_recomposer_float,    "float",            A_FLOAT,0);
    
    CLASS_ATTR_DOUBLE			(c,"ramp", 0, t_hoa_recomposer, f_ramp);
    CLASS_ATTR_LABEL			(c,"ramp", 0, "Ramp Time (ms)");
    CLASS_ATTR_CATEGORY			(c,"ramp", 0, "Behavior");
    CLASS_ATTR_ACCESSORS		(c,"ramp", NULL, ramp_set);
    CLASS_ATTR_ORDER			(c,"ramp", 0,  "2");
    // @description Ramp time in millisecond.
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.recomposer~"));
    
    hoa_recomposer_class = c;
    return 0;
}

