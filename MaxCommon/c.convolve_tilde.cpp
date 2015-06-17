/*
 // Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      c.convolve~.cpp
 @name      c.convolve~
 @realname  c.convolve~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A zero latency convolution reverberation processor
 
 @description
 <o>c.convolve~</o> is a zero latency convolution reverberation processor
 
 @discussion
 <o>c.convolve~</o> is a zero latency convolution reverberation processor
 
 @category hoa objects, audio, MSP
 
 @seealso hoa.fx.convolve~, c.freeverb~, hoa.fx.freeverb~, hoa.process~
 */

#include "HoaCommon.max.h"

#include <algorithm>
#include <cstring>
#include <numeric>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "FFTConvolver.h"

using namespace fftconvolver;

typedef struct _convolve
{
	t_pxobject      f_ob;
    FFTConvolver*   f_convolver;
    char            f_normalize;
    long            f_fftsize;
    t_atom          f_crop_size;
    
    t_buffer_ref*	f_buffer_ref;
	t_symbol*		f_buffer_name;
	long			f_channel_offset;
    
    char            f_buffer_need_update;
    char            f_buffer_is_valid;
    
    float           *f_sig_ins;
    float           *f_sig_outs;
} t_convolve;

t_class *convolve_class;

void *convolve_new(t_symbol *s, int argc, t_atom *argv);
void convolve_free(t_convolve *x);
void convolve_dsp64(t_convolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void convolve_perform64(t_convolve *x, t_object *d, double **ins, long ni, double **outs, long no, long sampleframes, long f,void *up);
t_max_err convolve_notify(t_convolve *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void convolve_assist(t_convolve *x, void *b, long m, long a, char *s);

void convolve_set(t_convolve *x, t_symbol *s, long ac, t_atom *av);
void convolve_dblclick(t_convolve *x);
void convolve_clear(t_convolve *x);

void convolve_fill_inner_buffer(t_convolve *x);
t_max_err convolve_setattr_buffername(t_convolve *x, t_symbol *s, long ac, t_atom* av);
t_max_err convolve_setattr_channel(t_convolve *x, t_symbol *s, long ac, t_atom* av);
t_max_err convolve_setattr_normalize(t_convolve *x, t_symbol *s, long ac, t_atom* av);
t_max_err convolve_setattr_fftsize(t_convolve *x, t_symbol *s, long ac, t_atom* av);
t_max_err convolve_setattr_cropsize(t_convolve *x, t_symbol *s, long ac, t_atom* av);

inline long next_power_of_two (long x);

#ifdef HOA_PACKED_LIB
int c_convolve_main(void)
#else
void ext_main(void *r)
#endif
{
	t_class *c;
    
	c = class_new("c.convolve~", (method)convolve_new, (method)convolve_free, (short)sizeof(t_convolve), 0L, A_GIMME, 0);
    class_setname((char *)"c.convolve~", (char *)"c.convolve~");
    
	hoa_initclass(c, (method)NULL);
    class_dspinit(c);
	
    // @method signal @digest Signal to reverberate
	// @description Signal to reverberate
    class_addmethod(c, (method) convolve_dsp64,     "dsp64",            A_CANT,  0);
    class_addmethod(c, (method) convolve_assist,  "assist",           A_CANT,  0);
    
    // @method set @digest Set a new impulse response by passing <o>buffer~</o> object name.
	// @description The <m>set</m> method sets a new impulse response by passing <o>buffer~</o> object name. An optionnal int can be passed after buffer name to set the buffer channel to be used.
	// @marg 0 @name buffer-name @optional 0 @type symbol
	// @marg 1 @name channel @optional 1 @type int
    class_addmethod(c, (method) convolve_set,       "set",              A_GIMME, 0);
    
    // @method clear @digest Clear the current reverberation tail
	// @description The <m>clear</m> method clear the current reverberation tail (may produce clicks)
    class_addmethod(c, (method) convolve_clear,     "clear",                     0);
    class_addmethod(c, (method) convolve_notify,	"notify",			A_CANT,  0);
    
    // @method (mouse) @digest Open the file buffer object window.
	// @description Double-clicking on the <o>c.convolve~</o> opens a display window where you can view the contents of the <o>buffer~</o> used to load the impulse response.
    class_addmethod(c, (method) convolve_dblclick,  "dblclick",         A_CANT,  0);
    
    CLASS_ATTR_SYM                  (c, "buffer", 0, t_convolve, f_buffer_name);
	CLASS_ATTR_CATEGORY             (c, "buffer", 0, "Behavior");
	CLASS_ATTR_ACCESSORS            (c, "buffer", NULL, convolve_setattr_buffername);
	CLASS_ATTR_LABEL                (c, "buffer", 0, "Buffer~ Object Name");
	CLASS_ATTR_SAVE                 (c, "buffer", 1);
    CLASS_ATTR_ORDER                (c, "buffer",  0, "1");
    // @description The <b>buffer</b> attribute is the name of the <o>buffer~</o> used to read the impulse response.
	
    CLASS_ATTR_LONG                 (c, "channel", 0, t_convolve, f_channel_offset);
    CLASS_ATTR_CATEGORY             (c, "channel", 0, "Behavior");
	CLASS_ATTR_ACCESSORS            (c, "channel", NULL, convolve_setattr_channel);
    CLASS_ATTR_LABEL                (c, "channel", 0, "Channel");
    CLASS_ATTR_SAVE                 (c, "channel", 1);
    CLASS_ATTR_ORDER                (c, "channel",  0, "2");
    // @description The <b>channel</b> attribute is the channel number of the impulse response buffer.
    
    CLASS_ATTR_LONG                 (c, "normalize",  0, t_convolve, f_normalize);
	CLASS_ATTR_CATEGORY             (c, "normalize",  0, "Behavior");
    CLASS_ATTR_STYLE_LABEL          (c, "normalize",  0, "onoff", "Normalize buffer content");
    CLASS_ATTR_ACCESSORS            (c, "normalize", NULL, convolve_setattr_normalize);
	CLASS_ATTR_SAVE                 (c, "normalize",  1);
    CLASS_ATTR_ORDER                (c, "normalize",  0, "3");
    // @description If the <b>normalize</b> attribute is checked, <o>c.convolve</o> object will normalize the <o>buffer~</o> content.
    
    CLASS_ATTR_LONG                 (c, "fftsize", 0, t_convolve, f_fftsize);
    CLASS_ATTR_CATEGORY             (c, "fftsize", 0, "Behavior");
	CLASS_ATTR_ACCESSORS            (c, "fftsize", NULL, convolve_setattr_fftsize);
    CLASS_ATTR_LABEL                (c, "fftsize", 0, "FFT size");
    CLASS_ATTR_SAVE                 (c, "fftsize", 1);
    CLASS_ATTR_ORDER                (c, "fftsize",  0, "4");
    // @description The <b>fftsize</b> attribute is the <o>c.convolve</o> FFT size.
    
    CLASS_ATTR_ATOM                 (c, "cropsize", 0, t_convolve, f_crop_size);
    CLASS_ATTR_CATEGORY             (c, "cropsize", 0, "Behavior");
	CLASS_ATTR_ACCESSORS            (c, "cropsize", NULL, convolve_setattr_cropsize);
    CLASS_ATTR_LABEL                (c, "cropsize", 0, "Crop size (ms)");
    CLASS_ATTR_SAVE                 (c, "cropsize", 1);
    CLASS_ATTR_ORDER                (c, "cropsize",  0, "5");
    // @description Use the <b>cropsize</b> attribute to limit the impulse response at a maximum size. It can be usefull to prevent too big CPU usage when loading big impulse responses.
    // the <b>cropsize</b> is set in milliseconds, dont set this if you want to play the entire IR.
    
    class_register(CLASS_BOX, c);
	convolve_class = c;
}

void *convolve_new(t_symbol *s, int ac, t_atom *av)
{
	t_convolve *x =  NULL;

    x = (t_convolve *)object_alloc(convolve_class);
    if (x)
	{
        x->f_buffer_need_update = 1;
        x->f_buffer_name = hoa_sym_nothing;
        x->f_channel_offset = 1;
        x->f_normalize = 0;
        x->f_fftsize = 1024;
        atom_setsym(&x->f_crop_size, gensym("<none>"));
        
        x->f_convolver = new FFTConvolver();
        
        x->f_sig_ins = (float *)malloc(SYS_MAXBLKSIZE * sizeof(float));
        x->f_sig_outs = (float *)malloc(SYS_MAXBLKSIZE * sizeof(float));
        
		dsp_setup((t_pxobject *)x, 1);
        outlet_new(x, "signal");
        
        x->f_ob.z_misc = Z_NO_INPLACE;
        
        attr_args_process(x, ac, av);
	}
    
	return (x);
}

void convolve_assist(t_convolve *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		sprintf(s,"(signal) Output");
	else
    {
		switch (a)
        {
			case 0:	sprintf(s,"(signal) Input, messages");	break;
			default: sprintf(s,"Undocumented");	break;
		}
	}
}

t_max_err convolve_setattr_buffername(t_convolve *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && atom_gettype(av) == A_SYM)
	{
		t_symbol* lastname = x->f_buffer_name;
		x->f_buffer_name = atom_getsym(av);
        
		if (lastname != x->f_buffer_name)
		{
			if (!x->f_buffer_ref)
				x->f_buffer_ref = buffer_ref_new((t_object*)x, x->f_buffer_name);
			else
				buffer_ref_set(x->f_buffer_ref, x->f_buffer_name);
		}
        
        x->f_buffer_need_update = 1;
	}
	
	return MAX_ERR_NONE;
}

t_max_err convolve_setattr_channel(t_convolve *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && atom_gettype(av) == A_LONG)
	{
        x->f_channel_offset = Math<long>::clip(atom_getlong(av), 1, 4);
        x->f_buffer_need_update = 1;
	}
	
	return MAX_ERR_NONE;
}

