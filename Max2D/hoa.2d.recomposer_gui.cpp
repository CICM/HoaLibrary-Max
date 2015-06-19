/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.recomposer.cpp
 @name      hoa.2d.recomposer
 @realname  hoa.2d.recomposer
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A Graphical User Interface to manipulate 2d plane waves.
 
 @description
 <o>hoa.2d.recomposer</o> helps you to set angle and widening value of channels to warp the sound field. It is mainly dedicated to the control of the <o>hoa.2d.recomposer~</o> processor object.
 
 @discussion
 <o>hoa.2d.recomposer</o> helps you to set angle and widening value of channels to warp the sound field. It is mainly dedicated to the control of the <o>hoa.2d.recomposer~</o> processor object.
 
 @category 2d, Ambisonics, Planewaves, GUI
 
 @seealso  hoa.2d.projector~, hoa.2d.recomposer~, hoa.2d.process~, hoa.fx.gain~, hoa.fx.delay~, hoa.gain~, hoa.2d.meter~, hoa.2d.map, hoa.2d.decoder~, hoa.2d.encoder~, hoa.2d.wider~
 
 @illustration on @caption the hoa.2d.recomposer GUI object
 @palette yes
 */

#include "Hoa2D.max.h"
#include "ChannelManager.h"

#define MAX_UI_CHANNELS 64
#define MIN_UI_CHANNELS 3
#define DEF_UI_CHANNELS 4

typedef struct  _hoa_2d_recomposer_gui
{
	t_jbox		j_box;
	t_rect		rect;
    
    // colors
	t_jrgba		f_color_bg;
    t_jrgba		f_color_inner_circle;
	t_jrgba		f_color_channel_point;
    t_jrgba		f_color_channel_point_selected;
	t_jrgba		f_color_selection;
    t_jrgba		f_color_channel_shape;
	t_jrgba		f_color_channel_shape_selected;
    t_jrgba     f_color_channel_point_text;
    t_jrgba     f_color_channel_point_text_sel;
	t_jrgba     f_color_fisheye;
    
    // channels
    ChannelManager*                     f_manager;
    long                                f_number_of_channels;
    long                                f_fisheye_show;
    double                              f_fisheye_azimuth;
	
	// viewer
    Encoder<Hoa2d, t_sample>::Basic*    f_encoder;
    Wider<Hoa2d, t_sample>*             f_wider;
    Scope<Hoa2d, t_sample>*             f_scope;
	t_sample*                           f_harmonicsValues;
    
    // mouse
    t_pt        f_rectSelectionBegin;
    t_rect      f_rectSelection;
    bool        f_rectSelectionExist;
	t_pt		f_last_mouseMove;
    int         f_last_mouseMoveOverChannel;
    t_pt		f_last_mouseDown;
    int         f_last_mouseDownOverChannel;
    t_pt		f_last_mouseUp;
    int         f_last_mouseUpOverChannel;
    t_pt		f_last_mouseDrag;
    double		f_last_mouseDragAngle;
    double      f_last_mouseDragRadius;
    bool        f_was_dragging;
    
    // draw utility
    double      f_channel_radius;
	double		f_mic_size;
    
    // outlets
	void*		f_out;
    void*		f_outlet_infos;

} t_hoa_2d_recomposer_gui;

t_class *hoa_2d_recomposer_gui_class;

void hoa_2d_recomposer_gui_free(t_hoa_2d_recomposer_gui *x)
{
	jbox_free(&x->j_box);
	delete x->f_manager;
	delete x->f_encoder;
	delete x->f_wider;
    delete x->f_scope;
	delete [] x->f_harmonicsValues;
}

void hoa_2d_recomposer_gui_assist(t_hoa_2d_recomposer_gui *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET)
	{
		sprintf(s,"(list) Set Channels Widening value and Angle");
	}
	else
	{
		if (a == 0)
			sprintf(s,"(list) Channels Widening value and Angle");
		else if (a == 1)
			sprintf(s,"Infos output");
	}
}

//========================= Output Methods :

void hoa_2d_recomposer_gui_output(t_hoa_2d_recomposer_gui *x)
{
    int channels = x->f_manager->getNumberOfChannels();
    t_atom*	av_left = new t_atom[channels];
    t_atom  av_right[2];
    
    // number of channels
    atom_setsym(av_right, hoa_sym_channels);
    atom_setlong(av_right+1, channels);
    outlet_list(x->f_outlet_infos, NULL, 2, av_right);
    
    // angles of channels
    for (int i=0; i<channels; i++)
        atom_setfloat(av_left+i, x->f_manager->getAzimuth(i));
    
    outlet_anything(x->f_out, hoa_sym_angles, channels, av_left);
    
    // wider values of channels
    for (int i=0; i<channels; i++)
        atom_setfloat(av_left+i, x->f_manager->getWidening(i));
    
    outlet_anything(x->f_out, hoa_sym_wide, channels, av_left);
    delete [] av_left;
}

void hoa_2d_recomposer_gui_bang(t_hoa_2d_recomposer_gui *x)
{
    hoa_2d_recomposer_gui_output(x);
}

void hoa_2d_recomposer_gui_outputAndNotifyChange(t_hoa_2d_recomposer_gui *x)
{
    object_notify(x, hoa_sym_modified, NULL);
    hoa_2d_recomposer_gui_output(x);
}

//========================= Presets Methods :

void hoa_2d_recomposer_gui_preset(t_hoa_2d_recomposer_gui *x)
{
    
    void* z;
    if(!(z = gensym("_preset")->s_thing))
        return;
    
    binbuf_vinsert(z, gensym("ossl")->s_name, x, object_classname(x), hoa_sym_channels, x->f_number_of_channels);
    
    for(int i = 0; i < x->f_number_of_channels; i++)
    {
        binbuf_vinsert(z, gensym("osslf")->s_name, x, object_classname(x), hoa_sym_angles, i, (float)x->f_manager->getAzimuth(i));
        binbuf_vinsert(z, gensym("osslf")->s_name, x, object_classname(x), hoa_sym_wide, i, (float)x->f_manager->getWidening(i));
    }
}

t_max_err hoa_2d_recomposer_gui_setvalueof(t_hoa_2d_recomposer_gui *x, long ac, t_atom *av)
{
    if (ac && av)
	{
        long index, i;
        for (i = index = 0; (index < x->f_number_of_channels) && (i <= ac); index++, i+=3)
        {
            x->f_manager->setAzimuth(index, Math<double>::azimuth(atom_getfloat(av+i), atom_getfloat(av+i+1)));
            x->f_manager->setWideningValue(index, atom_getfloat(av+i+2));
        }
        
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
        jbox_redraw((t_jbox *)x);
        
        hoa_2d_recomposer_gui_output(x);
	}
	return MAX_ERR_NONE;
}

