/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.connect.cpp
 @name      hoa.connect
 @realname  hoa.connect
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 automatically connect several hoa objects together.
 
 @description
 <o>hoa.connect</o> could be used to automatically connect several hoa objects together and to color patchlines
 
 @discussion
 <o>hoa.connect</o> could be used to automatically connect several hoa objects together and to color patchlines
 
 @category ambisonics, hoa objects, script
 
 @seealso hoa.process~
 */

#include "HoaCommon.max.h"

#define CONNECT_MAX_TAB 100

typedef struct  _connect
{
	t_object	f_ob;
	
	t_patcher*	f_patcher;
	t_object*	f_patcherview;
	
	t_object**	f_objects;
	
    Processor<Hoa2d, t_sample>::Harmonics* f_ambi2D;
    Processor<Hoa3d, t_sample>::Harmonics* f_ambi3D;
	
	t_jrgba		f_color_zero;
	t_jrgba		f_color_positiv;
	t_jrgba		f_color_negativ;
	t_jrgba		f_color_plane;
	
	int			f_nbSelected;
} t_connect;

static t_class *connect_class;

void connect_free(t_connect *x)
{
	if(x->f_patcherview)
		object_detach_byptr(x, x->f_patcherview);
	
	delete x->f_ambi2D;
	delete x->f_ambi3D;
	
	delete [] x->f_objects;
}

void connect_attach(t_connect *x)
{
	object_obex_lookup(x, hoa_sym_pound_P, &x->f_patcher);
	x->f_patcherview = object_attr_getobj(x->f_patcher, hoa_sym_firstview);
	object_attach_byptr_register(x, x->f_patcherview, CLASS_NOBOX);
}

void connect_assist(t_connect *x, void *b, long m, long a, char *s)
{
    // @in 0 @type bang @digest bang to connect objects and color patchlines
    sprintf(s, "(bang) Select objects you want to connect, then bang me to connect them");
}

// custom attr setter to change patchlines colors
t_max_err connect_setattr_zerocolor(t_connect *x, void *attr, long argc, t_atom *argv)
{
	if (argc >= 4 && argv)
	{
		x->f_color_zero.red = atom_getfloat(argv);
		x->f_color_zero.green = atom_getfloat(argv + 1);
		x->f_color_zero.blue = atom_getfloat(argv + 2);
		x->f_color_zero.alpha = atom_getfloat(argv + 3);
	}
	return MAX_ERR_NONE;
}

void connect_connect(t_patcher *p, t_object *send, int outlet, t_object *receive, int inlet)
{
    t_atom msg[4];
    t_atom rv;
    
    atom_setobj(msg, send);
    atom_setlong(msg + 1, outlet);
    atom_setobj(msg + 2, receive);
    atom_setlong(msg + 3, inlet);
    
    object_method_typed(p , hoa_sym_connect, 4, msg, &rv);
}

short is_obj_hoa_exotic(t_object *obj)
{
    int i;
    t_object *jb, *o;
    char objName[] = "nop.";
    
    if(object_classname(obj) == hoa_sym_jpatcher)
    {
        if(strlen(jpatcher_get_name(obj)->s_name) >= 4)
        {
            for(i = 0; i < 4; i++)
            {
                objName[i] = jpatcher_get_name(obj)->s_name[i];
            }
        }
        else
        {
            jb = jpatcher_get_firstobject(obj);
            while(jb)
            {
                o = jbox_get_object(jb);
                if(object_classname(o) == gensym("hoa.plug_script"))
                {
                    strcpy(objName, "hoa.");
                }
                jb = jbox_get_nextobject(jb);
            }
        }
    }
    else if (object_classname(obj) == gensym("dac~") || object_classname(obj) == gensym("sfrecord~") || object_classname(obj) == gensym("sfplay~"))
    {
        strcpy(objName, "hoa.");
    }
    else if (strlen(object_classname(obj)->s_name) >= 4)
    {
        for(i = 0; i < 4; i++)
        {
            objName[i] = object_classname(obj)->s_name[i];
        }
    }
    
    if(strcmp(objName, "hoa.") == 0)
        return 1;
    
    return 0;
}

