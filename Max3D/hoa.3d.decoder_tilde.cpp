/*
 // Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      hoa.3d.decoder~.cpp
 @name      hoa.3d.decoder~
 @realname  hoa.3d.decoder~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 3d ambisonic decoder.
 
 @description
 <o>hoa.3d.decoder~</o> decodes an ambisonics soundfield for several loudspeakers configuration or for headphones. First argument is the ambisonic order of decomposition. The number of channels, the offset, the angles can be set with attributes.
 
 @discussion
 <o>hoa.3d.decoder~</o> decodes an ambisonics soundfield for several loudspeakers configuration or for headphones. First argument is the ambisonic order of decomposition. The number of channels, the offset, the angles can be set with attributes.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.decoder~, hoa.3d.encoder~, hoa.3d.map~, hoa.3d.optim~, hoa.3d.scope~, hoa.3d.wider~, hoa.dac~
 */

#include "Hoa3D.max.h"

typedef struct _hoa_3d_decoder 
{
	t_pxobject                  f_ob;
    SharedPtr<Decoder<Hoa3d, float>>     f_decoder;
    float*                      f_ins;
    float*                      f_outs;
    float**                     f_ins_bin;
    float*                      f_outs_bin[2];
    long                        f_number_of_channels;
    double                      f_angles_of_channels[HOA_MAX_PLANEWAVES * 2];
    long                        f_number_of_angles;
    double                      f_offsets[3];
    t_symbol*                   f_mode;
    
} t_hoa_3d_decoder;

t_class *hoa_3d_decoder_class;

t_hoa_err hoa_getinfos(t_hoa_3d_decoder* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_3D;
	boxinfos->autoconnect_inputs = x->f_decoder->getNumberOfHarmonics();
	boxinfos->autoconnect_outputs = x->f_decoder->getNumberOfPlanewaves();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_3d_decoder_3D_perform64(t_hoa_3d_decoder *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    SharedPtr<Decoder<Hoa3d, float>> decoder = x->f_decoder;
    if(decoder->getMode() == Decoder<Hoa3d, float>::BinauralMode)
    {
        for(int i = 0; i < numins; i++)
        {
            for(ulong j = 0ul; j < sampleframes; j++)
            {
                x->f_ins_bin[i][j] = ins[i][j];
            }
        }
        (static_cast<Decoder<Hoa3d, float>::Binaural*>(decoder.get()))->processBlock((float const**)x->f_ins_bin, (float **)x->f_outs_bin);
        for(int i = 0; i < sampleframes; i++)
        {
            outs[0][i] = x->f_outs_bin[0][i];
            outs[1][i] = x->f_outs_bin[1][i];
        }
    }
    else
    {
        for(int i = 0; i < numins; i++)
        {
            ulong is = 0ul;
            ulong id = 0ul;
            float* dest = x->f_ins+i;
            for(ulong j = 0ul; j < sampleframes; j++)
            {
                dest[id] = ins[i][is];
                is += 1;
                id += numins;
            }
        }
        for(int i = 0; i < sampleframes; i++)
        {
            decoder->process(x->f_ins + numins * i, x->f_outs + numouts * i);
        }
        for(int i = 0; i < numouts; i++)
        {
            ulong is = 0ul;
            ulong id = 0ul;
            const float* src = x->f_outs+i;
            for(ulong j = 0ul; j < sampleframes; j++)
            {
                outs[i][id] = src[is];
                is += numouts;
                id += 1;
            }
        }
    }
}

void hoa_3d_decoder_dsp64(t_hoa_3d_decoder *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_decoder->computeRendering(maxvectorsize);
    for(ulong i = 0; i < x->f_decoder->getNumberOfHarmonics(); i++)
    {
        x->f_ins_bin[i] = x->f_ins+i*maxvectorsize;
    }
    x->f_outs_bin[0] = x->f_outs;
    x->f_outs_bin[1] = x->f_outs+maxvectorsize;
    
    object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_3d_decoder_3D_perform64, 0, NULL);
}