// get a list of mic value : (x, y, wider)*numberOfchannel
t_max_err hoa_2d_recomposer_gui_getvalueof(t_hoa_2d_recomposer_gui *x, long *ac, t_atom **av)
{
    if (ac && av)
    {
        if (*ac && *av)
        {
            long index, i;
            for (i = index = 0; index < x->f_number_of_channels; index++, i+=3)
            {
                atom_setfloat(*av+i, x->f_manager->getAbscissa(index) );
                atom_setfloat(*av+i+1, x->f_manager->getOrdinate(index) );
                atom_setfloat(*av+i+2, x->f_manager->getWidening(index) );
            }
		}
        else
        {
            *ac = (x->f_number_of_channels*3);
			*av = (t_atom *)getbytes( (x->f_number_of_channels*3) * sizeof(t_atom));
            long index, i;
            for (i = index = 0; index < x->f_number_of_channels; index++, i+=3)
            {
                atom_setfloat(*av+i, x->f_manager->getAbscissa(index) );
                atom_setfloat(*av+i+1, x->f_manager->getOrdinate(index) );
                atom_setfloat(*av+i+2, x->f_manager->getWidening(index) );
            }
        }
    }
	return MAX_ERR_NONE;
}

/* --------- */

void hoa_2d_recomposer_gui_reset(t_hoa_2d_recomposer_gui *x, t_symbol *s, short ac, t_atom *av)
{
    if (ac == 0)
    {
        x->f_manager->resetAzimuth();
        x->f_manager->resetWideningValue();
    }
    else if (ac >= 1 && atom_gettype(av) == A_SYM)
    {
        if (atom_getsym(av) == hoa_sym_angles)
        {
            if (ac >= 2)
            {
                for(int i = 1; i < ac ; i++)
                {
                    if ( (atom_gettype(av+i) == A_FLOAT || atom_gettype(av+i) == A_LONG))
                        x->f_manager->resetAzimuth(atom_getlong(av + i) - 1);
                }
            }
            else
            {
                x->f_manager->resetAzimuth();
            }
        }
        else if (atom_getsym(av) == hoa_sym_wide)
        {
            if (ac >= 2)
            {
                for(int i = 1; i < ac ; i++)
                {
                    if ( (atom_gettype(av+i) == A_FLOAT || atom_gettype(av+i) == A_LONG))
                        x->f_manager->resetWideningValue(atom_getlong(av + i) - 1);
                }
            }
            else
            {
                x->f_manager->resetWideningValue();
            }
        }
    }
    
    hoa_2d_recomposer_gui_outputAndNotifyChange(x);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
	jbox_redraw((t_jbox *)x);
}

void hoa_2d_recomposer_gui_set(t_hoa_2d_recomposer_gui *x, t_symbol *s, long ac, t_atom *av)
{
    int isSet = (s == hoa_sym_set) ? 1 : 0;
    t_symbol *name = atom_getsym(av);
    if (!isSet) name = s;
	
    if (ac && av)
    {
        if ( name == hoa_sym_angles )
        {
            if (atom_gettype(av+isSet) == A_LONG) // index + angle
            {
				long index = atom_getlong(av + isSet) - 1;
				if (!isInside<long>(index, -1, x->f_manager->getNumberOfChannels() - 1)) 
					return;

                if ( ac >= 2+isSet )
                    x->f_manager->setAzimuth(index, atom_getfloat(av+1+isSet));
            }
            else
            {
                double* list = new double[ac];
                for(int i = 0; i < ac - isSet ; i++)
                {
                    if (i > MAX_UI_CHANNELS-1) return;
                    if (atom_gettype(av+i+isSet) == A_FLOAT || atom_gettype(av+i+isSet) == A_LONG) {
                        list[i] = atom_getfloat(av + i + isSet);
                    } else list[i] = 0;
                }
                x->f_manager->setAzimuthList(list, ac);
				delete [] list;
            }
        }
        else if ( name == hoa_sym_wide )
        {
            if (atom_gettype(av+isSet) == A_LONG) // index + wide
            {
                long index;
                index = atom_getlong(av+isSet) - 1;
                if ( ac >= 2+isSet )
                    x->f_manager->setWideningValue(index, atom_getfloat(av+1+isSet));
            }
            else
            {
                double* list = new double[ac];
                for(int i = 0; i < ac - isSet ; i++)
                {
                    if (i > MAX_UI_CHANNELS-1) return;
                    if (atom_gettype(av+i+isSet) == A_FLOAT || atom_gettype(av+i+isSet) == A_LONG) {
                        list[i] = atom_getfloat(av + i + isSet);
                    } else list[i] = 0;
                }
                x->f_manager->setWideningValueList(list, ac);
				delete [] list;
            }
        }
    }
    
    object_notify(x, hoa_sym_modified, NULL);
    
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
	jbox_redraw((t_jbox *)x);
}

void hoa_2d_recomposer_gui_angles(t_hoa_2d_recomposer_gui *x, t_symbol *s, long ac, t_atom *av)
{
    hoa_2d_recomposer_gui_set(x, s, ac, av); // notifying in set method
    hoa_2d_recomposer_gui_output(x);
}

void hoa_2d_recomposer_gui_wide(t_hoa_2d_recomposer_gui *x, t_symbol *s, long ac, t_atom *av)
{
    hoa_2d_recomposer_gui_set(x, s, ac, av); // notifying in set method
    hoa_2d_recomposer_gui_output(x);
}

t_max_err set_number_of_channels(t_hoa_2d_recomposer_gui *x, void *attr, long ac, t_atom *av)
{
	if (ac && av && atom_gettype(av) == A_LONG)
    {
		long mics = atom_getlong(av);
		if (mics != x->f_manager->getNumberOfChannels())
        {
            x->f_manager->setNumberOfChannels(mics);
            x->f_number_of_channels = x->f_manager->getNumberOfChannels();
            
            ulong order;
            if (x->f_number_of_channels % 2 == 0)
                order = x->f_number_of_channels / 2 - 1;
            else
                order = x->f_number_of_channels / 2;
            
            delete [] x->f_harmonicsValues;
            delete x->f_encoder;
            delete x->f_scope;
            delete x->f_wider;
            
            x->f_encoder    = new Encoder<Hoa2d, t_sample>::Basic(order);
            x->f_wider      = new Wider<Hoa2d, t_sample>(order);
            x->f_scope      = new Scope<Hoa2d, t_sample>(order, HOA_DISPLAY_NPOINTS);
            x->f_harmonicsValues = new t_sample[order * 2 + 1];
            
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_background_layer);
            hoa_2d_recomposer_gui_reset(x, gensym("reset"), 0, NULL);
        }
    }
    return MAX_ERR_NONE;
}

void hoa_2d_recomposer_gui_anything(t_hoa_2d_recomposer_gui *x, t_symbol *s, short ac, t_atom *av)
{
	;
}

