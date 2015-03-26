/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.meter~.cpp
 @name      hoa.2d.meter~
 @realname  hoa.2d.meter~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A circular array of visual peak level indicators
 
 @description
 The <o>hoa.2d.meter~</o> object displays the peak levels for a circular array of loudspeakers. It computes and displays the energy and velocity vectors.
 
 @discussion
 The <o>hoa.2d.meter~</o> object displays the peak levels for a circular array of loudspeakers. It computes and displays the energy and velocity vectors.
 
 @category planewaves, hoa objects, audio, GUI, MSP
 
 @seealso meter~, live.gain~, hoa.gain~, hoa.2d.scope~, hoa.2d.map, hoa.2d.space, hoa.2d.recomposer, hoa.2d.encoder~, hoa.2d.decoder~
 
 @illustration on @caption the hoa.2d.meter~ GUI object
 @palette yes
 */

#include "Hoa2D.max.h"

#define MAX_UI_CHANNELS 64
#define OVERLED_DRAWTIME 1000

typedef enum {
    VECTOR_NONE = 0,
	VECTOR_ENERGY,
	VECTOR_VELOCITY,
	VECTOR_BOTH
} e_meter_vector_type;

typedef struct  _meter
{
	t_pxjbox	j_box;
    Meter<Hoa2d, t_sample>*     f_meter;
    Vector<Hoa2d, t_sample>*    f_vector;
    t_sample*                   f_signals;
    double                      f_vector_coords[4];
    long                        f_ramp;
    
    void*       f_attrs;
    
    t_clock*	f_clock;
	int			f_startclock;
	t_atom_long	f_interval;
    
	t_atom_long	f_dbperled;
	t_atom_long	f_nhotleds;
	t_atom_long	f_ntepidleds;
	t_atom_long	f_nwarmleds;
	t_atom_long	f_numleds;
	
	t_jrgba		f_color_bg;
	t_jrgba		f_color_mbg;
    t_jrgba		f_color_ledbg;
	t_jrgba		f_color_cold;
	t_jrgba		f_color_tepid;
	t_jrgba		f_color_warm;
	t_jrgba		f_color_hot;
	t_jrgba		f_color_over;
	t_jrgba		f_color_energy;
	t_jrgba		f_color_velocity;
    
    double      f_metersize;
    t_atom_long f_drawledsbg;
    t_atom_long f_drawvector;
    t_atom_long f_drawmborder;
    t_atom_long f_rotation;
    t_atom_long f_direction;
	
	double		f_center;
	double		f_rayonMax;
	double		f_rayonCircle;
	double		f_fontsize;
	
	double		f_strokeWidth;
	double		f_rayonExt;
	double		f_rayonInt;
	
} t_meter;

t_class *meter_class;

t_hoa_err hoa_getinfos(t_meter* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs    = x->f_meter->getNumberOfPlanewaves();
	boxinfos->autoconnect_outputs   = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void meter_free(t_meter *x)
{
	dsp_freejbox((t_pxjbox *)x);
	freeobject((t_object *)x->f_clock);
	jbox_free((t_jbox *)x);
    delete x->f_meter;
	delete x->f_vector;
    delete [] x->f_signals;
}

t_max_err channels_set(t_meter *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_isNumber(argv))
    {
        t_object *b = NULL;
        int numberOfPlanewaves = atom_getlong(argv);
        if(numberOfPlanewaves != x->f_meter->getNumberOfPlanewaves() && numberOfPlanewaves > 0 && numberOfPlanewaves <= MAX_UI_CHANNELS)
        {
            object_method(hoa_sym_dsp->s_thing, hoa_sym_stop);
			
            if (x->f_meter)
				delete x->f_meter;
			
			if (x->f_vector)
				delete x->f_vector;
			
            x->f_meter  = new Meter<Hoa2d, t_sample>(numberOfPlanewaves);
            x->f_vector  = new Vector<Hoa2d, t_sample>(numberOfPlanewaves);
            x->f_meter->computeRendering();
            x->f_vector->computeRendering();
            
            object_obex_lookup(x, gensym("#B"), (t_object **)&b);

            object_method(b, hoa_sym_dynlet_begin);
            dsp_resize((t_pxobject*)x, x->f_meter->getNumberOfPlanewaves());
            object_method(b, hoa_sym_dynlet_end);
            
            if (x->f_meter->getNumberOfPlanewaves() == 1)
            {
                object_attr_setdisabled((t_object*)x, hoa_sym_angles, 1);
                object_attr_setdisabled((t_object*)x, gensym("offset"), 1);
                object_attr_setdisabled((t_object*)x, gensym("rotation"), 1);
            }
            else
            {
                object_attr_setdisabled((t_object*)x, hoa_sym_angles, 0);
                object_attr_setdisabled((t_object*)x, gensym("offset"), 0);
                object_attr_setdisabled((t_object*)x, gensym("rotation"), 0);
            }
        }
        
        object_attr_setvalueof(x, hoa_sym_angles, 0, NULL);
    }
    return MAX_ERR_NONE;
}

t_max_err channels_get(t_meter *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = (t_atom *)malloc(sizeof(t_atom));
    if(argv[0] && argc[0])
    {
        atom_setfloat(argv[0], x->f_meter->getNumberOfPlanewaves());
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    return 0;
}

t_max_err angles_set(t_meter *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av)
    {
        object_method(hoa_sym_dsp->s_thing, gensym("stop"));
        
        for(long i = 0; i < ac && i < x->f_meter->getNumberOfPlanewaves(); i++)
        {
            if(atom_isNumber(av+i))
            {
                x->f_meter->setPlanewaveAzimuth(i, atom_getfloat(av+i) / 360.f * HOA_2PI);
                x->f_vector->setPlanewaveAzimuth(i, atom_getfloat(av+i) / 360.f * HOA_2PI);
            }
        }
        
        x->f_meter->computeRendering();
        x->f_vector->computeRendering();
        
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_skeleton_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_separator_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_leds_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_vectors_layer);
        jbox_redraw((t_jbox *)x);
    }
	
    return MAX_ERR_NONE;
}

