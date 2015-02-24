/**
 * Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 @file      hoa.3d.meter~.cpp
 @name      hoa.3d.meter~
 @realname  hoa.3d.meter~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 Displays the peak level indicators for a spherical array of loudspeakers
 
 @description
 <o>hoa.3d.meter~</o> is a spherical version of the <o>hoa.2d.meter~</o>. It computes and displays the peak level indicators for a spherical array of loudspeakers, and the energy and velocity vectors.
 
 @discussion
 <o>hoa.3d.meter~</o> is a spherical version of the <o>hoa.2d.meter~</o>. It computes and displays the peak level indicators for a spherical array of loudspeakers, and the energy and velocity vectors.
 
 @category planewaves, hoa objects, audio, GUI, MSP
 
 @seealso meter~, hoa.2d.meter~, hoa.3d.scope~, hoa.3d.decoder~, hoa.3d.encoder~, hoa.3d.map~, hoa.3d.optim~, hoa.3d.vector~, hoa.3d.wider~, hoa.dac~
 
 @illustration on @caption the hoa.3d.meter~ GUI object
 @palette yes
 */

#include "Hoa3D.max.h"

#define MAX_SPEAKER 128

typedef struct  _hoa_meter_3d
{
	t_pxjbox		j_box;
    Hoa3D::Meter*   f_meter;
    Hoa3D::Vector*  f_vector;
    t_sample*		f_signals;
    double			f_vector_coords[6];
    int*            f_over_leds;
    
	t_rect			f_rect;
	t_rect			f_presentation_rect;
    
    long            f_number_of_channels;
    double          f_angles_of_channels[MAX_SPEAKER * 2];
    double          f_offsets[3];
    t_symbol*       f_clockwise;
    t_symbol*       f_view;
    t_symbol*       f_vector_type;
    long            f_interval;
    
    t_jrgba         f_color_bg;
	t_jrgba			f_color_off_signal;
	t_jrgba         f_color_cold_signal;
	t_jrgba         f_color_tepid_signal;
	t_jrgba         f_color_warm_signal;
	t_jrgba         f_color_hot_signal;
	t_jrgba         f_color_over_signal;
	
	t_jrgba         f_color_energy_vector;
	t_jrgba         f_color_velocity_vector;
    
    long            f_ramp;
	int             f_startclock;
    
    double          f_radius;
    double          f_center;
	double          f_radius_center;
    
	t_clock*        f_clock;
    
} t_hoa_meter_3d;

t_class *hoa_meter_3d_class;

void *hoa_meter_3d_new(t_symbol *s, int argc, t_atom *argv);
void hoa_meter_3d_free(t_hoa_meter_3d *x);
void hoa_meter_3d_assist(t_hoa_meter_3d *x, void *b, long m, long a, char *s);

void hoa_meter_3d_dsp64(t_hoa_meter_3d *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void hoa_meter_3d_perform64(t_hoa_meter_3d *x, t_object *d, double **ins, long ni, double **outs, long no, long sf, long f,void *up);
void hoa_meter_3d_tick(t_hoa_meter_3d *x);

t_max_err hoa_meter_3d_notify(t_hoa_meter_3d *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_max_err channels_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv);
t_max_err angles_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv);
t_max_err offset_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv);
t_max_err vectors_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv);
t_max_err rotation_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv);
t_max_err view_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv);

long hoa_meter_3d_oksize(t_hoa_meter_3d *x, t_rect *newrect);

void hoa_meter_3d_paint(t_hoa_meter_3d *x, t_object *view);
void draw_background(t_hoa_meter_3d *x, t_object *view, t_rect *rect);
void draw_leds(t_hoa_meter_3d *x,  t_object *view, t_rect *rect);
void draw_vectors(t_hoa_meter_3d *x, t_object *view, t_rect *rect);

t_hoa_err hoa_getinfos(t_hoa_meter_3d* x, t_hoa_boxinfos* boxinfos);

t_symbol* hoa_sym_3d_none = gensym("none");
t_symbol* hoa_sym_3d_energy = gensym("energy");
t_symbol* hoa_sym_3d_velocity = gensym("velocity");
t_symbol* hoa_sym_3d_both = gensym("both");
t_symbol* hoa_sym_3d_clockwise = gensym("clockwise");
t_symbol* hoa_sym_3d_anticlock = gensym("anti-clockwise");
t_symbol* hoa_sym_3d_vector_layer = gensym("vectors_layer");
t_symbol* hoa_sym_3d_leds_layer = gensym("leds_layers");
t_symbol* hoa_sym_3d_background_layer = gensym("background_layers");
t_symbol* hoa_sym_3d_top = gensym("top");
t_symbol* hoa_sym_3d_bottom = gensym("bottom");
t_symbol* hoa_sym_3d_toponbottom = gensym("top/bottom");
t_symbol* hoa_sym_3d_topnextbottom = gensym("top-bottom");

#define  contrast_white 0.06
#define  contrast_black 0.14

