/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.space.cpp
 @name      hoa.2d.space
 @realname  hoa.2d.space
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A graphic user interface to design 2d ambisonic space
 
 @description
 <o>hoa.2d.space</o> is a circular array of slider allowing you to draw and set channel dependant coefficients that can transform your ambisonic soundfields mainly in the plane wave domain.
 
 @discussion
 <o>hoa.2d.space</o> is a circular array of slider allowing you to draw and set channel dependant coefficients that can transform your ambisonic soundfields mainly in the plane wave domain.
 
 @category ambisonics, hoa objects, GUI
 
 @seealso  hoa.2d.process~, hoa.fx.gain~, hoa.fx.delay~, hoa.gain~, hoa.2d.recomposer, hoa.2d.projector~, hoa.2d.recomposer~, hoa.2d.meter~, hoa.2d.map, hoa.2d.decoder~, hoa.2d.encoder~
 
 @illustration on @caption the hoa.2d.space GUI object
 @palette yes
 */

#include "Hoa2D.max.h"

typedef struct  _hoa_2d_space
{
	t_jbox		j_box;
	void*		f_out;
	void*		f_out_mouse;
    long        f_number_of_channels;
    double      f_minmax[2];
	long		f_floatoutput;
	t_symbol*	f_slider_style;
	long		f_draw_value;
	long		f_grid;
	long		f_snaptogrid;
	long		f_save_with_patcher;
    
    long		f_mode;
    double*     f_channel_values;
	double*     f_channel_refs;
    double*     f_channel_radius;
    double      f_value_ref;
    double      f_angle_ref;
    
	t_jrgba     f_color_bg;
    t_jrgba     f_color_lines;
	t_jrgba		f_color_sp;
	t_jrgba		f_color_pt;
	t_jrgba		f_color_sel;
	t_jrgba		f_color_text;

	double		f_center;
    double      f_radius;
	
	long		f_last_mouse_index;
} t_hoa_2d_space;

t_class *hoa_2d_space_class;

#define  contrast_white 0.06
#define  contrast_black 0.14

t_hoa_err hoa_getinfos(t_hoa_2d_space* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs    = 0;
	boxinfos->autoconnect_outputs   = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_2d_space_output(t_hoa_2d_space *x)
{
    t_atom argv[HOA_MAX_PLANEWAVES];
    for(int i = 0; i < x->f_number_of_channels; i++)
    {
        if (x->f_floatoutput)
            atom_setfloat(argv+i, x->f_channel_values[i]);
        else
            atom_setlong(argv+i, x->f_channel_values[i]);
    }
    
    outlet_list(x->f_out, 0L, x->f_number_of_channels, argv);
}

void hoa_2d_space_list(t_hoa_2d_space *x, t_symbol *s, long ac, t_atom *av)
{
    if(ac && av)
    {
		if (atom_gettype(av) == A_LONG)
		{
			long index = atom_getlong(av) - 1;
			if (index < 0 || index >= x->f_number_of_channels)
				return;
			
			if (ac > 1 && (atom_gettype(av+1) == A_LONG || atom_gettype(av+1) == A_FLOAT))
			{
                x->f_channel_values[index] = Math<double>::clip(atom_getfloat(av+1), x->f_minmax[0], x->f_minmax[1]);
			}
			
		}
		else
		{
			for(int i = 0; i < x->f_number_of_channels && i < ac; i++)
			{
				if(atom_gettype(av+i) == A_FLOAT || atom_gettype(av+i) == A_LONG)
				{
					x->f_channel_values[i] = Math<double>::clip(atom_getfloat(av+i), x->f_minmax[0], x->f_minmax[1]);
				}
			}
		}
        
        hoa_2d_space_output(x);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
        jbox_redraw((t_jbox *)x);
        object_notify(x, hoa_sym_modified, NULL);
    }
}


void hoa_2d_space_free(t_hoa_2d_space *x)
{
	jbox_free((t_jbox *)x);
    delete [] x->f_channel_values;
    delete [] x->f_channel_refs;
    delete [] x->f_channel_radius;
}

void hoa_2d_space_assist(t_hoa_2d_space *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET)
	{
		sprintf(s,"(anything) Behavior and appearance");
	}
	else
	{
		if (a == 0)
			sprintf(s,"(list) Channels coefficients");
		else if (a == 1)
			sprintf(s,"(int) Mouseover index");
	}
}