t_max_err angles_get(t_meter *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = x->f_meter->getNumberOfPlanewaves();
    argv[0] = (t_atom *)malloc(sizeof(t_atom) * x->f_meter->getNumberOfPlanewaves());
    if(argv[0] && argc[0])
    {
        for(int i = 0; i < x->f_meter->getNumberOfPlanewaves(); i++)
        {
            atom_setfloat(argv[0]+i, x->f_meter->getPlanewaveAzimuth(i) / HOA_2PI * 360.);
        }
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    return MAX_ERR_NONE;
}

t_max_err offset_get(t_meter *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = (t_atom *)malloc(sizeof(t_atom));
    if(argv[0] && argc[0])
    {
        double offset = x->f_meter->getPlanewavesRotationX() / HOA_2PI * 360.;
        
        if (offset > 180.)
        {
            offset = - (360. - offset);
        }
        
        atom_setfloat(argv[0], offset);
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    return MAX_ERR_NONE;
}

t_max_err offset_set(t_meter *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_isNumber(argv))
    {
        const double offset = atom_getfloat(argv) / 360. * HOA_2PI;
        x->f_vector->setPlanewavesRotation(offset, 0., 0.);
        x->f_meter->setPlanewavesRotation(offset, 0., 0.);
        x->f_vector->computeRendering();
        x->f_meter->computeRendering();
        
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_skeleton_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_separator_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_leds_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_vectors_layer);
        jbox_redraw((t_jbox *)x);
    }
    
    return MAX_ERR_NONE;
}

void meter_perform64(t_meter *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	for(int i = 0; i < numins; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_signals+i, numins);
    }
    for(x->f_ramp = 0; x->f_ramp < sampleframes; x->f_ramp++)
    {
        x->f_meter->process(x->f_signals + numins * x->f_ramp);
    }
    if(x->f_startclock)
	{
		x->f_startclock = 0;
		clock_delay(x->f_clock,0);
	}
}

void meter_dsp64(t_meter *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_meter->setVectorSize(maxvectorsize);
    object_method(dsp64, gensym("dsp_add64"), x, meter_perform64, 0, NULL);
    x->f_startclock = 1;
}

void meter_tick(t_meter *x)
{
	if(x->f_drawvector == VECTOR_BOTH)
    {
        x->f_vector->process(x->f_signals, x->f_vector_coords);
    }
    else if(x->f_drawvector == VECTOR_VELOCITY)
    {
        x->f_vector->processVelocity(x->f_signals, x->f_vector_coords);
    }
    else if(x->f_drawvector == VECTOR_ENERGY)
    {
		x->f_vector->processEnergy(x->f_signals, x->f_vector_coords + 2);
    }
    
    x->f_meter->tick(1000 / x->f_interval);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_leds_layer);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_vectors_layer);
	jbox_redraw((t_jbox *)x);
	
	if (sys_getdspstate())
		clock_fdelay(x->f_clock, x->f_interval);
}

void meter_assist(t_meter *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET)
        sprintf(s,"(signal) %s", x->f_meter->getPlanewaveName(a).c_str());
}

t_max_err meter_notify(t_meter *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	t_symbol *name;
	if (msg == hoa_sym_attr_modified)
	{
		name = (t_symbol *)object_method((t_object *)data, hoa_sym_getname);
		if(name == gensym("bgcolor") || name == gensym("mbgcolor") || name == gensym("ledsbg") || name == gensym("mborder") || name == gensym("ledbgcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_skeleton_layer);
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_separator_layer);
		}
		else if(name == gensym("cicolor") || name == gensym("coldcolor") || name == gensym("tepidcolor") || name == gensym("warmcolor") || name == gensym("hotcolor") || name == gensym("overloadcolor") || name == gensym("numleds"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_skeleton_layer);
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_separator_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_leds_layer);
		}
        else if(name == gensym("vectors") || name == gensym("energycolor") || name == gensym("velocitycolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_vectors_layer);
		}
		else if(name == gensym("offset") || name == gensym("metersize") || name == gensym("direction") || name == gensym("orientation") || gensym("rotation"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_skeleton_layer);
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_separator_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_leds_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_vectors_layer);
		}
		else if(name == gensym("dbperled") || name == gensym("nhotleds") || name == gensym("ntepidleds") || name == gensym("nwarmleds"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_leds_layer);
		}
		jbox_redraw((t_jbox *)x);
	}
	return jbox_notify((t_jbox *)x, s, msg, sender, data);
}