#ifdef HOA_PACKED_LIB
int hoa_3d_meter_main(void)
#else
int C74_EXPORT main(void)
#endif
{
	t_class *c;
	c = class_new("hoa.3d.meter~", (method)hoa_meter_3d_new, (method)hoa_meter_3d_free, (short)sizeof(t_hoa_meter_3d), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.3d.meter~", (char *)"hoa.3d.meter~");
    
	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	class_dspinitjbox(c);
	jbox_initclass(c, JBOX_COLOR);
	hoa_initclass(c, (method)hoa_getinfos);
	
	// @method signal @digest Array of signals to be visualize.
	// @description Array of signals to visualize.
	class_addmethod(c, (method) hoa_meter_3d_dsp64,           "dsp64",			A_CANT, 0);
	class_addmethod(c, (method) hoa_meter_3d_assist,          "assist",			A_CANT, 0);
	class_addmethod(c, (method) hoa_meter_3d_paint,           "paint",			A_CANT, 0);
	class_addmethod(c, (method) hoa_meter_3d_notify,          "notify",			A_CANT, 0);
    class_addmethod(c, (method) hoa_meter_3d_oksize,          "oksize",			A_CANT, 0);
	
	CLASS_ATTR_INVISIBLE            (c, "color", 0);
	CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0 0 150 150");
	
    CLASS_ATTR_LONG                 (c, "channels", ATTR_SET_DEFER_LOW, t_hoa_meter_3d, f_number_of_channels);
    CLASS_ATTR_ACCESSORS            (c, "channels", NULL, channels_set);
	CLASS_ATTR_ORDER                (c, "channels", 0, "1");
	CLASS_ATTR_LABEL                (c, "channels", 0, "Number of Channels");
	CLASS_ATTR_SAVE                 (c, "channels", 1);
    CLASS_ATTR_DEFAULT              (c, "channels", 0, "8");
	// @description The number of displayed channel and peak level indicators.
	
    CLASS_ATTR_DOUBLE_VARSIZE       (c, "angles", ATTR_SET_DEFER_LOW, t_hoa_meter_3d, f_angles_of_channels, f_number_of_channels, MAX_SPEAKER*2);
	CLASS_ATTR_ACCESSORS            (c, "angles", NULL, angles_set);
	CLASS_ATTR_ORDER                (c, "angles", 0, "2");
	CLASS_ATTR_LABEL                (c, "angles", 0, "Angles of Channels");
	CLASS_ATTR_SAVE                 (c, "angles", 1);
    CLASS_ATTR_DEFAULT              (c, "angles", 0, "0 45 90 45 180 45 270 45 0 -45 90 -45 180 -45 270 -45");
	// @description The angles of displayed channels and peak level indicators. Values are in degrees, wrapped between 0. and 360. The list lenght must be equal to 2*<m>channels</m> : interleaved azimuth and elevation value for each channels.
    
    CLASS_ATTR_DOUBLE_ARRAY         (c, "offset", ATTR_SET_DEFER_LOW, t_hoa_meter_3d, f_offsets, 3);
    CLASS_ATTR_ACCESSORS            (c, "offset", NULL, offset_set);
	CLASS_ATTR_ORDER                (c, "offset", 0, "3");
	CLASS_ATTR_LABEL                (c, "offset", 0, "Offset of Channels");
	CLASS_ATTR_DEFAULT              (c, "offset", 0, "0. 0. 0.");
	CLASS_ATTR_SAVE                 (c, "offset", 1);
	// @description Offset of channels and peak level indicators. The 3 values, corresponding to a rotation of the sphere over the (X, Y, Z) axis are in degree.
	
    CLASS_ATTR_SYM					(c, "rotation", 0, t_hoa_meter_3d, f_clockwise);
    CLASS_ATTR_ACCESSORS            (c, "rotation", NULL, rotation_set);
    CLASS_ATTR_ORDER                (c, "rotation", 0, "4");
	CLASS_ATTR_LABEL                (c, "rotation", 0, "Rotation of Channels");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "rotation", 0, "anti-clockwise");
    CLASS_ATTR_ENUM					(c, "rotation", 0, "anti-clockwise clockwise");
	// @description The rotation can either be <b>clockwise</b> or <b>anti-clockwise</b>
    
    CLASS_ATTR_SYM					(c, "view", 0 , t_hoa_meter_3d, f_view);
    CLASS_ATTR_ACCESSORS            (c, "view", NULL, view_set);
	CLASS_ATTR_ORDER                (c, "view", 0, "5");
	CLASS_ATTR_LABEL                (c, "view", 0, "View");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "view", 0, "top");
    CLASS_ATTR_ENUM					(c, "view", 0, "top bottom top-bottom top/bottom");
	// @description Select the view you want compared to the sphere. "top-bottom" will draw the bottom of the sphere at the right of the top while the "top/bottom" view will draw it at the bottom.
    
	CLASS_ATTR_SYM					(c, "vectors", 0, t_hoa_meter_3d, f_vector_type);
    CLASS_ATTR_ACCESSORS            (c, "vectors", NULL, vectors_set);
	CLASS_ATTR_ORDER                (c, "vectors", 0, "2");
    CLASS_ATTR_LABEL                (c, "vectors", 0, "Vectors");
	CLASS_ATTR_DEFAULT              (c, "vectors", 0, "Energy");
	CLASS_ATTR_SAVE                 (c, "vectors", 1);
    CLASS_ATTR_ENUM					(c, "vectors", 0, "none energy velocity both");
	// @description The vector(s) to draw.
    
    CLASS_ATTR_LONG                 (c, "interval", 0, t_hoa_meter_3d, f_interval);
	CLASS_ATTR_ORDER                (c, "interval", 0, "5");
	CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval (in ms)");
	CLASS_ATTR_FILTER_MIN           (c, "interval", 20);
	CLASS_ATTR_DEFAULT              (c, "interval", 0, "50");
	CLASS_ATTR_SAVE                 (c, "interval", 1);
	// @description The refresh interval time in milliseconds.
    
	CLASS_STICKY_CATEGORY(c, 0, "Color");
    CLASS_ATTR_RGBA					(c, "bgcolor", 0, t_hoa_meter_3d, f_color_bg);
	CLASS_ATTR_STYLE				(c, "bgcolor", 0, "rgba");
	CLASS_ATTR_LABEL				(c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT	(c, "bgcolor", 0, "0.76 0.76 0.76 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 1, "rgba");
	// @description Sets the RGBA values for the background color of the <o>hoa.3d.meter~</o> object
    
	CLASS_ATTR_RGBA                 (c, "offcolor", 0, t_hoa_meter_3d, f_color_off_signal);
	CLASS_ATTR_LABEL                (c, "offcolor", 0, "Off Signal Color");
	CLASS_ATTR_ORDER                (c, "offcolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "offcolor", 0, "0. 0. 0. 0.2");
	CLASS_ATTR_STYLE                (c, "offcolor", 1, "rgba");
	// @description Sets the RGBA values for the off signal color of the <o>hoa.2d.meter~</o> object
	
	CLASS_ATTR_RGBA                 (c, "coldcolor", 0, t_hoa_meter_3d, f_color_cold_signal);
	CLASS_ATTR_LABEL                (c, "coldcolor", 0, "Cold Signal Color");
	CLASS_ATTR_ORDER                (c, "coldcolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "coldcolor", 0, "0. 0.6 0. 0.8");
	CLASS_ATTR_STYLE                (c, "coldcolor", 1, "rgba");
	// @description Sets the RGBA values for the cold signal color of the <o>hoa.3d.meter~</o> object
    
	CLASS_ATTR_RGBA                 (c, "tepidcolor", 0, t_hoa_meter_3d, f_color_tepid_signal);
	CLASS_ATTR_LABEL                (c, "tepidcolor", 0, "Tepid Signal Color");
	CLASS_ATTR_ORDER                (c, "tepidcolor", 0, "5");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "tepidcolor", 0, "0.6 0.73 0. 0.8");
	CLASS_ATTR_STYLE                (c, "tepidcolor", 1, "rgba");
	// @description Sets the RGBA values for the LEDs color for the lower-middle "tepid" range of the <o>hoa.3d.meter~</o> object
    
	CLASS_ATTR_RGBA                 (c, "warmcolor", 0, t_hoa_meter_3d, f_color_warm_signal);
	CLASS_ATTR_LABEL                (c, "warmcolor", 0, "Warm Signal Color");
	CLASS_ATTR_ORDER                (c, "warmcolor", 0, "6");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "warmcolor", 0, ".85 .85 0. 0.8");
	CLASS_ATTR_STYLE                (c, "warmcolor", 1, "rgba");
	// @description Sets the RGBA values for the LEDs color for upper-middle "warm" range of the <o>hoa.3d.meter~</o> object
    
	CLASS_ATTR_RGBA                 (c, "hotcolor", 0, t_hoa_meter_3d, f_color_hot_signal);
	CLASS_ATTR_LABEL                (c, "hotcolor", 0, "Hot Signal Color");
	CLASS_ATTR_ORDER                (c, "hotcolor", 0, "7");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "hotcolor", 0, "1. 0.6 0. 0.8");
	CLASS_ATTR_STYLE                (c, "hotcolor", 1, "rgba");
	// @description Sets the RGBA values for the LEDs color for the upper "hot" range of the <o>hoa.3d.meter~</o> object
    
	CLASS_ATTR_RGBA                 (c, "overloadcolor", 0, t_hoa_meter_3d, f_color_over_signal);
	CLASS_ATTR_LABEL                (c, "overloadcolor", 0, "Overload Signal Color");
	CLASS_ATTR_ORDER                (c, "overloadcolor", 0, "8");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "overloadcolor", 0, "1. 0. 0. 0.8");
	CLASS_ATTR_STYLE                (c, "overloadcolor", 1, "rgba");
	// @description Sets the RGBA values for the LEDs color for the "over" indicator of the <o>hoa.3d.meter~</o> object
    
	CLASS_ATTR_RGBA                 (c, "energycolor", 0, t_hoa_meter_3d, f_color_energy_vector);
	CLASS_ATTR_LABEL                (c, "energycolor", 0, "Energy Vector Color");
	CLASS_ATTR_ORDER                (c, "energycolor", 0, "9");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "energycolor", 0, "0. 0. 1. 0.8");
    CLASS_ATTR_STYLE                (c, "energycolor", 1, "rgba");
	// @description Sets the RGBA values for the energy vector color of the <o>hoa.3d.meter~</o> object
    
    CLASS_ATTR_RGBA                 (c, "velocitycolor", 0, t_hoa_meter_3d, f_color_velocity_vector);
	CLASS_ATTR_LABEL                (c, "velocitycolor", 0, "Velocity Vector Color");
	CLASS_ATTR_ORDER                (c, "velocitycolor", 0, "9");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "velocitycolor", 0, "1. 0. 0. 0.8");
    CLASS_ATTR_STYLE                (c, "velocitycolor", 1, "rgba");
	// @description Sets the RGBA values for the velocity vector color of the <o>hoa.3d.meter~</o> object
	CLASS_STICKY_CATEGORY_CLEAR(c);
	
    class_register(CLASS_BOX, c);
	hoa_meter_3d_class = c;
    
    return 0;
}