t_max_err hoa_2d_space_notify(t_hoa_2d_space *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	t_symbol *name;
	if (msg == hoa_sym_attr_modified)
	{
		name = (t_symbol *)object_method((t_object *)data, hoa_sym_getname);
		
		if(name == hoa_sym_bgcolor || name == gensym("linescolor") || name == gensym("grid"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_background_layer);
		}
		else if(name == gensym("spcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
		}
		else if(name == gensym("ptcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
		}
		else if(name == gensym("sliderstyle"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
		}
		else if(name == gensym("textcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
		}
		else if(name == gensym("floatoutput"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
		}
		else if(name == gensym("selcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
		}
		jbox_redraw((t_jbox *)x);
	}
	return jbox_notify((t_jbox *)x, s, msg, sender, data);
}

void hoa_2d_space_preset(t_hoa_2d_space *x)
{
    void* z;
    t_atom av[HOA_MAX_PLANEWAVES + 3];
    
    if(!(z = gensym("_preset")->s_thing))
        return;
    
    atom_setobj(av, x);
    atom_setsym(av+1, object_classname(x));
    atom_setsym(av+2, hoa_sym_list);
    for(int i = 0; i < x->f_number_of_channels; i++)
        atom_setfloat(av+i+3, x->f_channel_values[i]);
    
    binbuf_insert(z, NULL, x->f_number_of_channels+3, av);
}

t_max_err hoa_2d_space_setvalueof(t_hoa_2d_space *x, long ac, t_atom *av)
{
    hoa_2d_space_list(x, NULL, ac, av);
	return MAX_ERR_NONE;
}

t_max_err hoa_2d_space_getvalueof(t_hoa_2d_space *x, long *ac, t_atom **av)
{
    int limit = 0;
	if(ac && av)
    {
		if(*ac && *av)
        {
            limit = *ac;
            if(limit > x->f_number_of_channels)
                limit = x->f_number_of_channels;
		}
        else
        {
			*av = (t_atom *)getbytes(x->f_number_of_channels * sizeof(t_atom));
            limit = *ac = x->f_number_of_channels;
        }
        
        for(int i = 0; i < limit; i++)
            atom_setfloat(*av+i, x->f_channel_values[i]);
    }
	return MAX_ERR_NONE;
}

void hoa_2d_space_jsave(t_hoa_2d_space *x, t_dictionary *d)
{
	if (x->f_save_with_patcher)
	{
		long ac = 0;
		t_atom* av = NULL;
		hoa_2d_space_getvalueof(x, &ac, &av);
		dictionary_appendatoms(d, gensym("hoa_2d_space_saved_state"), ac, av);
		freebytes(av, ac * sizeof(t_atom));
	}
	else if(dictionary_hasentry(d, gensym("hoa_2d_space_saved_state")))
	{
		dictionary_chuckentry(d, gensym("hoa_2d_space_saved_state"));
	}
}

void draw_background(t_hoa_2d_space *x,  t_object *view, t_rect *rect)
{
    int i;
	double y1, y2, rotateAngle, grid_rad, inner_radius, grid_range, grid_step;
    t_jmatrix transform;
	
	long grid = x->f_grid;
	inner_radius = (double) 1 / 5 * x->f_radius;
	grid_range = x->f_radius - inner_radius;
	grid_step = safediv(grid_range, grid);
    
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_background_layer, rect->width, rect->height);
    
	if (g)
	{
        jgraphics_rectangle(g, 0, 0, rect->width, rect->height);
        jgraphics_set_source_jrgba(g, &x->f_color_bg);
        jgraphics_fill(g);
        
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
        
        jgraphics_set_source_jrgba(g, &x->f_color_lines);
        jgraphics_set_line_width(g, 1);
        
        for(i = 0; i < x->f_number_of_channels ; i++)
		{
            rotateAngle = ((double)i / (x->f_number_of_channels) * HOA_2PI ) - (0.5 / (x->f_number_of_channels) * HOA_2PI);
			jgraphics_rotate(g, rotateAngle);
			
			y1 = x->f_radius / 5.;
			y2 = x->f_radius;
            
			jgraphics_move_to(g, 0, y1);
			jgraphics_line_to(g, 0, y2);
			jgraphics_stroke(g);
			
			jgraphics_rotate(g, -rotateAngle);
		}
        
        jgraphics_matrix_init(&transform, 1, 0, 0, 1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		
		for(i = grid; i >= 0; i--)
		{
			grid_rad = grid_step * i + inner_radius;
            jgraphics_arc(g, 0, 0, grid_rad,  0., HOA_2PI);
            jgraphics_stroke(g);
		}
        
		jbox_end_layer((t_object*)x, view, hoa_sym_background_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_background_layer, 0., 0.);
}

double cosine_interpolation(double y1, double y2, float mu)
{
    double mu2;
    mu2 = (1-cos(mu*HOA_PI))/2;
    return(y1*(1-mu2)+y2*mu2);;
}

void draw_space(t_hoa_2d_space *x,  t_object *view, t_rect *rect)
{
    int i, index1, index2;
    double angle, radius, abscissa, ordinate, mu, diff, ratio;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_space_layer, rect->width, rect->height);

	if (g)
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		jgraphics_set_line_width(g, 2);
        jgraphics_set_source_jrgba(g, &x->f_color_sp);
		
		diff = x->f_minmax[1] - x->f_minmax[0];
        ratio = x->f_radius / 5.;
		
        for(i = 0; i < x->f_number_of_channels; i++)
		{
			if(x->f_floatoutput)
				x->f_channel_radius[i] = (x->f_channel_values[i] - x->f_minmax[0]) / diff *  4 * ratio + ratio;
			else
				x->f_channel_radius[i] = ((int)x->f_channel_values[i] - x->f_minmax[0]) / diff *  4 * ratio + ratio;
		}
        
        abscissa = Math<double>::abscissa(x->f_channel_radius[0], 0);
        ordinate = Math<double>::ordinate(x->f_channel_radius[0], 0);
        jgraphics_move_to(g, abscissa, ordinate);
        for(i = 1; i < HOA_DISPLAY_NPOINTS; i++)
		{
            index1 = (double)i / (double)HOA_DISPLAY_NPOINTS * x->f_number_of_channels;
            index2 = index1+1;
            
            mu = (double)index1 / (double)x->f_number_of_channels * (double)HOA_DISPLAY_NPOINTS;
            mu = (double)(i - mu) / ((double)HOA_DISPLAY_NPOINTS / (double)x->f_number_of_channels);
            if(index2 >= x->f_number_of_channels)
                index2 = 0;
            
            radius = cosine_interpolation(x->f_channel_radius[index1], x->f_channel_radius[index2], mu);
			angle  = (double)i / (double)HOA_DISPLAY_NPOINTS * HOA_2PI;
            abscissa = Math<double>::abscissa(radius, angle);
            ordinate = Math<double>::ordinate(radius, angle);
            jgraphics_line_to(g, abscissa, ordinate);
        }
        
        jgraphics_close_path(g);
        jgraphics_fill_preserve(g);
        jgraphics_stroke(g);
		jbox_end_layer((t_object*)x, view, hoa_sym_space_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_space_layer, 0., 0.);
}

void draw_points(t_hoa_2d_space *x,  t_object *view, t_rect *rect)
{
    int i;
    double angle, radius, abscissa, ordinate;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_points_layer, rect->width, rect->height);

	if (g) 
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		jgraphics_set_source_jrgba(g, &x->f_color_pt);
        
        for(i = 0; i < x->f_number_of_channels; i++)
		{
			if(x->f_floatoutput)
				radius = x->f_channel_radius[i] - 3.5;
			else
				radius = x->f_channel_radius[i] - 3.5;
			
            angle  = (double)(i + 1.) / (double)x->f_number_of_channels * HOA_2PI;
            angle -= HOA_2PI / (double)x->f_number_of_channels;
            abscissa = Math<double>::abscissa(radius, angle);
            ordinate = Math<double>::ordinate(radius, angle);
            jgraphics_arc(g, abscissa, ordinate, 3., 0., HOA_2PI);
			
			if (x->f_last_mouse_index == i)
				jgraphics_set_source_jrgba(g, &x->f_color_sel);
			else
				jgraphics_set_source_jrgba(g, &x->f_color_pt);
			
            jgraphics_fill(g);
        }
        
        jbox_end_layer((t_object*)x, view, hoa_sym_points_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_points_layer, 0., 0.);
}

void draw_slider_bars(t_hoa_2d_space *x, t_object *view, t_rect *rect)
{
	int i;
    double diff, ratio, angle1, angle2;
	t_pt pt[2];
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_space_layer, rect->width, rect->height);
	
	if (g)
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		
		jgraphics_set_line_width(g, 1);
		
		diff = x->f_minmax[1] - x->f_minmax[0];
        ratio = x->f_radius / 5.;
		
        for(i = 0; i < x->f_number_of_channels; i++)
		{
			if(x->f_floatoutput)
				x->f_channel_radius[i] = (x->f_channel_values[i] - x->f_minmax[0]) / diff *  4 * ratio + ratio;
			else
				x->f_channel_radius[i] = ((int)x->f_channel_values[i] - x->f_minmax[0]) / diff *  4 * ratio + ratio;
			
			angle1 = -(HOA_2PI / (double)x->f_number_of_channels) * 0.5;
			angle2 =  (HOA_2PI / (double)x->f_number_of_channels) * 0.5;
			
			angle1 += i*(HOA_2PI / (double)x->f_number_of_channels);
			angle2 += i*(HOA_2PI / (double)x->f_number_of_channels);
			
			pt[0].x = Math<double>::abscissa(ratio, angle1);
			pt[0].y = Math<double>::ordinate(ratio, angle1);
			
			pt[1].x = Math<double>::abscissa(x->f_channel_radius[i], angle2);
			pt[1].y = Math<double>::ordinate(x->f_channel_radius[i], angle2);
			
			jgraphics_move_to(g, pt[0].x, pt[0].y);
			jgraphics_arc(g, 0, 0, ratio, angle1+HOA_PI2, angle2+HOA_PI2);
			jgraphics_line_to(g, pt[1].x, pt[1].y);
			jgraphics_arc_negative(g, 0, 0, x->f_channel_radius[i], angle2+HOA_PI2, angle1+HOA_PI2);
			
			jgraphics_close_path(g);
			if (x->f_last_mouse_index == i)
				jgraphics_set_source_jrgba(g, &x->f_color_sel);
			else
				jgraphics_set_source_jrgba(g, &x->f_color_sp);
			
			jgraphics_fill_preserve(g);
			jgraphics_set_source_jrgba(g, &x->f_color_pt);
			jgraphics_stroke(g);
		}
        
        jbox_end_layer((t_object*)x, view, hoa_sym_space_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_space_layer, 0., 0.);
}

void draw_text_value(t_hoa_2d_space *x, t_object *view, t_rect *rect)
{
	if (x->f_last_mouse_index < 0 || x->f_last_mouse_index > x->f_number_of_channels)
		return;
	
    t_jfont *jf;
	t_jtextlayout *jtl;
    char text[16];
    double fontsize = jbox_get_fontsize((t_object *)x);
		
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_text_layer, rect->width, rect->height);
	
	if (g)
	{
        jf = jfont_create(jbox_get_fontname((t_object *)x)->s_name, JGRAPHICS_FONT_SLANT_ITALIC, JGRAPHICS_FONT_WEIGHT_BOLD, fontsize);
        jtl = jtextlayout_withbgcolor(g, &x->f_color_bg);
		
		jtextlayout_settextcolor(jtl, &x->f_color_text);
		
		if (x->f_floatoutput)
			sprintf(text,"%.3f", x->f_channel_values[x->f_last_mouse_index]);
		else
			sprintf(text,"%ld", (long)x->f_channel_values[x->f_last_mouse_index]);
		
		jtextlayout_set(jtl, text, jf, 0, 0, rect->width, rect->height, JGRAPHICS_TEXT_JUSTIFICATION_CENTERED, JGRAPHICS_TEXTLAYOUT_NOWRAP);
		jtextlayout_draw(jtl, g);

		jbox_end_layer((t_object*)x, view, hoa_sym_text_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_text_layer, 0., 0.);
}

void hoa_2d_space_paint(t_hoa_2d_space *x, t_object *view)
{
    t_rect rect;
    jbox_get_rect_for_view((t_object *)x, view, &rect);
    
    x->f_center = rect.width * .5;
    x->f_radius = x->f_center * 0.95;
    
    draw_background(x, view, &rect);
    
    if (x->f_slider_style == gensym("Blob"))
    {
        draw_space(x, view, &rect);
        draw_points(x, view, &rect);
    }
    else
    {
        draw_slider_bars(x, view, &rect);
    }
    
    if (x->f_draw_value)
    {
        draw_text_value(x, view, &rect);
    }
}

/**********************************************************/
/*                      Souris                            */
/**********************************************************/

void hoa_2d_space_mouse_move(t_hoa_2d_space *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_pt mouse;
    mouse.x = pt.x - x->f_center;
    mouse.y = x->f_center * 2. - pt.y - x->f_center;
    double angle, radius;
	
	x->f_last_mouse_index = -1;
	
	angle   = Math<double>::wrap_twopi(Math<double>::azimuth(mouse.x, mouse.y) + (HOA_PI / (double)x->f_number_of_channels));
	radius  = Math<double>::radius(mouse.x, mouse.y);
	x->f_last_mouse_index  = angle / HOA_2PI * x->f_number_of_channels;
	
	if(modifiers == 132 || modifiers == 5) // ctrl : rotation
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_DRAGGINGHAND);
    else if(modifiers == 2) // shift : gain
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_RESIZE_FOURWAY);
    else
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_ARROW);
	
	if (x->f_last_mouse_index >= 0)
		outlet_int(x->f_out_mouse, x->f_last_mouse_index + 1);
	else
		outlet_int(x->f_out_mouse, -1);
    
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
    jbox_redraw((t_jbox *)x);
}

void hoa_2d_space_mouse_leave(t_hoa_2d_space *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->f_last_mouse_index = -1;
	outlet_int(x->f_out_mouse, x->f_last_mouse_index);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
    jbox_redraw((t_jbox *)x);
}

void hoa_2d_space_mouse_drag(t_hoa_2d_space *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int index, index2;
    t_pt mouse;
    mouse.x = pt.x - x->f_center;
    mouse.y = x->f_center * 2. - pt.y - x->f_center;
    double angle, radius, value, inc, mu;
	
	x->f_last_mouse_index = -1;
    
    if(x->f_mode == 1) // ctrl : rotation
    {
		angle   = Math<double>::wrap_twopi(Math<double>::azimuth(mouse.x, mouse.y));
        inc     = x->f_angle_ref - angle;
        for(int i = 0; i < x->f_number_of_channels; i++)
        {
            angle   = Math<double>::wrap_twopi((double)i / (double)x->f_number_of_channels * HOA_2PI + inc);
            index   = angle / HOA_2PI * x->f_number_of_channels;
            index2  = index+1;
            if(index2 >= x->f_number_of_channels)
                index2 = 0;
           
            mu = (double)index / (double)x->f_number_of_channels * (double)HOA_2PI;
            mu = (double)(angle - mu) / ((double)HOA_2PI / (double)x->f_number_of_channels);
            value = cosine_interpolation(x->f_channel_refs[index], x->f_channel_refs[index2], mu);
            x->f_channel_values[i] = Math<double>::clip(value, x->f_minmax[0], x->f_minmax[1]);
			
			if((x->f_snaptogrid || modifiers == 17) && x->f_grid)
			{
				double grid_step = (x->f_minmax[1] - x->f_minmax[0]) / (double)x->f_grid;
				double div = (int)((x->f_channel_values[i] - x->f_minmax[0]) / grid_step);
				double temp = grid_step * div + x->f_minmax[0];
				x->f_channel_values[i] = temp;
			}
        }
    }
    else if(x->f_mode == 2) // shift : gain
    {
        radius  = Math<double>::radius(mouse.x, mouse.y);
        inc     = (radius - (x->f_radius / 5.)) / (x->f_radius * 4. / 5.);
		inc    *= (x->f_minmax[1] - x->f_minmax[0]);
        inc    += x->f_minmax[0];
        inc     = inc - x->f_value_ref;
        for(int i = 0; i < x->f_number_of_channels; i++)
		{
            x->f_channel_values[i] = Math<double>::clip(x->f_channel_refs[i] + inc, x->f_minmax[0], x->f_minmax[1]);
			if((x->f_snaptogrid || modifiers == 17) && x->f_grid)
			{
				double grid_step = (x->f_minmax[1] - x->f_minmax[0]) / (double)x->f_grid;
				double div = (int)((x->f_channel_values[i] - x->f_minmax[0]) / grid_step);
				double temp = grid_step * div + x->f_minmax[0];
				x->f_channel_values[i] = temp;
			}
		}
    }
    else
    {
		angle   = Math<double>::wrap_twopi(Math<double>::azimuth(mouse.x, mouse.y) + (HOA_PI / (double)x->f_number_of_channels));
        radius  = Math<double>::radius(mouse.x, mouse.y);
        index   = angle / HOA_2PI * x->f_number_of_channels;
        value   = (radius - (x->f_radius / 5.)) / (x->f_radius * 4. / 5.);
        value  *= (x->f_minmax[1] - x->f_minmax[0]);
        value  += x->f_minmax[0];
		value   = Math<double>::clip(value, x->f_minmax[0], x->f_minmax[1]);
		
		if((x->f_snaptogrid || modifiers == 17) && x->f_grid)
		{
			double grid_step = (x->f_minmax[1] - x->f_minmax[0]) / (double)x->f_grid;
			double div = (int)((value - x->f_minmax[0]) / grid_step);
			double temp = grid_step * div + x->f_minmax[0];
			value = temp;
		}
		
		x->f_channel_values[index] = value;
		
		x->f_last_mouse_index = index;
    }
	
	if (x->f_last_mouse_index >= 0)
		outlet_int(x->f_out_mouse, x->f_last_mouse_index + 1);
	else
		outlet_int(x->f_out_mouse, -1);
    
	object_notify(x, hoa_sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
    jbox_redraw((t_jbox *)x);
    hoa_2d_space_output(x);
}

void hoa_2d_space_mouse_down(t_hoa_2d_space *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_pt mouse;
    double rad;
    mouse.x = pt.x - x->f_center;
    mouse.y = x->f_center * 2. - pt.y - x->f_center;
    
    if(modifiers == 148 || modifiers == 21) // ctrl : rotation
    {
        x->f_mode = 1;
        x->f_angle_ref = Math<double>::wrap_twopi(Math<double>::azimuth(mouse.x, mouse.y));
        memcpy(x->f_channel_refs, x->f_channel_values, x->f_number_of_channels * sizeof(double));
    }
    else if(modifiers == 18) // shift : gain
    {
        x->f_mode = 2;
        rad  = Math<double>::radius(mouse.x, mouse.y);
        x->f_value_ref   = (rad - (x->f_radius / 5.)) / (x->f_radius * 4. / 5.);
        x->f_value_ref  *= (x->f_minmax[1] - x->f_minmax[0]);
        x->f_value_ref  += x->f_minmax[0];
        x->f_value_ref   = Math<double>::clip(x->f_value_ref, x->f_minmax[0], x->f_minmax[1]);
        memcpy(x->f_channel_refs, x->f_channel_values, x->f_number_of_channels * sizeof(double));
    }
    else
    {
        x->f_mode = 0;
        hoa_2d_space_mouse_drag(x, patcherview, pt, modifiers);
    }
}

t_max_err channels_set(t_hoa_2d_space *x, t_object *attr, long argc, t_atom *argv)
{
    long numberOfChannels;
    if (argc && argv && atom_gettype(argv) == A_LONG)
    {
        numberOfChannels = atom_getlong(argv);
        if(numberOfChannels != x->f_number_of_channels && numberOfChannels > 2 && numberOfChannels <= HOA_MAX_PLANEWAVES)
        {
            x->f_number_of_channels  = numberOfChannels;
            for(int i = 0; i < x->f_number_of_channels; i++)
                x->f_channel_values[i] = 0.;
            
			object_notify(x, hoa_sym_modified, NULL);
            jbox_invalidate_layer((t_object*)x, NULL, hoa_sym_background_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
            jbox_redraw((t_jbox *)x);
        }
    }
	return 0;
}

t_max_err minmax_set(t_hoa_2d_space *x, t_object *attr, long argc, t_atom *argv)
{
    double min, max;
    if(argc && argv)
    {
		min = x->f_minmax[0];
		max = x->f_minmax[1];
		
        if(atom_gettype(argv) == A_FLOAT || atom_gettype(argv) == A_LONG)
            min = atom_getfloat(argv);

        if(argc > 1 && (atom_gettype(argv+1) == A_FLOAT || atom_gettype(argv+1) == A_LONG))
            max = atom_getfloat(argv+1);
        
        x->f_minmax[0] = std::min<double>(min, max);
		x->f_minmax[1] = std::max<double>(min, max);
        
        if (x->f_minmax[0] == x->f_minmax[1])
            x->f_minmax[1] = x->f_minmax[0] + 1;
		
        for(int i = 0; i < x->f_number_of_channels; i++)
            x->f_channel_values[i] = Math<double>::clip(x->f_channel_values[i], x->f_minmax[0], x->f_minmax[1]);
        
		object_notify(x, hoa_sym_modified, NULL);
		jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_space_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_points_layer);
        jbox_redraw((t_jbox *)x);
    }
	return 0;
}

void *hoa_2d_space_new(t_symbol *s, int argc, t_atom *argv)
{
    t_hoa_2d_space *x =  NULL;
    t_dictionary *d;
    long flags;
    
    if (!(d = object_dictionaryarg(argc,argv)))
        return NULL;
    
    x = (t_hoa_2d_space *)object_alloc(hoa_2d_space_class);
    flags = 0
    | JBOX_DRAWFIRSTIN
    | JBOX_DRAWINLAST
    | JBOX_NODRAWBOX
    | JBOX_TRANSPARENT
    | JBOX_GROWY
    | JBOX_DRAWBACKGROUND
    ;
    
    jbox_new((t_jbox *)x, flags, argc, argv);
    x->j_box.b_firstin = (t_object *)x;
    
    x->f_channel_values = new double[HOA_MAX_PLANEWAVES];
    x->f_channel_refs   = new double[HOA_MAX_PLANEWAVES];
    x->f_channel_radius = new double[HOA_MAX_PLANEWAVES];
    x->f_out_mouse		= intout(x);
    x->f_out			= listout(x);
    
    x->f_last_mouse_index = -1;
    x->f_save_with_patcher = 1;
    
    attr_dictionary_process(x, d);
    
    // restore object state
    long ac = 0;
    t_atom *av = NULL;
    dictionary_copyatoms(d, gensym("hoa_2d_space_saved_state"), &ac, &av);
    if (ac && av)
        hoa_2d_space_setvalueof(x, ac, av);
    
    jbox_ready((t_jbox *)x);
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_2d_space_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.space", (method)hoa_2d_space_new, (method)hoa_2d_space_free, (short)sizeof(t_hoa_2d_space), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.space", (char *)"hoa.2d.space");
    
    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
    jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH | JBOX_FONTATTR);
    
    hoa_initclass(c, (method)hoa_getinfos);
    class_addmethod(c, (method)hoa_2d_space_assist,          "assist",         A_CANT,	0);
    class_addmethod(c, (method)hoa_2d_space_paint,           "paint",          A_CANT,	0);
    class_addmethod(c, (method)hoa_2d_space_notify,          "notify",         A_CANT, 0);
    
    // @method bang @digest Output current values.
    // @description The <m>bang</m> message report current values.
    class_addmethod(c, (method)hoa_2d_space_output,          "bang",           A_CANT, 0);
    
    // @method list @digest set slider values.
    // @description Sets slider values with a <m>list</m>.
    // Set all the coefficients with a list of float. Set one coefficient with an index and a float.
    class_addmethod(c, (method)hoa_2d_space_list,            "list",           A_GIMME, 0);
    
    // @method (mouse) @digest Click and drag to change slider values.
    // @description Click and drag the sliders to set their value.
    // Press <b>ctrl</b> key while you drag to make a rotation of the slider values.
    // Press <b>shift</b> key while you drag to relatively increase or decrease all of the slider values.
    class_addmethod(c, (method)hoa_2d_space_mouse_down,      "mousedown",      A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_space_mouse_move,      "mousemove",      A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_space_mouse_drag,      "mousedrag",      A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_space_mouse_down,      "mouseup",        A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_space_mouse_leave,     "mouseleave",     A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_space_preset,          "preset",         0);
    class_addmethod(c, (method)hoa_2d_space_getvalueof,      "getvalueof",     A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_space_setvalueof,      "setvalueof",     A_CANT, 0);
    class_addmethod(c, (method)hoa_2d_space_jsave,           "jsave",			A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "color", 0);
    // @exclude hoa.2d.space
    CLASS_ATTR_INVISIBLE            (c, "textcolor", 0);
    // @exclude hoa.2d.space
    CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0 0 150 150");
    // @exclude hoa.2d.space
    
    CLASS_STICKY_CATEGORY(c, 0, "Value");
    CLASS_ATTR_LONG                 (c, "channels", 0, t_hoa_2d_space, f_number_of_channels);
    CLASS_ATTR_LABEL                (c, "channels", 0, "Number of Channels");
    CLASS_ATTR_ACCESSORS            (c, "channels", NULL, channels_set);
    CLASS_ATTR_ORDER                (c, "channels", 0, "1");
    CLASS_ATTR_DEFAULT              (c, "channels", 0, "4");
    CLASS_ATTR_SAVE                 (c, "channels", 1);
    // @description The number of channels.
    
    CLASS_ATTR_DOUBLE_ARRAY         (c, "minmax",   0, t_hoa_2d_space, f_minmax, 2);
    CLASS_ATTR_LABEL                (c, "minmax",   0, "Range");
    CLASS_ATTR_ACCESSORS            (c, "minmax", NULL, minmax_set);
    CLASS_ATTR_ORDER                (c, "minmax",   0, "2");
    CLASS_ATTR_DEFAULT              (c, "minmax",   0, "0. 1.");
    CLASS_ATTR_SAVE                 (c, "minmax",   1);
    // @description The minimum and maximum values that the sliders can reach.
    
    CLASS_ATTR_LONG					(c, "floatoutput", 0, t_hoa_2d_space, f_floatoutput);
    CLASS_ATTR_STYLE_LABEL			(c, "floatoutput", 0, "onoff", "Float Output");
    CLASS_ATTR_ORDER				(c, "floatoutput", 0, "3");
    CLASS_ATTR_DEFAULT				(c, "floatoutput", 0, "1");
    CLASS_ATTR_SAVE					(c, "floatoutput", 1);
    // @description The <m>floatoutput</m> attribute set the <o>hoa.2d.space</o> output type. If it is checked, sliders value are sent as floating-pont values, otherwise it will round slider values and send it as integers.
    
    CLASS_ATTR_LONG					(c, "snaptogrid", 0, t_hoa_2d_space, f_snaptogrid);
    CLASS_ATTR_STYLE_LABEL			(c, "snaptogrid", 0, "onoff", "Snap to Grid");
    CLASS_ATTR_ORDER				(c, "snaptogrid", 0, "4");
    CLASS_ATTR_DEFAULT				(c, "snaptogrid", 0, "0");
    CLASS_ATTR_SAVE					(c, "snaptogrid", 1);
    // @description If the <m>snaptogrid</m> attribute is checked, sliders will snap to the grid.
    
    CLASS_ATTR_LONG                 (c, "grid", 0, t_hoa_2d_space, f_grid);
    CLASS_ATTR_CATEGORY             (c, "grid", 0, "Value");
    CLASS_ATTR_LABEL                (c, "grid", 0, "Grid Size");
    CLASS_ATTR_FILTER_MIN			(c, "grid", 0);
    CLASS_ATTR_ORDER                (c, "grid", 0, "5");
    CLASS_ATTR_DEFAULT              (c, "grid", 0, "4");
    CLASS_ATTR_SAVE                 (c, "grid", 1);
    // @description Set the grid size
    
    CLASS_ATTR_LONG					(c, "save", 0, t_hoa_2d_space, f_save_with_patcher);
    CLASS_ATTR_STYLE_LABEL			(c, "save", 0, "onoff", "Save Object State with Patcher");
    CLASS_ATTR_DEFAULT				(c, "save", 0, "1");
    CLASS_ATTR_SAVE					(c, "save", 1);
    CLASS_ATTR_ORDER				(c, "save", 0, "6");
    // @description Check this to save object' state with patcher. Warning : if Parameter Mode and Initial are enable, this <m>save</m> method is no longer effective.
    CLASS_STICKY_CATEGORY_CLEAR(c);
    
    CLASS_ATTR_LONG					(c, "drawvalue", 0, t_hoa_2d_space, f_draw_value);
    CLASS_ATTR_CATEGORY				(c, "drawvalue", 0, "Style");
    CLASS_ATTR_STYLE_LABEL			(c, "drawvalue", 0, "onoff", "Draw Text Value");
    CLASS_ATTR_ORDER				(c, "drawvalue", 0, "1");
    CLASS_ATTR_DEFAULT				(c, "drawvalue", 0, "0");
    CLASS_ATTR_SAVE					(c, "drawvalue", 1);
    // @description When the <m>drawvalue</m> attribute is checked, slider values are posted at the center.
    
    CLASS_ATTR_SYM					(c, "sliderstyle", 0, t_hoa_2d_space, f_slider_style);
    CLASS_ATTR_CATEGORY				(c, "sliderstyle", 0, "Style");
    CLASS_ATTR_LABEL				(c, "sliderstyle", 0, "Slider Style");
    CLASS_ATTR_ENUM					(c, "sliderstyle", 0, "Blob Bar");
    CLASS_ATTR_ORDER				(c, "sliderstyle", 0, "2");
    CLASS_ATTR_DEFAULT				(c, "sliderstyle", 0, "Blob");
    CLASS_ATTR_SAVE					(c, "sliderstyle", 1);
    // @description Set the sliders style :
    // <ul>
    // <li>In <b>Blob</b> mode <o>hoa.2d.space</o> will draw slider values as a global shape.</li>
    // <li>In <b>Bar</b> mode, each slider is drawn separately as a slider bar.</li>
    // </ul>
    
    CLASS_STICKY_CATEGORY(c, 0, "Color");
    CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_hoa_2d_space, f_color_bg);
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.76 0.76 0.76 1.");
    // @description Sets the RGBA values for the background color of the <o>hoa.2d.space</o> object
    
    CLASS_ATTR_RGBA                 (c, "linescolor", 0, t_hoa_2d_space, f_color_lines);
    CLASS_ATTR_STYLE                (c, "linescolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "linescolor", 0, "Lines Color");
    CLASS_ATTR_ORDER                (c, "linescolor", 0, "2");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "linescolor", 0, "0.65098 0.666667 0.662745 1.");
    // @description Sets the RGBA values for the border color of the <o>hoa.2d.space</o> object
    
    CLASS_ATTR_RGBA					(c, "spcolor", 0, t_hoa_2d_space, f_color_sp);
    CLASS_ATTR_STYLE				(c, "spcolor", 0, "rgba");
    CLASS_ATTR_LABEL				(c, "spcolor", 0, "Space Color");
    CLASS_ATTR_ORDER                (c, "spcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT	(c, "spcolor", 0, "0. 0. 1. 0.25");
    // @description Sets the RGBA values for the space color of the <o>hoa.2d.space</o> object
    
    CLASS_ATTR_RGBA					(c, "selcolor", 0, t_hoa_2d_space, f_color_sel);
    CLASS_ATTR_STYLE				(c, "selcolor", 0, "rgba");
    CLASS_ATTR_LABEL				(c, "selcolor", 0, "Select Color");
    CLASS_ATTR_ORDER                (c, "selcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT	(c, "selcolor", 0, "1. 0. 0. 0.25");
    // @description Sets the RGBA values for the selected slider color of the <o>hoa.2d.space</o> object
    
    CLASS_ATTR_RGBA					(c, "textcolor", 0, t_hoa_2d_space, f_color_text);
    CLASS_ATTR_STYLE				(c, "textcolor", 0, "rgba");
    CLASS_ATTR_LABEL				(c, "textcolor", 0, "Text Color");
    CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT	(c, "textcolor", 0, "0.1 0.1 0.1 1.");
    // @description Sets the RGBA values for the text color of the <o>hoa.2d.space</o> object
    
    CLASS_ATTR_RGBA					(c, "ptcolor", 0, t_hoa_2d_space, f_color_pt);
    CLASS_ATTR_STYLE				(c, "ptcolor", 0, "rgba");
    CLASS_ATTR_LABEL				(c, "ptcolor", 0, "Channel point Color");
    CLASS_ATTR_ORDER                (c, "ptcolor", 0, "4");
    CLASS_ATTR_DEFAULT_SAVE_PAINT	(c, "ptcolor", 0, "0.25 0.25 0.25 1.");
    // @description Sets the RGBA values for the channel point color of the <o>hoa.2d.space</o> object
    CLASS_STICKY_CATEGORY_CLEAR(c);
    
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.space"));
    hoa_2d_space_class = c;
    
    return 0;
}