void draw_skeleton(t_meter *x,  t_object *view, t_rect *rect)
{
	int i,j;
	double deg1, deg2, rotateAngle, ledContainerSize, ledStroke, ledMargin, ledOffset, channelWidth;
	t_jmatrix transform;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_skeleton_layer, rect->width, rect->height);
	
	deg1 = HOA_PI2;
	ledContainerSize = x->f_rayonExt - x->f_rayonInt - (1*4);
	ledOffset = ledContainerSize / (x->f_numleds+1);
	ledStroke = ledOffset * 0.75;
	ledMargin = ledOffset * 0.5;
    
    t_jrgba black = rgba_addContrast(x->f_color_mbg, -0.12);
	
	if (g)
	{
		// Background :
        jgraphics_rectangle(g, 0., 0., rect->width, rect->height);
        jgraphics_set_source_jrgba(g, &x->f_color_bg);
        jgraphics_fill(g);
        
        // Meter Background :
		jgraphics_set_source_jrgba(g, &x->f_color_mbg);
		jgraphics_set_line_width(g, (x->f_rayonExt - x->f_rayonInt) - 0.5);
		jgraphics_arc(g, x->f_center, x->f_center, x->f_rayonExt*0.5+(x->f_rayonInt*0.5), 0., HOA_2PI);
		jgraphics_stroke(g);
		jgraphics_arc_negative(g, x->f_center, x->f_center, x->f_rayonExt*0.5+(x->f_rayonInt*0.5), HOA_PI, -HOA_2PI); // due to a stroke bug !
		jgraphics_stroke(g);
		
        // skelton circles :
        if (x->f_drawmborder == 1 || x->f_drawmborder == 3)
        {
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_set_line_width(g, 1);
            jgraphics_arc(g, x->f_center, x->f_center, x->f_rayonExt,  0., HOA_2PI);
            jgraphics_stroke(g);
            jgraphics_arc(g, x->f_center, x->f_center, x->f_rayonInt,  0., HOA_2PI);
            jgraphics_stroke(g);
        }
        
        if (x->f_drawledsbg)
        {
            jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
            jgraphics_set_matrix(g, &transform);
			
            // skelton separators and leds bg:
            for(i = 0; i < x->f_meter->getNumberOfPlanewaves(); i++)
            {
				channelWidth = radToDeg(x->f_meter->getPlanewaveWidth(i));
                deg2 = degToRad(90. + channelWidth);
				
                rotateAngle = radToDeg(x->f_meter->getPlanewaveAzimuthMapped(i)) - (channelWidth*0.5);
                if (!x->f_rotation)
				{
                    rotateAngle += channelWidth;
                    rotateAngle *= -1;
                }
				
                jgraphics_rotate(g, degToRad(rotateAngle));
                
                // leds Background :
                jgraphics_set_line_width(g, ledStroke);
                jgraphics_set_line_cap(g, JGRAPHICS_LINE_CAP_BUTT);
                jgraphics_set_source_jrgba(g, &x->f_color_ledbg);
                for( j=0; j < x->f_numleds; j++ )
                {
                    if (x->f_meter->getNumberOfPlanewaves() > 1)
					{
                        if ( x->f_meter->getNumberOfPlanewaves() < 24  && (channelWidth > 14.5))
						{
                            double tmpdeg1 = 0, tmpdeg2 = 0, tmprad = 0;
                            
                            if (x->f_direction == 0) // inside
							{
                                tmpdeg1 = deg1 + (0.008 * (j+4) * x->f_metersize);
                                tmpdeg2 = deg2 - (0.008 * (j+4) * x->f_metersize);
                                tmprad = x->f_rayonExt - (j*ledOffset) - ledMargin - 2;
                            }
							else // outside
							{
                                tmpdeg1 = deg1 + (0.008 * (x->f_numleds+3-(j)) * x->f_metersize);
                                tmpdeg2 = deg2 - (0.008 * (x->f_numleds+3-(j)) * x->f_metersize);
                                tmprad = x->f_rayonInt + (j*ledOffset) + ledMargin + 2;
                            }
                            if (tmpdeg1 < tmpdeg2)
								jgraphics_arc(g, 0, 0, tmprad,  tmpdeg1, tmpdeg2);
                            else
                                jgraphics_arc(g, 0, 0, tmprad, tmpdeg2, tmpdeg1);
                        }
                        else
						{
                            if (x->f_direction == 0)
                                jgraphics_arc_negative(g, 0, 0, x->f_rayonExt-(j*ledOffset) - ledMargin - 2, deg2, deg1);
                            else
                                jgraphics_arc_negative(g, 0, 0, x->f_rayonInt+(j*ledOffset) + ledMargin + 2, deg2, deg1);
                        }
                    }
                    else // mono
					{
                        if (x->f_direction == 0)
                            jgraphics_arc(g, 0, 0, x->f_rayonExt - (j*ledOffset) - ledMargin - 2, 0, HOA_2PI);
                        else
                            jgraphics_arc(g, 0, 0, x->f_rayonInt + (j*ledOffset) + ledMargin + 2, 0, HOA_2PI);
                    }
                    jgraphics_stroke(g);
                }
                jgraphics_rotate(g, degToRad(-rotateAngle));
            }
        }
		jbox_end_layer((t_object*)x, view, hoa_sym_skeleton_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_skeleton_layer, 0., 0.);
}

void draw_separator(t_meter *x,  t_object *view, t_rect *rect)
{
	double rotateAngle, channelWidth;
	t_jmatrix transform;
    t_jrgba black = rgba_addContrast(x->f_color_mbg, -0.12);
    
    t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_separator_layer, rect->width, rect->height);
    
	if (g)
	{
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		
		// skelton separators and leds bg:
		for(int i=0; i < x->f_meter->getNumberOfPlanewaves(); i++)
		{
			channelWidth = radToDeg(x->f_meter->getPlanewaveWidth(i));
            rotateAngle = radToDeg(x->f_meter->getPlanewaveAzimuthMapped(i)) - (channelWidth*0.5);
			if (!x->f_rotation)
			{
				rotateAngle += channelWidth;
				rotateAngle *= -1;
			}
			jgraphics_rotate(g, degToRad(rotateAngle));

            // separator
			if (x->f_meter->getNumberOfPlanewaves() > 1)
			{
                jgraphics_set_line_width(g, 1);
                jgraphics_set_source_jrgba(g, &black);
				jgraphics_move_to(g, 0., x->f_rayonInt);
				jgraphics_line_to(g, 0, x->f_rayonExt);
				jgraphics_stroke(g);
			}
			
			jgraphics_rotate(g, degToRad(-rotateAngle));
		}
		
		jbox_end_layer((t_object*)x, view, hoa_sym_separator_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_separator_layer, 0., 0.);
}