void *hoa_meter_3d_new(t_symbol *s, int argc, t_atom *argv)
{
	t_hoa_meter_3d *x =  NULL;
	t_dictionary *d;
	long flags;
	int numberOfChannels = 8;
	
	if(!(d = object_dictionaryarg(argc,argv)))
		return NULL;
	
	x = (t_hoa_meter_3d *)object_alloc(hoa_meter_3d_class);
	flags = 0
	| JBOX_DRAWFIRSTIN
	| JBOX_DRAWINLAST
    | JBOX_NODRAWBOX
	| JBOX_DRAWBACKGROUND
	| JBOX_TRANSPARENT
	| JBOX_GROWBOTH
	;
	
	jbox_new((t_jbox *)x, flags, argc, argv);
	x->j_box.z_box.b_firstin = (t_object *)x;

	jbox_get_patching_rect((t_object*)x, &x->f_rect);
	jbox_get_presentation_rect((t_object*)x, &x->f_presentation_rect);
    
    x->f_ramp = 0;
	x->f_meter  = new Hoa3D::Meter(numberOfChannels, 181, 360);
    x->f_vector = new Hoa3D::Vector(numberOfChannels);
    
    x->f_signals = new double[MAX_SPEAKER * SYS_MAXBLKSIZE];
    x->f_over_leds = new int[MAX_SPEAKER];
	
    for(int i = 0; i < MAX_SPEAKER; i++)
        x->f_over_leds[i] = 0;
	
    x->f_clock = (t_clock*)clock_new(x,(method)hoa_meter_3d_tick);
	x->f_startclock = 0;
	
    dsp_setupjbox((t_pxjbox *)x, x->f_meter->getNumberOfChannels());
    
    for(int i = 0; i < x->f_meter->getNumberOfChannels() * 2; i+= 2)
    {
        x->f_angles_of_channels[i] = x->f_meter->getChannelAzimuth(i/2) / HOA_2PI * 360;
        x->f_angles_of_channels[i+1] = x->f_meter->getChannelElevation(i/2) / HOA_2PI * 360;
    }

	attr_dictionary_process(x, d);
    
	jbox_ready((t_jbox *)x);
	return (x);
}

