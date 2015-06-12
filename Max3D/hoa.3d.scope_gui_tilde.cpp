/*
 // Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      hoa.3d.scope~.cpp
 @name      hoa.3d.scope~
 @realname  hoa.3d.scope~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A spherical harmonic visualizer
 
 @description
 <o>hoa.3d.scope~</o> displays spherical harmonics of an ambisonic sound field
 
 @discussion
 <o>hoa.3d.scope~</o> displays spherical harmonics of an ambisonic sound field
 
 @category ambisonics, hoa objects, audio, GUI, MSP
 
 @seealso hoa.3d.meter~, hoa.2d.scope~, hoa.3d.decoder~, hoa.3d.encoder~, hoa.3d.map~, hoa.3d.optim~, hoa.3d.vector~, hoa.3d.wider~, hoa.dac~
 
 @illustration on @caption the hoa.3d.scope~ GUI object
 @palette yes
 */

#include "Hoa3D.max.h"

#define  contrast_white 0.06
#define  contrast_black 0.14

typedef struct  _hoa_3d_scope 
{
	t_pxjbox                j_box;
    Scope<Hoa3d, t_sample>* f_scope;
    t_sample*               f_signals;
    t_clock*                f_clock;
    int                     f_startclock;
    t_atom_long             f_interval;
    
	t_atom_long             f_order;
    float                   f_gain;
    double                  f_view[3];
	
	t_jrgba                 f_color_bg;
	t_jrgba                 f_color_nh;
	t_jrgba                 f_color_ph;
	
	double                  f_center;
	double                  f_radius;
    
} t_hoa_3d_scope;

t_class *hoa_3d_scope_class;

t_hoa_err hoa_getinfos(t_hoa_3d_scope* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs    = x->f_scope->getNumberOfHarmonics();
	boxinfos->autoconnect_outputs   = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_3d_scope_perform64(t_hoa_3d_scope *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(sampleframes, ins[i], 1, x->f_signals+i, numins);
    }
    Signal<t_sample>::scale(numins * sampleframes, x->f_gain, x->f_signals);
    if(x->f_startclock)
    {
        x->f_startclock = 0;
        clock_delay(x->f_clock, 0);
    }
}

void hoa_3d_scope_dsp64(t_hoa_3d_scope *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp64, gensym("dsp_add64"), x, hoa_3d_scope_perform64, 0, NULL);
    x->f_startclock = 1;
}

void hoa_3d_scope_tick(t_hoa_3d_scope *x)
{
    x->f_scope->process(x->f_signals);

	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
	jbox_redraw((t_jbox *)x);
	if (sys_getdspstate())
		clock_fdelay(x->f_clock, x->f_interval);
}

void hoa_3d_scope_free(t_hoa_3d_scope *x)
{
	dsp_freejbox((t_pxjbox *)x);
	clock_free(x->f_clock);
	jbox_free((t_jbox *)x);
    
    delete x->f_scope;
    delete [] x->f_signals;
}

void hoa_3d_scope_assist(t_hoa_3d_scope *x, void *b, long m, long a, char *s)
{
    sprintf(s,"(signal) %s", x->f_scope->getHarmonicName(a).c_str());
}

t_max_err hoa_3d_scope_notify(t_hoa_3d_scope *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	t_symbol *name;
	if (msg == hoa_sym_attr_modified)
	{
		name = (t_symbol *)object_method((t_object *)data, hoa_sym_getname);
		if( name == hoa_sym_bgcolor || name == gensym("order"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_background_layer);
		}
		else if(name == gensym("phcolor") || name == gensym("nhcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
		}
		jbox_redraw((t_jbox *)x);
	}
	return jbox_notify((t_jbox *)x, s, msg, sender, data);
}

long hoa_3d_scope_oksize(t_hoa_3d_scope *x, t_rect *newrect)
{
	if (newrect->width < 20)
		newrect->width = newrect->height = 20;
	return 0;
}

void draw_background(t_hoa_3d_scope *x,  t_object *view, t_rect *rect)
{
    t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_background_layer, rect->width, rect->height);
    
    if (g)
    {
        jgraphics_rectangle(g, 0, 0, rect->width, rect->height);
        jgraphics_set_source_jrgba(g, &x->f_color_bg);
        jgraphics_fill(g);
        
        // Border
        jgraphics_rectangle(g, 0, 0, rect->width, rect->height);
        t_jrgba black = rgba_addContrast(x->f_color_bg, -contrast_black);
        jgraphics_set_source_jrgba(g, &black);
        jgraphics_set_line_width(g, 1.);
        jgraphics_stroke(g);
        
        jbox_end_layer((t_object*)x, view, hoa_sym_background_layer);
    }
    jbox_paint_layer((t_object *)x, view, hoa_sym_background_layer, 0., 0.);
}