void draw_leds(t_meter *x, t_object *view, t_rect *rect)
{
	int i, j, nbLed, tepidLimit, warmLimit, hotLimit;
	long nLoudSpeak = x->f_meter->getNumberOfPlanewaves();
	double deg1, deg2, rotateAngle, ledContainerSize, ledMargin, ledOffset, meter_dB, min_dB_to_display, channelWidth;
	double tmpdeg1, tmpdeg2, tmprad;
	t_jrgba ledColor;
	t_jmatrix transform;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_leds_layer, rect->width, rect->height);
	
	deg1 = HOA_PI2;
	nbLed = x->f_numleds+1;
	ledContainerSize = x->f_rayonExt - x->f_rayonInt - 4;
	ledOffset = ledContainerSize / nbLed;
	ledMargin = ledOffset * 0.50;
	hotLimit = x->f_numleds - x->f_nhotleds;
	warmLimit = hotLimit - x->f_nwarmleds;
	tepidLimit = warmLimit - x->f_ntepidleds;
	min_dB_to_display = -1 * ( (x->f_numleds * x->f_dbperled) - (x->f_dbperled * 0.5) );
	
	if (g)
	{
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
        
        jgraphics_set_line_cap(g, JGRAPHICS_LINE_CAP_BUTT);
		
		for(i = 0; i < nLoudSpeak; i++)
		{
			// dB (negatif) de -240 à 0;
            meter_dB = x->f_meter->getPlanewaveEnergy(i);
            
            channelWidth = radToDeg(x->f_meter->getPlanewaveWidth(i));
            deg2 = degToRad(90+(channelWidth));
            rotateAngle = radToDeg(x->f_meter->getPlanewaveAzimuthMapped(i)) - (channelWidth*0.5);
            
            if(!x->f_rotation)
            {
                rotateAngle += channelWidth;
                rotateAngle *= -1;
            }
            jgraphics_rotate(g, degToRad(rotateAngle));
            
			if ( meter_dB > min_dB_to_display ) // si on est en dessous pas la peine de dessiner
			{
				// leds :
				jgraphics_set_line_width(g, ledMargin);
				for(j=0; j<nbLed-1; j++)
				{
					if ( ( j < nbLed-1 ) && ( meter_dB > min_dB_to_display + (x->f_dbperled * j) ) )
					{
						if ( j < tepidLimit ) ledColor = x->f_color_cold;
						else if ( j < warmLimit ) ledColor = x->f_color_tepid;
						else if ( j < hotLimit ) ledColor = x->f_color_warm;
						else ledColor = x->f_color_hot;
						
						jgraphics_set_source_jrgba(g, &ledColor);
						jgraphics_set_line_width(g, ledMargin);
						
						if (nLoudSpeak > 1) {
							if ( nLoudSpeak < 24 && (channelWidth > 14.5 ))
                            {
                                tmpdeg1 = 0, tmpdeg2 = 0, tmprad = 0;
                                
                                if (x->f_direction == 0) // inside
								{
                                    tmpdeg1 = deg1 + (0.01 * (j+4) * x->f_metersize);
                                    tmpdeg2 = deg2 - (0.01 * (j+4) * x->f_metersize);
                                    tmprad = x->f_rayonExt - (j*ledOffset) - ledMargin - 2;
                                }
								else // outside
								{
                                    tmpdeg1 = deg1 + (0.01 * (nbLed+3-(j)) * x->f_metersize);
                                    tmpdeg2 = deg2 - (0.01 * (nbLed+3-(j)) * x->f_metersize);
                                    tmprad = x->f_rayonInt + (j*ledOffset) + ledMargin + 2;
                                }
                                if (tmpdeg1 < tmpdeg2)
									jgraphics_arc(g, 0, 0, tmprad,  tmpdeg1, tmpdeg2);
                                else
                                    jgraphics_arc(g, 0, 0, tmprad, tmpdeg2, tmpdeg1);
							}
							else
							{
                                if (x->f_direction == 0)
                                    jgraphics_arc_negative(g, 0, 0, x->f_rayonExt-(j*ledOffset) - ledMargin - 2, deg2, deg1);
                                else
                                    jgraphics_arc_negative(g, 0, 0, x->f_rayonInt+(j*ledOffset) + ledMargin + 2, deg2, deg1);
							}
						}
						else {
                            if (x->f_direction == 0)
                                jgraphics_arc(g, 0, 0, x->f_rayonExt-(j*ledOffset) - ledMargin - 2,  0, HOA_2PI);
                            else
                                jgraphics_arc(g, 0, 0, x->f_rayonInt+(j*ledOffset) + ledMargin + 2,  0, HOA_2PI);
						}
						jgraphics_stroke(g);
					}
					else
					{
						break;
					}
				}
			}
            
            if(x->f_meter->getPlanewaveOverLed(i))
            {
                jgraphics_set_source_jrgba(g, &x->f_color_over);
                jgraphics_set_line_width(g, ledMargin);
                
                if (nLoudSpeak > 1) {
                    
                    if ( nLoudSpeak < 24 && (channelWidth > 14.5 ))
                    {
                        double tmpdeg1 = 0, tmpdeg2 = 0, tmprad = 0;
                        
                        if (x->f_direction == 0) // inside
						{
                            tmpdeg1 = deg1 + (0.01 * (nbLed+1) * x->f_metersize);
                            tmpdeg2 = deg2 - (0.01 * (nbLed+1) * x->f_metersize);
                            tmprad = x->f_rayonInt + ledMargin + 1;
                        }
						else // outside
						{
                            tmpdeg1 = deg1+(0.008 * 3 * x->f_metersize);
                            tmpdeg2 = deg2-(0.008 * 3 * x->f_metersize);
                            tmprad = x->f_rayonExt - ledMargin - 1;
                        }
                        
                        if (tmpdeg1 < tmpdeg2)
							jgraphics_arc(g, 0, 0, tmprad,  tmpdeg1, tmpdeg2);
                        else
							jgraphics_arc_negative(g, 0, 0, tmprad,  tmpdeg1, tmpdeg2);
                    }
                    
                    else
                    {
                        if (x->f_direction == 0) // inside
                            jgraphics_arc_negative(g, 0, 0, x->f_rayonInt + ledMargin + 1, deg2, deg1);
						else // outside
                            jgraphics_arc_negative(g, 0, 0, x->f_rayonExt - ledMargin - 1, deg2, deg1);
                    }
                }
                else
				{
                    if (x->f_direction == 0)
                        jgraphics_arc(g, 0, 0, x->f_rayonInt + ledMargin,  0, HOA_2PI);
                    else
                        jgraphics_arc(g, 0, 0, x->f_rayonExt - ledMargin,  0, HOA_2PI);
                }
                jgraphics_stroke(g);
            }
            
            jgraphics_rotate(g, degToRad(-rotateAngle));
		}
		
		jbox_end_layer((t_object*)x, view, hoa_sym_leds_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_leds_layer, 0., 0.);
}

