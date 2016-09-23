/*
// Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.3d.vector~.cpp
 @name      hoa.3d.vector~
 @realname  hoa.3d.vector~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 3d soundfield energy and velocity vectors calculator.
 
 @description
 <o>hoa.3d.vector~</o> computes the coordinates of the velocity vector and the energy vector that characterize a sound field. It's an useful tool to analyse restitution quality. For futher information see <b>Michael A. Gerzon, General metatheorie of auditory localisation. Audio Engineering Society Preprint, 3306, 1992.</b>
 
 @discussion
 <o>hoa.3d.vector~</o> computes the coordinates of the velocity vector and the energy vector that characterize a sound field. It's an useful tool to analyse restitution quality. For futher information see <b>Michael A. Gerzon, General metatheorie of auditory localisation. Audio Engineering Society Preprint, 3306, 1992.</b>
 
 @category 3d, Planewaves, MSP
 
 @seealso hoa.2d.vector~, hoa.3d.meter~, hoa.3d.scope~, hoa.3d.optim~, hoa.3d.map~, hoa.3d.encoder~, hoa.3d.decoder~, hoa.3d.wider~
 */

#include "Hoa3D.max.h"

typedef struct _hoa_3d_vector 
{
	t_pxobject              f_ob;
    Vector<Hoa3d, t_sample>*f_vector;
    t_sample*               f_sig_ins;
    t_sample*               f_sig_outs;
    t_symbol*               f_mode;
	double					f_angles_of_channels[HOA_MAX_PLANEWAVES * 2];
	long					f_number_of_angles;
    
} t_hoa_3d_vector;

t_class *hoa_3d_vector_class;

t_hoa_err hoa_getinfos(t_hoa_3d_vector* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_3D;
	boxinfos->autoconnect_inputs = x->f_vector->getNumberOfPlanewaves();
	boxinfos->autoconnect_outputs = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_3d_vector_perform(t_hoa_3d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_sig_ins+i, numins);
    }
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_vector->process(x->f_sig_ins + i * numins, x->f_sig_outs + i * numouts);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_3d_vector_perform_velocity(t_hoa_3d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_sig_ins+i, numins);
    }
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_vector->processVelocity(x->f_sig_ins + i * numins, x->f_sig_outs + i * numouts);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_3d_vector_perform_energy(t_hoa_3d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_sig_ins+i, numins);
    }
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_vector->processEnergy(x->f_sig_ins + i * numins, x->f_sig_outs + i * numouts);
    }
    for(int i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_3d_vector_dsp64(t_hoa_3d_vector *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(x->f_mode == hoa_sym_both)
        object_method(dsp64, gensym("dsp_add64"), x, hoa_3d_vector_perform, 0, NULL);
    else if(x->f_mode == hoa_sym_velocity)
        object_method(dsp64, gensym("dsp_add64"), x, hoa_3d_vector_perform_velocity, 0, NULL);
    else
        object_method(dsp64, gensym("dsp_add64"), x, hoa_3d_vector_perform_energy, 0, NULL);
}

void hoa_3d_vector_assist(t_hoa_3d_vector *x, void *b, long m, long a, char *s)
{
    if(m == ASSIST_INLET)
    {
        sprintf(s,"(signal) %s", x->f_vector->getPlanewaveName(a).c_str());
    }
    else
    {
        const int both = (x->f_mode == hoa_sym_both) ? 3 : 0;
        
        if(both || x->f_mode == hoa_sym_velocity)
        {
            if(a == 0)
                sprintf(s,"(signal) Velocity Vector Abscissa");
            else if(a == 1)
                sprintf(s,"(signal) Velocity Vector Ordinate");
            else if(a == 2)
                sprintf(s,"(signal) Velocity Vector Height");
        }
        
        if(both || x->f_mode == hoa_sym_energy)
        {
            if(a == 0 + both)
                sprintf(s,"(signal) Energy Vector Abscissa");
            else if(a == 1 + both)
                sprintf(s,"(signal) Energy Vector Ordinate");
            else if(a == 2 + both)
                sprintf(s,"(signal) Energy Vector Height");
        }
    }
}

t_max_err angles_set(t_hoa_3d_vector *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        
        for(ulong i = 0; i < x->f_vector->getNumberOfPlanewaves() * 2 && i < argc; i++)
        {
            if(atom_isNumber(argv+i))
            {
                if(i%2)
                {
                    x->f_vector->setPlanewaveElevation((i-1)/2, atom_getfloat(argv+i) / 360.f * HOA_2PI);
                }
                else
                {
                    x->f_vector->setPlanewaveAzimuth(i/2, atom_getfloat(argv+i) / 360.f * HOA_2PI);
                }
            }
        }
        
        x->f_vector->computeRendering();
    }
    
    for(int i = 1, j = 0; i < x->f_vector->getNumberOfPlanewaves() * 2; i+= 2, j++)
    {
        x->f_angles_of_channels[i-1] = x->f_vector->getPlanewaveAzimuth(j) / HOA_2PI * 360;
        x->f_angles_of_channels[i] = x->f_vector->getPlanewaveElevation(j) / HOA_2PI * 360;
    }
    
    return MAX_ERR_NONE;
}

