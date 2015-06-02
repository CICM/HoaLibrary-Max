/*
 // Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      hoa.3d.encoder~.cpp
 @name      hoa.3d.encoder~
 @realname  hoa.3d.encoder~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 3d ambisonic encoder.
 
 @description
 <o>hoa.3d.encoder~</o> encodes a signal into the spherical harmonics domain depending on a given order of decomposition, an azimuth and an elevation value given in radians
 
 @discussion
 <o>hoa.3d.encoder~</o> encodes a signal into the spherical harmonics domain depending on a given order of decomposition, an azimuth and an elevation value given in radians
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.encoder~, hoa.3d.decoder~, hoa.3d.map~, hoa.3d.optim~, hoa.3d.scope~, hoa.3d.wider~, hoa.pi, hoa.pi~
 */

#include "Hoa3D.max.h"

typedef struct _hoa_3d_encoder 
{
	t_pxobject                  f_ob;
    Encoder<Hoa3d, t_sample>::Basic*   f_encoder;
    t_sample*                   f_signals;
    
} t_hoa_3d_encoder;

t_class *hoa_3d_encoder_class;

t_hoa_err hoa_getinfos(t_hoa_3d_encoder* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_3D;
	boxinfos->autoconnect_inputs = 0;
	boxinfos->autoconnect_outputs = x->f_encoder->getNumberOfHarmonics();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	return HOA_ERR_NONE;
}

void hoa_3d_encoder_float(t_hoa_3d_encoder *x, double f)
{
    if(proxy_getinlet((t_object *)x) == 1)
    {
        x->f_encoder->setAzimuth(f);
    }
	else if(proxy_getinlet((t_object *)x) == 2)
    {
        x->f_encoder->setElevation(f);
    }
}

void hoa_3d_encoder_int(t_hoa_3d_encoder *x, long n)
{
	if(proxy_getinlet((t_object *)x) == 1)
    {
        x->f_encoder->setAzimuth(n);
    }
	else if(proxy_getinlet((t_object *)x) == 2)
    {
        x->f_encoder->setElevation(n);
    }
}

void hoa_encoder_3D_perform(t_hoa_3d_encoder *x, t_object *dsp, t_sample **ins, long nins, t_sample **outs, long numouts, long sampleframes, long f,void *up)
{
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_encoder->setAzimuth(ins[1][i]);
        x->f_encoder->setElevation(ins[2][i]);
        x->f_encoder->process(ins[0]+i, x->f_signals + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_signals+i, numouts, outs[i], 1);
    }
}

void hoa_encoder_3D_perform_azimuth(t_hoa_3d_encoder *x, t_object *dsp, t_sample **ins, long nins, t_sample **outs, long numouts, long sampleframes, long f,void *up)
{
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_encoder->setAzimuth(ins[1][i]);
        x->f_encoder->process(ins[0]+i, x->f_signals + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_signals+i, numouts, outs[i], 1);
    }
}

void hoa_encoder_3D_perform_elevation(t_hoa_3d_encoder *x, t_object *dsp, t_sample **ins, long nins, t_sample **outs, long numouts, long sampleframes, long f,void *up)
{
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_encoder->setElevation(ins[2][i]);
        x->f_encoder->process(ins[0]+i, x->f_signals + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_signals+i, numouts, outs[i], 1);
    }
}

void hoa_encoder_3D_perform_offset(t_hoa_3d_encoder *x, t_object *dsp, t_sample **ins, long nins, t_sample **outs, long numouts, long sampleframes, long f,void *up)
{
    for(long i = 0; i < sampleframes; i++)
    {
        x->f_encoder->process(ins[0]+i, x->f_signals + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(sampleframes, x->f_signals+i, numouts, outs[i], 1);
    }
}

void hoa_3d_encoder_dsp64(t_hoa_3d_encoder *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(count[1] && count[2])
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_encoder_3D_perform, 0, NULL);
    else if(count[1] && !count[2])
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_encoder_3D_perform_azimuth, 0, NULL);
    else if(!count[1] && count[2])
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_encoder_3D_perform_elevation, 0, NULL);
    else
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_encoder_3D_perform_offset, 0, NULL);
}

void hoa_3d_encoder_assist(t_hoa_3d_encoder *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET)
	{
        if(a == 0)
            sprintf(s,"(signal) Input");
        else if(a == 1)
            sprintf(s,"(signal or float) Azimuth");
        else if(a == 2)
            sprintf(s,"(signal or float) Elevation");
	}
	else 
	{
		sprintf(s,"(signal) %s", x->f_encoder->getHarmonicName(a).c_str());
	}
}


void hoa_3d_encoder_free(t_hoa_3d_encoder *x) 
{
	dsp_free((t_pxobject *)x);
	delete x->f_encoder;
    delete [] x->f_signals;
}

void *hoa_3d_encoder_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name ambisonic-order @optional 0 @type int @digest The ambisonic order of decomposition
    // @description First argument is the ambisonic order of decomposition.
    
    t_hoa_3d_encoder *x = NULL;
    x = (t_hoa_3d_encoder *)object_alloc(hoa_3d_encoder_class);
    if (x)
    {
        ulong order = 1;
        if(argc && atom_gettype(argv) == A_LONG)
            order = max<long>(atom_getlong(argv), 1);
        
        x->f_encoder = new Encoder<Hoa3d, t_sample>::Basic(order);
        
        x->f_ob.z_misc = Z_NO_INPLACE;
        
        dsp_setup((t_pxobject *)x, 3);
        for (int i = 0; i < x->f_encoder->getNumberOfHarmonics(); i++)
            outlet_new(x, "signal");
        
        x->f_signals =  new t_sample[x->f_encoder->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
    }
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_3d_encoder_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.3d.encoder~", (method)hoa_3d_encoder_new, (method)hoa_3d_encoder_free, (long)sizeof(t_hoa_3d_encoder), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.3d.encoder~", (char *)"hoa.3d.encoder~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method int @digest Set the azimuth or the elevation of the encoding in radians (between 0 and 2π)
    // In the second inlet : The <m>float</m> method sets the azimuth of the encoding.
    // In the third inlet : The <m>float</m> method the elevation of the encoding.
    // @marg 0 @name azimuth/elevation @optional 0 @type float
    class_addmethod(c, (method)hoa_3d_encoder_float,		"float",	A_FLOAT, 0);
    
    // @method int @digest Set the azimuth or the elevation of the encoding in radians (between 0 and 2π)
    // In the second inlet : The <m>int</m> method sets the azimuth of the encoding.
    // In the third inlet : The <m>int</m> method the elevation of the encoding.
    // @marg 0 @name azimuth/elevation @optional 0 @type int
    class_addmethod(c, (method)hoa_3d_encoder_int,         "int",		A_LONG, 0);
    
    // @method signal @digest Signal to encode or azimuth / elevation of the encoding in radians.
    // @description In the left inlet, the signal to encode.
    // In the second inlet : set the azimuth of the encoding in radians
    // In the third inlet : set the elevation of the encoding in radians
    class_addmethod(c, (method)hoa_3d_encoder_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_3d_encoder_assist,      "assist",	A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    hoa_3d_encoder_class = c;
    
    return 0;
}