void hoa_meter_3d_free(t_hoa_meter_3d *x)
{
	jbox_free((t_jbox *)x);
    clock_free(x->f_clock);
    delete x->f_meter;
    delete x->f_vector;
    delete [] x->f_signals;
    delete [] x->f_over_leds;
}

t_hoa_err hoa_getinfos(t_hoa_meter_3d* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_3D;
	boxinfos->autoconnect_inputs    = x->f_meter->getNumberOfChannels();
	boxinfos->autoconnect_outputs   = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_PLANEWAVES;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

long hoa_meter_3d_oksize(t_hoa_meter_3d *x, t_rect *newrect)
{
	newrect->width = max<double>(newrect->width, 20.);
    newrect->height = max<double>(newrect->height, 20.);
	
    double min = std::min(newrect->width, newrect->height);
    
    if(x->f_view == hoa_sym_3d_topnextbottom)
    {
		newrect->width = min*2;
		newrect->height = min;
    }
    else if(x->f_view == hoa_sym_3d_toponbottom)
    {
		newrect->height = min*2;
		newrect->width = min;
    }
    else
    {
		newrect->width = min;
		newrect->height = min;
    }
	
	return 0;
}

t_max_err channels_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(atom_isNumber(argv))
        {
			t_object *b = NULL;
            long d = Math<long>::clip(atom_getlong(argv), 1, MAX_SPEAKER);
            if(d != x->f_meter->getNumberOfChannels())
            {
				object_obex_lookup(x, hoa_sym_pound_B, (t_object **)&b);
                int dspState = sys_getdspobjdspstate((t_object*)x);

				if (b)
				{
					if(dspState)
						object_method(gensym("dsp")->s_thing, hoa_sym_stop);
					
					delete x->f_meter;
					delete x->f_vector;
					x->f_meter = new Hoa3D::Meter(d, 181, 360);
                    x->f_vector = new Hoa3D::Vector(d);
					
					object_method(b, hoa_sym_dynlet_begin);
					dsp_resize((t_pxobject*)x, x->f_meter->getNumberOfChannels());
					object_method(b, hoa_sym_dynlet_end);
					
					if(dspState)
						object_method(gensym("dsp")->s_thing, hoa_sym_start);
                    
                    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
                    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
                    jbox_redraw((t_jbox *)x);
				}
            }
        }
    }
    
    for(int i = 0, j = 0; i < x->f_meter->getNumberOfChannels() * 2; i+=2, j++)
    {
        x->f_angles_of_channels[i] = x->f_meter->getChannelAzimuth(j) / HOA_2PI * 360.;
        x->f_angles_of_channels[i+1] = x->f_meter->getChannelElevation(j) / HOA_2PI * 360.;
    }
    
    x->f_number_of_channels = x->f_meter->getNumberOfChannels();
    
    object_attr_touch((t_object*)x, gensym("angles"));
    
    return MAX_ERR_NONE;
}