//========================= Notify Methods :

t_max_err hoa_2d_recomposer_gui_notify(t_hoa_2d_recomposer_gui *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	t_symbol *name;

    if (msg == hoa_sym_attr_modified)
	 {
         name = (t_symbol *)object_method((t_object *)data, hoa_sym_getname);
         if(name == hoa_sym_channels)
         {
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_background_layer);
         }
         else if(name == gensym("chacolor") || name == gensym("selchacolor"))
         {
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
         }
         else if(name == gensym("channeltextcolor") || name == gensym("selchanneltextcolor"))
         {
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
         }
         else if(name == hoa_sym_bgcolor || name == gensym("circlecolor"))
         {
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_background_layer);
         }
         else if(name == gensym("channelshapecolor") || name == gensym("selchannelshapecolor"))
         {
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
         }
         else if (name == gensym("fisheyecolor"))
         {
             jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_fisheye_layer);
         }
         jbox_redraw((t_jbox *)x);
	 }
	return jbox_notify((t_jbox *)x, s, msg, sender, data);
}

//========================= Paint Methods :

void draw_background(t_hoa_2d_recomposer_gui *x,  t_object *view, t_rect *rect)
{
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_background_layer, rect->width, rect->height);
	
	if (g)
	{
        const double w = x->rect.width;
        t_jrgba HpMarkerColor = {0.2,0.2,0.2,1};
        int colorFactor;
        t_jrgba shadLight, shadDark;
        t_jmatrix transform;
        double channel_azimuth = HOA_2PI / x->f_manager->getNumberOfChannels();
        
        shadLight = shadDark = x->f_color_inner_circle;
        shadDark = rgba_addContrast(shadDark, -0.05);
        shadLight = rgba_addContrast(shadDark, 0.05);
        
        HpMarkerColor = x->f_color_inner_circle;
        colorFactor = (x->f_color_inner_circle.red + x->f_color_inner_circle.green + x->f_color_inner_circle.blue > 1.5) ? -1 : 1;
        HpMarkerColor = rgba_addContrast(HpMarkerColor, (0.2*colorFactor));
        
        jgraphics_rectangle(g, 0., 0., rect->width, rect->height);
        jgraphics_set_source_jrgba(g, &x->f_color_bg);
        jgraphics_fill(g);
        
        jgraphics_rectangle(g, 0., 0., rect->width, rect->height);
        jgraphics_set_source_jrgba(g, &shadDark);
        jgraphics_set_line_width(g, 1.);
        jgraphics_stroke(g);
        
        jgraphics_set_line_cap(g, JGRAPHICS_LINE_CAP_ROUND);
		jgraphics_set_source_jrgba(g, &x->f_color_inner_circle);
		jgraphics_arc(g, w*0.5, w*0.5, x->f_channel_radius,  0., HOA_2PI);
		jgraphics_fill(g);
        
        jgraphics_matrix_init(&transform, 1, 0, 0, -1, w*0.5, w*0.5);
        jgraphics_set_matrix(g, &transform);
        jgraphics_set_source_jrgba(g, &HpMarkerColor);
        jgraphics_set_line_width(g, 1.);
        
        for (int i=0; i < x->f_manager->getNumberOfChannels(); i++)
		{
            jgraphics_rotate(g, channel_azimuth*i);
            jgraphics_translate(g, 0, x->f_channel_radius);
            jgraphics_arc(g, 0, 0, x->f_mic_size*1.4, 0, HOA_2PI);
            jgraphics_set_source_jrgba(g, &x->f_color_inner_circle);
            jgraphics_fill(g);
            jgraphics_translate(g, 0, -x->f_channel_radius);
            jgraphics_rotate(g, -channel_azimuth*i);
            
            jgraphics_rotate(g, channel_azimuth*i);
            jgraphics_translate(g, 0, x->f_channel_radius);
            jgraphics_move_to(g, 0, -x->f_mic_size*0.8);
            jgraphics_line_to(g, 0, x->f_mic_size*0.8);
            jgraphics_set_source_jrgba(g, &HpMarkerColor);
            jgraphics_stroke(g);
            jgraphics_translate(g, 0, -x->f_channel_radius);
            jgraphics_rotate(g, -channel_azimuth*i);
        }

		jbox_end_layer((t_object*)x, view, hoa_sym_background_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_background_layer, 0., 0.);
}

void draw_channels(t_hoa_2d_recomposer_gui *x, t_object *view, t_rect *rect)
{
    double w = rect->width;
	int numChannels = x->f_number_of_channels;
    double channel_azimuth;
    
    t_jrgba overChannelColor = x->f_color_channel_point_selected;
    overChannelColor = rgba_addContrast(overChannelColor, -0.1);
    
    t_jmatrix transform;

	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_channels_layer, rect->width, rect->height);
	
	if (g) {
        jgraphics_matrix_init(&transform, 1, 0, 0, -1, w*0.5, w*0.5);
		jgraphics_set_matrix(g, &transform);
        
		for(int i=numChannels-1; i>=0; i--)
		{
            jgraphics_set_source_jrgba(g, x->f_manager->isSelected(i) ? &x->f_color_channel_point_selected : ( (x->f_last_mouseMoveOverChannel == i) ? &overChannelColor : &x->f_color_channel_point) );
            channel_azimuth = x->f_manager->getAzimuth(i);
            
            jgraphics_rotate(g, channel_azimuth);
            jgraphics_translate(g, 0, x->f_channel_radius);
            //-- rotated and translated
            
            // head of the mic :
            jgraphics_arc(g, 0, 0, x->f_mic_size, 0, HOA_2PI);
            jgraphics_fill_preserve(g);
            
            jgraphics_set_line_width(g, 1);
            jgraphics_set_source_jrgba(g, x->f_manager->isSelected(i) ? &x->f_color_channel_point : ( (x->f_last_mouseMoveOverChannel == i) ? &overChannelColor : &x->f_color_channel_point_selected) );
            jgraphics_stroke(g);
            
            //-- inverse rotation and translate
            jgraphics_translate(g, 0, -x->f_channel_radius);
            jgraphics_rotate(g, -channel_azimuth);
		}
            
		jbox_end_layer((t_object*)x, view, hoa_sym_channels_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_channels_layer, 0., 0.);
}

