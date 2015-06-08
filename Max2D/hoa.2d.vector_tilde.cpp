/*
// Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.vector~.cpp
 @name      hoa.2d.vector~
 @realname  hoa.2d.vector~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 2d soundfield energy and velocity vectors calculator.
 
 @description
 <o>hoa.2d.vector~</o> computes the coordinates of the velocity vector and the energy vector that characterize a sound field. It's an useful tool to analyse restitution quality. For futher information : Michael A. Gerzon, General metatheorie of auditory localisation. Audio Engineering Society Preprint, 3306, 1992.
 
 @discussion
 <o>hoa.2d.vector~</o> computes the coordinates of the velocity vector and the energy vector that characterize a sound field. It's an useful tool to analyse restitution quality. For futher information : Michael A. Gerzon, General metatheorie of auditory localisation. Audio Engineering Society Preprint, 3306, 1992.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.meter~, hoa.2d.scope~, hoa.2d.decoder~, hoa.3d.vector~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_2d_vector 
{
	t_pxobject                  f_ob;
    Vector<Hoa2d, t_sample>*    f_vector;
    t_sample*                   f_ins;
    t_sample*                   f_outs;
    t_symbol*                   f_mode;
    void*                       f_attrs;
    
} t_hoa_2d_vector;

t_class *hoa_2d_vector_class;

t_hoa_err hoa_getinfos(t_hoa_2d_vector* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs = x->f_vector->getNumberOfPlanewaves();
	boxinfos->autoconnect_outputs = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_PLANEWAVES;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_2d_vector_perform(t_hoa_2d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
	for(long i = 0; i < sampleframes; i++)
    {
        x->f_vector->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_2d_vector_perform_velocity(t_hoa_2d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_vector->processVelocity(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_2d_vector_perform_energy(t_hoa_2d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_vector->processEnergy(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_2d_vector_dsp64(t_hoa_2d_vector *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_vector->computeRendering();
    
    if(x->f_mode == hoa_sym_both)
        object_method(dsp64, gensym("dsp_add64"), x, hoa_2d_vector_perform, 0, NULL);
    if(x->f_mode == hoa_sym_velocity)
        object_method(dsp64, gensym("dsp_add64"), x, hoa_2d_vector_perform_velocity, 0, NULL);
    else
        object_method(dsp64, gensym("dsp_add64"), x, hoa_2d_vector_perform_energy, 0, NULL);
}

void hoa_2d_vector_assist(t_hoa_2d_vector *x, void *b, long m, long a, char *s)
{
    if(m == ASSIST_INLET)
    {
        sprintf(s,"(signal) %s", x->f_vector->getPlanewaveName(a).c_str());
    }
    else
    {
        const int both = (x->f_mode == hoa_sym_both) ? 2 : 0;
        
        if(both || x->f_mode == hoa_sym_velocity)
        {
            if(a == 0)
                sprintf(s,"(signal) Velocity Vector Abscissa");
            else if(a == 1)
                sprintf(s,"(signal) Velocity Vector Ordinate");
        }
        
        if(both || x->f_mode == hoa_sym_energy)
        {
            if(a == 0 + both)
                sprintf(s,"(signal) Energy Vector Abscissa");
            else if(a == 1 + both)
                sprintf(s,"(signal) Energy Vector Ordinate");
        }
    }
}


void hoa_2d_vector_free(t_hoa_2d_vector *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_vector;
    delete [] x->f_ins;
	delete [] x->f_outs;
}

t_max_err channels_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = new t_atom[argc[0]];
    atom_setlong(argv[0], x->f_vector->getNumberOfPlanewaves());
    
    return MAX_ERR_NONE;
}

t_max_err channels_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv)
{
    t_object *b = NULL;
    if(argc && argv && atom_gettype(argv) == A_LONG)
    {
        int channels = atom_getlong(argv);
        if(channels != x->f_vector->getNumberOfPlanewaves() &&
           channels > 0 &&
           channels <= HOA_MAX_PLANEWAVES)
        {
            object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
            
            delete x->f_vector;
            x->f_vector = new Vector<Hoa2d, t_sample>(channels);
            x->f_vector->computeRendering();
        
            object_obex_lookup(x, hoa_sym_pound_B, (t_object **)&b);
            object_method(b, hoa_sym_dynlet_begin);
            dsp_resize((t_pxobject*)x, x->f_vector->getNumberOfPlanewaves());
            object_method(b, hoa_sym_dynlet_end);
        
            object_attr_setvalueof(x, hoa_sym_angles, 0, NULL);
        }
    }
    return MAX_ERR_NONE;
}

t_max_err offset_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = (t_atom *)sysmem_newptr(argc[0] * sizeof(t_atom));
    
    if(argv[0])
    {
        atom_setfloat(argv[0], wrap(x->f_vector->getPlanewavesRotationX() / HOA_2PI * 360.f, -180., 180.));
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    return MAX_ERR_NONE;
}

t_max_err offset_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && (atom_gettype(argv) == A_FLOAT || atom_gettype(argv) == A_LONG))
    {
        double offset = Math<double>::wrap_twopi(wrap(atom_getfloat(argv), -180., 180.) / 360. * HOA_2PI);
        if(offset != x->f_vector->getPlanewavesRotationX())
        {
            x->f_vector->setPlanewavesRotation(offset, 0., 0.);
        }
    }
    return MAX_ERR_NONE;
}

t_max_err angles_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = x->f_vector->getNumberOfPlanewaves();
    argv[0] = new t_atom[argc[0]];
    
    for(int i = 0; i < x->f_vector->getNumberOfPlanewaves(); i++)
    {
        atom_setfloat(argv[0]+i, x->f_vector->getPlanewaveAzimuth(i) / HOA_2PI * 360.);
    }
    
    return MAX_ERR_NONE;
}

t_max_err angles_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        for(int i = 0; i < argc && i < x->f_vector->getNumberOfPlanewaves(); i++)
        {
            if(atom_gettype(argv+i) == A_LONG || atom_gettype(argv+i) == A_FLOAT);
            x->f_vector->setPlanewaveAzimuth(i, Math<double>::clip(atom_getfloat(argv+i), -360., 360.) / 360. * HOA_2PI);
        }
        x->f_vector->computeRendering();
    }
    return MAX_ERR_NONE;
}

void *hoa_2d_vector_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name number-of-channels @optional 0 @type int @digest The number of channels
    // @description First argument sets the number of channels.
    
    // @arg 1 @name vector-type @optional 1 @type symbol @digest The vector type.
    // @description second argument sets the type of vector to compute (velocity or energy)
    
    t_hoa_2d_vector *x = NULL;
    x = (t_hoa_2d_vector *)object_alloc(hoa_2d_vector_class);
    
    if(x)
    {
        x->f_mode = hoa_sym_both;
        ulong nouts = 4ul;
        ulong channels = 4ul;
        
        if (argc && atom_gettype(argv) == A_LONG)
        {
            channels = max<long>(atom_getlong(argv), 1);
        }
        
        if(argc > 1 && atom_gettype(argv+1) == A_SYM)
        {
            t_symbol* mode = atom_getsym(argv+1);
            if(mode == hoa_sym_velocity || mode == hoa_sym_energy)
            {
                x->f_mode = mode;
                nouts = 2ul;
            }
        }
        
        x->f_vector = new Vector<Hoa2d, t_sample>(channels);
        x->f_vector->computeRendering();
        
        dsp_setup((t_pxobject *)x, x->f_vector->getNumberOfPlanewaves());
        for (int i = 0; i < nouts; i++)
            outlet_new(x, "signal");
        
        x->f_ins = new t_sample[x->f_vector->getNumberOfPlanewaves() * HOA_MAXBLKSIZE];
        x->f_outs = new t_sample[nouts * HOA_MAXBLKSIZE];
        
        attr_args_process(x, argc, argv);
    }
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_2d_vector_main(void)
#else
void ext_main(void *r)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.vector~", (method)hoa_2d_vector_new, (method)hoa_2d_vector_free, (long)sizeof(t_hoa_2d_vector), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.vector~", (char *)"hoa.2d.vector~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of channels signals.
    // @description Array of channels signals.
    class_addmethod(c, (method)hoa_2d_vector_dsp64,	"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_vector_assist,   "assist",	A_CANT, 0);
    
    CLASS_ATTR_LONG                 (c, "channels", 0, t_hoa_2d_vector, f_attrs);
    CLASS_ATTR_CATEGORY             (c, "channels", 0, "Planewaves");
    CLASS_ATTR_LABEL                (c, "channels", 0, "Number of Channels");
    CLASS_ATTR_ACCESSORS            (c, "channels", channels_get, channels_set);
    CLASS_ATTR_ORDER                (c, "channels", 0, "1");
    // @description The number of channels.
    
    CLASS_ATTR_DOUBLE               (c, "offset", 0, t_hoa_2d_vector, f_attrs);
    CLASS_ATTR_CATEGORY             (c, "offset", 0, "Planewaves");
    CLASS_ATTR_LABEL                (c, "offset", 0, "Offset of Channels");
    CLASS_ATTR_ACCESSORS            (c, "offset", offset_get, offset_set);
    CLASS_ATTR_ORDER                (c, "offset", 0, "2");
    // @description The offset of the channels.
    
    CLASS_ATTR_FLOAT_VARSIZE        (c, "angles", 0, t_hoa_2d_vector, f_attrs, f_attrs, HOA_MAX_PLANEWAVES);
    CLASS_ATTR_CATEGORY             (c, "angles", 0, "Planewaves");
    CLASS_ATTR_LABEL                (c, "angles", 0, "Angles of Channels");
    CLASS_ATTR_ACCESSORS            (c, "angles", angles_get, angles_set);
    CLASS_ATTR_ORDER                (c, "angles", 0, "3");
    // @description The angles of the channels. Angles are in degree wrapped between 0. and 360.
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.vector~"));
    hoa_2d_vector_class = c;
}