t_max_err angles_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        short dspstate = dsp_setloadupdate(false);
        double pos1, pos2;
        for(int i = 1, j = 0; i < x->f_meter->getNumberOfChannels() * 2 && i < argc; i+= 2, j++)
        {
            if( (atom_gettype(argv+i-1) == A_FLOAT || atom_gettype(argv+i-1) == A_LONG) &&
               (atom_gettype(argv+i) == A_FLOAT || atom_gettype(argv+i) == A_LONG) )
            {
                pos1 = atom_getfloat(argv+i-1) / 360. * HOA_2PI;
                pos2 = atom_getfloat(argv+i) / 360. * HOA_2PI;
            }
            
            x->f_meter->setChannelPosition(j, pos1, pos2);
            x->f_vector->setChannelPosition(j, pos1, pos2);
            
            x->f_angles_of_channels[i-1] = x->f_meter->getChannelAzimuth(j) / HOA_2PI * 360;
            x->f_angles_of_channels[i] = x->f_meter->getChannelElevation(j) / HOA_2PI * 360;
        }
        dsp_setloadupdate(dspstate);
    }
    
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_background_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
    jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
    jbox_redraw((t_jbox *)x);
    
    return MAX_ERR_NONE;
}

t_max_err offset_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv)
{
	if(argc && argv)
    {
        double ax, ay, az;
        
        if(atom_isNumber(argv))
            ax = atom_getfloat(argv) / 360. * HOA_2PI;
        else
            ax = x->f_meter->getChannelsRotationX();
        
        if(argc > 1 && argv+1 && atom_isNumber(argv+1))
            ay = atom_getfloat(argv+1) / 360. * HOA_2PI;
        else
            ay = x->f_meter->getChannelsRotationX();
        
        if(argc > 2 && argv+2 && atom_isNumber(argv+2))
            az = atom_getfloat(argv+2) / 360. * HOA_2PI;
        else
            az = x->f_meter->getChannelsRotationX();
				
        x->f_meter->setChannelsRotation(ax, ay, az);
        x->f_vector->setChannelsRotation(ax, ay, az);
        
		jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
		jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
		jbox_redraw((t_jbox *)x);
    }
    
    x->f_offsets[0] = x->f_meter->getChannelsRotationX() / HOA_2PI * 360.;
    x->f_offsets[1] = x->f_meter->getChannelsRotationY() / HOA_2PI * 360.;
    x->f_offsets[2] = x->f_meter->getChannelsRotationZ() / HOA_2PI * 360.;
	
    return MAX_ERR_NONE;
}

t_max_err vectors_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_SYM)
        {
            if(atom_getsym(argv) == hoa_sym_3d_energy)
                x->f_vector_type = hoa_sym_3d_energy;
            else if(atom_getsym(argv) == hoa_sym_3d_velocity)
                x->f_vector_type = hoa_sym_3d_velocity;
            else if(atom_getsym(argv) == hoa_sym_3d_both)
                x->f_vector_type = hoa_sym_3d_both;
            else
                x->f_vector_type = hoa_sym_3d_none;
        }
        else if(atom_isNumber(argv))
        {
            if(atom_getlong(argv) == 1)
                x->f_vector_type = hoa_sym_3d_energy;
            else if(atom_getlong(argv) == 2)
                x->f_vector_type = hoa_sym_3d_velocity;
            else if(atom_getlong(argv) == 3)
                x->f_vector_type = hoa_sym_3d_both;
            else
                x->f_vector_type = hoa_sym_3d_none;
        }
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
        jbox_redraw((t_jbox *)x);
    }
    return 0;
}

t_max_err rotation_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_SYM)
        {
            if(atom_getsym(argv) == hoa_sym_3d_clockwise)
                x->f_clockwise = hoa_sym_3d_clockwise;
            else
                x->f_clockwise = hoa_sym_3d_anticlock;
        }
        else if(atom_isNumber(argv))
        {
            if(atom_getlong(argv) == 1)
                x->f_clockwise = hoa_sym_3d_clockwise;
            else
                x->f_clockwise = hoa_sym_3d_anticlock;
        }
        
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
		jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
		jbox_redraw((t_jbox *)x);
    }
    return 0;
}

t_max_err view_set(t_hoa_meter_3d *x, t_object *attr, long argc, t_atom *argv)
{
	t_symbol* view = x->f_view;
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_SYM)
        {
            if(atom_getsym(argv) == hoa_sym_3d_bottom)
                view = hoa_sym_3d_bottom;
            else if(atom_getsym(argv) == hoa_sym_3d_topnextbottom)
                view = hoa_sym_3d_topnextbottom;
            else if(atom_getsym(argv) == hoa_sym_3d_toponbottom)
                view = hoa_sym_3d_toponbottom;
            else
                view = hoa_sym_3d_top;
        }
        else if(atom_isNumber(argv))
        {
            if(atom_getlong(argv) == 1)
                view = hoa_sym_3d_bottom;
            else if(atom_getlong(argv) == 2)
                view = hoa_sym_3d_topnextbottom;
            else if(atom_getlong(argv) == 3)
                view = hoa_sym_3d_toponbottom;
            else
                view = hoa_sym_3d_top;
        }
        if(view != x->f_view)
        {
            x->f_view = view;
			jbox_set_patching_rect((t_object*)x, &x->f_rect);
			jbox_set_presentation_rect((t_object*)x, &x->f_presentation_rect);
        }
        
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_background_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
        jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
        jbox_redraw((t_jbox *)x);
    }
    return MAX_ERR_NONE;
}

void hoa_meter_3d_dsp64(t_hoa_meter_3d *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_meter->setVectorSize(maxvectorsize);
    object_method(dsp, gensym("dsp_add64"), x, (method)hoa_meter_3d_perform64, 0, NULL);
	x->f_startclock = 1;
}