void draw_channels_text(t_hoa_2d_recomposer_gui *x, t_object *view, t_rect *rect)
{
    double w = rect->width;
	int numChannels = x->f_number_of_channels;
    double channel_azimuth;

    t_jfont *jf;
	t_jtextlayout *jtl;
    char text[16];
    double fontsize = rect->width * 0.04;
    double x1, y1;
	
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_text_layer, rect->width, rect->height);
    	
	if (g)
	{
        jf = jfont_create(jbox_get_fontname((t_object *)x)->s_name, JGRAPHICS_FONT_SLANT_ITALIC, JGRAPHICS_FONT_WEIGHT_BOLD, fontsize);
        jtl = jtextlayout_withbgcolor(g, &x->f_color_channel_point);
        
        for(int i = numChannels-1; i >= 0; i--)
        {
            jtextlayout_settextcolor(jtl, (x->f_manager->isSelected(i) || x->f_last_mouseMoveOverChannel == i) ? &x->f_color_channel_point_text_sel : &x->f_color_channel_point_text);
			
			channel_azimuth = HOA_2PI - x->f_manager->getAzimuth(i) + HOA_PI;
            x1 = long(Math<double>::abscissa(x->f_channel_radius, channel_azimuth) + (w*0.5))+0.5;
            y1 = long(Math<double>::ordinate(x->f_channel_radius, channel_azimuth) + (w*0.5))+0.5;
			
            sprintf(text,"%i", i+1);
			jtextlayout_set(jtl, text, jf, x1 - fontsize * 1.5, y1 - 10, fontsize * 3., 20, JGRAPHICS_TEXT_JUSTIFICATION_CENTERED, JGRAPHICS_TEXTLAYOUT_NOWRAP);
            jtextlayout_draw(jtl, g);
        }
        
		jbox_end_layer((t_object*)x, view, hoa_sym_text_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_text_layer, 0., 0.);
}

void computeRepresentation(t_hoa_2d_recomposer_gui *x, int index)
{
	x->f_encoder->setAzimuth(0);
    const t_sample val = 10.;
	x->f_encoder->process(&val, x->f_harmonicsValues);
	x->f_wider->setWidening(x->f_manager->getWidening(index));
	x->f_wider->process(x->f_harmonicsValues, x->f_harmonicsValues);
	x->f_scope->process(x->f_harmonicsValues);
}

void draw_channels_shape(t_hoa_2d_recomposer_gui *x, t_object *view, t_rect *rect)
{
    double w = rect->width;
    t_jrgba harmonicsFillColor = x->f_color_channel_shape;
	t_jrgba harmonicsFillColorSel = x->f_color_channel_shape_selected;
	double factor = (x->f_channel_radius*0.92);
	int breakIndex = 0;
	double rotation = 0;
	
	int nPoints = x->f_scope->getNumberOfPoints();
    
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_harmonics_layer, rect->width, rect->height);
    
    harmonicsFillColor.alpha = max<double>(x->f_color_channel_shape.alpha - 0.2, 0);
	harmonicsFillColorSel.alpha = max<double>(x->f_color_channel_shape.alpha - 0.2, 0);
    
	if (g)
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, (w*0.5), (w*0.5));
		jgraphics_set_matrix(g, &transform);
		jgraphics_set_line_width(g, 2);
        
        for (int i = 0; i < x->f_number_of_channels; i++)
		{
			rotation = x->f_manager->getAzimuth(i);
			jgraphics_rotate(g, rotation);
			
			breakIndex = 0;
			computeRepresentation(x, i);
			
			jgraphics_move_to(g, x->f_scope->getPointAbscissa(0) * factor, x->f_scope->getPointOrdinate(0) * factor);
			
			for(int j = 1; j < nPoints; j++)
			{
				if (x->f_scope->getPointValue(j) <= 0)
				{
					breakIndex = j;
					break;
				}
				jgraphics_line_to(g, x->f_scope->getPointAbscissa(j) * factor, x->f_scope->getPointOrdinate(j) * factor );
			}
			
			if (breakIndex)
				for(int j = nPoints - breakIndex; j < nPoints; j++)
					jgraphics_line_to(g, x->f_scope->getPointAbscissa(j) * factor, x->f_scope->getPointOrdinate(j) * factor );
			
			jgraphics_close_path(g);
			jgraphics_set_source_jrgba(g, x->f_manager->isSelected(i) ? &harmonicsFillColorSel : &harmonicsFillColor);
			jgraphics_fill_preserve(g);
			jgraphics_set_source_jrgba(g, x->f_manager->isSelected(i) ? &x->f_color_channel_shape_selected : &x->f_color_channel_shape);
			jgraphics_stroke(g);
			
			jgraphics_rotate(g, -rotation);
        }
        
		jbox_end_layer((t_object*)x, view, hoa_sym_harmonics_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_harmonics_layer, 0., 0.);
}

void draw_fishEye(t_hoa_2d_recomposer_gui *x, t_object *view, t_rect *rect)
{
    t_pt cartFisheyeDest, cart;
    double micAngle, w;
    w = x->rect.width;
    double dashes[2] = {4, 4};
    
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_fisheye_layer, rect->width, rect->height);
	
	if (g && x->f_fisheye_show) {
        
        jgraphics_set_source_jrgba(g, &x->f_color_fisheye);
        jgraphics_set_line_width(g, 1);
        jgraphics_set_dash(g, dashes, 2, 0);
        
        cartFisheyeDest.x = Math<double>::abscissa(x->f_channel_radius, x->f_fisheye_azimuth);
        cartFisheyeDest.y = Math<double>::ordinate(x->f_channel_radius, x->f_fisheye_azimuth);
        cartFisheyeDest.x = (cartFisheyeDest.x + (w*0.5));
        cartFisheyeDest.y = ( (w - cartFisheyeDest.y) - (w*0.5) );
        
        for (int i=0; i < x->f_number_of_channels; i++)
        {
            if (x->f_manager->isSelected(i))
            {
                micAngle = x->f_manager->getAzimuth(i);
				cart.x = Math<double>::abscissa(x->f_channel_radius, micAngle);
                cart.y = Math<double>::ordinate(x->f_channel_radius, micAngle);
                cart.x = (cart.x + (w*0.5));
                cart.y = ( (w - cart.y) - (w*0.5) );
                jgraphics_move_to(g, cartFisheyeDest.x, cartFisheyeDest.y);
                jgraphics_line_to(g, cart.x, cart.y);
                jgraphics_stroke(g);
                jgraphics_arc(g, cart.x, cart.y, 2, 0, HOA_2PI);
                jgraphics_fill(g);
            }
        }
        
        jgraphics_arc(g, cartFisheyeDest.x, cartFisheyeDest.y, 4, 0, HOA_2PI);
        jgraphics_fill(g);
        
		jbox_end_layer((t_object*)x, view, hoa_sym_fisheye_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_fisheye_layer, 0., 0.);
}

void draw_rect_selection(t_hoa_2d_recomposer_gui *x, t_object *view, t_rect *rect)
{
    t_jrgba fillColor = x->f_color_selection;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_rectselection_layer, rect->width, rect->height);
    fillColor.alpha = max(fillColor.alpha - 0.2, 0.);
	
	if (g && x->f_rectSelectionExist)
    {
        jgraphics_rectangle(g, x->f_rectSelection.x, x->f_rectSelection.y, x->f_rectSelection.width, x->f_rectSelection.height);
        
        jgraphics_set_source_jrgba(g, &fillColor);
        jgraphics_fill_preserve(g);
        jgraphics_set_source_jrgba(g, &x->f_color_selection);
        jgraphics_set_line_width(g, 2);
        jgraphics_stroke(g);
		
		jbox_end_layer((t_object*)x, view, hoa_sym_rectselection_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_rectselection_layer, 0., 0.);
}

