/*
// Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.decoder~.cpp
 @name      hoa.2d.decoder~
 @realname  hoa.2d.decoder~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 2d ambisonic decoder.
 
 @description
 <o>hoa.2d.decoder~</o> decodes an ambisonics soundfield for several loudspeakers configuration or for headphones. First argument is the ambisonic order of decomposition.
 
 @discussion
<o>hoa.2d.decoder~</o> decodes an ambisonics soundfield for several loudspeakers configuration or for headphones. First argument is the ambisonic order of decomposition.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.meter~, hoa.2d.scope~, hoa.dac~, hoa.2d.vector~, hoa.2d.optim~, hoa.2d.map~, hoa.2d.encoder~, hoa.3d.decoder~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_2d_decoder 
{
	t_pxobject                  f_ob;
    Decoder<Hoa2d, t_sample>*   f_decoder;
    t_sample*                   f_ins;
    t_sample*                   f_outs;
    long                        f_number_of_channels;
    double                      f_angles_of_channels[HOA_MAX_PLANEWAVES];
    double                      f_offset;
    t_symbol*                   f_mode;
    char                        f_send_config;
    
} t_hoa_2d_decoder;

t_class *hoa_2d_decoder_class;

void hoa_2d_decoder_free(t_hoa_2d_decoder *x)
{
    dsp_free((t_pxobject *)x);
    delete x->f_decoder;
    delete [] x->f_ins;
    delete [] x->f_outs;
}

t_hoa_err hoa_getinfos(t_hoa_2d_decoder* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs = x->f_decoder->getNumberOfHarmonics();
	boxinfos->autoconnect_outputs = x->f_decoder->getNumberOfPlanewaves();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_2d_decoder_perform(t_hoa_2d_decoder *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
    
	for(int i = 0; i < sampleframes; i++)
    {
        x->f_decoder->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    
    for(int i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

static void hoa_2d_decoder_perform_binaural(t_hoa_2d_decoder *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    const long max = numins < 11 ? numins : 11;
    for(long i = 0; i < max; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_ins+i, max);
    }
    for(long i = 0; i < sampleframes; i++)
    {
        (static_cast<Decoder<Hoa2d, t_sample>::Binaural*>(x->f_decoder))->process(x->f_ins + max * i, x->f_outs + 2 * i);
    }
    for(long i = 0; i < 2; i++)
    {
        cblas_dcopy(sampleframes, x->f_outs+i, 2, outs[i], 1);
    }
}


void hoa_2d_decoder_dsp64(t_hoa_2d_decoder *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_decoder->computeRendering(maxvectorsize);
    
    if(x->f_mode == hoa_sym_binaural)
    {
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_2d_decoder_perform_binaural, 0, NULL);
    }
    else
    {
        object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_2d_decoder_perform, 0, NULL);
    }
}

void hoa_2d_decoder_assist(t_hoa_2d_decoder *x, void *b, long m, long a, char *s)
{
    if(m == ASSIST_INLET)
        sprintf(s,"(signal) %s", x->f_decoder->getHarmonicName(a).c_str());
    else
        sprintf(s,"(signal) %s", x->f_decoder->getPlanewaveName(a).c_str());
}

void send_configuration(t_hoa_2d_decoder *x)
{
    if(!x->f_send_config)
        return;
    
    t_object *patcher;
    t_object *decoder;
    t_object *line;
    t_max_err err;
    t_atom msg[4];
    t_atom rv;
    
    err = object_obex_lookup(x, hoa_sym_pound_P, (t_object **)&patcher);
    if (err != MAX_ERR_NONE)
        return;
    
    err = object_obex_lookup(x, hoa_sym_pound_B, (t_object **)&decoder);
    if (err != MAX_ERR_NONE)
        return;
    
    t_atom nchannels;
    t_atom offset;
    t_atom *argv = new t_atom[x->f_decoder->getNumberOfPlanewaves()];
    if(argv)
    {
        atom_setlong(&nchannels, x->f_decoder->getNumberOfPlanewaves());
        atom_setfloat(&offset, wrap(x->f_decoder->getPlanewavesRotationX() / HOA_2PI * 360., -180., 180.));
        
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
            atom_setfloat(argv+i, x->f_decoder->getPlanewaveAzimuth(i) / HOA_2PI * 360.);
        
        vector<t_jbox *> boxes;
        
        for (line = jpatcher_get_firstline(patcher); line; line = jpatchline_get_nextline(line))
        {
            if (jpatchline_get_box1(line) == decoder)
            {
                t_jbox *box = (t_jbox*)jpatchline_get_box2(line);
                t_object *obj = jbox_get_object((t_object*)box);
                t_symbol* classname = object_classname(obj);
                
                if(classname == hoa_sym_hoa_2d_meter ||
                   classname == hoa_sym_hoa_2d_vector ||
                   classname == hoa_sym_hoa_gain ||
                   classname == hoa_sym_hoa_dac ||
                   classname == hoa_sym_dac)
                {
                    if (find(boxes.begin(), boxes.end(), box) != boxes.end())
                    {
                        continue;
                    }
                    
                    if (classname == hoa_sym_hoa_2d_meter ||
                        classname == hoa_sym_hoa_2d_vector ||
                        classname == hoa_sym_hoa_gain)
                    {
                        object_method_typed(obj, hoa_sym_channels, 1, &nchannels, NULL);
                        object_method_typed(obj, hoa_sym_angles, x->f_decoder->getNumberOfPlanewaves(), argv, NULL);
                        object_method_typed(obj, hoa_sym_offset, 1, &offset, NULL);
                    }
                    
                    boxes.push_back(box);
                }
            }
        }
        
        for (auto box : boxes)
        {
            // re-connect patchlines
            for(int i = 0; jbox_getinlet(box, i) != NULL && i < x->f_decoder->getNumberOfPlanewaves(); i++)
            {
                atom_setobj(msg, decoder);
                atom_setlong(msg + 1, i);
                atom_setobj(msg + 2, box);
                atom_setlong(msg + 3, i);
                object_method_typed(patcher , hoa_sym_connect, 4, msg, &rv);
            }
        }
        
        boxes.clear();
        free(argv);
    }
}

void hoa_decoder_resize_outlets(t_hoa_2d_decoder *x)
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

void hoa_decoder_config(t_hoa_2d_decoder *x, t_symbol* mode, long channels = 0, double offset = 0., long n_angles = 0, t_atom *angles = NULL)
{
    bool modeChanged    = false;
    const t_symbol* lastMode = x->f_mode;
    const long order = x->f_decoder->getDecompositionOrder();
    
    if(lastMode != mode && (mode == hoa_sym_ambisonic || mode == hoa_sym_irregular || mode == hoa_sym_binaural))
    {
        x->f_mode = mode;
        channels = order*2+2;
        modeChanged = true;
    }
    
    if (modeChanged || channels != x->f_decoder->getNumberOfPlanewaves())
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        
        if(mode == hoa_sym_ambisonic)
        {
            delete x->f_decoder;
            x->f_decoder = new Decoder<Hoa2d, t_sample>::Regular(order, Math<long>::clip(channels, order*2+1, HOA_MAX_PLANEWAVES));
            x->f_decoder->computeRendering(sys_getblksize());
        }
        else if(mode == hoa_sym_irregular)
        {
            delete x->f_decoder;
            x->f_decoder = new Decoder<Hoa2d, t_sample>::Irregular(order, Math<long>::clip(channels, 2, HOA_MAX_PLANEWAVES));
        }
        else if(modeChanged && mode == hoa_sym_binaural)
        {
            delete x->f_decoder;
            x->f_decoder = new Decoder<Hoa2d, t_sample>::Binaural(order);
        }
        
        x->f_decoder->computeRendering(sys_getblksize());
        
        x->f_offset = offset = 0.;
        x->f_number_of_channels = x->f_decoder->getNumberOfPlanewaves();
        
        n_angles = 0;
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
        {
            x->f_angles_of_channels[i] = x->f_decoder->getPlanewaveAzimuth(i) / HOA_2PI * 360.;
        }
        
        object_attr_touch((t_object*)x, hoa_sym_angles);
        object_attr_touch((t_object*)x, hoa_sym_offset);
        object_attr_touch((t_object*)x, hoa_sym_channels);
        
        object_attr_setdisabled((t_object *)x, hoa_sym_angles, (mode != hoa_sym_irregular));
        object_attr_setdisabled((t_object *)x, hoa_sym_channels, (mode == hoa_sym_binaural));
        object_attr_setdisabled((t_object *)x, hoa_sym_offset, (mode == hoa_sym_binaural));
        
        hoa_decoder_resize_outlets(x);
    }
    
    if(mode != hoa_sym_binaural)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        
        offset = Math<double>::wrap_twopi(wrap(offset, -180., 180.) / 360. * HOA_2PI);
        x->f_decoder->setPlanewavesRotation(offset, 0., 0.);
        x->f_offset = wrap(x->f_decoder->getPlanewavesRotationX() / HOA_2PI * 360.f, -180, 180);
        object_attr_touch((t_object*)x, hoa_sym_offset);
    }
    
    if(n_angles > 0 && mode == hoa_sym_irregular)
    {
        object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        
        for(int i = 0; i < n_angles && i < x->f_decoder->getNumberOfPlanewaves(); i++)
        {
            if(atom_isNumber(angles+i))
            {
                x->f_decoder->setPlanewaveAzimuth(i, atom_getfloat(angles+i) / 360. * HOA_2PI);
            }
            
            x->f_angles_of_channels[i] = x->f_decoder->getPlanewaveAzimuth(i) / HOA_2PI * 360.;
        }
        
        object_attr_touch((t_object*)x, hoa_sym_angles);
    }

    send_configuration(x);
}

t_max_err mode_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYM)
	{
        hoa_decoder_config(x, atom_getsym(argv), x->f_number_of_channels);
    }
    return MAX_ERR_NONE;
}

t_max_err offset_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_isNumber(argv))
    {
        hoa_decoder_config(x, x->f_mode, x->f_number_of_channels, atom_getfloat(argv));
    }
    return MAX_ERR_NONE;
}

t_max_err channel_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_LONG)
    {
        hoa_decoder_config(x, x->f_mode, atom_getfloat(argv));
    }
    return MAX_ERR_NONE;
}

t_max_err angles_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    hoa_decoder_config(x, x->f_mode, x->f_number_of_channels, x->f_offset, argc, argv);
    return MAX_ERR_NONE;
}

void *hoa_2d_decoder_new(t_symbol *s, long argc, t_atom *argv)
{
    // @arg 0 @name ambisonic-order @optional 0 @type int @digest The ambisonic order of decomposition
    // @description First argument is the ambisonic order of decomposition.
    
    t_hoa_2d_decoder *x = (t_hoa_2d_decoder *)object_alloc(hoa_2d_decoder_class);
    if (x)
    {
        ulong order = 1;
        if(argc && argv && atom_gettype(argv) == A_LONG)
            order = max<long>(atom_getlong(argv), 1);
        
        ulong number_of_channels = 4;
        if(argc > 1 && argv+1 && atom_gettype(argv+1) == A_LONG)
        {
            number_of_channels = Math<long>::clip(atom_getlong(argv+1), 2, HOA_MAX_PLANEWAVES);
        }
        else // default to ambisonic + 1
        {
            number_of_channels = order*2+2;
        }
        
        x->f_send_config = 1;
        x->f_mode = hoa_sym_ambisonic;
        
        x->f_decoder = new Decoder<Hoa2d, t_sample>::Regular(order, number_of_channels);
        x->f_number_of_channels = x->f_decoder->getNumberOfPlanewaves();
        
        dsp_setup((t_pxobject *)x, x->f_decoder->getNumberOfHarmonics());
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
            outlet_new(x, "signal");
        
        x->f_ob.z_misc = Z_NO_INPLACE;
        x->f_ins  = new t_sample[x->f_decoder->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs = new t_sample[HOA_MAX_PLANEWAVES * HOA_MAXBLKSIZE];
        
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
        {
            x->f_angles_of_channels[i] = x->f_decoder->getPlanewaveAzimuth(i) / HOA_2PI * 360.;
        }
        
        object_attr_setdisabled((t_object *)x, hoa_sym_angles, (x->f_mode != hoa_sym_irregular));
        object_attr_setdisabled((t_object *)x, hoa_sym_channels, (x->f_mode == hoa_sym_binaural));
        object_attr_setdisabled((t_object *)x, hoa_sym_offset, (x->f_mode == hoa_sym_binaural));
        
        attr_args_process(x, argc, argv);
        
        defer_low(x, (method)send_configuration, NULL, 0, NULL);
    }
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_2d_decoder_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.decoder~", (method)hoa_2d_decoder_new, (method)hoa_2d_decoder_free, (long)sizeof(t_hoa_2d_decoder), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.decoder~", (char *)"hoa.2d.decoder~");
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of circular harmonic signals to decode for an array of loudspeakers
    // @description Array of circular harmonic signals to decode for an array of loudspeakers
    class_addmethod(c, (method)hoa_2d_decoder_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_decoder_assist,		"assist",	A_CANT, 0);
    
    CLASS_ATTR_CHAR             (c, "autoconnect", 0, t_hoa_2d_decoder, f_send_config);
    CLASS_ATTR_CATEGORY			(c, "autoconnect", 0, "Behavior");
    CLASS_ATTR_STYLE_LABEL      (c, "autoconnect", 0, "onoff", "Auto connection");
    CLASS_ATTR_ORDER            (c, "autoconnect", 0, "1");
    // @description If the <m>autoconnect</m> attribute is checked, connected objects like the <o>hoa.2d.meter~</o>, <o>hoa.2d.vector~</o>, <o>hoa.dac~</o> or <o>hoa.gain~</o> will be notified of changes and adapt their behavior accordingly.
    
    CLASS_ATTR_SYM              (c, "mode", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_mode);
    CLASS_ATTR_LABEL            (c, "mode", 0, "Mode");
    CLASS_ATTR_ENUM             (c, "mode", 0, "ambisonic binaural irregular");
    CLASS_ATTR_ACCESSORS		(c, "mode", NULL, mode_set);
    CLASS_ATTR_ORDER            (c, "mode", 0, "1");
    // @description There is three decoding <m>mode</m> :
    // <ul>
    // <li>- <b>Ambisonics</b> : for a regular loudspeakers repartition over a circular array.</li>
    // <li>- <b>Binaural</b> : for headphones.</li>
    // <li>- <b>Irregular</b> : for an irregular loudspeakers repartition over a circular array.</li>
    // </ul>
    
    CLASS_ATTR_LONG             (c, "channels", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_number_of_channels);
    CLASS_ATTR_LABEL            (c, "channels", 0, "Number of Channels");
    CLASS_ATTR_ACCESSORS		(c, "channels", NULL, channel_set);
    CLASS_ATTR_ORDER            (c, "channels", 0, "2");
    // @description The number of Channels. NB : in <b>ambisonic</b> <m>mode</m>, the number of channels must be equal or higher to the number of harmonics : (order *2 + 1), (default : order * 2 + 2).
    
    CLASS_ATTR_DOUBLE           (c, "offset", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_offset);
    CLASS_ATTR_LABEL            (c, "offset", 0, "Offset of Channels");
    CLASS_ATTR_ACCESSORS		(c, "offset", NULL, offset_set);
    CLASS_ATTR_ORDER            (c, "offset", 0, "3");
    // @description The offset of channels, in degrees between 0. and 360.
    
    CLASS_ATTR_DOUBLE_VARSIZE   (c, "angles", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_angles_of_channels, f_number_of_channels, HOA_MAX_PLANEWAVES);
    CLASS_ATTR_LABEL            (c, "angles", 0, "Angles of Channels");
    CLASS_ATTR_ACCESSORS		(c, "angles", NULL, angles_set);
    CLASS_ATTR_ORDER            (c, "angles", 0, "4");
    // @description Angles of each channels. The angles of channels are only settable in <b>irregular</b> <m>mode</m>. Each angle are in degrees and is wrapped between 0. and 360. So you can also set an angle with a negative value. ex : angles for a 5.1 loudspeakers restitution system can be setted either by the "angles 0 30 110 250 330" or by "angles 0 30 110 -110 -30".

    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.decoder~"));
    hoa_2d_decoder_class = c;
    
    return 0;
}

