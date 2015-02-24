/*
 // Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

// based on dynamic.out~ Copyright 2010 Alex Harker. All rights reserved.

/**
 @file      hoa.out.cpp
 @name      hoa.out
 @realname  hoa.out
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 message outlet for a patcher loaded by hoa.process~
 
 @description
 <o>hoa.out</o> defines a message outlet for a patcher loaded by hoa.process~
 
 @discussion
 <o>hoa.out</o> defines a message outlet for a patcher loaded by hoa.process~
 
 @category ambisonics, hoa objects, audio, msp
 
 @seealso hoa.process~, hoa.out~, hoa.in~, hoa.in
 */

#include "HoaCommon.max.h"
#include "HoaProcessSuite.h"

typedef struct _hoa_out
{
    t_object	x_obj;
	long outlet_num;
	t_object *parent_processor;
	long parent_patcher_index;
	
	t_symbol*	comment;
	long		extra;
	
} t_hoa_out;

t_class *hoa_out_class;

void *hoa_out_new(t_symbol *s, short ac, t_atom *av);
void hoa_out_free(t_hoa_out *x);
void hoa_out_assist(t_hoa_out *x, void *b, long m, long a, char *s);

void hoa_out_bang(t_hoa_out *x);
void hoa_out_int(t_hoa_out *x, long n);
void hoa_out_float(t_hoa_out *x, double f);
void hoa_out_list(t_hoa_out *x, t_symbol *s, short argc, t_atom *argv);
void hoa_out_anything(t_hoa_out *x, t_symbol *s, short argc, t_atom *argv);

t_max_err hoa_out_setattr_extra(t_hoa_out *x, void *attr, long ac, t_atom *av);
t_max_err hoa_out_setattr_comment(t_hoa_out *x, void *attr, long ac, t_atom *av);

#ifdef HOA_PACKED_LIB
int hoa_out_main(void)
#else
int C74_EXPORT main(void)
#endif
{
	t_class* c;
	c = class_new("hoa.out", (method)hoa_out_new, (method)hoa_out_free, sizeof(t_hoa_out), NULL, A_GIMME, 0);
	class_setname((char *)"hoa.out", (char *)"hoa.out");
    
	hoa_initclass(c, (method)NULL);
    class_addmethod(c, (method)hoa_out_assist,			"assist",			A_CANT,	 0);
    
    // @method bang @digest output bang message in the corresponding hoa.process~ object's message outlet
	// @description The <m>bang</m> output bang message in the corresponding hoa.process~ object's message outlet
	class_addmethod(c, (method)hoa_out_bang,			"bang",						 0);
    
    // @method int @digest output int message in the corresponding hoa.process~ object's message outlet
	// @description The <m>int</m> output int message in the corresponding hoa.process~ object's message outlet
	// @marg 0 @name number @optional 0 @type int
	class_addmethod(c, (method)hoa_out_int,				"int",				A_LONG,	 0);
    
    // @method float @digest output int message in the corresponding hoa.process~ object's message outlet
	// @description The <m>float</m> output int message in the corresponding hoa.process~ object's message outlet
	// @marg 0 @name number @optional 0 @type float
	class_addmethod(c, (method)hoa_out_float,			"float",			A_FLOAT, 0);
    
    // @method list @digest output a list in the corresponding hoa.process~ object's message outlet
	// @description The <m>list</m> output a list in the corresponding hoa.process~ object's message outlet
	// @marg 0 @name va-args @optional 0 @type list
	class_addmethod(c, (method)hoa_out_list,			"list",				A_GIMME, 0);
    
    // @method anything @digest output a message in the corresponding hoa.process~ object's message outlet
	// @description The <m>list</m> output a message in the corresponding hoa.process~ object's message outlet
	// @marg 0 @name anything @optional 0 @type symbol
	class_addmethod(c, (method)hoa_out_anything,		"anything",			A_GIMME, 0);
	
	CLASS_ATTR_LONG		(c, "extra", 0, t_hoa_out, extra);
	CLASS_ATTR_ACCESSORS(c, "extra", 0, hoa_out_setattr_extra);
	CLASS_ATTR_LABEL	(c, "extra", 0, "extra index");
	CLASS_ATTR_INVISIBLE(c, "extra", 1);
	CLASS_ATTR_SAVE		(c, "extra", 0);
    // @description Defines an extra outlet. Extra outlet are added to the "normal" instance outlet and can be used to receive messages from all instances.
	
	CLASS_ATTR_SYM		(c, "comment", 0, t_hoa_out, comment);
	CLASS_ATTR_ACCESSORS(c, "comment", 0, hoa_out_setattr_comment);
	CLASS_ATTR_LABEL	(c, "comment", 0, "Description");
	CLASS_ATTR_SAVE		(c, "comment", 1);
    // @description Sets a description to the outlet which will be shown in the assist outlet of the <o>hoa.process~</o> that load this <o>hoa.out</o>.
    // Only works if the <m>extra</m> parameter is greater than 0.
    
	class_register(CLASS_BOX, c);
	hoa_out_class = c;
	return 0;
}

