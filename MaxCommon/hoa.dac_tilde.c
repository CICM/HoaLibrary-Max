/*
 // Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      hoa.dac~.cpp
 @name      hoa.dac~
 @realname  hoa.dac~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A custom dac~ object with MathLab syntax.
 
 @description
 <o>hoa.dac~</o> is a wrapped <o>dac~</o> object (Digital-To-Analog-Converter) through which you will route all signals from MSP out to your computer speakers or audio hardware to be audible to the human ear. It also gives you access to the Audio Status window which controls your audio settings and hardware. Unlike <o>dac~</o> object, you can use a mathLab syntax to set channel routing. ex : "1:16" produce "1 2 3 ... 16"
 
 @discussion
 <o>hoa.dac~</o> is a wrapped <o>dac~</o> object (Digital-To-Analog-Converter) through which you will route all signals from MSP out to your computer speakers or audio hardware to be audible to the human ear. It also gives you access to the Audio Status window which controls your audio settings and hardware. Unlike <o>dac~</o> object, you can use a mathLab syntax to set channel routing. ex : "1:16" produce "1 2 3 ... 16"
 
 @category ambisonics, hoa objects, audio, msp
 
 @seealso dac~, adc~, adstatus, ezdac~
 */

#include "HoaCommon.max.h"

typedef struct _hoa_dac
{
	t_pxobject			f_ob;
    t_object*			f_dac;
	int					f_number_of_channels;
} t_hoa_dac;

t_class *hoa_dac_class;