void hoa_3d_decoder_assist(t_hoa_3d_decoder *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET)
    {
		sprintf(s,"(signal) %s",x->f_decoder->getHarmonicName(a).c_str());
    }
	else
	{
		sprintf(s,"(signal) Channel %ld (%.1f° %.1f°)", a+1,
				x->f_decoder->getPlanewaveAzimuth(a) / HOA_2PI * 360,
				x->f_decoder->getPlanewaveElevation(a) / HOA_2PI * 360);
	}
}

void hoa_3d_decoder_resize_outlets(t_hoa_3d_decoder *x)
{
    t_object *b = NULL;
    object_obex_lookup(x, hoa_sym_pound_B, (t_object **)&b);
    
    if (b)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        object_method(b, hoa_sym_dynlet_begin);
        
        if(outlet_count((t_object *)x) > x->f_decoder->getNumberOfPlanewaves())
        {
            for(int i = outlet_count((t_object *)x); i > x->f_decoder->getNumberOfPlanewaves(); i--)
            {
                outlet_delete(outlet_nth((t_object*)x, i-1));
            }
        }
        else if(outlet_count((t_object *)x) < x->f_decoder->getNumberOfPlanewaves())
        {
            for(int i = outlet_count((t_object *)x); i < x->f_decoder->getNumberOfPlanewaves(); i++)
            {
                outlet_append((t_object*)x, NULL, hoa_sym_signal);
            }
        }
        
        object_method(b, hoa_sym_dynlet_end);
    }
}

t_max_err mode_set(t_hoa_3d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
	if(argc && argv && atom_gettype(argv) == A_SYM)
	{
        t_symbol* mode = atom_getsym(argv);
        
        if(mode == hoa_sym_ambisonic) // retro-compatibility
        {
            mode = hoa_sym_regular;
        }
        
        if (mode != x->f_mode)
        {
            if(mode == hoa_sym_regular)
            {
                object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
                
                ulong order = x->f_decoder->getDecompositionOrder();
                x->f_decoder = SharedPtr<Decoder<Hoa3d, float>>(new Decoder<Hoa3d, float>::Regular(order, (order+1)*(order+1)));
                
                x->f_mode = mode;
                object_attr_setdisabled((t_object *)x, hoa_sym_angles, 0);
                object_attr_setdisabled((t_object *)x, hoa_sym_offset, 0);
                object_attr_setdisabled((t_object *)x, hoa_sym_channels, 0);
                
                object_attr_setdouble_array(x, hoa_sym_offset, 3, x->f_offsets);
                x->f_number_of_channels = x->f_decoder->getNumberOfPlanewaves();
                object_attr_touch((t_object *)x, hoa_sym_channels);
                object_attr_touch((t_object *)x, hoa_sym_angles);
                hoa_3d_decoder_resize_outlets(x);
            }
            else if(mode == hoa_sym_binaural)
            {
                object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
                
                ulong order = x->f_decoder->getDecompositionOrder();
                x->f_decoder = SharedPtr<Decoder<Hoa3d, float>>(new Decoder<Hoa3d, float>::Binaural(order));
                
                x->f_mode = mode;
                object_attr_setdisabled((t_object *)x, hoa_sym_angles, 1);
                object_attr_setdisabled((t_object *)x, hoa_sym_offset, 1);
                object_attr_setdisabled((t_object *)x, hoa_sym_channels, 1);
                
                x->f_mode = mode;
                x->f_number_of_channels = 2;
                object_attr_touch((t_object *)x, hoa_sym_channels);
                object_attr_touch((t_object *)x, hoa_sym_angles);
                hoa_3d_decoder_resize_outlets(x);
            }
        }
    }
    return MAX_ERR_NONE;
}

t_max_err channel_set(t_hoa_3d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
	if(argc && argv && atom_isNumber(argv))
	{
        if (x->f_mode == hoa_sym_regular)
        {
            object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
            long channels = Math<long>::clip(atom_getlong(argv), 4, HOA_MAX_PLANEWAVES);
            ulong order = x->f_decoder->getDecompositionOrder();
            x->f_decoder = SharedPtr<Decoder<Hoa3d, float>>(new Decoder<Hoa3d, float>::Regular(order, channels));
            x->f_number_of_angles = x->f_decoder->getNumberOfPlanewaves() * 2;
        }
        
        x->f_number_of_channels = x->f_decoder->getNumberOfPlanewaves();
        
        object_attr_touch((t_object *)x, hoa_sym_angles);
        hoa_3d_decoder_resize_outlets(x);
    }
    return MAX_ERR_NONE;
}

