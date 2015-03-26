/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.map~.cpp
 @name      hoa.2d.map~
 @realname  hoa.2d.map~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 2d ambisonic multisource spatializer.
 
 @description
 <o>hoa.2d.map~</o> is a tool that encodes several sources in the circular harmonics domain with distance compensation and muting system. It's easy to use and works with <o>hoa.2d.map</o>. You could look at <o>hoa.2d.map</o>'s help for futher informations.
 
 @discussion
 <o>hoa.2d.map~</o> is a tool that encodes several sources in the circular harmonics domain with distance compensation and muting system. It's easy to use and works with <o>hoa.2d.map</o>. You could look at <o>hoa.2d.map</o>'s help for futher informations.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.map, hoa.2d.encoder~, hoa.2d.wider~, hoa.2d.scope~, hoa.2d.decoder~
 */

#include "Hoa2D.max.h"

#define MAX_NUMBER_OF_SOURCES 64

typedef struct _hoa_map_tilde
{
	t_pxobject                          f_ob;
    Encoder<Hoa2d, t_sample>::Multi*    f_map;
    PolarLines<Hoa2d, t_sample>*        f_lines;
    t_sample*                           f_sig_ins;
    t_sample*                           f_sig_outs;
    t_sample*                           f_lines_vector;
    int                             	f_mode;
    double                          	f_ramp;
    
} t_hoa_map_tilde;

t_class *hoa_map_tilde_class;