void *hoa_dac_new(t_symbol *s, int argc, t_atom *argv);
void hoa_dac_free(t_hoa_dac *x);
void hoa_dac_assist(t_hoa_dac *x, void *b, long m, long a, char *s);
void hoa_dac_dsp64(t_hoa_dac *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void hoa_dac_int(t_hoa_dac *x, long l);
void hoa_dac_list(t_hoa_dac *x, t_symbol *s, long argc, t_atom *argv);
void hoa_dac_set(t_hoa_dac *x, t_symbol *s, long argc, t_atom *argv);
void hoa_dac_anything(t_hoa_dac *x, t_symbol *s, long argc, t_atom *argv);
void hoa_dac_dblclick(t_hoa_dac *x);

void hoa_dac_start(t_hoa_dac *x);
void hoa_dac_stop(t_hoa_dac *x);
void hoa_dac_startwindow(t_hoa_dac *x);
void hoa_dac_open(t_hoa_dac *x);
void hoa_dac_wclose(t_hoa_dac *x);

t_hoa_err hoa_getinfos(t_hoa_dac* x, t_hoa_boxinfos* boxinfos);

#ifdef HOA_PACKED_LIB
int hoa_dac_main(void)
#else
int C74_EXPORT main(void)
#endif
{
	t_class *c;
    
	c = class_new("hoa.dac~", (method)hoa_dac_new, (method)hoa_dac_free, (short)sizeof(t_hoa_dac), 0L, A_GIMME, 0);
	class_setname((char *)"hoa.dac~", (char *)"hoa.dac~");
    
	hoa_initclass(c, (method)hoa_getinfos);
	
    // @method signal @digest Function depends on inlet
	// @description The <m>signal</m> A signal coming into an inlet of dac~ is sent to the audio output channel corresponding to the inlet. The signal must be between -1 and 1 to avoid clipping by the DAC.
	class_addmethod(c, (method)hoa_dac_dsp64,		"dsp64",		A_CANT,  0);
	class_addmethod(c, (method)hoa_dac_assist,		"assist",		A_CANT,	 0);
    
    // @method (mouse) @digest Double-click to open the Audio Status window
	class_addmethod(c, (method)hoa_dac_dblclick,	"dblclick",		A_CANT,  0);
    
    // @method int @digest Enable/disable audio processing
	// @description A non-zero number turns on audio processing in all loaded patches. 0 turns off audio processing in all loaded patches.
    // @marg 0 on/off-flag @optional 0 @type int
	class_addmethod(c, (method)hoa_dac_int,			"int",			A_LONG,  0);
    
    // @method list @digest Enable/disable audio processing
	// @description List comprised of integers, sets the logical output channels for each signal inlet in order from left to right.
    // @marg 0 output-channel-designation @optional 0 @type list
	class_addmethod(c, (method)hoa_dac_list,		"list",			A_GIMME, 0);
	//class_addmethod(c, (method)hoa_dac_set,			"set",			A_GIMME, 0); // Todo : proxy_setinlet() ??
    
    // @method start @digest Turns on audio processing in all loaded patches.
	// @description Turns on audio processing in all loaded patches.
	class_addmethod(c, (method)hoa_dac_start,		"start",		A_NOTHING,  0);
    
    // @method stop @digest Turns off audio processing in all loaded patches.
	// @description Turns off audio processing in all loaded patches.
	class_addmethod(c, (method)hoa_dac_stop,		"stop",			A_NOTHING,  0);
    
    // @method startwindow @digest Enable audio processing in local patch and subpatches only.
	// @description Turns on audio processing only in the patch in which this <o>hoa.dac~</o> is located, and in subpatches of that patch. Turns off audio processing in all other patches.
	class_addmethod(c, (method)hoa_dac_startwindow,	"startwindow",	A_NOTHING,  0);
    
    // @method open @digest Opens the Audio Status window.
	// @description Opens the Audio Status window.
	class_addmethod(c, (method)hoa_dac_open,		"open",			A_NOTHING,  0);
    
    // @method wclose @digest Close the Audio Status window.
	// @description Close the Audio Status window.
	class_addmethod(c, (method)hoa_dac_wclose,		"wclose",		A_NOTHING,  0);
    
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	hoa_dac_class = c;
	return 0;
}

void *hoa_dac_new(t_symbol *s, int argc, t_atom *argv)
{
    // @arg 0 @name outputs @optional 1 @type int/symbol @digest Output routing or/and bus name
    // @description You can create a <o>hoa.dac~</o> object that uses one or more audio output channel numbers between 1 and 512. These numbers refer to logical channels and can be dynamically reassigned to physical device channels of a particular driver using either the Audio Status window, its I/O Mappings subwindow, or an adstatus object with an output keyword argument. Arguments, If the computer's built-in audio hardware is being used, there will be two input channels available. Other audio drivers and/or devices may have more than two channels. If no argument is typed in, dac~ will have two inlets, for input channels 1 and 2.
    // If a symbol is provided as the first argument to a dac~ object, then it will get an independent control section in the Max mixer. If two dac~ instances in a patcher hierarchy have the same name they will be on the same "bus" and share controls.
    
	int i, j, count = 0;
	t_hoa_dac *x;
	t_atom channels[512];
	int min, max;
    int symPrepend = 0;
    
    x = (t_hoa_dac *)object_alloc(hoa_dac_class);
    
    if (x)
    {
        if (argc && atom_gettype(argv) == A_SYM)
        {
            char *dac_bus_name = atom_getsym(argv)->s_name;
            if (isalpha(dac_bus_name[0])) // only works if the first letter isn't a number
            {
                symPrepend = 1;
                atom_setsym(channels, atom_getsym(argv));
            }
        }
        
        for(i = 0; i < (argc-symPrepend); i++)
        {
            if(atom_gettype(argv+i+symPrepend) == A_SYM)
            {
                min = atoi(atom_getsym(argv+i+symPrepend)->s_name);
                if (min < 10)
                    max = atoi(atom_getsym(argv+i+symPrepend)->s_name+2);
                else if (min < 100)
                    max = atoi(atom_getsym(argv+i+symPrepend)->s_name+3);
                else if (min < 1000)
                    max = atoi(atom_getsym(argv+i+symPrepend)->s_name+4);
                else
                    max = atoi(atom_getsym(argv+i+symPrepend)->s_name+5);
                if (max > min)
                {
                    for(j = min; j <= max; j++)
                    {
                        atom_setlong(channels + symPrepend + count++, j);
                    }
                }
                else
                {
                    for(j = min; j >= max; j--)
                    {
                        atom_setlong(channels + symPrepend + count++, j);
                    }
                }
            }
            else if(atom_gettype(argv + symPrepend + i) == A_LONG)
            {
                atom_setlong(channels + symPrepend + count++, atom_getlong(argv + symPrepend + i));
            }
        }
        
        x->f_number_of_channels = count;
        dsp_setup((t_pxobject *)x, x->f_number_of_channels);
        x->f_dac = (t_object *)object_new_typed(CLASS_BOX, gensym("dac~"), count + symPrepend, channels);
    }
	
    return x->f_dac;
	//return x;
}

t_hoa_err hoa_getinfos(t_hoa_dac* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_STANDARD;
	boxinfos->autoconnect_inputs = x->f_number_of_channels;
	boxinfos->autoconnect_outputs = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_dac_dsp64(t_hoa_dac *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method((t_object *)x->f_dac, gensym("dsp64"), dsp64, count, samplerate, maxvectorsize, flags);
}

void hoa_dac_start(t_hoa_dac *x)
{
	object_method(x->f_dac, gensym("start"));
}
void hoa_dac_stop(t_hoa_dac *x)
{
	object_method(x->f_dac, gensym("stop"));
}
void hoa_dac_startwindow(t_hoa_dac *x)
{
	object_method(x->f_dac, gensym("startwindow"));
}
void hoa_dac_open(t_hoa_dac *x)
{
	object_method(x->f_dac, gensym("open"));
}
void hoa_dac_wclose(t_hoa_dac *x)
{
	object_method(x->f_dac, gensym("wclose"));
}

void hoa_dac_int(t_hoa_dac *x, long l)
{
    object_method(x->f_dac, gensym("int"), l);
	//object_method(x->f_dac, l == 1 ? gensym("start") : gensym("stop"));
}

void hoa_dac_list(t_hoa_dac *x, t_symbol *s, long argc, t_atom *argv)
{
	object_method_typed(x->f_dac, gensym("list"), argc, argv, NULL);
}

void hoa_dac_set(t_hoa_dac *x, t_symbol *s, long argc, t_atom *argv)
{
    /*
     long inletnum = proxy_getinlet((t_object*)x);
     
     t_jbox* dac = NULL;
     void *inletptr = NULL;
     t_max_err err;
     
     err = object_obex_lookup(x->f_dac, gensym("#B"), (t_object **)&dac);
     if (err != MAX_ERR_NONE)
     return;
     
     if (dac)
     inletptr = jbox_getinlet(dac, inletnum);
     
     if (inletptr)
     object_method(inletptr, gensym("set"), argc, argv);
     */
    
	/*
     long inletnum = proxy_getinlet((t_object*)x);
     post("set message in inlet %ld", inletnum);
     long inletnum2 = proxy_getinlet((t_object*)x->f_dac);
     post("inletnum2 %ld", inletnum2);
     void* inptr = proxy_getinletptr((t_object*)x);
     //proxy_setinletptr(x->f_dac, inptr);
     proxy_setinletptr(x->f_ob.z_proxy, x->f_dac->o_inlet);
     inletnum2 = proxy_getinlet((t_object*)x->f_dac);
     post("inletnum2 %ld", inletnum2);
     */
	//object_method_typed(x->f_dac, gensym("set"), argc, argv, NULL);
}

void hoa_dac_assist(t_hoa_dac *x, void *b, long m, long a, char *s)
{
    // @in 0 @loop 1 @type signal @digest input channel and incoming messages.
	object_method(x->f_dac, gensym("assist"), b, m, a, s);
}

void hoa_dac_dblclick(t_hoa_dac *x)
{
    object_method(x->f_dac, gensym("dblclick"), 0, NULL);
}

void hoa_dac_free(t_hoa_dac *x)
{
	dsp_free((t_pxobject*)x);
}