void draw_harmonics(t_hoa_3d_scope *x, t_object *view, t_rect *rect)
{
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_harmonics_layer, rect->width, rect->height);
    
	if (g)
	{
        char pathLength;
        t_jmatrix transform;
        t_jrgba color_pos;
        t_jrgba color_neg;
        t_jrgba black = rgba_addContrast(x->f_color_bg, -contrast_black);
        t_jrgba white = rgba_addContrast(x->f_color_bg, contrast_white);
        
        jgraphics_rotate(g, HOA_PI);
        jgraphics_set_line_width(g, 1);
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
        
        for(ulong j = 0; j < x->f_scope->getNumberOfRows() * 0.5; j++)
        {
            pathLength = 0;
            double constrast = (j - x->f_scope->getNumberOfRows() * 0.5) / (double)x->f_scope->getNumberOfRows();
            color_pos = rgba_addContrast(x->f_color_ph, constrast);
            color_neg = rgba_addContrast(x->f_color_nh, constrast);
            jgraphics_set_source_jrgba(g, &color_pos);
            double elev = x->f_scope->getPointElevation(j);
            
            for(int i = 0; i < x->f_scope->getNumberOfColumns(); i++)
            {
                double azim = x->f_scope->getPointAzimuth(i);
                double value = x->f_scope->getPointValue(j, i);
                if(value >= 0)
                {
                    value *= x->f_radius;
                    if(!pathLength)
                    {
                        jgraphics_move_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                        pathLength++;
                    }
                    else
                    {
                        jgraphics_line_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                    }
                }
            }
            jgraphics_close_path(g);
            if(pathLength)
                jgraphics_fill(g);
            
            pathLength = 0;
            jgraphics_set_source_jrgba(g, &color_neg);
            
            for(int i = 0; i < x->f_scope->getNumberOfColumns(); i++)
            {
                double azim = x->f_scope->getPointAzimuth(i);
                double value = x->f_scope->getPointValue(j, i);
                if(value < 0)
                {
                    value *= -x->f_radius;
                    if(!pathLength)
                    {
                        jgraphics_move_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                        pathLength++;
                    }
                    else
                    {
                        jgraphics_line_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                    }
                }
            }
            jgraphics_close_path(g);
            if(pathLength)
                jgraphics_fill(g);
        }
        
		double angle, x1, x2, y1, y2, cosa, sina;
        for(int i = 0; i < (x->f_order * 2 + 2) ; i++)
		{
            angle = ((double)(i - 0.5) / (x->f_order * 2 + 2) * HOA_2PI);
			cosa = cos(angle);
            sina = sin(angle);
            x1 = cosa * x->f_radius * 0.2;
			y1 = sina * x->f_radius * 0.2;
            x2 = cosa * x->f_radius;
			y2 = sina * x->f_radius;
            
            jgraphics_move_to(g, x1, y1);
			jgraphics_line_to(g, x2, y2);
            jgraphics_set_line_width(g, 3);
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_stroke(g);
            jgraphics_set_source_jrgba(g, &black);
			jgraphics_set_line_width(g, 1);
			jgraphics_stroke(g);
		}
        
        for(int i = 5; i > 0; i--)
		{
            jgraphics_set_line_width(g, 3);
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_arc(g, 0, 0, (double)i * 0.2 * x->f_radius,  0., HOA_2PI);
            jgraphics_stroke(g);
            jgraphics_set_line_width(g, 1);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_stroke(g);
		}
        
        for(int j = x->f_scope->getNumberOfRows() * 0.5; j < x->f_scope->getNumberOfRows(); j++)
        {
            // positive harmonics
            pathLength = 0;
            double constrast = (j - x->f_scope->getNumberOfRows() * 0.5) / (double)x->f_scope->getNumberOfRows();
            color_pos = rgba_addContrast(x->f_color_ph, constrast);
            color_neg = rgba_addContrast(x->f_color_nh, constrast);
            jgraphics_set_source_jrgba(g, &color_pos);
            double elev = x->f_scope->getPointElevation(j);
            for(int i = 0; i < x->f_scope->getNumberOfColumns(); i++)
            {
                double azim = x->f_scope->getPointAzimuth(i);
                double value = x->f_scope->getPointValue(j, i);
                if(value >= 0)
                {
                    value *= x->f_radius;
                    if(!pathLength)
                    {
                        jgraphics_move_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                        pathLength++;
                    }
                    else
                    {
                        jgraphics_line_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                    }
                }
            }
            jgraphics_close_path(g);
            if(pathLength)
                jgraphics_fill(g);
            
            pathLength = 0;
            // negative harmonics
            jgraphics_set_source_jrgba(g, &color_neg);
            for(int i = 0; i < x->f_scope->getNumberOfColumns(); i++)
            {
                double azim = x->f_scope->getPointAzimuth(i);
                double value = x->f_scope->getPointValue(j, i);
                if(value < 0)
                {
                    value *= -x->f_radius;
                    if(!pathLength)
                    {
                        jgraphics_move_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                        pathLength++;
                    }
                    else
                    {
                        jgraphics_line_to(g, Math<double>::abscissa(value, azim,elev), Math<double>::ordinate(value, azim, elev));
                    }
                }
            }
            jgraphics_close_path(g);
            if(pathLength)
                jgraphics_fill(g);
        }
		
		jbox_end_layer((t_object*)x, view, hoa_sym_harmonics_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_harmonics_layer, 0., 0.);
}