void make_patchline(t_connect *x)
{
	int connexions, valid_objects, i, j;
	valid_objects = 0;
	t_object *obj1, *obj2;
	t_hoa_err err[2];
	t_hoa_boxinfos* startobj_infos;
	t_hoa_boxinfos* endobj_infos;
	short startobj_type, endobj_type;
	long outlets, inlets;
	
	if (x->f_nbSelected > 1)
	{
		for (i = 0; i < x->f_nbSelected; i++)
		{
            obj1 = jbox_get_object(x->f_objects[i]);
			if(object_is_hoa(obj1) || is_obj_hoa_exotic(obj1))
            {
				x->f_objects[valid_objects++] = x->f_objects[i]; // ! store BOX objects
            }
		}
		
		if (valid_objects > 1)
		{
			startobj_infos = (t_hoa_boxinfos*) malloc( sizeof(t_hoa_boxinfos));
			endobj_infos = (t_hoa_boxinfos*) malloc( sizeof(t_hoa_boxinfos));
			
			for(i = 1; i < valid_objects; i++)
			{
				obj1 = jbox_get_object(x->f_objects[i-1]);
				obj2 = jbox_get_object(x->f_objects[ i ]);
				
				outlets = inlets = 0;
				
				// referenced object or exotic hoa one;
				startobj_type = object_is_hoa(obj1);
				endobj_type	  = object_is_hoa(obj2);
				
				hoa_boxinfos_init(startobj_infos);
				hoa_boxinfos_init(endobj_infos);
				err[0] = (t_hoa_err) object_method(obj1, hoa_sym_hoa_getinfos, startobj_infos, NULL);
				err[1] = (t_hoa_err) object_method(obj2, hoa_sym_hoa_getinfos, endobj_infos, NULL);
				
				// get number of outlets
				if (startobj_type == 1 && err[0] == HOA_ERR_NONE)
					outlets = startobj_infos->autoconnect_outputs;
				else
					outlets = outlet_count(obj1);
				
				// get number of inlets
				if (endobj_type == 1 && err[1] == HOA_ERR_NONE)
					inlets = endobj_infos->autoconnect_inputs;
				else
					inlets = inlet_count(obj2);
				
				connexions = MIN(outlets, inlets);
				for(j = 0; j < connexions; j++)
					connect_connect(x->f_patcher, x->f_objects[i-1], j, x->f_objects[i], j);
			}
			
			free(startobj_infos);
			free(endobj_infos);
		}
		
		for(i = 0; i < CONNECT_MAX_TAB; i++)
			x->f_objects[i] = NULL;
		
		jpatcher_set_dirty(x->f_patcherview, true);
	}
	x->f_nbSelected = 0;
}

void color_patchline(t_connect *x)
{
	t_object *line, *startobj;
	t_hoa_err err;
	t_jrgba* linecolor = NULL;
	long outletnum, sign;
	t_hoa_boxinfos* startobj_infos = (t_hoa_boxinfos*) malloc( sizeof(t_hoa_boxinfos));
	line = jpatcher_get_firstline(x->f_patcher);
    
	while (line)
	{
		startobj = jbox_get_object(jpatchline_get_box1(line));
		
		if(object_is_hoa(startobj) || is_obj_hoa_exotic(startobj))
		{
			hoa_boxinfos_init(startobj_infos);
			err = (t_hoa_err) object_method(startobj, hoa_sym_hoa_getinfos, startobj_infos, NULL);
			
			if (err == HOA_ERR_NONE)
			{
				// ambisonics colors (zero | neg | pos) (ex: hoa.encoder~ => hoa.optim~)
				if (startobj_infos->autoconnect_outputs_type == HOA_CONNECT_TYPE_AMBISONICS)
				{
					outletnum = jpatchline_get_outletnum(line);
                    
                    sign = 0;
					
					if (startobj_infos->object_type == HOA_OBJECT_2D)
						sign = x->f_ambi2D->getHarmonicOrder(outletnum);
					if (startobj_infos->object_type == HOA_OBJECT_3D)
						sign = x->f_ambi3D->getHarmonicOrder(outletnum);
					
					if (sign > 0)
						linecolor = &x->f_color_positiv;
					else if (sign < 0)
						linecolor = &x->f_color_negativ;
					else
						linecolor = &x->f_color_zero;
					
                    object_attr_setcolor(line, hoa_sym_patchlinecolor, linecolor);
				}
                
				// planewave color (ex: hoa.projector~ => hoa.recomposer~)
				else if (startobj_infos->autoconnect_outputs_type == HOA_CONNECT_TYPE_PLANEWAVES)
				{
                    object_attr_setcolor(line, hoa_sym_patchlinecolor, &x->f_color_plane);
				}
			}
		}
		
		line = jpatchline_get_nextline(line);
	}
	
	free(startobj_infos);
}

