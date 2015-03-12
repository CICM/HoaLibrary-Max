/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.gain~.cpp
 @name      hoa.gain~
 @realname  hoa.gain~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A multichannel gain~ object.
 
 @description
 <o>hoa.gain~</o> is a multichannel exponential scaling volume slider. hoa.gain~ is a slider that scales signals. it can also make a smooth transition as you move from one value of the slider to the next.
 
 @discussion
 <o>hoa.gain~</o> is a multichannel exponential scaling volume slider. hoa.gain~ is a slider that scales signals. it can also make a smooth transition as you move from one value of the slider to the next.
 
 @category ambisonics, hoa objects, GUI, msp
 
 @seealso gain~, hoa.process~, hoa.fx.gain~
 
 @illustration on @caption the hoa.gain~ GUI object
 @palette yes
 */

#include "HoaCommon.max.h"

#define MAX_IO 64
#define MIN_IO 1
#define DEF_IO 8

// mouse tracking stuff

static t_pt s_hoa_gain_cum;
static double s_hoa_gain_startval;

#define hoa_gain_CORNERSIZE				(6.)

#define hoa_gain_STRIPEWIDTH            (2.5)	// stripes width
#define hoa_gain_DISPLAYINSET			(4.)	// amount subtracted from rect for value
#define knobMargin                      (2)		// Knob Margin

enum inputmode {
	DECIBELS	= 0,
	AMPLITUDE	= 1,
	MIDI		= 2
};

typedef struct _hoa_gain
{
	t_pxjbox        	j_box;
	double          	j_min;
	double          	j_size;			// number of steps
	double				j_val;
	char                j_relative;		// relative mousing (like orig miller slider, but not like h/uslider)
	char                j_orientation;	// 0 = auto, 1 = horiz, 2 = vertical
	t_jrgba             j_brgba;
    t_jrgba             j_knobcolor;
    t_jrgba             j_stripecolor;
    
    // gain
    Line<t_sample>*		f_amp;
    char                f_inputMode;
    float               f_range[2];
    double              j_valdB;
    double              j_defaultValuedB;
	
    // inputs/output
    void*               f_inlet_val;
    void*               f_outlet_infos;
    
    // options
    double              f_interp;
    t_atom_long     	f_numberOfChannels;
    
} t_hoa_gain;

t_class	*s_hoa_gain_class;