t_max_err convolve_setattr_normalize(t_convolve *x, t_symbol *s, long ac, t_atom* av)
{
    if (ac && av && atom_gettype(av) == A_LONG)
	{
		x->f_normalize = (char)atom_getlong(av);
        if(x->f_normalize < 1)
            x->f_normalize = 0;
        else
            x->f_normalize = 1;
        x->f_buffer_need_update = 1;
	}
    
    return MAX_ERR_NONE;
}

inline long next_power_of_two (long x)
{
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}

t_max_err convolve_setattr_fftsize(t_convolve *x, t_symbol *s, long ac, t_atom* av)
{
    if (ac && av && atom_gettype(av) == A_LONG)
	{
        x->f_fftsize = Math<long>::clip(next_power_of_two(atom_getlong(av)), 64, 16384);
        x->f_buffer_need_update = 1;
	}
    
    return MAX_ERR_NONE;
}

t_max_err convolve_setattr_cropsize(t_convolve *x, t_symbol *s, long ac, t_atom* av)
{
    if (ac && av && atom_isNumber(av))
	{
        atom_setfloat(&x->f_crop_size, max<double>(atom_getfloat(av), 1));
        x->f_buffer_need_update = 1;
	}
    else
    {
        atom_setsym(&x->f_crop_size, gensym("<none>"));
        x->f_buffer_need_update = 1;
    }
    
    return MAX_ERR_NONE;
}