t_max_err angles_set(t_hoa_3d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
		object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        
        for(int i = 1, j = 0; i < x->f_decoder->getNumberOfPlanewaves() * 2 && i < argc; i+= 2, j++)
        {
            if(atom_isNumber(argv+i-1) && atom_isNumber(argv+i))
			{
                x->f_decoder->setPlanewaveAzimuth(j, atom_getfloat(argv+i-1) / 360. * HOA_2PI);
                x->f_decoder->setPlanewaveElevation(j, atom_getfloat(argv+i) / 360. * HOA_2PI);
			}
        }
    }
    
    if (x->f_mode == hoa_sym_regular)
    {
        for(int i = 1, j = 0; i < x->f_decoder->getNumberOfPlanewaves() * 2 && i < argc; i+= 2, j++)
        {
            if(atom_isNumber(argv+i-1) && atom_isNumber(argv+i))
            {
                x->f_decoder->setPlanewaveAzimuth(j, atom_getfloat(argv+i-1) / 360. * HOA_2PI);
                x->f_decoder->setPlanewaveElevation(j, atom_getfloat(argv+i) / 360. * HOA_2PI);
            }
            
            x->f_angles_of_channels[i-1] = x->f_decoder->getPlanewaveAzimuth(j) / HOA_2PI * 360;
            x->f_angles_of_channels[i] = x->f_decoder->getPlanewaveElevation(j) / HOA_2PI * 360;
        }
    }
    else if (x->f_mode == hoa_sym_binaural)
    {
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
        {
            x->f_angles_of_channels[i] = x->f_decoder->getPlanewaveAzimuth(i) / HOA_2PI * 360.;
        }
    }
    
    return MAX_ERR_NONE;
}

t_max_err offset_set(t_hoa_3d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        
        double ax, ay, az;
        if(atom_isNumber(argv))
            ax = atom_getfloat(argv) / 360. * HOA_2PI;
        else
            ax = x->f_decoder->getPlanewavesRotationX();
        if(argc > 1 && atom_isNumber(argv+1))
            ay = atom_getfloat(argv+1) / 360. * HOA_2PI;
        else
            ay = x->f_decoder->getPlanewavesRotationY();
        if(argc > 2 &&  atom_isNumber(argv+2))
            az = atom_getfloat(argv+2) / 360. * HOA_2PI;
        else
            az = x->f_decoder->getPlanewavesRotationZ();
        
        x->f_decoder->setPlanewavesRotation(ax, ay, az);
    }
    
    x->f_offsets[0] = x->f_decoder->getPlanewavesRotationX() / HOA_2PI * 360.;
    x->f_offsets[1] = x->f_decoder->getPlanewavesRotationY() / HOA_2PI * 360.;
    x->f_offsets[2] = x->f_decoder->getPlanewavesRotationZ() / HOA_2PI * 360.;
    
    return MAX_ERR_NONE;
}

void hoa_3d_decoder_free(t_hoa_3d_decoder *x)
{
	dsp_free((t_pxobject *)x);
    delete [] x->f_ins;
    delete [] x->f_outs;
    delete [] x->f_ins_bin;
}