t_hoa_err hoa_getinfos(t_hoa_gain* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_STANDARD;
	boxinfos->autoconnect_inputs = x->f_numberOfChannels;
	boxinfos->autoconnect_outputs = x->f_numberOfChannels;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_gain_assist(t_hoa_gain *x, void *b, long m, long a, char *s)
{
    // @in 0 @loop 1 @type signal @digest input signals to be scaled.
    // @in 1 @type float @digest slider value.
    
	if (m == ASSIST_INLET)
    {
        if (a != x->f_numberOfChannels)
        {
            sprintf(s,"(signal) Audio Signal to be scaled (ch %ld)", a+1);
        }
        else
        {
            switch (x->f_inputMode)
            {
                case DECIBELS:
                    sprintf(s,"(float/int) Set dB Value (%.2f-%.2f)", x->f_range[0], x->f_range[1]);
                    break;
                case AMPLITUDE:
                    sprintf(s,"(float/int) Set Amplitude Value (dB range : %.2f-%.2f)", x->f_range[0], x->f_range[1]);
                    break;
                case MIDI:
                    sprintf(s,"(int) Set Midi Value (dB range : %.2f-%.2f)", x->f_range[0], x->f_range[1]);
                    break;
                    
                default:
                    break;
            }
        }
	}
    
    // @out 0 @loop 1 @type signal @digest output corresponding input scaled signal.
    // @out 1 @type float @digest output slider value.
    
	else
    {
		if (a != x->f_numberOfChannels)
        {
            sprintf(s,"(signal) Scaled Signal (ch %ld)", a+1);
        }
        else
        {
            switch (x->f_inputMode)
            {
                case DECIBELS:
                    sprintf(s,"(float) dB Value (%.2f-%.2f)", x->f_range[0], x->f_range[1]);
                    break;
                case AMPLITUDE:
                    sprintf(s,"(float) Amplitude Value (dB range : %.2f-%.2f)", x->f_range[0], x->f_range[1]);
                    break;
                case MIDI:
                    sprintf(s,"(int) Midi Value (dB range : %.2f-%.2f)", x->f_range[0], x->f_range[1]);
                    break;
                    
                default:
                    break;
            }
        }
	}
}

/* DSP ------------------------------------- */
void hoa_gain_perform64(t_hoa_gain *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	int i, j;
	double gain;
    
    for(i = 0; i < sampleframes; i++)
	{
		gain = x->f_amp->process();
		
		for (j=0; j < x->f_numberOfChannels; j++)
		{
			outs[j][i] = gain * ins[j][i];
		}
	}
}
void hoa_gain_dsp64(t_hoa_gain *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_amp->setRamp(x->f_interp / 1000. * samplerate);
    object_method(dsp64, gensym("dsp_add64"), x, hoa_gain_perform64, 0, NULL);
}

/* Utilities ------------------------------------- */
double hoa_gain_getInputModeValue(t_hoa_gain *x)
{
    switch (x->f_inputMode)
    {
        case DECIBELS :
            return x->j_valdB;
            break;
        case AMPLITUDE :
            return x->j_valdB > -70 ? dbtoa(x->j_valdB) : 0;
            break;
        case MIDI :
            return scale(x->j_valdB, -70, 0, 0, 128);
            break;
            
        default:
            break;
    }
    return 0;
}

void hoa_gain_set_gain(t_hoa_gain *x)
{
    x->f_amp->setValue(x->j_valdB > -70.0f ? dbtoa(x->j_valdB) : 0.0f);
}

double hoa_gain_constrain_real_value(t_hoa_gain *x, double f)
{
    return Math<double>::clip(f, double(x->j_min), x->j_size + x->j_min);
}

double hoa_gain_constrain(t_hoa_gain *x, double f)
{
    return Math<double>::clip(f, double(0), x->j_size);
}

bool hoa_gain_ishorizontal(t_hoa_gain *x, t_rect *r)
{
    if (x->j_orientation == 0) // automatic mode
    {
        return (r->height < r->width);
    }
    else
    {
        return (x->j_orientation == 2 ? 0 : 1);
    }
}

double hoa_gain_postoval(t_hoa_gain *x, t_pt pt, t_rect *r, char ishoriz)
{
    double pos, range;
    
    if(ishoriz)
    {
        pos = pt.x;
        range = r->width;
    }
    else
    {
        pos = r->height - pt.y;
        range = r->height;
    }
    if(pos <= hoa_gain_DISPLAYINSET)
    {
        pos = 0.;
    }
    else
    {
        pos -= hoa_gain_DISPLAYINSET;
        pos = (pos / (range - (hoa_gain_DISPLAYINSET*2))) * x->j_size;
    }
    
    return hoa_gain_constrain(x,pos);
}

double hoa_gain_valtopos(t_hoa_gain *x, double val, t_rect *r, char ishoriz)
{
    double pos, range;
    double size;
    
    range = ishoriz ? r->width : r->height;
    size = max<double>(x->j_size, 0.);
    
    if (size)
    {
        pos = (val / size) * (range - (hoa_gain_DISPLAYINSET*2));
    }
    else
    {
        pos = 0;
    }
    
    pos += hoa_gain_DISPLAYINSET;
    if (ishoriz)
        return pos;
    else
        return r->height - pos;
}

double hoa_gain_dBvaltopos(t_hoa_gain *x, double dBval, t_rect *r, char ishoriz)
{
    double val = scale(dBval, x->f_range[0], x->f_range[1], 0, x->j_size);
    return hoa_gain_valtopos(x, val, r, ishoriz);
}

double hoa_gain_dbtoval(t_hoa_gain *x, double dBval)
{
    return scale(dBval, x->f_range[0], x->f_range[1], 0, x->j_size);
}
double hoa_gain_valtodb(t_hoa_gain *x, double val)
{
    return scale(val, 0, x->j_size, x->f_range[0], x->f_range[1]);
}

void hoa_gain_setInputModeValue(t_hoa_gain *x, double value, bool notify)
{
    double dBValue = x->j_valdB;
    
    switch (x->f_inputMode)
    {
        case DECIBELS :
            dBValue = value;
            break;
        case AMPLITUDE :
            dBValue = atodb(value);
            break;
        case MIDI :
            dBValue = scale(value, 0, 128, -70, 0);
            break;
            
        default:
            break;
    }
    
    x->j_val = hoa_gain_constrain_real_value(x, dBValue) - x->j_min;
    x->j_valdB = x->j_val + x->j_min;
    
    hoa_gain_set_gain(x);
    
    if (notify)
    {
        object_notify(x, gensym("modified"), NULL);
    }
    
    jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
    jbox_invalidate_layer((t_object *)x, NULL, gensym("valuestripe_layer"));
    jbox_redraw((t_jbox *)x);
}

void hoa_gain_contextValue(t_hoa_gain *x, long valueType, double value)
{
    x->f_inputMode = Math<long>::clip(valueType, 0, 2);
    hoa_gain_setInputModeValue(x, value, true);
}

void hoa_gain_set_dB(t_hoa_gain *x, double dBValue)
{
    x->j_val = hoa_gain_constrain_real_value(x, dBValue) - x->j_min;
    x->j_valdB = x->j_val + x->j_min;
    hoa_gain_set_gain(x);
    object_notify(x, gensym("modified"), NULL);
    jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
    jbox_invalidate_layer((t_object *)x, NULL, gensym("valuestripe_layer"));
    jbox_redraw((t_jbox *)x);
}

void hoa_gain_assign(t_hoa_gain *x, double f, long notify)
{
    hoa_gain_setInputModeValue(x, f, notify);
}

/* Paint ------------------------------------- */
void draw_background(t_hoa_gain *x, t_object *view, t_rect *rect, char isHoriz)
{
    int zerodBpos;
    zerodBpos = hoa_gain_dBvaltopos(x, 0, rect, isHoriz);
    
    t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("bg_layer"), rect->width, rect->height);
    
    if (g)
    {
        // background
        jgraphics_rectangle(g, 0, 0, rect->width, rect->height);
        jgraphics_set_source_jrgba(g, &x->j_brgba);
        jgraphics_fill(g);
        
        jgraphics_set_source_jrgba(g, &x->j_knobcolor);
        
        if (isHoriz)
        {
            jgraphics_line_draw_fast(g, zerodBpos - hoa_gain_DISPLAYINSET*0.5-knobMargin, knobMargin*0.5, zerodBpos + hoa_gain_DISPLAYINSET, knobMargin*0.5, 1);
            jgraphics_line_draw_fast(g, zerodBpos - hoa_gain_DISPLAYINSET*0.5-knobMargin, rect->height - knobMargin*0.5, zerodBpos + hoa_gain_DISPLAYINSET, rect->height - knobMargin*0.5, 1);
        }
        else
        {
            jgraphics_line_draw_fast(g, knobMargin*0.5, zerodBpos - hoa_gain_DISPLAYINSET*0.5-knobMargin, knobMargin*0.5, zerodBpos + hoa_gain_DISPLAYINSET, 1);
            jgraphics_line_draw_fast(g, rect->width - knobMargin*0.5, zerodBpos - hoa_gain_DISPLAYINSET*0.5-knobMargin, rect->width - knobMargin*0.5, zerodBpos + hoa_gain_DISPLAYINSET, 1);
        }
    }
    
    jbox_end_layer((t_object*)x, view, gensym("bg_layer"));
    jbox_paint_layer((t_object *)x, view, gensym("bg_layer"), 0, 0);
}