void hoa_meter_3d_perform64(t_hoa_meter_3d *x, t_object *dsp, double **ins, long numins, double **outs, long no, long sampleframes, long f,void *up)
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

void hoa_meter_3d_tick(t_hoa_meter_3d *x)
{
    double peak;
    
    if(x->f_vector_type == hoa_sym_3d_both)
        x->f_vector->process(x->f_signals, x->f_vector_coords);
    else if(x->f_vector_type == hoa_sym_3d_velocity)
        x->f_vector->processVelocity(x->f_signals, x->f_vector_coords);
    else if(x->f_vector_type == hoa_sym_3d_energy)
        x->f_vector->processEnergy(x->f_signals, x->f_vector_coords + 3);
    
    for (int i = 0; i < x->f_meter->getNumberOfChannels(); i++)
    {
        peak = x->f_meter->getChannelEnergy(i);
        if(peak >= 0.)
            x->f_over_leds[i] = 1000;
        else
            x->f_over_leds[i] -= x->f_interval;
        
        if(x->f_over_leds[i] < 0)
            x->f_over_leds[i] = 0;
    }
    
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
	jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
  	jbox_redraw((t_jbox *)x);
    
	if (sys_getdspstate())
		clock_delay(x->f_clock, x->f_interval);
}

void hoa_meter_3d_assist(t_hoa_meter_3d *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET)
        sprintf(s,"(signal) %s", x->f_meter->getChannelName(a).c_str());
}