void hoa_2d_recomposer_gui_paint(t_hoa_2d_recomposer_gui *x, t_object *view)
{
    t_rect rect;
    jbox_get_rect_for_view((t_object *)x, view, &rect);
    x->rect = rect;
    x->f_channel_radius = rect.width * 0.46;
    x->f_mic_size = rect.width * 0.025;
    draw_background(x, view, &rect);
    draw_channels_shape(x, view, &rect);
    draw_channels(x, view, &rect);
    draw_fishEye(x, view, &rect);
    draw_channels_text(x, view, &rect);
    draw_rect_selection(x, view, &rect);
}

//========================= Mouse Methods :

/* ---- Utilities ---- */

int channel_hit_test(t_hoa_2d_recomposer_gui *x, t_pt *pt)
{
    double w = x->rect.width;
    double abscissa, ordinate, azimuth, radius, chanSize;
    abscissa = pt->x-(w*0.5);
    ordinate = (w - pt->y)-(w*0.5);
    azimuth = Math<double>::azimuth(abscissa, ordinate);
    radius  = Math<double>::radius(abscissa, ordinate);
    
    chanSize = w / x->f_mic_size / HOA_2PI;
    
    for(int i = x->f_number_of_channels-1; i >= 0; i--)
    {
        if ( hoa::isInsideRad(azimuth, x->f_manager->getAzimuth(i) - chanSize, x->f_manager->getAzimuth(i) + chanSize) &&
            hoa::isInside(radius, x->f_channel_radius - x->f_mic_size, x->f_channel_radius + x->f_mic_size))
        {
            return i;
        }
    }
    return -1;
}

bool channel_hit_test_rect(t_hoa_2d_recomposer_gui *x, int micIndex, t_rect rectSelection)
{
    double w = x->rect.width;
    t_pt micPoint;
    micPoint.x = Math<double>::abscissa(x->f_channel_radius, x->f_manager->getAzimuth(micIndex));
    micPoint.y = Math<double>::ordinate(x->f_channel_radius, x->f_manager->getAzimuth(micIndex));
    micPoint.x = (micPoint.x + (w*0.5));
    micPoint.y = ( (w - micPoint.y) - (w*0.5) );
    return jgraphics_ptinrect(micPoint, rectSelection);
}

void begin_rect_selection(t_hoa_2d_recomposer_gui *x, t_pt pt)
{
    x->f_rectSelectionExist = true;
    x->f_rectSelection.width = x->f_rectSelection.height = 0;
    x->f_rectSelection.x = x->f_rectSelectionBegin.x = pt.x;
    x->f_rectSelection.y = x->f_rectSelectionBegin.y = pt.y;
}

void end_rect_selection(t_hoa_2d_recomposer_gui *x, t_pt pt)
{
    for (int i=0; i < x->f_number_of_channels; i++)
    {
        if (channel_hit_test_rect(x, i, x->f_rectSelection))
        {
            x->f_manager->setSelected(i, -1); // toggle selection state
        }
    }
    x->f_rectSelectionExist = false;
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_rectselection_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
    jbox_redraw((t_jbox *)x);
}

void do_rect_selection(t_hoa_2d_recomposer_gui *x, t_pt pt)
{
    x->f_rectSelectionExist = true;
    if (pt.x > x->f_rectSelectionBegin.x)
    {
        x->f_rectSelection.width = pt.x - x->f_rectSelectionBegin.x;
        x->f_rectSelection.x = x->f_rectSelectionBegin.x;
    }
    else
    {
        x->f_rectSelection.width = x->f_rectSelectionBegin.x -  pt.x;
        x->f_rectSelection.x = x->f_rectSelectionBegin.x - x->f_rectSelection.width;
    }
    
    if (pt.y > x->f_rectSelectionBegin.y)
    {
        x->f_rectSelection.height = pt.y - x->f_rectSelectionBegin.y;
        x->f_rectSelection.y = x->f_rectSelectionBegin.y;
    }
    else
    {
        x->f_rectSelection.height = x->f_rectSelectionBegin.y -  pt.y;
        x->f_rectSelection.y = x->f_rectSelectionBegin.y - x->f_rectSelection.height;
    }
    
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_rectselection_layer);
    jbox_redraw((t_jbox *)x);
}

void hoa_2d_recomposer_gui_mousedown(t_hoa_2d_recomposer_gui *x, t_object *patcherview, t_pt pt, long modifiers)
{
    double w = x->rect.width;
    x->f_last_mouseDown = pt;
    x->f_was_dragging = false;

    int channelHitTest = -1;
    channelHitTest = channel_hit_test(x, &pt);
    
    if (x->f_rectSelectionExist) 
	{
        end_rect_selection(x, pt);
    }
	// Start fish eye
#ifdef _WINDOWS
    else if (modifiers == 8 || x->f_fisheye_show)  // Alt
#else
	else if (modifiers == 148 || x->f_fisheye_show)  // ctrl
#endif
    {
        t_pt ptCart = {pt.x-(w*0.5), (w - pt.y)-(w*0.5)};
        x->f_last_mouseDragRadius = Math<double>::radius(ptCart.x, ptCart.y);
        x->f_manager->setFisheyeStartAzimuth(-2);
		x->f_manager->setFisheyeDestAzimuth(x->f_fisheye_azimuth);
    }
    
    x->f_last_mouseDownOverChannel = channelHitTest;
    jmouse_setcursor(patcherview, (t_object *)x, (x->f_last_mouseMoveOverChannel != -1) ? JMOUSE_CURSOR_POINTINGHAND : JMOUSE_CURSOR_ARROW);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
    jbox_redraw((t_jbox *)x);
}