void draw_offstripes(t_hoa_gain *x, t_object *view, t_rect *rect, char isHoriz)
{
    t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("offstripe_layer"), rect->width - (knobMargin*2), rect->height - (knobMargin*2));
    
    if (g)
    {
        jgraphics_translate(g, -hoa_gain_STRIPEWIDTH, -hoa_gain_STRIPEWIDTH);
        jgraphics_set_source_jrgba(g, &x->j_stripecolor);
        jgraphics_diagonal_line_fill(g, hoa_gain_STRIPEWIDTH, 0, 0, rect->width, rect->height);
    }
    
    jbox_end_layer((t_object*)x, view, gensym("offstripe_layer"));
    jbox_paint_layer((t_object *)x, view, gensym("offstripe_layer"), knobMargin, knobMargin);
}

void draw_offrect(t_hoa_gain *x, t_object *view, t_rect *rect, char isHoriz)
{
    t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("offstripe_layer"), rect->width - (knobMargin*2), rect->height - (knobMargin*2));
    
    if (g)
    {
        jgraphics_translate(g, -hoa_gain_STRIPEWIDTH, -hoa_gain_STRIPEWIDTH);
        jgraphics_set_source_jrgba(g, &x->j_stripecolor);
        jgraphics_diagonal_line_fill(g, hoa_gain_STRIPEWIDTH, 0, 0, rect->width, rect->height);
    }
    
    jbox_end_layer((t_object*)x, view, gensym("offstripe_layer"));
    jbox_paint_layer((t_object *)x, view, gensym("offstripe_layer"), knobMargin, knobMargin);
}

void draw_cursor(t_hoa_gain *x, t_object *view, t_rect *rect, char isHoriz)
{    
	t_jgraphics *g;
    
	int pos = hoa_gain_dBvaltopos(x, CLAMP(x->j_valdB, x->f_range[0], x->f_range[1]), rect, isHoriz);
    
    g = jbox_start_layer((t_object *)x, view, gensym("cursor_layer"), rect->width, rect->height);
    
	if (g)
	{
        // draw knob rect
        jgraphics_set_source_jrgba(g, &x->j_knobcolor);
        
        if (isHoriz)
            jgraphics_rectangle(g, pos - hoa_gain_DISPLAYINSET*0.5, knobMargin, hoa_gain_DISPLAYINSET, rect->height - (knobMargin*2));
        else
            jgraphics_rectangle(g, knobMargin, pos - hoa_gain_DISPLAYINSET*0.5, rect->width - (knobMargin*2), hoa_gain_DISPLAYINSET);
        
        jgraphics_fill(g);
	}

    jbox_end_layer((t_object*)x, view, gensym("cursor_layer"));
	jbox_paint_layer((t_object *)x, view, gensym("cursor_layer"), 0., 0.);
}

void draw_valuestripes(t_hoa_gain *x, t_object *view, t_rect *rect, char isHoriz)
{
    t_jgraphics *g;
    t_rect layer;
    
    int pos = hoa_gain_dBvaltopos(x, CLAMP(x->j_valdB, x->f_range[0], x->f_range[1]), rect, isHoriz);
    
    if (isHoriz)
    {
        layer.x = layer.y = knobMargin;
        layer.width = pos - hoa_gain_DISPLAYINSET*0.5 - knobMargin*1.5;
        layer.height = rect->height - (knobMargin*2);
    }
    else
    {
        layer.x = knobMargin;
        layer.y = pos + hoa_gain_DISPLAYINSET*0.5 + knobMargin*0.5;
        layer.width = rect->width - (knobMargin*2);
        layer.height = rect->height - layer.y - knobMargin;
    }
    
    if (MAX(layer.width, 0) == 0 || MAX(layer.height, 0) == 0)
        return;
    
    g = jbox_start_layer((t_object *)x, view, gensym("valuestripe_layer"), layer.width, layer.height);
    
    if (g)
    {
        if (isHoriz)
            jgraphics_translate(g, -hoa_gain_STRIPEWIDTH, -hoa_gain_STRIPEWIDTH);
        else
            jgraphics_translate(g, -hoa_gain_STRIPEWIDTH, hoa_gain_STRIPEWIDTH - pos);
        
        jgraphics_set_source_jrgba(g, &x->j_knobcolor);
        jgraphics_diagonal_line_fill(g, hoa_gain_STRIPEWIDTH, 0, 0, rect->width, rect->height);
    }
    
    jbox_end_layer((t_object*)x, view, gensym("valuestripe_layer"));
    jbox_paint_layer((t_object *)x, view, gensym("valuestripe_layer"), layer.x, layer.y);
}