t_max_err hoa_meter_3d_notify(t_hoa_meter_3d *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	t_symbol *name;
	if (msg == hoa_sym_attr_modified)
	{
		name = (t_symbol *)object_method((t_object *)data, hoa_sym_getname);
		if(name == gensym("bgcolor") || name == gensym("cicolor") || name == gensym("coldcolor") || name == gensym("tepidcolor") || name == gensym("warmcolor") || name == gensym("hotcolor") || name == gensym("overloadcolor") || name == gensym("offcolor"))
		{
            jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_background_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
		}
        else if(name == gensym("vectors") || name == gensym("energycolor") || name == gensym("velocitycolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
		}
		else if(name == gensym("offset") || name == gensym("rotation"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_background_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_leds_layer);
			jbox_invalidate_layer((t_object *)x, NULL, hoa_sym_3d_vector_layer);
		}
		jbox_redraw((t_jbox *)x);
	}
	return jbox_notify((t_jbox *)x, s, msg, sender, data);
}

void hoa_meter_3d_paint(t_hoa_meter_3d *x, t_object *view)
{
	t_rect rect;
	jbox_get_rect_for_view((t_object *)x, view, &rect);
	x->f_rect = rect;
	
	t_rect pres_rect;
	jbox_get_presentation_rect((t_object*)x, &pres_rect);
	x->f_presentation_rect = pres_rect;
	
	if(x->f_view == hoa_sym_3d_topnextbottom)
        x->f_center = rect.width * .25;
    else
        x->f_center = rect.width * .5;
	x->f_radius = x->f_center * 0.95;
	
	draw_background(x, view, &rect);
    draw_leds(x, view, &rect);
    draw_vectors(x, view, &rect);
}

void draw_background(t_hoa_meter_3d *x,  t_object *view, t_rect *rect)
{
    t_jrgba black, white;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_3d_background_layer, rect->width, rect->height);
    
    black = rgba_addContrast(x->f_color_bg, -contrast_black);
    white = rgba_addContrast(x->f_color_bg, contrast_white);
    
	if (g)
	{
        jgraphics_rectangle(g, 0, 0, rect->width, rect->height);
        jgraphics_set_source_jrgba(g, &x->f_color_bg);
        jgraphics_fill(g);
        
		jgraphics_set_line_width(g, 1.);
		
		if(x->f_view == hoa_sym_3d_topnextbottom)
        {
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_set_line_width(g, 3.);
            jgraphics_arc(g, x->f_center, x->f_center, x->f_radius,  0., HOA_2PI);
            jgraphics_stroke(g);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_set_line_width(g, 1.);
            jgraphics_stroke(g);
            
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_line_draw_fast(g, x->f_center * 2, 0, x->f_center * 2, x->f_center * 2, 3);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_line_draw_fast(g, x->f_center * 2, 0, x->f_center * 2, x->f_center * 2, 1);
            
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_set_line_width(g, 3.);
            jgraphics_arc(g, x->f_center * 3, x->f_center, x->f_radius,  0., HOA_2PI);
            jgraphics_stroke(g);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_set_line_width(g, 1.);
            jgraphics_stroke(g);
        }
        else if(x->f_view == hoa_sym_3d_toponbottom)
        {
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_set_line_width(g, 3.);
            jgraphics_arc(g, x->f_center, x->f_center, x->f_radius,  0., HOA_2PI);
            jgraphics_stroke(g);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_set_line_width(g, 1.);
            jgraphics_stroke(g);
            
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_line_draw_fast(g, 0, x->f_center * 2, x->f_center * 2, x->f_center * 2, 3);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_line_draw_fast(g, 0, x->f_center * 2, x->f_center * 2, x->f_center * 2, 1);
            
            jgraphics_set_source_jrgba(g, &white);
            jgraphics_set_line_width(g, 3.);
            jgraphics_arc(g, x->f_center, x->f_center * 3, x->f_radius,  0., HOA_2PI);
            jgraphics_stroke(g);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_set_line_width(g, 1.);
            jgraphics_stroke(g);
        }
        else
        {
			jgraphics_set_source_jrgba(g, &white);
            jgraphics_set_line_width(g, 3.);
            jgraphics_arc(g, x->f_center, x->f_center, x->f_radius,  0., HOA_2PI);
            jgraphics_stroke(g);
            jgraphics_set_source_jrgba(g, &black);
            jgraphics_set_line_width(g, 1.);
            jgraphics_stroke(g);
		}
        
		jbox_end_layer((t_object*)x, view, hoa_sym_3d_background_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_3d_background_layer, 0., 0.);
}

void draw_leds(t_hoa_meter_3d *x, t_object *view, t_rect *rect)
{
    t_jmatrix transform;
    t_jrgba black = rgba_addContrast(x->f_color_bg, -contrast_black);
    t_jrgba white = rgba_addContrast(x->f_color_bg, contrast_white);
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_3d_leds_layer, rect->width, rect->height);
    t_jrgba mcolor;
    bool viewd = 1;
    if(x->f_view == hoa_sym_3d_bottom)
        viewd = 0;
	
	if (g)
	{
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
        jgraphics_set_matrix(g, &transform);
        
		for(int i = 0; i < x->f_meter->getNumberOfChannels(); i++)
		{
			if(x->f_over_leds[i])
				mcolor = x->f_color_over_signal;
            else if(x->f_meter->getChannelEnergy(i) < -90.)
                mcolor = x->f_color_off_signal;
            else if(x->f_meter->getChannelEnergy(i) < -30.)
                mcolor = x->f_color_cold_signal;
            else if(x->f_meter->getChannelEnergy(i) < -21)
                mcolor = x->f_color_tepid_signal;
            else if(x->f_meter->getChannelEnergy(i) < -12)
                mcolor = x->f_color_warm_signal;
            else
                mcolor = x->f_color_hot_signal;
            
            jgraphics_set_source_jrgba(g, &mcolor);
            int npt = x->f_meter->getChannelNumberOfPoints(i, viewd);
            int factor = max<int>(sqrt((rect->width * 100)) / (double)(npt * 0.1) - 1, 1);
			if(npt)
			{
				double azi = x->f_meter->getChannelPointAzimuth(i, 0, viewd);
                if(x->f_clockwise == hoa_sym_3d_clockwise)
                    azi =-azi;
				double ele = x->f_meter->getChannelPointElevation(i, 0, viewd);
                double abs = Math<double>::abscissa(x->f_radius, azi, ele);
				double ord = Math<double>::ordinate(x->f_radius, azi, ele);
                
				jgraphics_move_to(g, abs, ord);
				for(int j = factor; j < npt; j += factor)
				{
					azi = x->f_meter->getChannelPointAzimuth(i, j, viewd);
                    if(x->f_clockwise == hoa_sym_3d_clockwise)
                        azi =-azi;
                    
					ele = x->f_meter->getChannelPointElevation(i, j, viewd);
					abs = Math<double>::abscissa(x->f_radius, azi, ele);
					ord = Math<double>::ordinate(x->f_radius, azi, ele);
					jgraphics_line_to(g, abs, ord);
					
				}
				
				jgraphics_close_path(g);
				jgraphics_fill_preserve(g);
				
				jgraphics_set_source_jrgba(g, &white);
				jgraphics_set_line_width(g, 2);
				jgraphics_stroke(g);
                
                jgraphics_set_source_jrgba(g, &black);
				jgraphics_set_line_width(g, 1);
				jgraphics_stroke(g);
			}
			
		}
        
        if(x->f_view == hoa_sym_3d_toponbottom || x->f_view == hoa_sym_3d_topnextbottom)
        {
            viewd = 0;
            if(x->f_view == hoa_sym_3d_toponbottom)
                jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center * 3);
            else
                jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center * 3, x->f_center);
            
            jgraphics_set_matrix(g, &transform);
            
            for(int i = 0; i < x->f_meter->getNumberOfChannels(); i++)
            {
                if(x->f_over_leds[i])
					mcolor = x->f_color_over_signal;
				else if(x->f_meter->getChannelEnergy(i) < -90.)
					mcolor = x->f_color_off_signal;
				else if(x->f_meter->getChannelEnergy(i) < -30.)
					mcolor = x->f_color_cold_signal;
				else if(x->f_meter->getChannelEnergy(i) < -21)
					mcolor = x->f_color_tepid_signal;
				else if(x->f_meter->getChannelEnergy(i) < -12)
					mcolor = x->f_color_warm_signal;
				else
					mcolor = x->f_color_hot_signal;
                
                jgraphics_set_source_jrgba(g, &mcolor);
                int npt = x->f_meter->getChannelNumberOfPoints(i, viewd);
                int factor = max<int>(sqrt((rect->width * 100)) / (double)(npt * 0.1) - 1, 1);
                if(npt)
                {
                    double azi = x->f_meter->getChannelPointAzimuth(i, 0, viewd);
                    if(x->f_clockwise == hoa_sym_3d_clockwise)
                        azi =-azi;
                    double ele = x->f_meter->getChannelPointElevation(i, 0, viewd);
                    double abs = Math<double>::abscissa(x->f_radius, azi, ele);
                    double ord = Math<double>::ordinate(x->f_radius, azi, ele);
                    
                    jgraphics_move_to(g, abs, ord);
                    for(int j = factor; j < npt; j += factor)
                    {
                        azi = x->f_meter->getChannelPointAzimuth(i, j, viewd);
                        if(x->f_clockwise == hoa_sym_3d_clockwise)
                            azi =-azi;
                        
                        ele = x->f_meter->getChannelPointElevation(i, j, viewd);
                        abs = Math<double>::abscissa(x->f_radius, azi, ele);
                        ord = Math<double>::ordinate(x->f_radius, azi, ele);
                        jgraphics_line_to(g, abs, ord);
                        
                    }
                    jgraphics_close_path(g);
                    jgraphics_fill_preserve(g);
                    
                    jgraphics_set_source_jrgba(g, &white);
                    jgraphics_set_line_width(g, 2);
                    jgraphics_stroke(g);
                    
                    jgraphics_set_source_jrgba(g, &black);
                    jgraphics_set_line_width(g, 1);
                    jgraphics_stroke(g);
                }
                
            }
        }
		jbox_end_layer((t_object*)x, view, hoa_sym_3d_leds_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_3d_leds_layer, 0., 0.);
}