void hoa_2d_recomposer_gui_mouseup(t_hoa_2d_recomposer_gui *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_last_mouseDragRadius = x->f_channel_radius;
    x->f_last_mouseUp = pt;
    int channelHitTest = -1;
    channelHitTest = channel_hit_test(x, &pt);
    
    if(x->f_fisheye_show)
    {
        x->f_fisheye_show = 0;
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_fisheye_layer);
    }
    
    if (x->f_rectSelectionExist) {
        end_rect_selection(x, pt);
    }
    
    if(!x->f_was_dragging)
    {
        if (x->f_last_mouseDownOverChannel == -1)
        {
#ifdef _WINDOWS
            if (modifiers == 21)  // Control
#else
            if (modifiers == 17)  // Cmd
#endif
            {
                begin_rect_selection(x, pt);
            }
            else if (modifiers == 16) // Nothing
            {
                x->f_manager->setSelected(-1, 0); // tout deselectionné
            }
        }
        else if(x->f_last_mouseDownOverChannel == channelHitTest)
        {
            if (modifiers == 16) // Nothing
            {
                if (!x->f_manager->isSelected(x->f_last_mouseDownOverChannel))
                {
                    x->f_manager->setSelected(-1, 0); // tout deselectionné
                    x->f_manager->setSelected(x->f_last_mouseDownOverChannel, 1);
                }
            }
#ifdef _WINDOWS
            else if (modifiers == 21)  // Control
#else
            else if (modifiers == 17)  // Cmd / shift
#endif
            {
                x->f_manager->setSelected(x->f_last_mouseDownOverChannel, -1);
            }
        }
    }
    
    x->f_last_mouseUpOverChannel = channelHitTest;
    x->f_was_dragging = false;
    jmouse_setcursor(patcherview, (t_object *)x, (x->f_last_mouseMoveOverChannel != -1) ? JMOUSE_CURSOR_POINTINGHAND : JMOUSE_CURSOR_ARROW);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
    jbox_redraw((t_jbox *)x);
}

void hoa_2d_recomposer_gui_mousedrag(t_hoa_2d_recomposer_gui *x, t_object *patcherview, t_pt pt, long modifiers)
{
    double w = x->rect.width;
    t_pt ptCart = {pt.x-(w*0.5), (w - pt.y)-(w*0.5)};
	double radiusDrag = Math<double>::radius(ptCart.x, ptCart.y);
	double angleDrag  = Math<double>::azimuth(ptCart.x, ptCart.y);
    x->f_was_dragging = true;

    if (x->f_rectSelectionExist)
    {
        do_rect_selection(x, pt);
    }
	// Fish eye
#ifdef _WINDOWS
   else if (modifiers == 8 || x->f_fisheye_show)  // Alt
#else
	else if (modifiers == 148 || x->f_fisheye_show)  // ctrl
#endif
    {
        double fisheyeAngle = x->f_manager->getFisheyeDestAzimuth();
        double factor = isInsideRad(angleDrag, fisheyeAngle - HOA_PI2, fisheyeAngle + HOA_PI2) ? 1 : -1;
        double radiusDelta = (x->f_last_mouseDragRadius - radiusDrag) * factor / x->f_channel_radius;
        x->f_manager->setFisheyeStepWithDelta(-2, radiusDelta);
        hoa_2d_recomposer_gui_outputAndNotifyChange(x);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_fisheye_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
        jbox_redraw((t_jbox *)x);
    }
    else if (x->f_last_mouseDownOverChannel != -1)
    {
        if (modifiers == 18) // shift => set wider
        {
            double micAngle = x->f_manager->getAzimuth(x->f_last_mouseDownOverChannel);
            double factor = isInsideRad(angleDrag, micAngle-HOA_PI2, micAngle+HOA_PI2) ? 1 : -1;
            double radiusDelta = (x->f_last_mouseDragRadius - radiusDrag) * factor / x->f_channel_radius;
            
            x->f_manager->setSelected(x->f_last_mouseDownOverChannel, 1);
			
			for (int i=0; i < x->f_number_of_channels; i++)
			{
				if (x->f_manager->isSelected(i))
					x->f_manager->setWideningValue( i, x->f_manager->getWidening(i) + radiusDelta);
			}
        }
        else // => simply rotate
        {
			#ifdef _WINDOWS
				int magnet = (modifiers == 21) ? 1 : 0;  // ctrl
			#else
				int magnet = (modifiers == 17) ? 1 : 0;  // ctrl
			#endif
            
            x->f_manager->setSelected(x->f_last_mouseDownOverChannel, 1);
            x->f_manager->rotateSelectedChannels(angleDrag, x->f_last_mouseDownOverChannel, magnet);
        }
        
        hoa_2d_recomposer_gui_outputAndNotifyChange(x);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
        jbox_redraw((t_jbox *)x);
    }
    
	x->f_last_mouseDrag = pt;
    x->f_last_mouseDragAngle = angleDrag;
    x->f_last_mouseDragRadius = radiusDrag;
}

void hoa_2d_recomposer_gui_mousemove(t_hoa_2d_recomposer_gui *x, t_object *patcherview, t_pt pt, long modifiers)
{
    double w = x->rect.width;
    if ( (x->f_last_mouseMove.x != pt.x) || (x->f_last_mouseMove.y != pt.y) ) 
	{
        x->f_last_mouseMoveOverChannel = channel_hit_test(x, &pt);
        x->f_last_mouseMove = pt;
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
        jbox_redraw((t_jbox *)x);
    }
    
    // fisheye :
#ifdef _WINDOWS
    if (modifiers == 8) // Alt
#else
	if (modifiers == 132) // Control
#endif
    {
        x->f_fisheye_show = 1;
        t_pt ptCart = {pt.x-(w*0.5), (w - pt.y)-(w*0.5)};
        x->f_fisheye_azimuth = Math<double>::azimuth(ptCart.x, ptCart.y);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_fisheye_layer);
        jbox_redraw((t_jbox *)x);
    }
    else if (x->f_fisheye_show)
    {
        x->f_fisheye_show = 0;
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_fisheye_layer);
        jbox_redraw((t_jbox *)x);
    }
    
	// Selection //
#ifdef _WINDOWS
    if (x->f_last_mouseMoveOverChannel == -1 &&  modifiers == 5) // Control
#else
	if (x->f_last_mouseMoveOverChannel == -1 && modifiers == 1) // cmd
#endif
    {
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_CROSSHAIR);
    }
	// Fish eye //
#ifdef _WINDOWS
    else if (modifiers == 8) // Alt
#else
	else if (modifiers == 132) // ctrl
#endif
    {
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_DRAGGINGHAND);
    }
    else
    {
        jmouse_setcursor(patcherview, (t_object *)x, (x->f_last_mouseMoveOverChannel != -1) ? JMOUSE_CURSOR_POINTINGHAND : JMOUSE_CURSOR_ARROW);
    }
}

void hoa_2d_recomposer_gui_mousedoubleclick(t_hoa_2d_recomposer_gui *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if (x->f_last_mouseDownOverChannel != -1)
	{
        x->f_manager->setAzimuthToClosestDefChannelAzimuth(x->f_last_mouseDownOverChannel);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
        jbox_redraw((t_jbox *)x);
        hoa_2d_recomposer_gui_outputAndNotifyChange(x);
    }
}