void draw_valuerect(t_hoa_gain *x, t_object *view, t_rect *rect, char isHoriz)
{
    t_jgraphics *g;
    t_rect layer;
    
    int pos = hoa_gain_dBvaltopos(x, CLAMP(x->j_valdB, x->f_range[0], x->f_range[1]), rect, isHoriz);
    
    if (isHoriz)
    {
        layer.x = layer.y = knobMargin;
        layer.width = pos - hoa_gain_DISPLAYINSET*0.5 - knobMargin*1.5;
        layer.height = rect->height - (knobMargin*2);
    }
    else
    {
        layer.x = knobMargin;
        layer.y = pos + hoa_gain_DISPLAYINSET*0.5 + knobMargin*0.5;
        layer.width = rect->width - (knobMargin*2);
        layer.height = rect->height - layer.y - knobMargin;
        //post("layer.y : %f layer.height : %f", layer.y, layer.height);
    }
    
    post("layer.x : %f layer.y : %f layer.width : %f layer.height : %f", layer.x, layer.y, layer.width, layer.height);
    
    if (max(layer.width, 0.) == 0 || max(layer.height, 0.) == 0)
        return;
    
    g = jbox_start_layer((t_object *)x, view, gensym("valuestripe_layer"), rect->width, rect->height);
    
    if (g)
    {
        post("--- g --- layer.x : %f layer.y : %f layer.width : %f layer.height : %f", layer.x, layer.y, layer.width, layer.height);
        jgraphics_rectangle(g, layer.x, layer.y, layer.width, layer.height);
        jgraphics_set_source_jrgba(g, &x->j_knobcolor);
        jgraphics_fill(g);
    }
    
    jbox_end_layer((t_object*)x, view, gensym("valuestripe_layer"));
    jbox_paint_layer((t_object *)x, view, gensym("valuestripe_layer"), 0., 0.);
}

void hoa_gain_paint(t_hoa_gain *x, t_object *view)
{
    t_rect rect;
    char isHoriz;
    jbox_get_rect_for_view((t_object *)x, view, &rect);
    isHoriz = hoa_gain_ishorizontal(x, &rect);
    draw_background(x, view, &rect, isHoriz);
    draw_offrect(x, view, &rect, isHoriz);
    draw_valuerect(x, view, &rect, isHoriz);
    //draw_offstripes(x, view, &rect, isHoriz);
    //draw_valuestripes(x, view, &rect, isHoriz);
    draw_cursor(x, view, &rect, isHoriz);
}

void hoa_gain_bang(t_hoa_gain *x)
{
    if (x->f_inputMode == MIDI)
    {
        outlet_int(x->f_outlet_infos, (long)hoa_gain_getInputModeValue(x));
    }
    else
    {
        outlet_float(x->f_outlet_infos, hoa_gain_getInputModeValue(x));
    }
}

void hoa_gain_float_dB(t_hoa_gain *x, double dBValue)
{
    x->j_val = hoa_gain_constrain_real_value(x, dBValue) - x->j_min;
    x->j_valdB = x->j_val + x->j_min;
    hoa_gain_set_gain(x);
    object_notify(x, gensym("modified"), NULL);
    jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
    jbox_invalidate_layer((t_object *)x, NULL, gensym("valuestripe_layer"));
    jbox_redraw((t_jbox *)x);
    
    hoa_gain_bang(x);
}

void hoa_gain_int(t_hoa_gain *x, long n)
{
    hoa_gain_assign(x, double(n), true);
	hoa_gain_bang(x);
}

void hoa_gain_float(t_hoa_gain *x, double f)
{
    hoa_gain_assign(x, f, true);
	hoa_gain_bang(x);
}

void hoa_gain_mousedoubleclick(t_hoa_gain *x, t_object *patcherview, t_pt pt, long modifiers)
{
    hoa_gain_float_dB(x, x->j_defaultValuedB);
}

void hoa_gain_mousedown(t_hoa_gain *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
    s_hoa_gain_cum.x = s_hoa_gain_cum.y = 0;
    bool ishoriz = hoa_gain_ishorizontal(x, &rect);
    double val = hoa_gain_postoval(x, pt, &rect, ishoriz);
    
    if (!x->j_relative)
    {
        hoa_gain_float_dB(x, val + x->j_min);	// set value immediately
    }
    s_hoa_gain_startval = x->j_val;
}

void hoa_gain_mousedragdelta(t_hoa_gain *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    double inc;
    double val, cval;
    double factor;
    
    jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
    
    if (modifiers & eShiftKey)
    {
        pt.y *= 0.2;
        pt.x *= 0.2;
    }
    
    // need to cancel cum change if it is beyond zero or the max value
    s_hoa_gain_cum.x += pt.x;
    s_hoa_gain_cum.y -= pt.y;
    if (hoa_gain_ishorizontal(x, &rect))
    {
        factor = x->j_size / (rect.width - (hoa_gain_DISPLAYINSET*2));
        inc = s_hoa_gain_cum.x * factor;
        val = s_hoa_gain_startval + inc;
        cval = hoa_gain_constrain(x,val);
        hoa_gain_float_dB(x,cval + x->j_min);
        // in case we were constrained, adjust the cum
        if (cval != val)
            s_hoa_gain_cum.x = (cval-s_hoa_gain_startval) / factor;
    }
    else
    {
        factor = x->j_size / (rect.height - (hoa_gain_DISPLAYINSET*2));
        inc = s_hoa_gain_cum.y * factor;
        val = s_hoa_gain_startval + inc;
        cval = hoa_gain_constrain(x,val);
        hoa_gain_float_dB(x, cval + x->j_min);
        if (cval != val)
            s_hoa_gain_cum.y = (cval - s_hoa_gain_startval) / factor;
    }
    
    // force to 0dB :
    if (isInside(x->j_valdB, -0.05, 0.05) )
        hoa_gain_float_dB(x, 0);
}

void hoa_gain_mouseup(t_hoa_gain *x, t_object *patcherview, t_pt pt, long modifiers)
{
    double pos, vp;
    t_rect rect;
    double mx, my;
    char ishoriz;
    
    jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
    ishoriz = hoa_gain_ishorizontal(x, &rect);
    pos = hoa_gain_valtopos(x, x->j_val, &rect, ishoriz);
    vp = pos + 0.5;
    if (ishoriz)
    {
        mx = vp;
        my = pt.y;	// have y position be same as where mouse down clicked
    }
    else
    {
        mx = pt.x;	// have x position be same as where mouse down clicked
        my = vp;
    }
    
    if (fabs(pt.x-mx) > 1. || fabs(pt.y-my) > 1.) // this make sure that if you click at the "same" location we don't move the mouse.
        jmouse_setposition_box(patcherview, (t_object*) x, mx, my);
}