void hoa_3d_scope_paint(t_hoa_3d_scope *x, t_object *view)
{
    t_rect rect;
    jbox_get_rect_for_view((t_object *)x, view, &rect);
    
    x->f_center = rect.width * .5;
    x->f_radius = x->f_center * 0.95;
    
    draw_background(x, view, &rect);
    draw_harmonics(x, view, &rect);
}

t_max_err set_order(t_hoa_3d_scope *x, t_object *attr, long ac, t_atom *av)
{
    long order;
    t_object *b = NULL;
	if (ac && av && atom_gettype(av) == A_LONG)
    {
        order = atom_getlong(av);
        if(order != x->f_scope->getDecompositionOrder() && order > 0)
        {
            object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
            
            delete x->f_scope;
            delete [] x->f_signals;
            x->f_scope      =  new Scope<Hoa3d, t_sample>(order, HOA_DISPLAY_NPOINTS * 0.25, HOA_DISPLAY_NPOINTS * 0.5);
            x->f_order      = x->f_scope->getDecompositionOrder();
            x->f_signals    = new double[x->f_scope->getNumberOfHarmonics() * SYS_MAXBLKSIZE];
            
            object_obex_lookup(x, hoa_sym_pound_B, (t_object **)&b);
            
            object_method(b, hoa_sym_dynlet_begin);
            dsp_resize((t_pxobject*)x, x->f_scope->getNumberOfHarmonics());
            object_method(b, hoa_sym_dynlet_end);
            
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_background_layer);
            jbox_redraw((t_jbox *)x);
        }
	}
    
	return MAX_ERR_NONE;
}