long hoa_2d_recomposer_gui_key(t_hoa_2d_recomposer_gui *x, t_object *patcherview, long keycode, long modifiers, long textcharacter)
{	
	//post("keycode : %ld , modifiers : %ld , textcharacter : %ld ", keycode, modifiers, textcharacter);
    int filter = 0;

	// Select all
#ifdef _WINDOWS
    if (keycode == 97 && modifiers == 5 && textcharacter == 1) // Control + a
#else
	if (keycode == 97 && modifiers == 1 && textcharacter == 0)  // cmd + a
#endif
    {
		x->f_manager->setSelected(-1, 1); // tout selectionné
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
		jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_harmonics_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
        jbox_redraw((t_jbox *)x);
        filter = 1;
	}
	return filter;	// returns 1 if you want to filter it from the key object (otherwise return 0)
}

void hoa_2d_recomposer_gui_focuslost(t_hoa_2d_recomposer_gui *x)
{
	x->f_manager->setSelected(-1, 0);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_rectselection_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_channels_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_text_layer);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_fisheye_layer);
    jbox_redraw((t_jbox *)x);
}

void *hoa_2d_recomposer_gui_new(t_symbol *s, int argc, t_atom *argv)
{
    t_hoa_2d_recomposer_gui *x =  NULL;
    t_dictionary *d;
    long flags;
    
    if (!(d = object_dictionaryarg(argc,argv)))
        return NULL;
    
    x = (t_hoa_2d_recomposer_gui *)object_alloc(hoa_2d_recomposer_gui_class);
    flags = 0
    | JBOX_DRAWFIRSTIN
    | JBOX_DRAWINLAST
    | JBOX_NODRAWBOX
    | JBOX_TRANSPARENT
    | JBOX_DRAWBACKGROUND
    | JBOX_GROWY
    | JBOX_HILITE
    ;
    jbox_new(&x->j_box, flags, argc, argv);
    
    x->f_outlet_infos = listout(x);
    x->f_out = outlet_new(x, NULL);
    
    x->f_number_of_channels = DEF_UI_CHANNELS;
    x->f_last_mouseMoveOverChannel = x->f_last_mouseDownOverChannel = x->f_last_mouseUpOverChannel = -1;
    x->f_fisheye_show = 0;
    
    x->f_manager = new ChannelManager(x->f_number_of_channels);
    
    long order = 7;
    
    x->f_encoder    = new Encoder<Hoa2d, t_sample>::Basic(order);
    x->f_wider      = new Wider<Hoa2d, t_sample>(order);
    x->f_scope      = new Scope<Hoa2d, t_sample>(order, HOA_DISPLAY_NPOINTS);
    x->f_harmonicsValues = new t_sample[order * 2 + 1];
    
    x->f_manager->setNumberOfChannels(x->f_number_of_channels);
    
    x->j_box.b_firstin = (t_object*) x;
    
    attr_dictionary_process(x, d);
    jbox_ready(&x->j_box);
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_2d_recomposer_gui_main(void)
#else
void ext_main(void *r)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.recomposer", (method)hoa_2d_recomposer_gui_new, (method)hoa_2d_recomposer_gui_free, (short)sizeof(t_hoa_2d_recomposer_gui), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.recomposer", (char *)"hoa.2d.recomposer");
    
    hoa_initclass(c, NULL);
    
    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
    jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH | JBOX_FONTATTR);
    
    
    class_addmethod(c, (method) hoa_2d_recomposer_gui_assist,          "assist",        A_CANT,	0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_preset,          "preset",					0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_getvalueof,      "getvalueof",    A_CANT,	0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_setvalueof,      "setvalueof",    A_CANT,	0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_paint,           "paint",         A_CANT,	0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_notify,          "notify",        A_CANT,	0);
    
    // @method bang @digest Output current values.
    // @description The <m>bang</m> message report current values.
    class_addmethod(c, (method) hoa_2d_recomposer_gui_bang,            "bang",                  0);
    
    // @method set @digest Set angle or widening value for one or more channels without output.
    // @description The word <m>set</m> followed by the symbol <b>wide</b> sets the widening value.
    // The word <m>set</m> followed by the symbol <b>angles</b> sets angles values.
    // Second argument is the index of the channel. Pass 0 if you want to set all of the channels.
    // last argument is the value. <b>angles</b> are in radian, wrapped between 0. and 2π. <b>wide</b> is clipped between 0. and 1.
    // @marg 0 @name wide/angles @optional 0 @type symbol
    // @marg 1 @name channel-index @optional 0 @type int
    // @marg 2 @name value @optional 0 @type float
    class_addmethod(c, (method) hoa_2d_recomposer_gui_set,             "set",           A_GIMME,  0);
    
    // @method angles @digest Set angle value for one or more channels and trigger output.
    // @description The word <m>angles</m> followed by an index and a float set a specific channel angle.
    // The word <m>angles</m> followed by a list of float set several channel angles.
    // @marg 0 @name channel-index/list @optional 0 @type int/list
    // @marg 1 @name value @optional 0 @type float
    class_addmethod(c, (method) hoa_2d_recomposer_gui_angles,          "angles",        A_GIMME,  0);
    
    // @method wide @digest Set widening value for one or more channels and trigger output.
    // @description The word <m>wide</m> followed by an index and a float set a specific channel widening value. Pass 0 to set all widening values.
    // The word <m>wide</m> followed by a list of float set widening value for a list of channel.
    // @marg 0 @name channel-index/list @optional 0 @type int/list
    // @marg 1 @name value @optional 0 @type float
    class_addmethod(c, (method) hoa_2d_recomposer_gui_wide,             "wide",			 A_GIMME,  0);
    
    // @method reset @digest Reset angle and widening value of one or more channels and trigger output.
    // @description Without argument, the word <m>reset</m> will reset all of the angles and widening values of channels to default.
    // The word <m>reset</m> followed by the symbol <m>wide</m> will reset all widening value to 1. To reset specific channels widening values pass their index as a list of int as second argument.
    // The word <m>reset</m> followed by the symbol <m>angle</m> will reset all angles to default. To reset specific channels angles pass their index as a list of int as second argument.
    // @marg 0 @name wide/angles @optional 1 @type symbol
    // @marg 1 @name channel-index @optional 1 @type list
    class_addmethod(c, (method) hoa_2d_recomposer_gui_reset,            "reset",         A_GIMME,  0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_anything,         "anything",      A_GIMME,  0);
    
    // @method (mouse) @digest Click and drag to change angles and widening values of the channels.
    // @description Click and drag the channel points select them and to set their angles and widening values.
    // <ul>
    // <li> <b>drag</b> to move selected channels angles</li>
    // <li> <b>shift+drag</b> to change selected channels' widening value </li>
    // <li> <b>cmd+a</b> to select All (<b>ctrl+a</b> for win)</li>
    // <li> <b>cmd+drag</b> to do a rectangular selection (<b>ctrl+drag</b> for win)</li>
    // <li> <b>cmd+click</b> on a channel to change this channel selection state (<b>cmd+click</b> for win)</li>
    // <li> <b>ctrl+move</b> to have a preview of the fisheye effect (<b>alt+move</b> for win).</li>
    // <li> <b>ctrl+click</b> lock fisheye start and destination angles value for the selected channels (<b>alt+click</b> for win).</li>
    // <li> <b>ctrl+drag</b> apply a "fisheye effect" to the selected channels (drag from/to fisheye point from/to center of the circle) (<b>alt+drag</b> for win)</li>
    // <li> double-click on a channel to set it to the closest default channel angle.</li>
    // <li> Press <b>cmd</b> key when you drag to activate magnetism on the currently dragged channel</li>
    // </ul>
    class_addmethod(c, (method) hoa_2d_recomposer_gui_mousedown,        "mousedown",     A_CANT,   0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_mousedrag,        "mousedrag",     A_CANT,   0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_mouseup,          "mouseup",       A_CANT,   0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_mousemove,        "mousemove",     A_CANT,   0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_mousedoubleclick, "mousedoubleclick", A_CANT,0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_key,              "key",           A_CANT,   0);
    class_addmethod(c, (method) hoa_2d_recomposer_gui_focuslost,        "focuslost",     A_CANT,   0);
    
    CLASS_ATTR_DEFAULT			(c, "patching_rect", 0, "0 0 150 150");
    // @exclude hoa.2d.map
    CLASS_ATTR_INVISIBLE		(c, "color", 0);
    // @exclude hoa.2d.map
    CLASS_ATTR_INVISIBLE		(c, "textcolor", 0);
    // @exclude hoa.2d.map
    
    CLASS_STICKY_CATEGORY(c, 0, "Color");
    CLASS_ATTR_RGBA				(c, "bgcolor", 0, t_hoa_2d_recomposer_gui, f_color_bg);
    CLASS_ATTR_STYLE			(c, "bgcolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER			(c, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bgcolor", 0, "0.760784 0.760784 0.760784 1.0");
    // @description Sets the RGBA values for the background color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "circlecolor", 0, t_hoa_2d_recomposer_gui, f_color_inner_circle);
    CLASS_ATTR_STYLE			(c, "circlecolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "circlecolor", 0, "Circle Inner Color");
    CLASS_ATTR_ORDER			(c, "circlecolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "circlecolor", 0, "0.396078 0.396078 0.396078 0.2");
    // @description Sets the RGBA values for the inner circle color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "chacolor", 0, t_hoa_2d_recomposer_gui, f_color_channel_point);
    CLASS_ATTR_STYLE			(c, "chacolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "chacolor", 0, "Channels Color");
    CLASS_ATTR_ORDER			(c, "chacolor", 0, "4");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "chacolor", 0, "0.396078 0.396078 0.396078 0.2");
    // @description Sets the RGBA values for the channel point color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "selchacolor", 0, t_hoa_2d_recomposer_gui, f_color_channel_point_selected);
    CLASS_ATTR_STYLE			(c, "selchacolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "selchacolor", 0, "Selected Channel Color");
    CLASS_ATTR_ORDER			(c, "selchacolor", 0, "5");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "selchacolor", 0, "0.86 0.86 0.86 1.");
    // @description Sets the RGBA values for the selected channel point color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "channelshapecolor", 0, t_hoa_2d_recomposer_gui, f_color_channel_shape);
    CLASS_ATTR_STYLE			(c, "channelshapecolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "channelshapecolor", 0, "Channel Shape color");
    CLASS_ATTR_ORDER			(c, "channelshapecolor", 0, "6");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "channelshapecolor", 0, "0.4 0.4 0.4 0.3");
    // @description Sets the RGBA values for the channel shape color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "selchannelshapecolor", 0, t_hoa_2d_recomposer_gui, f_color_channel_shape_selected);
    CLASS_ATTR_STYLE			(c, "selchannelshapecolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "selchannelshapecolor", 0, "Channel Shape color");
    CLASS_ATTR_ORDER			(c, "selchannelshapecolor", 0, "6");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "selchannelshapecolor", 0, "0.4 0.4 0.8 0.3");
    // @description Sets the RGBA values for the selected channel shape color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "fisheyecolor", 0, t_hoa_2d_recomposer_gui, f_color_fisheye);
    CLASS_ATTR_STYLE			(c, "fisheyecolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "fisheyecolor", 0, "FishEye color");
    CLASS_ATTR_ORDER			(c, "fisheyecolor", 0, "7");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "fisheyecolor", 0, "0.69 0.18 0.18 1.");
    // @description Sets the RGBA values for the fisheye color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "rectselectcolor", 0, t_hoa_2d_recomposer_gui, f_color_selection);
    CLASS_ATTR_STYLE			(c, "rectselectcolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "rectselectcolor", 0, "Rect Selection color");
    CLASS_ATTR_ORDER			(c, "rectselectcolor", 0, "8");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "rectselectcolor", 0, "0.39 0.52 0.8 1.");
    // @description Sets the RGBA values for the rectangular selection color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "channeltextcolor", 0, t_hoa_2d_recomposer_gui, f_color_channel_point_text);
    CLASS_ATTR_STYLE			(c, "channeltextcolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "channeltextcolor", 0, "Channels text color");
    CLASS_ATTR_ORDER			(c, "channeltextcolor", 0, "9");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "channeltextcolor", 0, "0.9 0.9 0.9 1.");
    // @description Sets the RGBA values for the channel text color of the <o>hoa.2d.recomposer</o> object
    
    CLASS_ATTR_RGBA				(c, "selchanneltextcolor", 0, t_hoa_2d_recomposer_gui, f_color_channel_point_text_sel);
    CLASS_ATTR_STYLE			(c, "selchanneltextcolor", 0, "rgba");
    CLASS_ATTR_LABEL			(c, "selchanneltextcolor", 0, "Selected Channels text color");
    CLASS_ATTR_ORDER			(c, "selchanneltextcolor", 0, "10");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "selchanneltextcolor", 0, "0.4 0.4 0.4 1.");
    // @description Sets the RGBA values for the selected channel text color of the <o>hoa.2d.recomposer</o> object
    CLASS_STICKY_CATEGORY_CLEAR(c);
    
    CLASS_ATTR_LONG				(c, "channels",0, t_hoa_2d_recomposer_gui, f_number_of_channels);
    CLASS_ATTR_CATEGORY			(c, "channels",0,"Custom");
    CLASS_ATTR_FILTER_CLIP		(c, "channels", MIN_UI_CHANNELS, MAX_UI_CHANNELS);
    CLASS_ATTR_ACCESSORS		(c, "channels", NULL, set_number_of_channels);
    CLASS_ATTR_LABEL			(c, "channels", 0, "Number of Channels");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c,"channels",0,"8");
    // @description Sets the number of channels.
    
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.recomposer"));
    hoa_2d_recomposer_gui_class = c;
}