void hoa_gain_setminmax(t_hoa_gain *x, t_symbol *s, long argc, t_atom *argv)
{
	double a, b;
	
	if (argc > 1) {
		double old_min = x->j_min;
		double old_size = x->j_size;
		a = b = 0;
		
		if (argv[0].a_type == A_LONG)
			a = (double)atom_getlong(&argv[0]);
		else if (argv[0].a_type == A_FLOAT)
			a = (double)atom_getfloat(&argv[0]);

		if (argv[1].a_type == A_LONG)
			b = (double)atom_getlong(&argv[1]);
		else if (argv[1].a_type == A_FLOAT)
			b = (double)atom_getfloat(&argv[1]);
		
		if (a == b) {
			x->j_min = 0.;
			x->j_size = 1.;
		} else  if (a < b) {
			x->j_min = a;
			x->j_size = b - a;
		} else {			
			x->j_min = b;
			x->j_size = a - b;
		}
        
        x->f_range[0] = x->j_min;
        x->f_range[1] = x->j_size + x->j_min;
		
		if (old_min != x->j_min || old_size != x->j_size)
        {
            jbox_invalidate_layer((t_object *)x, NULL, gensym("bg_layer"));
            jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
            jbox_invalidate_layer((t_object *)x, NULL, gensym("valuestripe_layer"));
            jbox_redraw((t_jbox *)x);
        }
	}
}

void hoa_gain_set(t_hoa_gain *x, t_symbol *s, long argc, t_atom *argv)
{
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG))
		hoa_gain_assign(x, atom_getfloat(argv), true);
}

t_max_err hoa_gain_setattr_interp(t_hoa_gain *x, t_object *attr, long ac, t_atom *av)
{
	if (ac && av && atom_isNumber(av))
    {
        x->f_amp->setRamp(atom_getfloat(av) / 1000. * sys_getsr());
        x->f_interp = x->f_amp->getRamp() / sys_getsr() * 1000;
	}
	return MAX_ERR_NONE;
}

void hoa_gain_resize_io(t_hoa_gain *x, long newNumberOfChannel)
{
    int dspState = sys_getdspobjdspstate((t_object*)x);
    int lastNumberOfChannels = x->f_numberOfChannels;
    newNumberOfChannel = Math<long>::clip(newNumberOfChannel, MIN_IO, MAX_IO);
    
    if (lastNumberOfChannels != newNumberOfChannel)
    {
        if(dspState)
            object_method(gensym("dsp")->s_thing, gensym("stop"));
        
        t_object *b = NULL;
		object_obex_lookup(x, gensym("#B"), (t_object **)&b);
        object_method(b, gensym("dynlet_begin"));
        
        dsp_resize((t_pxobject*)x, newNumberOfChannel+1);
        
        outlet_delete(outlet_nth((t_object*)x, lastNumberOfChannels)); // delete value out outlet
        
        if(lastNumberOfChannels > newNumberOfChannel)
        {
            for(int i = lastNumberOfChannels; i > newNumberOfChannel; i--)
            {
                outlet_delete(outlet_nth((t_object*)x, i-1));
            }
        }
        else if(lastNumberOfChannels < newNumberOfChannel)
        {
            for(int i = lastNumberOfChannels; i < newNumberOfChannel; i++)
            {
                outlet_append((t_object*)x, NULL, gensym("signal"));
            }
        }
        
        x->f_outlet_infos = outlet_append((t_object*)x, NULL, NULL); // restore value out outlet
        
        object_method(b, gensym("dynlet_end"));
        
        if(dspState)
            object_method(gensym("dsp")->s_thing, gensym("start"));
        
        x->f_numberOfChannels = newNumberOfChannel;
    }
}

void hoa_gain_tometer(t_hoa_gain *x, t_symbol *s, long argc, t_atom *argv)
{
    t_object *patcher;
    t_object *gain;
    t_object *object;
    t_object *line;
    t_max_err err;
    
    if(argc && argv && ((s == gensym("angles") || s == gensym("offset") || s == gensym("channels"))))
    {
        err = object_obex_lookup(x, gensym("#P"), (t_object **)&patcher);
        if (err != MAX_ERR_NONE)
            return;
        
        err = object_obex_lookup(x, gensym("#B"), (t_object **)&gain);
        if (err != MAX_ERR_NONE)
            return;
        
        for (line = jpatcher_get_firstline(patcher); line; line = jpatchline_get_nextline(line))
        {
            if (jpatchline_get_box1(line) == gain)
            {
                object = jpatchline_get_box2(line);
                t_symbol *classname = object_classname(jbox_get_object(object));
                if(classname == gensym("hoa.2d.meter~") || classname == gensym("hoa.2d.gain~") || classname == gensym("hoa.2d.vector~"))
                {
                    object_method_typed(jbox_get_object(object), s, argc, argv, NULL);
                }
            }
        }
    }
}

void HoaGain_reconnect_outlet(t_hoa_gain *x)
{
    t_object *patcher;
    t_object *gain_box;
    t_object *obj2_box;
    t_object *obj2;
    t_object *line;
    t_max_err err;
    
    err = object_obex_lookup(x, hoa_sym_pound_P, (t_object **)&patcher);
    if (err != MAX_ERR_NONE)
        return;
    
    err = object_obex_lookup(x, hoa_sym_pound_B, (t_object **)&gain_box);
    if (err != MAX_ERR_NONE)
        return;
    
    for (line = jpatcher_get_firstline(patcher); line; line = jpatchline_get_nextline(line))
    {
        if (jpatchline_get_box1(line) == gain_box)
        {
            obj2_box = jpatchline_get_box2(line);
            obj2 = jbox_get_object(obj2_box);
            t_symbol* classname = object_classname(obj2);
            
            if ( classname == hoa_sym_dac || (object_is_hoa(obj2) && classname != hoa_sym_hoa_pi && classname != hoa_sym_hoa_pi_tilde))
            {
                for(int i = 0; jbox_getinlet((t_jbox *)obj2_box, i) != NULL && i < x->f_numberOfChannels; i++)
                {
                    t_atom msg[4];
                    t_atom rv;
                    
                    atom_setobj(msg, gain_box);
                    atom_setlong(msg + 1, i);
                    atom_setobj(msg + 2, obj2_box);
                    atom_setlong(msg + 3, i);
                    
                    object_method_typed(patcher , hoa_sym_connect, 4, msg, &rv);
                }
            }
        }
    }
}