void connect_bang(t_connect *x)
{
    make_patchline(x);
    color_patchline(x);
}

t_max_err connect_setattr_poscolor(t_connect *x, void *attr, long argc, t_atom *argv)
{
    if (argc >= 4 && argv)
    {
        x->f_color_positiv.red = atom_getfloat(argv);
        x->f_color_positiv.green = atom_getfloat(argv + 1);
        x->f_color_positiv.blue = atom_getfloat(argv + 2);
        x->f_color_positiv.alpha = atom_getfloat(argv + 3);
        connect_bang(x);
    }
    return MAX_ERR_NONE;
}
t_max_err connect_setattr_negcolor(t_connect *x, void *attr, long argc, t_atom *argv)
{
    if (argc >= 4 && argv)
    {
        x->f_color_negativ.red = atom_getfloat(argv);
        x->f_color_negativ.green = atom_getfloat(argv + 1);
        x->f_color_negativ.blue = atom_getfloat(argv + 2);
        x->f_color_negativ.alpha = atom_getfloat(argv + 3);
        connect_bang(x);
    }
    return MAX_ERR_NONE;
}
t_max_err connect_setattr_planecolor(t_connect *x, void *attr, long argc, t_atom *argv)
{
    if (argc >= 4 && argv)
    {
        x->f_color_plane.red = atom_getfloat(argv);
        x->f_color_plane.green = atom_getfloat(argv + 1);
        x->f_color_plane.blue = atom_getfloat(argv + 2);
        x->f_color_plane.alpha = atom_getfloat(argv + 3);
        connect_bang(x);
    }
    return MAX_ERR_NONE;
}

t_max_err connect_notify(t_connect *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	int i, j, k, tabcheck;
    if(msg == hoa_sym_free && sender == x->f_patcherview)
	{
        x->f_patcherview = NULL;
    }
	else if(msg == hoa_sym_attr_modified && sender == x->f_patcherview)
	{
		t_symbol *attrname;
		attrname = (t_symbol *)object_method(data, hoa_sym_getname);
			
		if (attrname == hoa_sym_selectedboxes)
		{
			t_atom *av = NULL;
			long current_nb_selected = 0;
			
			object_attr_getvalueof(sender, attrname, &current_nb_selected, &av);
            
            current_nb_selected = min<long>(current_nb_selected, CONNECT_MAX_TAB - 1);
            
			if (current_nb_selected && av)
			{				
				// on supprime tous les objets du tableau qui ne sont plus dans la selection
				if(x->f_nbSelected < 0)
					x->f_nbSelected = 0;
				if(current_nb_selected < x->f_nbSelected)
				{
					for(i = 0 ; i < current_nb_selected ; i++)
					{
						tabcheck = 0;
						for(j = 0; j < x->f_nbSelected; j++)
						{
							if (atom_gettype(av+i) == A_OBJ && x->f_objects[j] == (t_object*)atom_getobj(av+i)) 
							{	
								tabcheck = 1;
								break;
							}
						}
						
						if (!tabcheck)
							x->f_objects[j] = NULL;
					}
				
					// puis on retrie le tableau
					for(i = 0; i < x->f_nbSelected; i++)
					{
						if (x->f_objects[i] == NULL)
						{
							for(k = i; k < x->f_nbSelected; k++)
								x->f_objects[k] = x->f_objects[k+1];
							
							x->f_nbSelected--;
							break;
						}
					}
				}
				else if(current_nb_selected > x->f_nbSelected)
				{
					for(i = 0 ; i < current_nb_selected ; i++)
					{
						tabcheck = 0;
						for(j = 0; j < x->f_nbSelected; j++)
						{
							if (atom_gettype(av+i) == A_OBJ && x->f_objects[j] == (t_object*)atom_getobj(av+i)) 
							{	
								tabcheck = 1;
							}
						}
						
						if (!tabcheck)
							x->f_objects[x->f_nbSelected] = (t_object*)atom_getobj(av+i);
					}
					x->f_nbSelected++;
				}
			}
			freebytes(av, sizeof(t_atom) * current_nb_selected);
		}			
	}


	if (msg == hoa_sym_startdrag)
	{
		if (jkeyboard_getcurrentmodifiers() != 18) // shift key
		{
			for(i = 0; i < CONNECT_MAX_TAB; i++)
				x->f_objects[i] = NULL;
			
			x->f_nbSelected = 0;
		}
	}
	
	return MAX_ERR_NONE;
}

