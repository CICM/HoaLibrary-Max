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
 
 @seealso hoa.2d.meter~, hoa.2d.scope~, hoa.dac~, hoa.vector~, hoa.2d.optim~, hoa.2d.map~, hoa.2d.encoder~, hoa.3d.decoder~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_2d_decoder 
{
	t_pxobject                  f_ob;
    
    Decoder<Hoa2d, t_sample>::Regular*      f_decoder_regular;
    Decoder<Hoa2d, t_sample>::Irregular*    f_decoder_irregular;
    Decoder<Hoa2d, t_sample>::Binaural*     f_decoder_binaural;
    Decoder<Hoa2d, t_sample>*               f_decoder;
    t_sample*                               f_ins;
    t_sample*                               f_outs;
    t_symbol*                               f_mode;
    t_atom_long                             f_send_config;
    void*                                   f_attr;
    
} t_hoa_2d_decoder;

t_class *hoa_2d_decoder_class;

void hoa_2d_decoder_free(t_hoa_2d_decoder *x)
{
    dsp_free((t_pxobject *)x);
    delete x->f_decoder_regular;
    delete x->f_decoder_irregular;
    delete x->f_decoder_binaural;
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

void hoa_2d_decoder_perform64(t_hoa_2d_decoder *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
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

void hoa_2d_decoder_dsp64(t_hoa_2d_decoder *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp64, gensym("dsp_add64"), x, (method)hoa_2d_decoder_perform64, 0, NULL);
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
    t_object *object;
    t_object *line;
    t_object *obj;
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
        atom_setfloat(&offset, Math<double>::wrap(x->f_decoder->getPlanewavesRotation() / HOA_2PI * 360.f, -180, 180));
        
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
            atom_setfloat(argv+i, x->f_decoder->getPlanewaveAzimuth(i) / HOA_2PI * 360.);
        
        for (line = jpatcher_get_firstline(patcher); line; line = jpatchline_get_nextline(line))
        {
            if (jpatchline_get_box1(line) == decoder)
            {
                object = jpatchline_get_box2(line);
                obj = jbox_get_object(object);
                t_symbol* classname = object_classname(obj);
                
                if(classname == hoa_sym_hoa_2d_meter || classname == hoa_sym_hoa_2d_vector || classname == hoa_sym_hoa_gain || classname == hoa_sym_hoa_dac || hoa_sym_dac)
                {
                    if (classname == hoa_sym_hoa_2d_meter || classname == hoa_sym_hoa_2d_vector || classname == hoa_sym_hoa_gain)
                    {
                        object_method_typed(obj, hoa_sym_channels, 1, &nchannels, NULL);
                        object_method_typed(obj, hoa_sym_angles, x->f_decoder->getNumberOfPlanewaves(), argv, NULL);
                        object_method_typed(obj, hoa_sym_offset, 1, &offset, NULL);
                    }
                    
                    // connection
                    for(int i = 0; jbox_getinlet((t_jbox *)object, i) != NULL && i < x->f_decoder->getNumberOfPlanewaves(); i++)
                    {
                        atom_setobj(msg, decoder);
                        atom_setlong(msg + 1, i);
                        atom_setobj(msg + 2, object);
                        atom_setlong(msg + 3, i);
                        object_method_typed(patcher , hoa_sym_connect, 4, msg, &rv);
                    }
                }
            }
        }
        
        free(argv);
    }
}

t_max_err mode_get(t_hoa_2d_decoder *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = (t_atom *)sysmem_newptr(argc[0] * sizeof(t_atom));
    if(argv[0])
    {
        atom_setsym(argv[0], x->f_mode);
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    
    return MAX_ERR_NONE;
}

t_max_err mode_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYM)
	{
        t_symbol* lastMode = x->f_mode;
        
        if(atom_getsym(argv) == hoa_sym_ambisonic && lastMode != hoa_sym_ambisonic)
        {
            object_method(gensym("dsp")->s_thing, hoa_sym_stop);
            x->f_mode = atom_getsym(argv);
            x->f_decoder = x->f_decoder_regular;
            object_attr_setdisabled((t_object *)x, hoa_sym_angles, 1);
            object_attr_setdisabled((t_object *)x, hoa_sym_channels, 0);
            object_attr_setdisabled((t_object *)x, hoa_sym_offset, 0);
        }
        else if(atom_getsym(argv) == hoa_sym_irregular && lastMode != hoa_sym_irregular)
        {
            object_method(gensym("dsp")->s_thing, hoa_sym_stop);
            x->f_decoder = x->f_decoder_irregular;
            x->f_mode = atom_getsym(argv);
            object_attr_setdisabled((t_object *)x, hoa_sym_angles, 0);
            object_attr_setdisabled((t_object *)x, hoa_sym_channels, 0);
            object_attr_setdisabled((t_object *)x, hoa_sym_offset, 0);
        }
        else if(atom_getsym(argv) == hoa_sym_binaural && lastMode != hoa_sym_binaural)
        {
            object_method(gensym("dsp")->s_thing, hoa_sym_stop);
            x->f_decoder = x->f_decoder_binaural;
            x->f_mode = atom_getsym(argv);
            object_attr_setdisabled((t_object *)x, hoa_sym_angles, 1);
            object_attr_setdisabled((t_object *)x, hoa_sym_channels, 1);
            object_attr_setdisabled((t_object *)x, hoa_sym_offset, 1);
        }
        
        object_attr_setlong(x, hoa_sym_channels, x->f_decoder->getNumberOfPlanewaves());
        send_configuration(x);
    }
    return MAX_ERR_NONE;
}