void draw_vectors(t_meter *x, t_object *view, t_rect *rect)
{
	double pointSize = rect->width*0.02;
	double maxRadius = (x->f_rayonInt-pointSize-1);
	if (pointSize > maxRadius)
		return;
	
	double vecX, vecY;
	t_jmatrix transform;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_vectors_layer, rect->width, rect->height);
	
	if (g)
	{
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
        
        /*
		if (x->f_rotation)
			jgraphics_rotate(g, radToDeg(x->f_meter->getPlanewavesRotationX()));
		else
			jgraphics_rotate(g, -radToDeg(x->f_meter->getPlanewavesRotationX()));
		*/
        
		if (x->f_drawvector == VECTOR_BOTH || x->f_drawvector == VECTOR_ENERGY)
		{
			if (x->f_rotation)
			{
				vecX = x->f_vector_coords[2] * maxRadius;
				vecY = x->f_vector_coords[3] * maxRadius;
			}
			else
			{
                double rad = Math<double>::radius(x->f_vector_coords[2], x->f_vector_coords[3]) * maxRadius;
                double ang = -Math<double>::azimuth(x->f_vector_coords[2], x->f_vector_coords[3]);
                vecX = Math<double>::abscissa(rad, ang);
                vecY = Math<double>::ordinate(rad, ang);
			}
			
			jgraphics_set_source_jrgba(g, &x->f_color_energy);
			jgraphics_arc(g, vecX, vecY, pointSize, 0., HOA_2PI);
			jgraphics_fill_preserve(g);
			jgraphics_set_source_rgba(g, 0.2, 0.2, 0.2, 1.);
			jgraphics_stroke(g);
		}
		
		if (x->f_drawvector == VECTOR_BOTH || x->f_drawvector == VECTOR_VELOCITY)
		{
			if (x->f_rotation)
			{
				double rad = min(Math<double>::radius(x->f_vector_coords[0], x->f_vector_coords[1]), 1.) * maxRadius;
                double ang = Math<double>::azimuth(x->f_vector_coords[0], x->f_vector_coords[1]);
                vecX = Math<double>::abscissa(rad, ang);
                vecY = Math<double>::ordinate(rad, ang);
			}
			else
			{
				double rad = min(Math<double>::radius(x->f_vector_coords[0], x->f_vector_coords[1]), 1.) * maxRadius;
                double ang = -Math<double>::azimuth(x->f_vector_coords[0], x->f_vector_coords[1]);
                vecX = Math<double>::abscissa(rad, ang);
                vecY = Math<double>::ordinate(rad, ang);
			}
						
			jgraphics_set_source_jrgba(g, &x->f_color_velocity);
			jgraphics_arc(g, vecX, vecY, pointSize, 0., HOA_2PI);
			jgraphics_fill_preserve(g);
			jgraphics_set_source_rgba(g, 0.2, 0.2, 0.2, 1.);
			jgraphics_stroke(g);
		}
		jbox_end_layer((t_object*)x, view, hoa_sym_vectors_layer);
	}
	
	jbox_paint_layer((t_object *)x, view, hoa_sym_vectors_layer, 0., 0.);
}

void meter_paint(t_meter *x, t_object *view)
{
    t_rect rect;
    jbox_get_rect_for_view((t_object *)x, view, &rect);
    
    x->f_center = rect.width * 0.5;
    x->f_rayonMax = x->f_center;
    x->f_fontsize = (x->f_rayonMax / 14.) - 1.;
    x->f_rayonCircle = x->f_rayonMax / 32;
    x->f_rayonExt = x->f_rayonMax - 2.5;
    x->f_rayonInt = x->f_rayonExt * (1 - x->f_metersize);
    
    draw_skeleton(x, view, &rect);
    draw_leds(x, view, &rect);
    if (x->f_drawmborder == 2 || x->f_drawmborder == 3)
        draw_separator(x, view, &rect);
    if (x->f_drawvector != VECTOR_NONE)
        draw_vectors(x, view, &rect);
}