t_max_err hoa_gain_setattr_channels(t_hoa_gain *x, t_object *attr, long ac, t_atom *av)
{
    long d;
    if (ac && av)
    {
        if (atom_gettype(av) == A_LONG)
        {
            d = atom_getlong(av);
            hoa_gain_resize_io(x, d);
            HoaGain_reconnect_outlet(x);
            hoa_gain_tometer(x, gensym("channels"), ac, av);
        }
    }
    return MAX_ERR_NONE;
}

t_max_err hoa_gain_setattr_range(t_hoa_gain *x, t_object *attr, long ac, t_atom *av)
{
    hoa_gain_setminmax(x, NULL, ac, av);
	return MAX_ERR_NONE;
}

t_max_err hoa_gain_setvalueof(t_hoa_gain *x, long ac, t_atom *av)
{
    if (ac >= 2 && av && atom_isNumber(av))
    {
        x->f_inputMode = Math<char>::clip(atom_getlong(av), 0, 2);
        
        if (av+1 && atom_isNumber(av+1))
        {
            hoa_gain_setInputModeValue(x, atom_getfloat(av+1), false);
        }
        
        hoa_gain_bang(x);
	}
	return MAX_ERR_NONE;
}

t_max_err hoa_gain_getvalueof(t_hoa_gain *x, long *ac, t_atom **av)
{
    if (ac && av)
    {
		if (*ac && *av)
        {
            atom_setlong(*av+0, (long)x->f_inputMode);
            if (x->f_inputMode == MIDI)
            {
                atom_setlong(*av+1, (long)hoa_gain_getInputModeValue(x));
            }
            else
            {
                atom_setfloat(*av+1, hoa_gain_getInputModeValue(x));
            }
		}
        else
        {
            *ac = 2;
			*av = (t_atom *)getbytes(2 * sizeof(t_atom));
        }
        
        atom_setlong(*av+0, (long)x->f_inputMode);
        
        if (x->f_inputMode == MIDI)
        {
            atom_setlong(*av+1, (long)hoa_gain_getInputModeValue(x));
        }
        else
        {
            atom_setfloat(*av+1, hoa_gain_getInputModeValue(x));
        }
    }
	return MAX_ERR_NONE;
}

t_max_err hoa_gain_notify(t_hoa_gain *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	long argc = 0;
	t_atom *argv = NULL;
	t_symbol *name;
	
	if (msg == hoa_sym_attr_modified)
	{
		name = (t_symbol *)object_method((t_object *)data, hoa_sym_getname);
		if (name == gensym("color")) 
		{
			object_attr_getvalueof(x, gensym("color"), &argc, &argv);
			if (argc && argv) 
			{
				object_attr_setvalueof(x, gensym("bgcolor"), argc, argv);
				sysmem_freeptr(argv);
			}
		}
        else if (name == gensym("knobcolor"))
        {
            jbox_invalidate_layer((t_object *)x, NULL, gensym("bg_layer"));
            jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
            jbox_invalidate_layer((t_object *)x, NULL, gensym("valuestripe_layer"));
        }
        else if(name == gensym("bgcolor"))
		{
            jbox_invalidate_layer((t_object *)x, NULL, gensym("bg_layer"));
		}
        else if (name == gensym("stripecolor"))
        {
            jbox_invalidate_layer((t_object *)x, NULL, gensym("offstripe_layer"));
        }
        jbox_redraw((t_jbox *)x);
	}
    
	return jbox_notify((t_jbox *)x, s, msg, sender, data);
}

long hoa_gain_oksize(t_hoa_gain *x, t_rect *newrect)
{
    int isHoriz = hoa_gain_ishorizontal(x, newrect);
    long minWidth, minHeight;
    minWidth = minHeight = 10;
    if (isHoriz) minWidth = 30; // horizontal
    else minHeight = 30; // vertical
    if (newrect->width < minWidth) newrect->width = minWidth;
    if (newrect->height < minHeight) newrect->height = minHeight;
    return 0;
}

void hoa_gain_preset(t_hoa_gain *x)
{
    void *z;
    if (!(z = gensym("_preset")->s_thing))
        return;
    
    binbuf_vinsert(z,(char*)"osslf", x, object_classname(x), gensym("contextvalue"), x->f_inputMode, hoa_gain_getInputModeValue(x));
}

void hoa_gain_free(t_hoa_gain *x)
{
    dsp_freejbox((t_pxjbox *)x);
    jbox_free((t_jbox *)x);
    delete x->f_amp;
}