t_max_err offset_get(t_hoa_2d_decoder *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = (t_atom *)sysmem_newptr(argc[0] * sizeof(t_atom));
    
    if(argv[0])
    {
        atom_setfloat(argv[0], Math<double>::wrap(x->f_decoder->getPlanewavesRotation() / HOA_2PI * 360.f, -180, 180));
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    return MAX_ERR_NONE;
}

t_max_err offset_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_isNumber(argv))
    {
        double offset =  Math<double>::wrap_twopi( Math<double>::wrap(atom_getfloat(argv), -180, 180) / 360. * HOA_2PI);
        if(offset != x->f_decoder->getPlanewavesRotation())
        {
            object_method(gensym("dsp")->s_thing, hoa_sym_stop);
            x->f_decoder->setPlanewavesRotation(offset);
            x->f_decoder->computeMatrix();
            send_configuration(x);
        }
    }
    return MAX_ERR_NONE;
}

t_max_err channel_get(t_hoa_2d_decoder *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = (t_atom *)sysmem_newptr(argc[0] * sizeof(t_atom));
    if(argv[0])
    {
        atom_setlong(argv[0], x->f_decoder->getNumberOfPlanewaves());
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    
    return MAX_ERR_NONE;
}

t_max_err channel_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_LONG && atom_getlong(argv) != outlet_count((t_object *)x))
    {
        t_object *b = NULL;
        object_method(gensym("dsp")->s_thing, hoa_sym_stop);
        
        object_obex_lookup(x, hoa_sym_pound_B, (t_object **)&b);
        object_method(b, hoa_sym_dynlet_begin);
        
        ulong order = x->f_decoder->getDecompositionOrder();
        ulong number_of_channels = Math<ulong>::clip(atom_getlong(argv), 2, HOA_MAX_PLANEWAVES);
        
        if(x->f_mode == hoa_sym_ambisonic || x->f_mode == hoa_sym_irregular)
        {
            delete x->f_decoder_regular;
            delete x->f_decoder_irregular;
            
            x->f_decoder_regular = new Decoder<Hoa2d, t_sample>::Regular(order, number_of_channels);
            x->f_decoder_irregular = new Decoder<Hoa2d, t_sample>::Irregular(order, number_of_channels);
            
            if (x->f_mode == hoa_sym_ambisonic)
                x->f_decoder = x->f_decoder_regular;
            else if (x->f_mode == hoa_sym_irregular)
                x->f_decoder = x->f_decoder_irregular;
        }
        
        x->f_decoder->computeMatrix();
        
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
                outlet_append((t_object*)x, NULL, gensym("signal"));
            }
        }
        
        object_method(b, hoa_sym_dynlet_end);
        
        object_attr_touch((t_object *)x, hoa_sym_angles);
        send_configuration(x);
    }
    return 0;
}