void *connect_new(t_symbol *s, long argc, t_atom *argv)
{
    t_connect *x = NULL;
    
    x = (t_connect *)object_alloc((t_class *)connect_class);
    if (x)
    {
        // load Ambisonic instances to query harmonics band or argument in 2D or 3D
        x->f_ambi2D = new Processor<Hoa2d, t_sample>::Harmonics(HOA_MAX_PLANEWAVES*0.5 -1);
        x->f_ambi3D = new Processor<Hoa3d, t_sample>::Harmonics(sqrt((long double)HOA_MAX_PLANEWAVES)-1);
        
        x->f_objects = new t_object*[CONNECT_MAX_TAB];
        
        // colors setup
        x->f_color_zero.green = x->f_color_positiv.red = x->f_color_negativ.blue = 1.;
        x->f_color_plane.red = x->f_color_plane.green = x->f_color_plane.blue = 1.;
        x->f_color_zero.red = x->f_color_zero.green = x->f_color_zero.blue = 0.2;
        x->f_color_zero.alpha = x->f_color_positiv.alpha = x->f_color_negativ.alpha = x->f_color_plane.alpha = 1.;
        
        x->f_nbSelected = 0;
        defer_low(x, (method)connect_attach, NULL, 0, NULL);
    }
    
    return x;
}

#ifdef HOA_PACKED_LIB
int hoa_connect_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class *c;
    
    c = class_new("hoa.connect", (method)connect_new, (method)connect_free, sizeof(t_connect), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.connect", (char *)"hoa.connect");
    
    hoa_initclass(c, (method)NULL);
    
    class_addmethod(c, (method)connect_assist,	"assist",	A_CANT,	0);
    class_addmethod(c, (method)connect_notify,	"notify",	A_CANT, 0);
    
    // @method bang @digest trigger autoconnection and color
    // @description The <m>bang</m> message connect all of the appropriate selected hoa objects together
    class_addmethod(c, (method)connect_bang,	"bang",		A_CANT,	0);
    
    CLASS_STICKY_ATTR		(c, "category", 0, "Behavior");
    CLASS_ATTR_RGBA			(c, "zhcolor", 0, t_connect, f_color_zero);
    CLASS_ATTR_ACCESSORS	(c, "zhcolor", NULL, connect_setattr_zerocolor);
    CLASS_ATTR_SAVE			(c, "zhcolor", 1);
    CLASS_ATTR_STYLE_LABEL	(c, "zhcolor", 0, "rgba", "zero harmonics color");
    // @description Sets the RGBA values for the zero harmonics color of the ambisonic domain patchlines
    
    CLASS_ATTR_RGBA			(c, "phcolor", 0, t_connect, f_color_positiv);
    CLASS_ATTR_ACCESSORS	(c, "phcolor", NULL, connect_setattr_poscolor);
    CLASS_ATTR_SAVE			(c, "phcolor", 1);
    CLASS_ATTR_STYLE_LABEL	(c, "phcolor", 0, "rgba", "positive harmonics color");
    // @description Sets the RGBA values for the positive harmonics color of the ambisonic domain patchlines
    
    CLASS_ATTR_RGBA			(c, "nhcolor", 0, t_connect, f_color_negativ);
    CLASS_ATTR_ACCESSORS	(c, "nhcolor", NULL, connect_setattr_negcolor);
    CLASS_ATTR_SAVE			(c, "nhcolor", 1);
    CLASS_ATTR_STYLE_LABEL	(c, "nhcolor", 0, "rgba", "negative harmonics color");
    // @description Sets the RGBA values for the negative harmonics color of the ambisonic domain patchlines
    
    CLASS_ATTR_RGBA			(c, "planecolor", 0, t_connect, f_color_plane);
    CLASS_ATTR_ACCESSORS	(c, "planecolor", NULL, connect_setattr_planecolor);
    CLASS_ATTR_STYLE_LABEL	(c, "planecolor", 0, "rgba", "planewaves signals color");
    CLASS_ATTR_SAVE			(c, "planecolor", 1);
    // @description Sets the RGBA values for the planewaves signals color of the planewaves domain patchlines
    
    CLASS_STICKY_ATTR_CLEAR	(c, "category");
    
    class_register(CLASS_BOX, c);
    connect_class = c;
    hoa_print_credit();
    return 0;
}