t_max_err convolve_notify(t_convolve *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	t_symbol *name;
	
	if (msg == gensym("attr_modified"))
	{
		name = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		
		if(name == gensym("normalize"))
		{
            ;
		}
	}
	else if (msg == gensym("buffer_modified"))
	{
        ;
	}
	
	return buffer_ref_notify(x->f_buffer_ref, s, msg, sender, data);
}

void convolve_clear(t_convolve *x)
{
    x->f_buffer_need_update = 1;
}

void convolve_fill_inner_buffer(t_convolve *x)
{
    t_buffer_obj *buffer = NULL;
    float* temp;
    float *buffer_samples;
    long frames, nc, chan, buffer_size;
    //float buffer_sr;
    float sr = sys_getsr();
    
    if ( (x->f_buffer_name == hoa_sym_nothing) || !x->f_buffer_ref)
        return;
    
    buffer = buffer_ref_getobject(x->f_buffer_ref);
    if (!buffer)
        return;
    
    buffer_samples = buffer_locksamples(buffer);
	if (!buffer_samples)
		return;
    
    frames = buffer_getframecount(buffer);
	nc = buffer_getchannelcount(buffer);
    //buffer_sr = buffer_getsamplerate(buffer);
    chan = (long) Math<long>::clip(x->f_channel_offset-1, 0, nc);
    
    if (atom_gettype(&x->f_crop_size) == A_FLOAT)
        buffer_size = min<long>(frames, (sr / 1000) * atom_getlong(&x->f_crop_size));
    else
        buffer_size = frames;
    
    temp = (float *) malloc( buffer_size * sizeof(float));
    
    for (int i = 0; i < buffer_size; i++)
    {
        temp[i] = buffer_samples[i*nc+(chan)];
    }
    
    if(x->f_normalize)
    {
        float max = 0;
        for(int i = 0; i < buffer_size; i++)
        {
            if(fabs(temp[i]) > max)
                max = fabs(temp[i]);
        }
        if(max != 0)
        {
            max = 1.f / max;
            for(int i = 0; i < buffer_size; i++)
                temp[i] *= max;
        }
    }
    
    x->f_convolver->init(x->f_fftsize, temp, buffer_size);
    free(temp);
    
    buffer_unlocksamples(buffer);
}