void *hoa_out_new(t_symbol *s, short ac, t_atom *av)
{
    t_hoa_out *x = (t_hoa_out *)object_alloc(hoa_out_class);
	void *hoaprocessor_parent = Get_HoaProcessor_Object();
	
	x->extra = 0;
	x->comment = gensym("");
	
	attr_args_process(x, ac, av);
	
	x->parent_processor = (t_object*) hoaprocessor_parent;
	x->parent_patcher_index = Get_HoaProcessor_Patch_Index(hoaprocessor_parent);
	x->outlet_num = 1;
	
	long out = HoaProcessor_Get_IO_Index(hoaprocessor_parent, x->parent_patcher_index, (t_object*)x);
	
	if (out > 0)
		x->outlet_num = out;
		
    return (x);
}

void hoa_out_assist(t_hoa_out *x, void *b, long m, long a, char *s)
{
    // @in 0 @type message @digest output messages to a hoa.process~ object's outlet
    
    if (x->extra > 0)
	{
		if (x->comment != hoa_sym_nothing)
			sprintf(s,"(messages) hoa.process~ extra output, %s", x->comment->s_name);
		else
			sprintf(s,"(messages) hoa.process~ extra output");
	}
	else
	{
		sprintf(s,"(messages) hoa.process~ instance output");
	}
}

t_max_err hoa_out_setattr_extra(t_hoa_out *x, void *attr, long ac, t_atom *av)
{
	if (ac && av && atom_gettype(av) == A_LONG)
	{
		x->extra = atom_getlong(av);
		if (x->extra < 0) x->extra = 0;
	}
	return MAX_ERR_NONE;
}

t_max_err hoa_out_setattr_comment(t_hoa_out *x, void *attr, long ac, t_atom *av)
{
	if (ac && av && atom_gettype(av) == A_SYM)
		x->comment = atom_getsym(av);
	
	return MAX_ERR_NONE;
}

void hoa_out_bang(t_hoa_out *x)
{
	t_args_struct args;
	args.msg = hoa_sym_bang;
	args.index = x->outlet_num;
	args.argc = 0;
	args.argv = NULL;
	object_method(x->parent_processor, hoa_sym_out_message, &args);
}

void hoa_out_int(t_hoa_out *x, long n)
{
	t_args_struct args;
	args.msg = hoa_sym_int;
	args.index = x->outlet_num;
	args.argc = 1;
	args.argv = (t_atom*) malloc(sizeof(t_atom));
	atom_setfloat(args.argv, n);
	object_method(x->parent_processor, hoa_sym_out_message, &args);
}

void hoa_out_float(t_hoa_out *x, double f)
{
	t_args_struct args;
	args.msg = hoa_sym_float;
	args.index = x->outlet_num;
	args.argc = 1;
	args.argv = (t_atom*) malloc(sizeof(t_atom));
	atom_setfloat(args.argv, f);
	object_method(x->parent_processor, hoa_sym_out_message, &args);
}

void hoa_out_list(t_hoa_out *x, t_symbol *s, short argc, t_atom *argv)
{
	t_args_struct args;
	args.msg = hoa_sym_list;
	args.index = x->outlet_num;
	args.argc = argc;
	args.argv = argv;
	object_method(x->parent_processor, hoa_sym_out_message, &args);
}

void hoa_out_anything(t_hoa_out *x, t_symbol *s, short argc, t_atom *argv)
{
	t_args_struct args;
	args.msg = s;
	args.index = x->outlet_num;
	args.argc = argc;
	args.argv = argv;
	object_method(x->parent_processor, hoa_sym_out_message, &args);
}

void hoa_out_free(t_hoa_out *x)
{
	;
}