void *hoa_map_tilde_new(t_symbol *s, long argc, t_atom *argv)
{
	// @arg 0 @name ambisonic-order @optional 0 @type int @digest The ambisonic order of decomposition
	// @description First argument is the ambisonic order of decomposition.
	
	// @arg 1 @name number-of-sources @optional 0 @type int @digest The number of sources
	// @description Second argument is the number of sources to spatialize.
	// If there is a single source, <o>hoa.2d.map~</o> object owns 3 inlets, the first one correspond to the signal to encode, the two other ones are usefull  to control source position at signal rate. If you have more than one source to spatialize, the number of signal inlets will be equal to the number of sources to encode, and coordinates will be given with list messages.
	
    t_hoa_map_tilde *x = (t_hoa_map_tilde *)object_alloc(hoa_map_tilde_class);
	if (x)
	{
        ulong order = 1;
        ulong numberOfSources = 1;
        
		x->f_ramp       = 100;
        x->f_mode       = 0; // polar
		
		if(argc && atom_gettype(argv) == A_LONG)
			order = max<long>(atom_getlong(argv), 1);
        
        if(argc > 1 && atom_gettype(argv+1) == A_LONG)
            numberOfSources = Math<long>::clip(atom_getlong(argv+1), 1, HOA_MAX_PLANEWAVES);
		
		if(argc > 2 && atom_gettype(argv+2) == A_SYM)
		{
			if(atom_getsym(argv+2) == hoa_sym_cartesian)
				x->f_mode = 1;
		}
        
        x->f_ramp       = 100;
        x->f_map        = new Encoder<Hoa2d, t_sample>::Multi(order, numberOfSources);
        x->f_lines      = new PolarLines<Hoa2d, t_sample>(x->f_map->getNumberOfSources());
        x->f_lines->setRamp(0.1 * sys_getsr());
		
        for(ulong i = 0; i < x->f_map->getNumberOfSources(); i++)
        {
            x->f_lines->setRadiusDirect(i, 1);
            x->f_lines->setAzimuthDirect(i, 0.);
        }
        
        if(x->f_map->getNumberOfSources() == 1)
        {
            x->f_sig_ins = new t_sample[3 * HOA_MAXBLKSIZE];
            dsp_setup((t_pxobject *)x, 3);
        }
        else
        {
            x->f_sig_ins = new t_sample[x->f_map->getNumberOfSources() * HOA_MAXBLKSIZE];
            dsp_setup((t_pxobject *)x, x->f_map->getNumberOfSources());
            
        }
        
        for (ulong i = 0; i < x->f_map->getNumberOfHarmonics(); i++)
            outlet_new(x, "signal");
        
        x->f_sig_outs       = new t_sample[x->f_map->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_lines_vector   = new t_sample[x->f_map->getNumberOfSources() * 2];
        
        attr_args_process(x, argc, argv);
	}
	
	return (x);
}

void hoa_map_tilde_float(t_hoa_map_tilde *x, double f)
{
    if(x->f_map->getNumberOfSources() == 1)
    {
        if(x->f_mode == 0)
        {
            if(proxy_getinlet((t_object *)x) == 1)
            {
                x->f_lines->setRadius(0, max<t_sample>(f, 0.));
            }
            else if(proxy_getinlet((t_object *)x) == 2)
            {
                x->f_lines->setAzimuth(0, f);
            }
        }
        else if(x->f_mode == 1) // cartesian
        {
            if(proxy_getinlet((t_object *)x) == 1)
            {
                float ord = Math<t_sample>::ordinate(x->f_lines->getRadius(0), x->f_lines->getAzimuth(0));
                x->f_lines->setRadius(0, Math<t_sample>::radius(f, ord));
                x->f_lines->setAzimuth(0, Math<t_sample>::azimuth(f, ord));
            }
            else if(proxy_getinlet((t_object *)x) == 2)
            {
                float abs = Math<t_sample>::abscissa(x->f_lines->getRadius(0), x->f_lines->getAzimuth(0));
                x->f_lines->setRadius(0, Math<t_sample>::radius(abs, f));
                x->f_lines->setAzimuth(0, Math<t_sample>::azimuth(abs, f));
            }
        }
    }
}

void hoa_map_tilde_int(t_hoa_map_tilde *x, long n)
{
	hoa_map_tilde_float(x, (double) n);
}

void hoa_map_tilde_list(t_hoa_map_tilde *x, t_symbol* s, long argc, t_atom* argv)
{
    if(argc > 2 && argv && atom_gettype(argv) == A_LONG && atom_gettype(argv+1) == A_SYM)
    {
        int index = atom_getlong(argv) -1;
        if(index < 0 || index >= x->f_map->getNumberOfSources())
            return;
        
        if(argc > 3 && (atom_getsym(argv+1) == hoa_sym_polar || atom_getsym(argv+1) == hoa_sym_pol))
        {
            x->f_lines->setRadius(index, atom_getfloat(argv+2));
            x->f_lines->setAzimuth(index, atom_getfloat(argv+3));
        }
        else if(argc > 3 && (atom_getsym(argv+1) == hoa_sym_cartesian || atom_getsym(argv+1) == hoa_sym_car))
        {
            x->f_lines->setRadius(index, Math<t_sample>::radius(atom_getfloat(argv+2), atom_getfloat(argv+3)));
            x->f_lines->setAzimuth(index, Math<t_sample>::azimuth(atom_getfloat(argv+2), atom_getfloat(argv+3)));
        }
        else if(argc > 2 && atom_getsym(argv+1) == hoa_sym_mute)
        {
            x->f_map->setMute(index, atom_getlong(argv+2));
        }
    }
}

void hoa_map_tilde_cartesian(t_hoa_map_tilde *x)
{
	x->f_mode = 1;
}

void hoa_map_tilde_polar(t_hoa_map_tilde *x)
{
	x->f_mode = 0;
}

t_max_err ramp_set(t_hoa_map_tilde *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_isNumber(argv))
    {
        x->f_ramp = max<double>(atom_getfloat(argv), 0);
        x->f_lines->setRamp(x->f_ramp / 1000. * sys_getsr());
    }
    
    return MAX_ERR_NONE;
}