void *hoa_gain_new(t_symbol *s, short argc, t_atom *argv)
{
    t_hoa_gain* x;
    long flags;
    t_dictionary *d=NULL;
    
    if (!(d=object_dictionaryarg(argc,argv)))
        return NULL;
    
    x = (t_hoa_gain *) object_alloc(s_hoa_gain_class);
    if (!x)
        return NULL;
    
    flags = 0
    | JBOX_DRAWFIRSTIN
    | JBOX_NODRAWBOX
    | JBOX_DRAWINLAST
    | JBOX_GROWBOTH
    | JBOX_DRAWBACKGROUND
    | JBOX_MOUSEDRAGDELTA
    ;
    
    x->j_val = 0;
    x->j_defaultValuedB = 0;
    x->j_valdB = x->j_defaultValuedB;
    x->f_interp = 20;
    x->f_numberOfChannels = 8;
    
    jbox_new((t_jbox *)x, flags, argc, argv);
    x->j_box.z_box.b_firstin = (t_object *)x;
    
    x->f_amp = new Line<t_sample>();
    x->f_amp->setRamp(0.1 * sys_getsr());
    x->f_amp->setValueDirect(1.f);
    
    // inputs
    dsp_setupjbox((t_pxjbox *)x, x->f_numberOfChannels + 1);
    
    // outputs
    x->f_outlet_infos = outlet_new(x, NULL);
    for (int i=0; i < x->f_numberOfChannels; i++)
        outlet_new(x,"signal");
    
    attr_dictionary_process(x,d); // handle attribute args
    
    hoa_gain_set_dB(x, x->j_defaultValuedB);
    
    jbox_ready((t_jbox *)x);
    x->j_box.z_misc = Z_NO_INPLACE;
    
    return x;
}