void hoa_3d_vector_free(t_hoa_3d_vector *x)
{
    dsp_free((t_pxobject *)x);
    delete x->f_vector;
    delete [] x->f_sig_ins;
    delete [] x->f_sig_outs;
}

void *hoa_3d_vector_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name number-of-channels @optional 0 @type int @digest The number of channels
    // @description First argument sets the number of channels.
    
    // @arg 1 @name vector-type @optional 1 @type symbol @digest The vector type.
    // @description second argument sets the type of vector to compute (velocity or energy)
    
    t_hoa_3d_vector *x = (t_hoa_3d_vector *)object_alloc(hoa_3d_vector_class);
    if (x)
    {
        int	channels = 4;
        
        x->f_mode = hoa_sym_both;
        ulong nouts = 6ul;
        
        if(argc && atom_gettype(argv) == A_LONG)
            channels = max<long>(atom_getlong(argv), 4);
        
        if(argc > 1 && atom_gettype(argv+1) == A_SYM)
        {
            t_symbol* mode = atom_getsym(argv+1);
            if(mode == hoa_sym_velocity || mode == hoa_sym_energy)
            {
                x->f_mode = mode;
                nouts = 3ul;
            }
        }
        
        x->f_vector = new Vector<Hoa3d, t_sample>(channels);
        x->f_vector->computeRendering();
        
        for (ulong i = 0; i < nouts; i++)
            outlet_new(x, "signal");
        
        dsp_setup((t_pxobject *)x, x->f_vector->getNumberOfPlanewaves());
        x->f_sig_ins =  new t_sample[x->f_vector->getNumberOfPlanewaves() * SYS_MAXBLKSIZE];
        x->f_sig_outs = new t_sample[nouts * SYS_MAXBLKSIZE];
        
        x->f_number_of_angles = x->f_vector->getNumberOfPlanewaves() * 2;
        
        for(int i = 1, j = 0; i < x->f_vector->getNumberOfPlanewaves() * 2; i+= 2, j++)
        {
            x->f_angles_of_channels[i-1] = x->f_vector->getPlanewaveAzimuth(j) / HOA_2PI * 360;
            x->f_angles_of_channels[i] = x->f_vector->getPlanewaveElevation(j) / HOA_2PI * 360;
        }
        
        attr_args_process(x, argc, argv);
    }
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_3d_vector_main(void)
#else
void ext_main(void *r)
#endif
{
    
    t_class *c;
    
    c = class_new("hoa.3d.vector~", (method)hoa_3d_vector_new, (method)hoa_3d_vector_free, (long)sizeof(t_hoa_3d_vector), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.3d.vector~", (char *)"hoa.3d.vector~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of channels signals.
    // @description Array of channels signals.
    class_addmethod(c, (method)hoa_3d_vector_dsp64,     "dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_3d_vector_assist,    "assist",	A_CANT, 0);
    
    CLASS_ATTR_DOUBLE_VARSIZE	(c, "angles", ATTR_SET_DEFER_LOW, t_hoa_3d_vector, f_angles_of_channels, f_number_of_angles, HOA_MAX_PLANEWAVES*2);
    CLASS_ATTR_LABEL			(c, "angles", 0, "Angles of Planewaves");
    CLASS_ATTR_ACCESSORS		(c, "angles", NULL, angles_set);
    CLASS_ATTR_ORDER			(c, "angles", 0, "2");
    // @description Set the angles of each channels in degrees. Each angles are in degrees, wrapped between 0. and 360. You must specify 2 values per channel corresponding to the azimuth value followed by the elevation value.
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    hoa_3d_vector_class = c;
}