void *hoa_3d_decoder_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name decomposition-order @optional 0 @type int @digest The ambisonic order of decomposition
    // @description First argument is the ambisonic order of decomposition.
    
    t_hoa_3d_decoder *x = (t_hoa_3d_decoder *)object_alloc((t_class*)hoa_3d_decoder_class);
    if(x)
    {
        ulong order = 1;
        ulong number_of_channels = 4;
        
        if(argc && atom_gettype(argv) == A_LONG)
            order = max<long>(atom_getlong(argv), 1);
        
        if(argc > 1 && atom_gettype(argv+1) == A_LONG)
            number_of_channels	= max<long>(atom_getlong(argv+1), 2);
        else
            number_of_channels = (order+1)*(order+1);
        
        x->f_mode = hoa_sym_regular;
        x->f_decoder = SharedPtr<Decoder<Hoa3d, float>>(new Decoder<Hoa3d, float>::Regular(order, number_of_channels));
        
        x->f_number_of_angles = x->f_decoder->getNumberOfPlanewaves() * 2;
        x->f_number_of_channels = x->f_decoder->getNumberOfPlanewaves();
        
        dsp_setup((t_pxobject *)x, x->f_decoder->getNumberOfHarmonics());
        for (int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
            outlet_new(x, "signal");
        
        x->f_ins  = new float[x->f_decoder->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs = new float[HOA_MAX_PLANEWAVES * HOA_MAXBLKSIZE];
        x->f_ins_bin = new float*[x->f_decoder->getNumberOfHarmonics()];
        
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves() * 2; i+= 2)
        {
            x->f_angles_of_channels[i] = x->f_decoder->getPlanewaveAzimuth(i/2) / HOA_2PI * 360;
            x->f_angles_of_channels[i+1] = x->f_decoder->getPlanewaveElevation(i/2) / HOA_2PI * 360;
        }
        
        attr_args_process(x, argc, argv);
    }
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_3d_decoder_main(void)
#else
void ext_main(void *r)
#endif
{
    t_class *c;
    
    c = class_new("hoa.3d.decoder~", (method)hoa_3d_decoder_new, (method)hoa_3d_decoder_free, (long)sizeof(t_hoa_3d_decoder), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.3d.decoder~", (char *)"hoa.3d.decoder~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of spherical harmonic signals to decode for a set of loudspeakers.
    // @description Array of spherical harmonic signals to decode for a set of loudspeakers.
    class_addmethod(c, (method)hoa_3d_decoder_dsp64,             "dsp64",      A_CANT, 0);
    class_addmethod(c, (method)hoa_3d_decoder_assist,            "assist",     A_CANT, 0);
    
    CLASS_ATTR_SYM              (c, "mode", 0, t_hoa_3d_decoder, f_mode);
    CLASS_ATTR_LABEL            (c, "mode", 0, "Mode");
    CLASS_ATTR_ENUM             (c, "mode", 0, "ambisonic binaural");
    CLASS_ATTR_ACCESSORS		(c, "mode", NULL, mode_set);
    CLASS_ATTR_ORDER            (c, "mode", 0, "1");
    // @description There is two decoding <m>mode</m> :
    // <ul>
    // <li><b>regular</b> : for a regular or irregular loudspeakers repartition over a sphere.</li>
    // <li><b>binaural</b> : for headphones.</li>
    // </ul>
    
    CLASS_ATTR_DOUBLE_VARSIZE	(c, "angles", ATTR_SET_DEFER_LOW, t_hoa_3d_decoder, f_angles_of_channels, f_number_of_angles, HOA_MAX_PLANEWAVES*2);
    CLASS_ATTR_LABEL			(c, "angles", 0, "Angles of Channels");
    CLASS_ATTR_ACCESSORS		(c, "angles", NULL, angles_set);
    CLASS_ATTR_ORDER			(c, "angles", 0, "2");
    // @description Set the angles of each channels in degrees. The angles of channels are only settable in <b>irregular</b> <m>mode</m>. Each angles are in degrees, wrapped between 0. and 360. You must specify 2 values per channel corresponding to the azimuth value followed by the elevation value.
    
    CLASS_ATTR_DOUBLE_ARRAY     (c, "offset", ATTR_SET_DEFER_LOW, t_hoa_3d_decoder, f_offsets, 3);
    CLASS_ATTR_LABEL            (c, "offset", 0, "Offset of Channels");
    CLASS_ATTR_ACCESSORS		(c, "offset", NULL, offset_set);
    CLASS_ATTR_ORDER            (c, "offset", 0, "3");
    // @description Set the offsets of channels with a list of 2 float values corresponding to the azimuth and elevation offset, in degrees between 0. and 360.
    
    CLASS_ATTR_LONG             (c, "channels", ATTR_SET_DEFER_LOW, t_hoa_3d_decoder, f_number_of_channels);
    CLASS_ATTR_LABEL            (c, "channels", 0, "Number of Channels");
    CLASS_ATTR_ACCESSORS		(c, "channels", NULL, channel_set);
    CLASS_ATTR_ORDER            (c, "channels", 0, "2");
    // @description The number of channels.
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);	
    hoa_3d_decoder_class = c;
}