void convolve_set(t_convolve *x, t_symbol *s, long ac, t_atom *av)
{
    if (ac && av && atom_gettype(av) == A_SYM)
	{
        object_method(x, gensym("buffer"), 1, av);
        if (ac > 1 && atom_gettype(av+1) == A_LONG)
        {
            object_method(x, gensym("channel"), 1, av+1);
        }
	}
}

void convolve_free(t_convolve *x)
{
	dsp_free(&x->f_ob);
    object_free(x->f_buffer_ref);
    free(x->f_sig_ins);
    free(x->f_sig_outs);
}

void convolve_perform64(t_convolve *x, t_object *d, double **ins, long ni, double **outs, long no, long sampleframes, long f,void *up)
{
    if (x->f_buffer_need_update)
    {
        convolve_fill_inner_buffer(x);
        x->f_buffer_need_update = 0;
    }
    
#ifdef _APPLE_
    vDSP_vdpsp(ins[0], 1, x->f_sig_ins, 1, sampleframes);
#else
    for (int i = 0; i < sampleframes; i++)
        x->f_sig_ins[i] = (float)ins[0][i];
#endif
    
    x->f_convolver->process(x->f_sig_ins, x->f_sig_outs, sampleframes);
    
#ifdef _APPLE_
    vDSP_vspdp(x->f_sig_outs, 1, outs[0], 1, sampleframes);
#else
    for (int i = 0; i < sampleframes; i++)
        outs[0][i] = x->f_sig_outs[i];
#endif
}

void convolve_dsp64(t_convolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add64"), x, (method)convolve_perform64, 0, NULL);
}

void convolve_dblclick(t_convolve *x)
{
    if (buffer_ref_exists(x->f_buffer_ref))
        buffer_view(buffer_ref_getobject(x->f_buffer_ref));
}