void *meter_new(t_symbol *s, int argc, t_atom *argv)
{
    t_meter *x =  NULL;
    t_dictionary *d;
    long flags;
    
    if(!(d = object_dictionaryarg(argc, argv)))
        return NULL;
    
    x = (t_meter *)object_alloc(meter_class);
    flags = 0
    | JBOX_DRAWFIRSTIN
    | JBOX_NODRAWBOX
    | JBOX_DRAWINLAST
    | JBOX_TRANSPARENT
    | JBOX_DRAWBACKGROUND
    | JBOX_GROWY
    ;
    
    jbox_new((t_jbox *)x, flags, argc, argv);
    x->j_box.z_box.b_firstin = (t_object *)x;
    
    ulong channels = 4;
    dictionary_getlong(d, hoa_sym_channels, (t_atom_long *)&channels);
    if(channels < 1)
        channels = 1;
    
    x->f_meter  = new Meter<Hoa2d, t_sample>(channels);
    x->f_vector = new Vector<Hoa2d, t_sample>(channels);
    
    x->f_signals  = new t_sample[MAX_UI_CHANNELS * HOA_MAXBLKSIZE];
    x->f_meter->computeRendering();
    x->f_vector->computeRendering();
    
    dsp_setupjbox((t_pxjbox *)x, x->f_meter->getNumberOfPlanewaves());
    x->f_clock = (t_clock*)clock_new(x,(method)meter_tick);
    x->f_startclock = 0;
    
    attr_dictionary_process(x, d);
    jbox_ready((t_jbox *)x);
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_2d_meter_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.2d.meter~", (method)meter_new, (method)meter_free, (short)sizeof(t_meter), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.meter~", (char *)"hoa.2d.meter~");
    
    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
    class_dspinitjbox(c);
    jbox_initclass(c, JBOX_COLOR);
    
    hoa_initclass(c, (method)hoa_getinfos);
    
    // @method signal @digest Array of signals to be visualize.
    // @description Array of signals to be visualize.
    // @marg 0 @name channel-signal @optional 0 @type signal
    class_addmethod(c, (method) meter_dsp64,		 "dsp64",		  A_CANT, 0);
    class_addmethod(c, (method) meter_assist,		 "assist",		  A_CANT, 0);
    class_addmethod(c, (method) meter_paint,		 "paint",		  A_CANT, 0);
    class_addmethod(c, (method) meter_notify,        "notify",		  A_CANT, 0);
    
    CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0 0 150 150");
    CLASS_ATTR_INVISIBLE            (c, "color", 0);
    
    /* APPEARANCE */
    CLASS_STICKY_CATEGORY           (c, 0, "Appearance");
    CLASS_ATTR_LONG                 (c, "ledsbg", 0, t_meter, f_drawledsbg);
    CLASS_ATTR_ORDER                (c, "ledsbg", 0, "1");
    CLASS_ATTR_STYLE_LABEL          (c, "ledsbg", 0, "onoff", "Draw Leds Background");
    CLASS_ATTR_DEFAULT              (c, "ledsbg", 0, "1");
    CLASS_ATTR_SAVE                 (c, "ledsbg", 1);
    // @description Draw leds background ?
    
    CLASS_ATTR_LONG                 (c, "vectors", 0, t_meter, f_drawvector);
    CLASS_ATTR_ORDER                (c, "vectors", 0, "2");
    CLASS_ATTR_LABEL                (c, "vectors", 0, "Draw Vectors");
    CLASS_ATTR_ENUMINDEX4           (c, "vectors", 0, "none", "energy", "velocity", "both")
    CLASS_ATTR_DEFAULT              (c, "vectors", 0, "1");
    CLASS_ATTR_SAVE                 (c, "vectors", 1);
    // @description The vector(s) to draw.
    
    CLASS_ATTR_LONG                 (c, "mborder", 0, t_meter, f_drawmborder);
    CLASS_ATTR_ORDER                (c, "mborder", 0, "3");
    CLASS_ATTR_LABEL                (c, "mborder", 0, "Draw Meter Borders");
    CLASS_ATTR_ENUMINDEX4           (c, "mborder", 0, "none", "Circles", "Axes", "both")
    CLASS_ATTR_DEFAULT              (c, "mborder", 0, "3");
    CLASS_ATTR_SAVE                 (c, "mborder", 1);
    // @description The meter border(s) to draw.
    
    CLASS_ATTR_DOUBLE               (c, "metersize", 0, t_meter, f_metersize);
    CLASS_ATTR_ORDER                (c, "metersize", 0, "4");
    CLASS_ATTR_LABEL                (c, "metersize", 0, "Meter Circle Size");
    CLASS_ATTR_FILTER_CLIP          (c, "metersize", 0., 1);
    CLASS_ATTR_DEFAULT              (c, "metersize", 0, "0.8");
    CLASS_ATTR_SAVE                 (c, "metersize", 1);
    // @description The size of the inner circle of the <o>hoa.2d.meter~</o>.
    
    CLASS_ATTR_LONG                 (c, "orientation", 0, t_meter, f_direction);
    CLASS_ATTR_LABEL                (c, "orientation", 0, "Meter Fill Orientation");
    CLASS_ATTR_ORDER                (c, "orientation", 0, "5");
    CLASS_ATTR_ENUMINDEX            (c, "orientation", 0, "inside outside");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "orientation", 0, "0");
    // @description The filling orientation of the peak level indicators <o>hoa.2d.meter~</o>.
    
    CLASS_STICKY_CATEGORY_CLEAR (c);
    
    CLASS_STICKY_CATEGORY           (c, 0, "Behavior");
    CLASS_ATTR_LONG                 (c, "channels", 0 , t_meter, f_attrs);
    CLASS_ATTR_ACCESSORS            (c, "channels", channels_get, channels_set);
    CLASS_ATTR_ORDER                (c, "channels", 0, "1");
    CLASS_ATTR_LABEL                (c, "channels", 0, "Number of Channels");
    CLASS_ATTR_SAVE                 (c, "channels", 1);
    CLASS_ATTR_DEFAULT              (c, "channels", 0, "8");
    // @description The number of displayed channel and peak level indicators.
    
    CLASS_ATTR_DOUBLE_VARSIZE       (c, "angles", ATTR_SET_DEFER_LOW, t_meter, f_attrs, f_attrs, MAX_UI_CHANNELS);
    CLASS_ATTR_ACCESSORS            (c, "angles", angles_get, angles_set);
    CLASS_ATTR_ORDER                (c, "angles", 0, "2");
    CLASS_ATTR_LABEL                (c, "angles", 0, "Angles of Channels");
    CLASS_ATTR_SAVE                 (c, "angles", 1);
    //CLASS_ATTR_DEFAULT              (c, "angles", 0, "0 45 90 135 180 225 270 315");
    // @description The angles of the displayed channels and peak level indicators. Values are in degrees, wrapped between 0. and 360., so you can also set the angles with negative values.
    
    CLASS_ATTR_DOUBLE               (c, "offset", 0, t_meter, f_attrs);
    CLASS_ATTR_ACCESSORS            (c, "offset", offset_get, offset_set);
    CLASS_ATTR_ORDER                (c, "offset", 0, "3");
    CLASS_ATTR_LABEL                (c, "offset", 0, "Offset of Channels");
    //CLASS_ATTR_FILTER_CLIP			(c, "offset", -180, 180);
    CLASS_ATTR_DEFAULT              (c, "offset", 0, "0");
    CLASS_ATTR_SAVE                 (c, "offset", 1);
    // @description Display offset of channels and peak level indicators. the value is in degree, clipped between -180. and 180.
    
    CLASS_ATTR_LONG                 (c, "rotation", 0, t_meter, f_rotation);
    CLASS_ATTR_ORDER                (c, "rotation", 0, "4");
    CLASS_ATTR_LABEL                (c, "rotation", 0, "Rotation of Channels");
    CLASS_ATTR_ENUMINDEX            (c, "rotation", 0, "clockwise anti-clockwise");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "rotation",0, "1");
    // @description The rotation can either be <b>clockwise</b> or <b>anti-clockwise</b>
    
    CLASS_ATTR_LONG                 (c, "interval", 0, t_meter, f_interval);
    CLASS_ATTR_ORDER                (c, "interval", 0, "5");
    CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval in Milliseconds");
    CLASS_ATTR_FILTER_MIN           (c, "interval", 20);
    CLASS_ATTR_DEFAULT              (c, "interval", 0, "50");
    CLASS_ATTR_SAVE                 (c, "interval", 1);
    // @description The refresh interval time in milliseconds.
    
    CLASS_STICKY_CATEGORY_CLEAR     (c);
    
    CLASS_ATTR_LONG                 (c, "dbperled", 0, t_meter, f_dbperled);
    CLASS_ATTR_CATEGORY             (c, "dbperled", 0, "Value");
    CLASS_ATTR_ORDER                (c, "dbperled", 0, "4");
    CLASS_ATTR_LABEL                (c, "dbperled", 0, "DeciBels per Led");
    CLASS_ATTR_FILTER_CLIP          (c, "dbperled", 1, 12);
    CLASS_ATTR_DEFAULT              (c, "dbperled", 0, "3");
    CLASS_ATTR_SAVE                 (c, "dbperled", 1);
    // @description Sets the amount of signal level in deciBels represented by each LED. By default each LED represents a 3dB change in volume from its neighboring LEDs.
    
    CLASS_ATTR_LONG                 (c, "nhotleds", 0, t_meter, f_nhotleds);
    CLASS_ATTR_CATEGORY             (c, "nhotleds", 0, "Value");
    CLASS_ATTR_ORDER                (c, "nhotleds", 0, "5");
    CLASS_ATTR_LABEL                (c, "nhotleds", 0, "Number of Hot Leds");
    CLASS_ATTR_FILTER_CLIP          (c, "nhotleds", 0, 20);
    CLASS_ATTR_DEFAULT              (c, "nhotleds", 0, "3");
    CLASS_ATTR_SAVE                 (c, "nhotleds", 1);
    // @description Sets the total number "hot" warning LEDs displayed on the <o>hoa.2d.meter~</o> object (corresponding to the color set by the <b>hotcolor</b> message).
    
    CLASS_ATTR_LONG                 (c, "ntepidleds", 0, t_meter, f_ntepidleds);
    CLASS_ATTR_CATEGORY             (c, "ntepidleds", 0, "Value");
    CLASS_ATTR_ORDER                (c, "ntepidleds", 0, "6");
    CLASS_ATTR_LABEL                (c, "ntepidleds", 0, "Number of Tepid Leds");
    CLASS_ATTR_FILTER_CLIP          (c, "ntepidleds", 0, 20);
    CLASS_ATTR_DEFAULT              (c, "ntepidleds", 0, "3");
    CLASS_ATTR_SAVE                 (c, "ntepidleds", 1);
    // @description Sets the total number "tepid" mid-range LEDs displayed on the <o>hoa.2d.meter~</o> object (corresponding to the color set by the <b>tepidcolor</b> message).
    
    CLASS_ATTR_LONG                 (c, "nwarmleds", 0, t_meter, f_nwarmleds);
    CLASS_ATTR_CATEGORY             (c, "nwarmleds", 0, "Value");
    CLASS_ATTR_ORDER                (c, "nwarmleds", 0, "7");
    CLASS_ATTR_LABEL                (c, "nwarmleds", 0, "Number of Warm Leds");
    CLASS_ATTR_FILTER_CLIP          (c, "nwarmleds", 0, 20);
    CLASS_ATTR_DEFAULT              (c, "nwarmleds", 0, "3");
    CLASS_ATTR_SAVE                 (c, "nwarmleds", 1);
    // @description Sets the total number "warm" lower-mid-range LEDs displayed on the <o>hoa.2d.meter~</o> object (corresponding to the color set by the <b>warmcolor</b> message).
    
    CLASS_ATTR_LONG                 (c, "numleds", 0, t_meter, f_numleds);
    CLASS_ATTR_CATEGORY             (c, "numleds", 0, "Value");
    CLASS_ATTR_ORDER                (c, "numleds", 0, "8");
    CLASS_ATTR_LABEL                (c, "numleds", 0, "Total Number of Leds");
    CLASS_ATTR_FILTER_CLIP          (c, "numleds", 10, 20);
    CLASS_ATTR_DEFAULT              (c, "numleds", 0, "12");
    CLASS_ATTR_SAVE                 (c, "numleds", 1);
    // @description The word numleds, followed by a number between 10 and 20, sets the total number of LEDs displayed on the <o>hoa.2d.meter~</o> object. The range is 10-20 LEDs.
    
    CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_meter, f_color_bg);
    CLASS_ATTR_CATEGORY             (c, "bgcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.76 0.76 0.76 1.");
    // @description Sets the RGBA values for the background color of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "mbgcolor", 0, t_meter, f_color_mbg);
    CLASS_ATTR_LABEL                (c, "mbgcolor", 0, "Meter Background Color");
    CLASS_ATTR_CATEGORY             (c, "mbgcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "mbgcolor", 0, "rgba");
    CLASS_ATTR_ORDER                (c, "mbgcolor", 0, "2");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "mbgcolor", 0, "0.61 0.61 0.61 1.");
    // @description Sets the RGBA values for the filled circle background color of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "ledbgcolor", 0, t_meter, f_color_ledbg);
    CLASS_ATTR_LABEL                (c, "ledbgcolor", 0, "Leds Background Color");
    CLASS_ATTR_CATEGORY             (c, "ledbgcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "ledbgcolor", 0, "rgba");
    CLASS_ATTR_ORDER                (c, "ledbgcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ledbgcolor", 0, "0. 0. 0. 0.05");
    // @description Sets the RGBA values for leds background color of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "coldcolor", 0, t_meter, f_color_cold);
    CLASS_ATTR_CATEGORY             (c, "coldcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "coldcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "coldcolor", 0, "Cold Signal Color");
    CLASS_ATTR_ORDER                (c, "coldcolor", 0, "4");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "coldcolor", 0, "0. 0.6 0. 0.8");
    // @description Sets the RGBA values for the cold signal color of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "tepidcolor", 0, t_meter, f_color_tepid);
    CLASS_ATTR_CATEGORY             (c, "tepidcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "tepidcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "tepidcolor", 0, "Tepid Signal Color");
    CLASS_ATTR_ORDER                (c, "tepidcolor", 0, "5");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "tepidcolor", 0, "0.6 0.73 0. 0.8");
    // @description Sets the RGBA values for the LEDs color for the lower-middle "tepid" range of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "warmcolor", 0, t_meter, f_color_warm);
    CLASS_ATTR_CATEGORY             (c, "warmcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "warmcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "warmcolor", 0, "Warm Signal Color");
    CLASS_ATTR_ORDER                (c, "warmcolor", 0, "6");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "warmcolor", 0, ".85 .85 0. 0.8");
    // @description Sets the RGBA values for the LEDs color for upper-middle "warm" range of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "hotcolor", 0, t_meter, f_color_hot);
    CLASS_ATTR_CATEGORY             (c, "hotcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "hotcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "hotcolor", 0, "Hot Signal Color");
    CLASS_ATTR_ORDER                (c, "hotcolor", 0, "7");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "hotcolor", 0, "1. 0.6 0. 0.8");
    // @description Sets the RGBA values for the LEDs color for the upper "hot" range of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "overloadcolor", 0, t_meter, f_color_over);
    CLASS_ATTR_CATEGORY             (c, "overloadcolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "overloadcolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "overloadcolor", 0, "Overload Signal Color");
    CLASS_ATTR_ORDER                (c, "overloadcolor", 0, "8");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "overloadcolor", 0, "1. 0. 0. 0.8");
    // @description Sets the RGBA values for the LEDs color for the "over" indicator of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "energycolor", 0, t_meter, f_color_energy);
    CLASS_ATTR_CATEGORY             (c, "energycolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "energycolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "energycolor", 0, "Energy Vector Color");
    CLASS_ATTR_ORDER                (c, "energycolor", 0, "9");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "energycolor", 0, "0. 0. 1. 0.8");
    // @description Sets the RGBA values for the energy vector color of the <o>hoa.2d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "velocitycolor", 0, t_meter, f_color_velocity);
    CLASS_ATTR_CATEGORY             (c, "velocitycolor", 0, "Color");
    CLASS_ATTR_STYLE                (c, "velocitycolor", 0, "rgba");
    CLASS_ATTR_LABEL                (c, "velocitycolor", 0, "Velocity Vector Color");
    CLASS_ATTR_ORDER                (c, "velocitycolor", 0, "9");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "velocitycolor", 0, "1. 0. 0. 0.8");
    // @description Sets the RGBA values for the velocity vector color of the <o>hoa.2d.meter~</o> object
    
    class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.meter~"));
    meter_class = c;
    return 0;
}