void hoa_map_tilde_perform_multisources(t_hoa_map_tilde *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    long nsources = x->f_map->getNumberOfSources();
    for(long i = 0; i < numins; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_sig_ins+i, numins);
    }
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_lines->process(x->f_lines_vector);
        for(long j = 0; j < nsources; j++)
            x->f_map->setRadius(j, x->f_lines_vector[j]);
        for(long j = 0; j < nsources; j++)
            x->f_map->setAzimuth(j, x->f_lines_vector[j + nsources]);
        
        x->f_map->process(x->f_sig_ins + numins * i, x->f_sig_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_perform(t_hoa_map_tilde *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_lines->process(x->f_lines_vector);
        x->f_map->setRadius(0, x->f_lines_vector[0]);
        x->f_map->setAzimuth(0, x->f_lines_vector[1]);
        x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_perform_in1(t_hoa_map_tilde *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    if(!x->f_mode)
    {
        for(long i = 0; i < sampleframes; i++)
        {
            x->f_lines->process(x->f_lines_vector);
            x->f_map->setRadius(0, ins[1][i]);
            x->f_map->setAzimuth(0, x->f_lines_vector[1]);
            x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
        }
    }
    else
    {
        for(long i = 0; i < sampleframes; i++)
        {
            x->f_lines->process(x->f_lines_vector);
            x->f_map->setAzimuth(0, Math<t_sample>::azimuth(ins[1][i], x->f_lines_vector[1]));
            x->f_map->setRadius(0, Math<t_sample>::radius(ins[1][i], x->f_lines_vector[1]));
            x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
        }
    }
    for(long i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_perform_in2(t_hoa_map_tilde *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    if(!x->f_mode)
    {
        for(long i = 0; i < sampleframes; i++)
        {
            x->f_lines->process(x->f_lines_vector);
            x->f_map->setRadius(0, x->f_lines_vector[0]);
            x->f_map->setAzimuth(0, ins[2][i]);
            x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
        }
    }
    else
    {
        for(long i = 0; i < sampleframes; i++)
        {
            x->f_lines->process(x->f_lines_vector);
            x->f_map->setAzimuth(0, Math<t_sample>::azimuth(x->f_lines_vector[0], ins[2][i]));
            x->f_map->setRadius(0, Math<t_sample>::radius(x->f_lines_vector[0], ins[2][i]));
            x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
        }
    }
    for(long i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_perform_in1_in2(t_hoa_map_tilde *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    if(!x->f_mode)
    {
        for(long i = 0; i < sampleframes; i++)
        {
            x->f_map->setRadius(0, ins[1][i]);
            x->f_map->setAzimuth(0, ins[2][i]);
            x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
        }
    }
    else
    {
        for(long i = 0; i < sampleframes; i++)
        {
            x->f_map->setAzimuth(0, Math<t_sample>::azimuth(ins[1][i], ins[2][i]));
            x->f_map->setRadius(0, Math<t_sample>::radius(ins[1][i], ins[2][i]));
            x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
            
        }
    }
    for(long i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_dsp64(t_hoa_map_tilde *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_lines->setRamp(x->f_ramp / 1000. * samplerate);
    if(x->f_map->getNumberOfSources() == 1)
    {
        if(count[1] && count[2])
            object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform_in1_in2, 0, NULL);
        else if(count[1] && !count[2])
            object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform_in1, 0, NULL);
        else if(!count[1] && count[2])
            object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform_in2, 0, NULL);
        else if(!count[1] && !count[2])
            object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform, 0, NULL);
    }
    else
    {
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform_multisources, 0, NULL);
    }
}

t_hoa_err hoa_getinfos(t_hoa_map_tilde* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	
	if(x->f_map->getNumberOfSources() == 1)
		boxinfos->autoconnect_inputs = 1;
	else
		boxinfos->autoconnect_inputs = x->f_map->getNumberOfSources();
	
	boxinfos->autoconnect_outputs = x->f_map->getNumberOfHarmonics();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	return HOA_ERR_NONE;
}

void hoa_map_tilde_assist(t_hoa_map_tilde *x, void *b, long m, long a, char *s)
{
    if(m == ASSIST_INLET)
	{
        if(x->f_map->getNumberOfSources() == 1)
        {
            if(a == 0)
                sprintf(s,"(signal) Source 0");
            else if(a == 1)
				sprintf(s,"(signal/float) %s", (x->f_mode == 0) ? "Radius" : "Abscissa");
			else if(a == 2)
				sprintf(s,"(signal/float) %s", (x->f_mode == 0) ? "Azimuth" : "Ordinate");
        }
        else
        {
            if(a == 0)
                sprintf(s,"(signal/messages) Source 1 and sources messages");
            else
                sprintf(s,"(signal) Source %ld", a+1);
        }
        
	}
	else 
	{
		sprintf(s,"(signal) %s", x->f_map->getHarmonicName(a).c_str());
	}
}


void hoa_map_tilde_free(t_hoa_map_tilde *x) 
{
	dsp_free((t_pxobject *)x);
    delete x->f_lines;
    delete x->f_map;
    delete [] x->f_sig_ins;
    delete [] x->f_sig_outs;
    delete [] x->f_lines_vector;
}

#ifdef HOA_PACKED_LIB
int hoa_2d_map_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.map~", (method)hoa_map_tilde_new, (method)hoa_map_tilde_free, (long)sizeof(t_hoa_map_tilde), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.map~", (char *)"hoa.2d.map~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Sources signals to encode.
    // @description If you have a single source, the first signal inlet is for the source to encode, the two other ones are to control source position at signal rate. if you have more than one single source to spatialize, all of the inputs represent a signal to encode and coordonates are given with messages.
    class_addmethod(c, (method)hoa_map_tilde_dsp64,		"dsp64",	A_CANT, 0);
    
    // @method float @digest Set single source coordinate with messages depending on the mode.
    // @description Set single source coordinate with messages depending on the mode.
    // @marg 0 @name coord @optional 0 @type float
    class_addmethod(c, (method)hoa_map_tilde_float,     "float",	A_FLOAT, 0);
    
    // @method float @digest Set single source coordinate with messages depending on the mode.
    // @description Set single source coordinate with messages depending on the mode.
    // @marg 0 @name coord @optional 0 @type float
    class_addmethod(c, (method)hoa_map_tilde_int,       "int",		A_LONG, 0);
    
    // @method cartesian @digest Swith the input mode to cartesian (default).
    // @description The <m>cartesian</m> message swith the input mode to cartesian
    class_addmethod(c, (method)hoa_map_tilde_cartesian, "cartesian",A_NOTHING, 0);
    
    // @method polar @digest Swith the input mode to polar.
    // @description The <m>polar</m> message swith the input mode to polar
    class_addmethod(c, (method)hoa_map_tilde_polar,     "polar",	A_NOTHING, 0);
    
    // @method list @digest Send source messages like coordinates and mute state.
    // @description Send source messages like coordinates and mute state. The list must be formatted like this : source-index input-mode radius/abscissa azimuth/ordinate to set source positions or like this : source-index 'mute' mute-state to set the mute state of a source.
    // marg 0 @name source-index @optional 0 @type int
    // marg 1 @name input-mode/mute @optional 0 @type symbol
    // marg 2 @name coord-1/mute-state @optional 0 @type float/int
    // marg 3 @name coord-2 @optional 0 @type float
    class_addmethod(c, (method)hoa_map_tilde_list,       "list",	A_GIMME, 0);
    class_addmethod(c, (method)hoa_map_tilde_assist,     "assist",	A_CANT, 0);
    
    CLASS_ATTR_DOUBLE           (c, "ramp", 0, t_hoa_map_tilde, f_ramp);
    CLASS_ATTR_CATEGORY			(c, "ramp", 0, "Behavior");
    CLASS_ATTR_LABEL			(c, "ramp", 0, "Ramp Time (ms)");
    CLASS_ATTR_ACCESSORS		(c, "ramp", NULL, ramp_set);
    CLASS_ATTR_ORDER			(c, "ramp", 0, "1");
    // @description The ramp time in milliseconds.
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.map~"));
    hoa_map_tilde_class = c;
    return 0;
}