t_max_err angles_get(t_hoa_2d_decoder *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = x->f_decoder->getNumberOfPlanewaves();
    argv[0] = (t_atom *)sysmem_newptr(argc[0] * sizeof(t_atom));
    if(argv[0])
    {
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
        {
            atom_setfloat(argv[0]+i, x->f_decoder->getPlanewaveAzimuth(i) / HOA_2PI * 360.);
        }
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    
    return MAX_ERR_NONE;
}

t_max_err angles_set(t_hoa_2d_decoder *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        int state = sys_getdspstate();
        if (state)
            canvas_stop_dsp();
        
        for(int i = 0; i < argc && i < x->f_decoder->getNumberOfPlanewaves(); i++)
        {
            if(atom_isNumber(argv+i))
                x->f_decoder->setPlanewaveAzimuth(i, atom_getfloat(argv+i) / 360. * HOA_2PI);
        }
        x->f_decoder->computeMatrix();
        send_configuration(x);
        
        if (state)
            canvas_start_dsp();
    }
    
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
            order = max<ulong>(atom_getlong(argv), 1);
        
        ulong number_of_channels = 4;
        if(argc > 1 && argv+1 && atom_gettype(argv+1) == A_LONG)
        {
            number_of_channels = Math<ulong>::clip(atom_getlong(argv+1), 2, HOA_MAX_PLANEWAVES);
        }
        else // default to ambisonic + 1
        {
            number_of_channels = order*2+2;
        }
        
        x->f_send_config = 1;
        x->f_mode = hoa_sym_ambisonic;
        
        x->f_decoder_regular    = new Decoder<Hoa2d, t_sample>::Regular(order, number_of_channels);
        x->f_decoder_irregular  = new Decoder<Hoa2d, t_sample>::Irregular(order, number_of_channels);
        x->f_decoder_binaural   = new Decoder<Hoa2d, t_sample>::Binaural(order);
        x->f_decoder = x->f_decoder_regular;
        
        dsp_setup((t_pxobject *)x, x->f_decoder->getNumberOfHarmonics());
        for(int i = 0; i < x->f_decoder->getNumberOfPlanewaves(); i++)
            outlet_new(x, "signal");
        
        x->f_ob.z_misc = Z_NO_INPLACE;
        x->f_ins = new t_sample[x->f_decoder->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_outs= new t_sample[HOA_MAX_PLANEWAVES * HOA_MAXBLKSIZE];
        
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
    
    CLASS_ATTR_LONG             (c, "autoconnect", 0, t_hoa_2d_decoder, f_send_config);
    CLASS_ATTR_CATEGORY			(c, "autoconnect", 0, "Behavior");
    CLASS_ATTR_STYLE_LABEL      (c, "autoconnect", 0, "onoff", "Auto connection");
    CLASS_ATTR_ORDER            (c, "autoconnect", 0, "1");
    // @description If the <m>autoconnect</m> attribute is checked, connected objects like the <o>hoa.2d.meter~</o>, <o>hoa.2d.vector~</o>, <o>hoa.dac~</o> or <o>hoa.gain~</o> will be notified of changes and adapt their behavior accordingly.
    
    CLASS_ATTR_SYM              (c, "mode", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_mode);
    CLASS_ATTR_LABEL            (c, "mode", 0, "Mode");
    CLASS_ATTR_ENUM             (c, "mode", 0, "ambisonic binaural irregular");
    CLASS_ATTR_ACCESSORS		(c, "mode", mode_get, mode_set);
    CLASS_ATTR_ORDER            (c, "mode", 0, "1");
    // @description There is three decoding <m>mode</m> :
    // <ul>
    // <li><b>Ambisonics</b> : for a regular loudspeakers repartition over a circular array.</li>
    // <li><b>Binaural</b> : for headphones.</li>
    // <li><b>Irregular</b> : for an irregular loudspeakers repartition over a circular array.</li>
    // </ul>
    
    CLASS_ATTR_LONG             (c, "channels", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_attr);
    CLASS_ATTR_LABEL            (c, "channels", 0, "Number of Channels");
    CLASS_ATTR_ACCESSORS		(c, "channels", channel_get, channel_set);
    CLASS_ATTR_DEFAULT          (c, "channels", 0, "4");
    CLASS_ATTR_ORDER            (c, "channels", 0, "2");
    // @description The number of Channels. In <b>ambisonic</b> <m>mode</m>, the number of channels must be equal or higher to the number of harmonics : (order *2 + 1), (default : order * 2 + 2).
    
    CLASS_ATTR_DOUBLE           (c, "offset", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_attr);
    CLASS_ATTR_LABEL            (c, "offset", 0, "Offsets of Channels");
    CLASS_ATTR_ACCESSORS		(c, "offset", offset_get, offset_set);
    CLASS_ATTR_DEFAULT          (c, "offset", 0, "0");
    CLASS_ATTR_ORDER            (c, "offset", 0, "3");
    // @description The offset of channels, in degrees between 0. and 360.
    
    CLASS_ATTR_DOUBLE_VARSIZE   (c, "angles", ATTR_SET_DEFER_LOW, t_hoa_2d_decoder, f_attr, f_attr, HOA_MAX_PLANEWAVES);
    CLASS_ATTR_LABEL            (c, "angles", 0, "Angles of Channels");
    CLASS_ATTR_ACCESSORS		(c, "angles", angles_get, angles_set);
    CLASS_ATTR_ORDER            (c, "angles", 0, "4");
    // @description Angles of each channels. The angles of channels are only settable in <b>irregular</b> <m>mode</m>. Each angle are in degrees and is wrapped between 0. and 360. So you can also set an angle with a negative value. ex : angles for a 5.1 loudspeakers restitution system can be setted either by the "angles 0 30 110 250 330" or by "angles 0 30 110 -110 -30".

    class_dspinit(c);
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.decoder~"));
    hoa_2d_decoder_class = c;
    
    return 0;
}