void draw_vectors(t_hoa_meter_3d *x, t_object *view, t_rect *rect)
{
	double x1, y1, size;
	t_jmatrix transform;
	double distance;
	t_jrgba color;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, hoa_sym_3d_vector_layer, rect->width, rect->height);
	
	if (g)
	{
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		size = x->f_center / 20;
        
        if(x->f_vector_type == hoa_sym_3d_both || x->f_vector_type == hoa_sym_3d_energy)
        {
            double rad = Math<float>::radius(x->f_vector_coords[3], x->f_vector_coords[4], x->f_vector_coords[5]);
            distance = (fabs(rad) * 0.5 + 0.5);
            color = rgba_addContrast(x->f_color_energy_vector, -(1. - distance));
            jgraphics_set_source_jrgba(g, &color);
            if(x->f_clockwise == hoa_sym_3d_anticlock)
            {
                x1 = x->f_vector_coords[3] * x->f_radius;
                y1 = x->f_vector_coords[4] * x->f_radius;
            }
            else
            {
                double ang = -Math<double>::azimuth(x->f_vector_coords[3], x->f_vector_coords[4], x->f_vector_coords[5]);
                x1 = Math<double>::abscissa(rad * x->f_radius, ang);
                y1 = Math<double>::ordinate(rad * x->f_radius, ang);
            }
            
            if((x->f_vector_coords[5] >= 0 && (x->f_view == hoa_sym_3d_top || x->f_view == hoa_sym_3d_toponbottom || x->f_view == hoa_sym_3d_topnextbottom)) ||  (x->f_vector_coords[5] <= 0 && x->f_view == hoa_sym_3d_bottom))
            {
                jgraphics_arc(g, x1, y1, size * distance, 0., HOA_2PI);
                jgraphics_fill(g);
            }
            else if(x->f_vector_coords[5] <= 0 && x->f_view == hoa_sym_3d_toponbottom)
            {
                jgraphics_arc(g, x1, y1 - x->f_center * 2, size * distance, 0., HOA_2PI);
                jgraphics_fill(g);
            }
            else if(x->f_vector_coords[5] <= 0 && x->f_view == hoa_sym_3d_topnextbottom)
            {
                jgraphics_arc(g, x1 + x->f_center * 2, y1, size * distance, 0., HOA_2PI);
                jgraphics_fill(g);
            }
		}
        if(x->f_vector_type == hoa_sym_3d_both || x->f_vector_type == hoa_sym_3d_velocity)
        {
            double rad = Math<double>::radius(x->f_vector_coords[0], x->f_vector_coords[1], x->f_vector_coords[2]);
            distance = (fabs(rad) * 0.5 + 0.5);
            color = rgba_addContrast(x->f_color_velocity_vector, -(1. - distance));
            jgraphics_set_source_jrgba(g, &color);
            if(x->f_clockwise == hoa_sym_3d_anticlock)
            {
                x1 = x->f_vector_coords[0] * x->f_radius;
                y1 = x->f_vector_coords[1] * x->f_radius;
            }
            else
            {
                double ang = -Math<double>::azimuth(x->f_vector_coords[0], x->f_vector_coords[1], x->f_vector_coords[2]);
                x1 = Math<double>::abscissa(rad * x->f_radius, ang);
                y1 = Math<double>::ordinate(rad * x->f_radius, ang);
            }
            
            if((x->f_vector_coords[2] >= 0 && (x->f_view == hoa_sym_3d_top || x->f_view == hoa_sym_3d_toponbottom || x->f_view == hoa_sym_3d_topnextbottom)) || (x->f_vector_coords[2] <= 0 && x->f_view == hoa_sym_3d_bottom))
            {
                jgraphics_arc(g, x1, y1, size * distance, 0., HOA_2PI);
                jgraphics_fill(g);
            }
            else if(x->f_vector_coords[2] <= 0 && x->f_view == hoa_sym_3d_toponbottom)
            {
                jgraphics_arc(g, x1, y1 - x->f_center * 2, size * distance, 0., HOA_2PI);
                jgraphics_fill(g);
            }
            else if(x->f_vector_coords[2] <= 0 && x->f_view == hoa_sym_3d_topnextbottom)
            {
                jgraphics_arc(g, x1 + x->f_center * 2, y1, size * distance, 0., HOA_2PI);
                jgraphics_fill(g);
            }
		}
        jbox_end_layer((t_object*)x, view, hoa_sym_3d_vector_layer);
	}
	jbox_paint_layer((t_object *)x, view, hoa_sym_3d_vector_layer, 0., 0.);
}