t_max_err view_set(t_hoa_3d_scope *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av && atom_isNumber(av))
    {
        //object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
        
        if(atom_gettype(av) == A_FLOAT)
            x->f_view[0] = atom_getfloat(av);
        else
            x->f_view[0] = x->f_scope->getViewRotationX() * 360. / HOA_2PI;
        if(ac > 1 && atom_gettype(av+1) == A_FLOAT)
            x->f_view[1] = atom_getfloat(av+1);
        else
            x->f_view[1] = x->f_scope->getViewRotationY() * 360. / HOA_2PI;
        if(ac > 2 && atom_isNumber(av+2))
            x->f_view[2] = atom_getfloat(av+2);
        else
            x->f_view[2] = x->f_scope->getViewRotationZ() * 360. / HOA_2PI;
        
        x->f_scope->setViewRotation(x->f_view[0] / 360. * HOA_2PI, x->f_view[1] / 360. * HOA_2PI, x->f_view[2] / 360. * HOA_2PI);
        x->f_scope->computeRendering();
        
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
        jbox_redraw((t_jbox *)x);
    }
    
    return MAX_ERR_NONE;
}

void *hoa_3d_scope_new(t_symbol *s, int argc, t_atom *argv)
{
    t_hoa_3d_scope *x =  NULL;
    t_dictionary *d;
    long flags;
    
    if(!(d = object_dictionaryarg(argc,argv)))
        return NULL;
    
    x = (t_hoa_3d_scope *)object_alloc(hoa_3d_scope_class);
    if (x && d)
    {
        flags = 0
        | JBOX_DRAWFIRSTIN
        | JBOX_DRAWINLAST
        | JBOX_NODRAWBOX
        | JBOX_DRAWBACKGROUND
        | JBOX_TRANSPARENT
        | JBOX_GROWY
        ;
        
        jbox_new((t_jbox *)x, flags, argc, argv);
        x->j_box.z_box.b_firstin = (t_object *)x;
        
        dictionary_getlong(d, gensym("order"), (t_atom_long *)&x->f_order);
        if(x->f_order < 1)
            x->f_order = 1;
        
        x->f_order      = 1;
        x->f_startclock = 0;
        x->f_scope      = new Scope<Hoa3d, t_sample>(x->f_order, HOA_DISPLAY_NPOINTS * 0.25, HOA_DISPLAY_NPOINTS * 0.5);
        x->f_order      = x->f_scope->getDecompositionOrder();
        x->f_signals    = new t_sample[x->f_scope->getNumberOfHarmonics() * HOA_MAXBLKSIZE];
        x->f_view[0] = x->f_view[1] = x->f_view[2] = 0.;
        
        dsp_setupjbox((t_pxjbox *)x, x->f_scope->getNumberOfHarmonics());
        
        x->f_clock = (t_clock*)clock_new(x,(method)hoa_3d_scope_tick);
        x->f_startclock = 0;
        
        attr_dictionary_process(x, d);
        jbox_ready((t_jbox *)x);
    }
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_3d_scope_main(void)
#else
void ext_main(void *r)
#endif
{
    t_class *c;
    
    c = class_new("hoa.3d.scope~", (method)hoa_3d_scope_new, (method)hoa_3d_scope_free, (short)sizeof(t_hoa_3d_scope), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.3d.scope~", (char *)"hoa.3d.scope~");
    
    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
    class_dspinitjbox(c);
    jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH);
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of spherical harmonic signals that represent a sound field
    // @description Array of spherical harmonic signals that represent a sound field
    class_addmethod(c, (method)hoa_3d_scope_dsp64,		"dsp64",		A_CANT, 0);
    class_addmethod(c, (method)hoa_3d_scope_assist,		"assist",		A_CANT,	0);
    class_addmethod(c, (method)hoa_3d_scope_paint,		"paint",		A_CANT,	0);
    class_addmethod(c, (method)hoa_3d_scope_notify,		"notify",		A_CANT, 0);
    class_addmethod(c, (method)hoa_3d_scope_oksize,		"oksize",		A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "color", 0);
    CLASS_ATTR_INVISIBLE            (c, "textcolor", 0);
    CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0 0 150 150");
    
    CLASS_ATTR_LONG                 (c, "order", 0, t_hoa_3d_scope, f_order);
    CLASS_ATTR_ACCESSORS            (c, "order", NULL, set_order);
    CLASS_ATTR_CATEGORY             (c, "order", 0, "Ambisonic");
    CLASS_ATTR_ORDER                (c, "order", 0, "1");
    CLASS_ATTR_LABEL                (c, "order", 0, "Ambisonic Order");
    CLASS_ATTR_FILTER_MIN           (c, "order", 1);
    CLASS_ATTR_DEFAULT              (c, "order", 0, "1");
    CLASS_ATTR_SAVE                 (c, "order", 1);
    // @description The ambisonic order of decomposition. Will adapt the number of input accordingly.
    
    CLASS_ATTR_FLOAT                (c, "gain", 0, t_hoa_3d_scope, f_gain);
    CLASS_ATTR_CATEGORY             (c, "gain", 0, "Behavior");
    CLASS_ATTR_ORDER                (c, "gain", 0, "1");
    CLASS_ATTR_LABEL                (c, "gain", 0, "Gain");
    CLASS_ATTR_FILTER_MIN           (c, "gain", 1.);
    CLASS_ATTR_DEFAULT              (c, "gain", 0, "1.");
    CLASS_ATTR_SAVE                 (c, "gain", 1);
    // @description The <b>gain</b> factor can be used to offer a better visualisation of low amplitude sound fields.
    
    CLASS_ATTR_LONG                 (c, "interval", 0, t_hoa_3d_scope, f_interval);
    CLASS_ATTR_CATEGORY             (c, "interval", 0, "Behavior");
    CLASS_ATTR_ORDER                (c, "interval", 0, "2");
    CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval in Milliseconds");
    CLASS_ATTR_FILTER_MIN           (c, "interval", 20);
    CLASS_ATTR_DEFAULT              (c, "interval", 0, "100");
    CLASS_ATTR_SAVE                 (c, "interval", 1);
    // @description The refresh interval time in milliseconds.
    
    CLASS_ATTR_DOUBLE_ARRAY         (c, "view", 0, t_hoa_3d_scope, f_view, 3);
    CLASS_ATTR_CATEGORY             (c, "view", 0, "Behavior");
    CLASS_ATTR_ORDER                (c, "view", 0, "3");
    CLASS_ATTR_LABEL                (c, "view", 0, "Offset of the View");
    CLASS_ATTR_ACCESSORS            (c, "view", NULL, view_set);
    CLASS_ATTR_DEFAULT              (c, "view", 0, "0. 0. 0.");
    CLASS_ATTR_SAVE                 (c, "view", 1);
    // @description Set the offset of the view with a list of 3 double values corresponding to the x y z offset, in degrees between 0. and 360.
    
    CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_hoa_3d_scope, f_color_bg);
    CLASS_ATTR_CATEGORY             (c, "bgcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.76 0.76 0.76 1.");
    // @description Sets the RGBA values for the background color of the <o>hoa.3d.scope~</o> object
    
    CLASS_ATTR_RGBA                 (c, "phcolor", 0, t_hoa_3d_scope, f_color_ph);
    CLASS_ATTR_CATEGORY             (c, "phcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "phcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "phcolor", 0, "Positive Harmonics Color");
    CLASS_ATTR_ORDER                (c, "phcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "phcolor", 0, "1. 0. 0. 1.");
    // @description Sets the RGBA values for the positive harmonics color of the <o>hoa.3d.scope~</o> object
    
    CLASS_ATTR_RGBA                 (c, "nhcolor", 0, t_hoa_3d_scope, f_color_nh);
    CLASS_ATTR_CATEGORY             (c, "nhcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "nhcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "nhcolor", 0, "Negative Harmonics Color");
    CLASS_ATTR_ORDER                (c, "nhcolor", 0, "4");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "nhcolor", 0, "0. 0. 1. 1.");
    // @description Sets the RGBA values for the negative harmonics color of the <o>hoa.3d.scope~</o> object
    
    class_register(CLASS_BOX, c);
    hoa_3d_scope_class = c;
}