#ifdef HOA_PACKED_LIB
int hoa_gain_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.gain~", (method)hoa_gain_new, (method)hoa_gain_free, sizeof(t_hoa_gain), (method)NULL, A_GIMME, 0L);
    class_setname((char *)"hoa.gain~", (char *)"hoa.gain~");
    
    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
    class_dspinitjbox(c);
    jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR );
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    class_addmethod (c, (method) hoa_gain_assist,              "assist",               A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_paint,               "paint",                A_CANT, 0);
    
    // @method signal @digest The inputs signals to be scaled by the slider.
    // @description The inputs signals to be scaled by the slider.
    class_addmethod (c, (method) hoa_gain_dsp64,               "dsp64",                A_CANT, 0);
    
    // @method int @digest Set the value of the slider
    // @description The <m>int</m> method sets the value of the slider, ramps the output signal to the level corresponding to the new value over the specified ramp time, and outputs the slider's value out the right outlet.
    // @marg 0 @name value @optional 0 @type int
    class_addmethod (c, (method) hoa_gain_int,                 "int",                  A_LONG, 0);
    
    // @method float @digest Set the value of the slider
    // @description The <m>float</m> method sets the value of the slider, ramps the output signal to the level corresponding to the new value over the specified ramp time, and outputs the slider's value out the right outlet.
    // @marg 0 @name value @optional 0 @type int
    class_addmethod (c, (method) hoa_gain_float,               "float",                A_FLOAT, 0);
    
    // @method contextvalue @digest Set the value of the slider with DeciBels, Amplitude or MIDI value type.
    // @description Set the value of the slider with DeciBels, Amplitude or MIDI value type.
    // @marg 0 @name inputmode @optional 0 @type int @description 0 = DeciBels, 1 = 1mplitude, 2 = MIDI.
    // @marg 1 @name value @optional 0 @type float @description The slider value
    class_addmethod (c, (method) hoa_gain_contextValue,        "contextvalue", A_LONG, A_FLOAT, 0);
    
    // @method bang @digest Send current value out right outlet.
    // @description The <m>bang</m> message send current value out right outlet.
    class_addmethod (c, (method) hoa_gain_bang,                "bang",                         0);
    
    // @method set @digest Set the value of the slider without outputting slider value.
    // @description The word set, followed by a number, sets the value of the slider, ramps the output signal to the level corresponding to the new value over the specified ramp time, but does not output the slider's value out the right outlet.
    // @marg 1 @name value @optional 0 @type float @description The slider value
    class_addmethod (c, (method) hoa_gain_set,                 "set",                  A_GIMME, 0);
    class_addmethod (c, (method) hoa_gain_tometer,             "anything",             A_GIMME, 0);
    
    // @method (mouse) @digest click and drag to set the slider outlet.
    // @description Clicking and dragging with the mouse sets the value of the slider, ramps the output signal to the level corresponding to the new value over the specified ramp time, and outputs the slider’s value out the right outlet. double-click to set the slider value to <m>defvaldb</m>
    
    class_addmethod (c, (method) hoa_gain_mousedoubleclick,    "mousedoubleclick",     A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_mousedown,           "mousedown",            A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_mousedragdelta,      "mousedragdelta",       A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_mouseup,             "mouseup",              A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_getvalueof,          "getvalueof",           A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_setvalueof,          "setvalueof",           A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_preset,              "preset",                       0);
    class_addmethod (c, (method) hoa_gain_notify,              "notify",               A_CANT, 0);
    class_addmethod (c, (method) hoa_gain_oksize,              "oksize",               A_CANT, 0);
    
    CLASS_ATTR_DEFAULT(c,"patching_rect",0, "0. 0. 140. 22.");
    
    CLASS_STICKY_CATEGORY		(c,0,"Value");
    
    CLASS_ATTR_DOUBLE			(c, "interp", 0, t_hoa_gain, f_interp);
    CLASS_ATTR_ACCESSORS		(c, "interp",(method)NULL,(method)hoa_gain_setattr_interp);
    CLASS_ATTR_LABEL			(c, "interp", 0, "Ramp Time (ms)");
    CLASS_ATTR_FILTER_MIN		(c, "interp", 0);
    // @description Ramp time in milliseconds
    
    CLASS_ATTR_CHAR				(c,"relative", 0, t_hoa_gain, j_relative);
    CLASS_ATTR_LABEL			(c,"relative", 0, "Mousing Mode");
    CLASS_ATTR_ENUMINDEX2		(c, "relative", 0, "Absolute", "Relative");
    CLASS_ATTR_BASIC			(c, "relative", 0);
    // @description Mousing can either be <b>absolute</b> or <b>relative</b>
    
    CLASS_ATTR_CHAR				(c,"inputmode", 0, t_hoa_gain, f_inputMode);
    CLASS_ATTR_LABEL			(c,"inputmode", 0, "Input Mode");
    CLASS_ATTR_ENUMINDEX3		(c, "inputmode", 0, "DeciBels", "Amplitude", "Midi");
    // @description Input mode can either be in <b>DeciBels</b>, <b>Amplitude</b> or <b>Midi</b>
    
    CLASS_ATTR_DOUBLE			(c, "defvaldb", 0, t_hoa_gain, j_defaultValuedB);
    CLASS_ATTR_LABEL			(c, "defvaldb", 0, "Default Value (dB)");
    // @description Default value in <b>DeciBels</b>, <b>Amplitude</b> or <b>Midi</b>
    
    CLASS_STICKY_CATEGORY_CLEAR(c);
    
    CLASS_ATTR_CHAR				(c,"orientation",0,t_hoa_gain,j_orientation);
    CLASS_ATTR_LABEL			(c,"orientation",0,"Orientation");
    CLASS_ATTR_ENUMINDEX3		(c,"orientation", 0,"Automatic", "Horizontal", "Vertical");
    CLASS_ATTR_CATEGORY			(c, "orientation", 0, "Appearance");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "orientation", 0, "0");
    // @description Orientation can either be in <b>Automatic</b>, <b>Horizontal</b> or <b>Vertical</b>
    
    CLASS_STICKY_CATEGORY(c, 0, "Color");
    
    CLASS_ATTR_RGBA_LEGACY		(c, "bgcolor", "brgb", 0, t_hoa_gain, j_brgba);
    CLASS_ATTR_ALIAS			(c,"bgcolor", "brgba");
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c,"bgcolor",0,"0.290196 0.309804 0.301961 1.");
    CLASS_ATTR_STYLE_LABEL		(c, "bgcolor", 0, "rgba", "Background Color");
    class_parameter_register_default_color(c, gensym("bgcolor"), ps_control_text_bg);
    CLASS_ATTR_BASIC			(c, "bgcolor", 0);
    // @description Sets the RGBA values for the background color of the <o>hoa.gain~</o> object
    
    CLASS_ATTR_RGBA				(c, "knobcolor", 0, t_hoa_gain, j_knobcolor);
    CLASS_ATTR_STYLE_LABEL      (c, "knobcolor", 0, "rgba","Value Color");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "knobcolor", 0, "0.803922 0.898039 0.909804 1.0");
    // @description Sets the RGBA values for the knob color of the <o>hoa.gain~</o> object
    
    CLASS_ATTR_RGBA				(c, "stripecolor", 0, t_hoa_gain, j_stripecolor);
    CLASS_ATTR_STYLE_LABEL      (c, "stripecolor", 0, "rgba","Off Color");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "stripecolor", 0, "0.376471 0.384314 0.4 1.0");
    // @description Sets the RGBA values for the "Off" value stripe color of the <o>hoa.gain~</o> object
    
    CLASS_STICKY_CATEGORY_CLEAR(c);
    
    CLASS_ATTR_INVISIBLE(c, "color", 0);
    CLASS_ATTR_ATTR_PARSE(c, "color","save", USESYM(long), 0, "0");
    // @exclude hoa.gain~
    
    CLASS_ATTR_DEFAULT_SAVE		(c,"relative",0,"0");
    CLASS_ATTR_DEFAULT_SAVE		(c,"defvaldb",0,"0");
    CLASS_ATTR_DEFAULT_SAVE		(c,"inputmode",0,"0");
    CLASS_ATTR_DEFAULT_SAVE		(c,"interp",0,"20");
    CLASS_ATTR_LABEL			(c,"min", 0, "Output Minimum");
    CLASS_ATTR_LABEL			(c,"mult", 0, "Output Multiplier");
    CLASS_ATTR_STYLE_LABEL		(c,"floatoutput",0,"onoff","Float Output");
    
    CLASS_ATTR_ORDER			(c, "relative",		0, "2");
    CLASS_ATTR_ORDER			(c, "size",			0, "3");
    CLASS_ATTR_ORDER			(c, "min",			0, "4");
    CLASS_ATTR_ORDER			(c, "mult",			0, "5");
    
    CLASS_ATTR_CATEGORY			(c, "channels", 0, "Custom");
    CLASS_ATTR_LONG				(c, "channels", 0, t_hoa_gain, f_numberOfChannels);
    CLASS_ATTR_ACCESSORS        (c, "channels", (method)NULL,(method)hoa_gain_setattr_channels);
    CLASS_ATTR_ORDER			(c, "channels", 0, "1");
    CLASS_ATTR_LABEL			(c, "channels", 0, "Number of Channels");
    CLASS_ATTR_FILTER_CLIP		(c, "channels", 1, MAX_IO);
    CLASS_ATTR_DEFAULT			(c, "channels", 0, "8");
    CLASS_ATTR_SAVE				(c, "channels", 1);
    
    CLASS_ATTR_CATEGORY			(c, "range", 0, "Value");
    CLASS_ATTR_FLOAT_ARRAY      (c, "range", 0, t_hoa_gain, f_range, 2);
    CLASS_ATTR_ACCESSORS        (c, "range", (method)NULL,(method)hoa_gain_setattr_range);
    CLASS_ATTR_ORDER			(c, "range", 0, "2");
    CLASS_ATTR_LABEL			(c, "range", 0, "Range (dB)");
    CLASS_ATTR_DEFAULT			(c, "range", 0, "-70. 18.");
    CLASS_ATTR_SAVE             (c, "range", 1);
    
    class_register(CLASS_BOX, c);
    s_hoa_gain_class = c;
    
    return 0;
}
